#include <fstream>
#include "FirstAidSimulation.h"
#include <iostream>
#include <string>

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
}

FirstAidSimulation::~FirstAidSimulation(){
	//Destruktor leer
}

void FirstAidSimulation::loadSimulationDataFromFile(){
	string currLine = "";

	ifstream currFile(this->simulationDataFile.c_str(), ios::in);
	if (currFile){
		getline(currFile, currLine);
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

void FirstAidSimulation::runSimulation(){
	cout << "Running simulations..." << endl;
	string data = "data/file.csv";
	this->setSimulationDataFile(data);
	this->loadSimulationDataFromFile();
}