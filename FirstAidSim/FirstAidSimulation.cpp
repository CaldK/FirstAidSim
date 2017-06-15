#include <fstream>
#include "FirstAidSimulation.h"
#include "SimRandom.h"
#include "SimTime.h"
#include <iostream>
#include <fstream>
#include <string>

#include <map>

int FirstAidSimulation::SIM_DURATION = 10080;

int main(){
	try {
		FirstAidSimulation simulation;
		simulation.runSimulation();
	}
	catch (const bad_alloc& e){
		cout << e.what() << endl;
		cout << "Caught bad_alloc" << endl;
		cout << "Terminating program" << endl;
	}
	catch (const out_of_range& e){
		cout << e.what() << endl;
		cout << "Caught out_of_range" << endl;
		cout << "Terminating program" << endl;
	}
	catch (const exception& e){
		cout << e.what() << endl;
		cout << "Caught exception" << endl;
		cout << "Terminating program" << endl;
	}

	char pauseBeforeTerminate = ' ';

	cin >> pauseBeforeTerminate;

	return 0;
}

FirstAidSimulation::FirstAidSimulation(){
	//Konstruktor
	this->simulationDataFile = "";
	this->SimDataPtr = SimData::getInstance();
	this->EmergencyListPtr = EmergencyList::getInstance();
}

FirstAidSimulation::~FirstAidSimulation(){
	//Destruktor leer
}

void FirstAidSimulation::loadSimulationDataFromFile(){
	string currLine = "";

	ifstream currFile(this->simulationDataFile.c_str(), ios::in);
	if (currFile){
		getline(currFile, currLine); //Simulation Duration(min):
		getline(currFile, currLine);
		this->SimDataPtr->setSimDuration(atoi(currLine.c_str()));
		getline(currFile, currLine); //Urgent Emergency Ratio(0.0-1.0):
		getline(currFile, currLine);
		SimRandom::setUrgentRatio(atof(currLine.c_str()));
		getline(currFile, currLine); //Population - Travel Time Matrix:
		while (!currFile.eof()){
			int population;
			vector<int> distance;
			getline(currFile, currLine);
			if (currLine != ""){
				this->parseLine(currLine, population, distance);
				this->SimDataPtr->addPopulation(population);
				this->SimDataPtr->addDistance(distance);
			}
		}
	}
	else{
		throw exception("ifstream operation failed!");
	}
}

void FirstAidSimulation::parseLine(const string& currLine, int& population, vector<int>& distance) const{
	string s = currLine;
	string delimiter = " ";
	string populationStr;
	string distanceStr;

	int pos = 0;
	if ((pos = s.find(delimiter)) != string::npos){
		populationStr = s.substr(0, pos);
		population = atoi(populationStr.c_str());
		s.erase(0, pos + delimiter.length());
	}
	while ((pos = s.find(delimiter)) != string::npos) {
		distanceStr = s.substr(0, pos);
		distance.push_back(atoi(distanceStr.c_str()));
		s.erase(0, pos + delimiter.length());
	}
	distanceStr = s;
	distance.push_back(atoi(distanceStr.c_str()));
}

void FirstAidSimulation::generateEmergencies(){
	int time = 0;
	int index = 0;
	ofstream myfile;
	myfile.open("log.txt");
	time += SimRandom::getNextEmergencyTime();
	while (time < SIM_DURATION){
		Emergency emergency;
		emergency.setStartTime(time);
		emergency.setDistrict(SimRandom::getNextEmergencyDistrict());
		if (SimRandom::isEmergencyUrgent()){
			emergency.setUrgent(true);
			int durationTime = SimRandom::getCareDuration(true);
			emergency.setCareDurationTime(durationTime);
		}
		else{
			emergency.setUrgent(false);
			int durationTime = SimRandom::getCareDuration(false);
			emergency.setCareDurationTime(durationTime);
		}
		this->EmergencyListPtr->addEmergency(emergency);
		if (DEBUG_MODE){
			myfile << "-------------------------------" << endl;
			myfile << "Queue Emergency " << index << endl;
			myfile << "-------------------------------" << endl;
			myfile << "Time: " << time << endl;
			myfile << "District: " << this->EmergencyListPtr->getEmergencyDistrictAt(index) << endl;
			myfile << "Urgent: ";
			if (this->EmergencyListPtr->getEmergencyUrgentAt(index)) myfile << "Yes" << endl;
			else myfile << "No" << endl;
			myfile << "Care Duration Time: " << this->EmergencyListPtr->getEmergencyCareDurationAt(index) << endl;
		}
		
		index++;
		time += SimRandom::getNextEmergencyTime();
	}
	myfile.close();
}

void FirstAidSimulation::runSimulation(){
	static const int NUM_OF_RUNS = 1;
	cout << "Running simulations..." << endl;
	string data = "data/file.csv";
	this->setSimulationDataFile(data);
	this->loadSimulationDataFromFile();
	SimRandom::setUpDiscreteDist(this->SimDataPtr->getPopulationList());

	SIM_DURATION = this->SimDataPtr->getSimDuration();

	for (int i = 0; i < NUM_OF_RUNS; i++){
		generateEmergencies();
		QueueStrategy qs(DistrictStrategy);
		ofstream myfile;
		myfile.open("log2.txt");
		int district = 0;
		while (true){
			//DoNextMove
			SimTime::addTime(1);
			if (SimTime::getTime()%50==0){
				myfile << "Time: " << SimTime::getTime() << endl;
				int index = qs.getNextEmergency(district);
				if (index != -1){
					myfile << "Next Emergency: " << index << endl;
					district = EmergencyListPtr->getEmergencyDistrictAt(index);
					myfile << "District: " << district << endl;
					EmergencyListPtr->setEmergencyStatusAt(index, Emergency::COMPLETED);
					if (EmergencyListPtr->getEmergencyUrgentAt(index)) myfile << "URGENT!" << endl;
				}
				else myfile << "No Next Emergency!" << endl;
				myfile << "-------------------------" << endl;
			}
			if (this->SimTimePtr->getTime() > SIM_DURATION) break;
		}
		myfile.close();
	}

}