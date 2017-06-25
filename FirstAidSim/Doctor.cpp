#include <iostream>
#include "Doctor.h"
#include <string>

Doctor::Doctor(function<int(int)> f,int run) {
	this->setMoveState(Doctor::IS_AT_STATION);
	this->timeAtStation = 0;
	this->simDataPtr = SimData::getInstance();
	this->emergencyListPtr = EmergencyList::getInstance();
	string number = to_string(run);
	number = string(3 - number.length(), '0') + number;
	string data = "logs/doctor/doctor" + number + ".txt";
	this->logfile.open(data);
	this->qs = new QueueStrategy(f);
}

Doctor::~Doctor() {
	this->logfile.close();
	delete this->qs;
}

int Doctor::getFastestWay(int emergencyEventNumber, int travelTime) {
	int coveredRoute = emergencyListPtr->getEmergencyStartTimeAt(emergencyEventNumber) - SimTime::getTime();
	int diff = travelTime - coveredRoute;

	int districtEmergency = this->emergencyListPtr->getEmergencyDistrictAt(emergencyEventNumber);
	int fromOldPos = this->simDataPtr->getDistance(this->currPosition, districtEmergency);
	int fromStation = this->simDataPtr->getDistance(0, districtEmergency);
	if (fromOldPos+coveredRoute < fromStation+diff) {
		return SimRandom::getActualTravelTime(fromOldPos) + 2*coveredRoute;
	}
	else {
		return SimRandom::getActualTravelTime(fromStation) + travelTime;
	}
}

void Doctor::doNextMove() {
	//Nächstes Event Abholen
	int nextEventNumber = qs->getNextEmergency(this->currPosition);
	if (nextEventNumber == -1){
		//Keine Notrufe mehr vorhanden
		int time = SimTime::getTime();
		SimTime::setTime(this->simDataPtr->getSimDuration()+1);
		this->timeAtStation += SimTime::getTime() - time;
		return;
	}

	//Zustand aktualisieren:
	switch (this->moveState) {
	case IS_AT_STATION: {
		//Arzt ist in der Zentrale; Setze Position auf Zentralbezirk; Füge Fahrtzeit der Simulationsuhr hinzu
		int waitTimeAtStation = 0;
		if (emergencyListPtr->getEmergencyStatusAt(nextEventNumber) == Emergency::NOT_SET) waitTimeAtStation = emergencyListPtr->getEmergencyStartTimeAt(nextEventNumber) - SimTime::getTime();
		SimTime::addTime(waitTimeAtStation);
		this->timeAtStation += waitTimeAtStation;
		this->activeEmergency = nextEventNumber;
		this->destPosition = emergencyListPtr->getEmergencyDistrictAt(nextEventNumber);
		this->moveState = MOVE_TO_PATIENT;
		break;
	}
	case MOVE_TO_PATIENT: {
		//Arzt fährt zum Patienten
		this->logfile << "-----------------------------" << endl;
		this->logfile << "Moving To Patient" << endl;
		this->logfile << "-----------------------------" << endl;
		this->logfile << "Time: " << SimTime::getTime() << endl;
		this->logfile << "Emergency Number: " << this->activeEmergency << endl;
		this->logfile << "Moving: " << this->currPosition << "-->" << this->destPosition << endl;
		int traveltime = this->genTravelTime(this->currPosition, this->destPosition);
		this->logfile << "Duration: " << traveltime << endl;
		SimTime::addTime(traveltime);
		this->emergencyListPtr->setEmergencyDoctorArrivalTimeAt(this->activeEmergency, SimTime::getTime());
		this->moveState = HELP_PATIENT;
		break;
	}
	case HELP_PATIENT: {
		//Patient wird versorgt; Setze neue Position; Versorgungsdauer der Simulationsuhr hinzufügen
		this->setCurrPosition(this->getDestPosition());
		SimTime::addTime(emergencyListPtr->getEmergencyCareDurationAt(this->activeEmergency));
		this->emergencyListPtr->setEmergencyStatusAt(this->activeEmergency, Emergency::COMPLETED);
		this->emergencyListPtr->setEmergencyCompletionTimeAt(this->activeEmergency, SimTime::getTime());
		this->destPosition = this->emergencyListPtr->getEmergencyDistrictAt(activeEmergency);
		this->logfile << "-----------------------------" << endl;
		this->logfile << "Completed Emergency" << endl;
		this->logfile << "-----------------------------" << endl;
		this->logfile << "Time: " << SimTime::getTime() << endl;
		this->logfile << "Completed Emergency Number: " << this->activeEmergency << endl;
		int starttime = this->emergencyListPtr->getEmergencyStartTimeAt(activeEmergency);
		this->logfile << "Start Time: " << starttime << endl;
		this->logfile << "Completion Time: " << SimTime::getTime() - starttime << endl;
		this->moveState = CALL_STATION;
		break;
	}
	case CALL_STATION: {
		//Zentral anrufen; den naechsten Notruf abholen; Fahrzeit zum nächsten Notruf der Simulationsuhr hinzufügen
		if (emergencyListPtr->getEmergencyStatusAt(nextEventNumber)==Emergency::QUEUED_WAITING){
			this->setDestPosition(emergencyListPtr->getEmergencyDistrictAt(nextEventNumber));
			this->activeEmergency = nextEventNumber;
			this->moveState = MOVE_TO_PATIENT;
		}
		else {
			//Wenn Ereignisliste leer, fahre zur Zentrale
			this->moveState = MOVE_TO_STATION;
		}
		break;
	}
	case MOVE_TO_STATION: {
		//Auf Fahrt zur Zentrale wird überprüft, ob während der Fahrt ein Notruf eingeht
		int travelTime = this->genTravelTime(this->currPosition, 0);
		if (emergencyListPtr->getEmergencyStartTimeAt(nextEventNumber) < (SimTime::getTime() + travelTime)) {
			//Finde die theoretisch schnellere Route
			travelTime = getFastestWay(nextEventNumber, travelTime);
			activeEmergency = nextEventNumber;
			this->destPosition = this->emergencyListPtr->getEmergencyDistrictAt(activeEmergency);
			this->logfile << "-----------------------------" << endl;
			this->logfile << "Moving To Patient" << endl;
			this->logfile << "-----------------------------" << endl;
			this->logfile << "Time: " << SimTime::getTime() << endl;
			this->logfile << "Emergency Number: " << this->activeEmergency << endl;
			this->logfile << "Moving: " << this->currPosition << "-->" << this->destPosition << endl;
			this->logfile << "Duration: " << travelTime << endl;
			SimTime::addTime(travelTime);
			this->emergencyListPtr->setEmergencyDoctorArrivalTimeAt(this->activeEmergency, SimTime::getTime());
			this->moveState = HELP_PATIENT;
		}
		else{
			this->destPosition = this->emergencyListPtr->getEmergencyDistrictAt(activeEmergency);
			this->logfile << "-----------------------------" << endl;
			this->logfile << "Moving To Station" << endl;
			this->logfile << "-----------------------------" << endl;
			this->logfile << "Time: " << SimTime::getTime() << endl;
			this->logfile << "Moving: " << this->currPosition << "-->" << "0" << endl;
			this->logfile << "Duration: " << travelTime << endl;
			SimTime::addTime(travelTime);
			this->moveState = IS_AT_STATION;
			this->setCurrPosition(0);
		}
		break;
	}
	case INVALID_MOVE_STATE:
	default: {
		throw exception("Invalid Doctor MoveState!");
		break;
		}
	}
}