#include <iostream>
#include "Doctor.h"

Doctor::Doctor() {
	this->setMoveState(Doctor::IS_AT_STATION);
	this->simDataPtr = SimData::getInstance();
	this->emergencyListPtr = EmergencyList::getInstance();
}

Doctor::~Doctor() {

}

int Doctor::getFastestWay(int emergencyEventNumber, int travelTime) {

	int diff = travelTime - (emergencyListPtr->getEmergencyStartTimeAt(emergencyEventNumber)-SimTime::getTime());

	int fromOldPos = this->genTravelTime(emergencyEventNumber, this->getCurrPosition());
	int fromStation = this->genTravelTime(emergencyEventNumber, 0);
	if (fromOldPos < fromStation) {
		return fromOldPos + diff;
	}
	else {
		return fromStation + diff;
	}
}

void Doctor::doNextMove() {
	int nextEventNumber = qs.getNextEmergency(this->currPosition);
	switch (this->moveState) {
	case IS_AT_STATION: {
		this->moveState = MOVE_TO_PATIENT;
		this->setCurrPosition(1);
		SimTime::addTime(this->genTravelTime(nextEventNumber,this->getCurrPosition()));
		break;
	}
	case MOVE_TO_PATIENT: {
		this->moveState = HELP_PATIENT;
		break;
	}
	case HELP_PATIENT: {
		this->moveState = CALL_STATION;
		this->setCurrPosition(this->getDestPosition());
		SimTime::addTime(emergencyListPtr->getEmergencyCareDurationAt(nextEventNumber));
		break;
	}
	case CALL_STATION: {
		if (emergencyListPtr->getEmergencyStatusAt(nextEventNumber)!=Emergency::NOT_SET) {
			this->moveState = MOVE_TO_PATIENT;
			this->setDestPosition(emergencyListPtr->getEmergencyDistrictAt(nextEventNumber));
			SimTime::addTime(this->genTravelTime(nextEventNumber, this->getCurrPosition()));
		}
		else {
			this->moveState = MOVE_TO_STATION;
			this->setDestPosition(1);
		}
		break;
	}
	case MOVE_TO_STATION: {
		this->moveState = MOVE_TO_PATIENT;
		//gen travle time
		int travelTime = this->genTravelTime(nextEventNumber, this->getCurrPosition());
		if (emergencyListPtr->getEmergencyStartTimeAt(nextEventNumber) < (SimTime::getTime() + travelTime)) {
			travelTime = getFastestWay(travelTime, nextEventNumber);
		}
		SimTime::addTime(travelTime);
		break;
	}
	case INVALID_MOVE_STATE:
	default: {
		throw exception("Invalid Doctor MoveState!");
		break;
		}
	}
}