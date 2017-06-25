#pragma warning(disable:4996)
#include "FirstAidSimulation.h"
#include "SimRandom.h"
#include "SimTime.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include <map>

int FirstAidSimulation::SIM_DURATION = 10080;
int FirstAidSimulation::NUM_OF_RUNS = 10;
function<int(int)> FirstAidSimulation::STRATEGY = FIFOStrategy;
string FirstAidSimulation::STRATEGY_STRING = "FIFO";

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
	char ** itr = std::find(begin, end, option);
	if (itr != end && ++itr != end)
	{
		return *itr;
	}
	return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
	return std::find(begin, end, option) != end;
}

std::string exec(const char* cmd) {
	std::array<char, 128> buffer;
	std::string result;
	std::shared_ptr<FILE> pipe(_popen(cmd, "r"), _pclose);
	if (!pipe) throw std::runtime_error("popen() failed!");
	while (!feof(pipe.get())) {
		if (fgets(buffer.data(), 128, pipe.get()) != NULL)
			result += buffer.data();
	}
	return result;
}

int main(int argc, char * argv[]){
	try {
		FirstAidSimulation simulation;

		char * filename = getCmdOption(argv, argv + argc, "-f");
		if(filename) simulation.setSimulationDataFile(filename);
		else simulation.setSimulationDataFile("data/file.csv");

		char * numberofruns = getCmdOption(argv, argv + argc, "-n");
		if (numberofruns) FirstAidSimulation::NUM_OF_RUNS = atoi(numberofruns);

		char * strategy = getCmdOption(argv, argv + argc, "-s");
		if (strategy){
			if (strcmp(strategy, "fifo") == 0){
				FirstAidSimulation::STRATEGY = FIFOStrategy;
				FirstAidSimulation::STRATEGY_STRING = "FIFO";
			}
			if (strcmp(strategy, "district") == 0){
				FirstAidSimulation::STRATEGY = DistrictStrategy;
				FirstAidSimulation::STRATEGY_STRING = "District";
			}
		}

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

	//exec("pause");

	return 0;
}


FirstAidSimulation::FirstAidSimulation(){
	//Konstruktor
	this->simulationDataFile = "";
	this->SimDataPtr = SimData::getInstance();
	this->EmergencyListPtr = EmergencyList::getInstance();
	this->doctorPtr = new Doctor(STRATEGY,0);
	this->keyfiguresfile.open("keyfigures.dat");
	//time_t t = time(NULL);
	//keyfiguresfile << "##Date: " << put_time(localtime(&t),"%F-%T") << endl;
}

FirstAidSimulation::~FirstAidSimulation(){
	this->keyfiguresfile.close();
}

void FirstAidSimulation::resetSimulation(int run){
	this->EmergencyListPtr->resetEmergencyList();
	this->SimTimePtr->setTime(0);
	delete this->doctorPtr;
	this->doctorPtr = new Doctor(STRATEGY,run+1);
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
		getline(currFile, currLine); //Interarrival Time: Erwartungswert für Zeit zwischen den Notrufen (Exponentialverteilung, Angabe der mittleren Zwischenankunftszeit in Minuten)
		getline(currFile, currLine);
		this->SimDataPtr->setInterarrivalTime(atof(currLine.c_str()));
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

void FirstAidSimulation::generateEmergencies(int run){
	int time = 0;
	int index = 0;
	ofstream myfile;
	string number = to_string(run);
	number = string(3 - number.length(), '0') + number;
	string data = "logs/lists/list" + number + ".txt";
	myfile.open(data);
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

void FirstAidSimulation::calculateKeyFigures(){
	double averageCompletionTime = 0;
	double averageWaitTime = 0;
	double averageUrgentCompletionTime = 0;
	double averageUrgentWaitTime = 0;
	double doctorWorkload = 0;
	int currentTimeOfCompletion = 0;
	int currentWaitTime = 0;
	int currentCompletionTime = 0;
	int currentStartTime = 0;
	int currentArrivalTime = 0;
	int cumulativeWaitTime = 0;
	int cumulativeCompletionTime = 0;
	int cumulativeUrgentWaitTime = 0;
	int cumulativeUrgentCompletionTime = 0;
	int numNormalEmergencies = 0;
	int numUrgentEmergencies = 0;
	

	int numEmergencies = this->EmergencyListPtr->getEmergencyListSize();

	for (int index = 0; index < numEmergencies; ++index){
		if (this->EmergencyListPtr->getEmergencyStatusAt(index) == Emergency::COMPLETED){
			currentStartTime = this->EmergencyListPtr->getEmergencyStartTimeAt(index);
			currentArrivalTime = this->EmergencyListPtr->getEmergencyDoctorArrivalTimeAt(index);
			currentTimeOfCompletion = this->EmergencyListPtr->getEmergencyCompletionTimeAt(index);

			currentWaitTime = currentArrivalTime - currentStartTime;
			if (currentWaitTime < 0){
				throw exception("Wait Time negative!");
			}
			currentCompletionTime = currentTimeOfCompletion - currentStartTime;
			if (currentCompletionTime < 0){
				throw exception("Completion Time negative!");
			}

			if (this->EmergencyListPtr->getEmergencyUrgentAt(index)){
				cumulativeUrgentWaitTime += currentWaitTime;
				cumulativeUrgentCompletionTime += currentCompletionTime;
				numUrgentEmergencies++;
			}
			else{
				cumulativeWaitTime += currentWaitTime;
				cumulativeCompletionTime += currentCompletionTime;
				numNormalEmergencies++;
			}
		}
	}

	averageWaitTime = (double)cumulativeWaitTime / (double)numNormalEmergencies;
	averageCompletionTime = (double)cumulativeCompletionTime / (double)numNormalEmergencies;
	averageUrgentWaitTime = (double)cumulativeUrgentWaitTime / (double)numUrgentEmergencies;
	averageUrgentCompletionTime = (double)cumulativeUrgentCompletionTime / (double)numUrgentEmergencies;
	doctorWorkload = 1 - ((double)this->doctorPtr->getTimeAtStation() / (double)SIM_DURATION);

	averageWaitTime = averageWaitTime > 0 ? averageWaitTime : 0;
	averageCompletionTime = averageCompletionTime > 0 ? averageCompletionTime : 0;
	averageUrgentWaitTime = averageUrgentWaitTime > 0 ? averageUrgentWaitTime : 0;
	averageUrgentCompletionTime = averageUrgentCompletionTime > 0 ? averageUrgentCompletionTime : 0;
	doctorWorkload = doctorWorkload > 0 ? doctorWorkload : 0;

	this->avgWaitTimes.push_back(averageWaitTime);
	this->avgCompletionTimes.push_back(averageCompletionTime);
	this->avgUrgentWaitTimes.push_back(averageUrgentWaitTime);
	this->avgUrgentCompletionTimes.push_back(averageUrgentCompletionTime);
	this->doctorWorkload.push_back(doctorWorkload);
}

void FirstAidSimulation::outputKeyFigures(int run){
	if (this->avgWaitTimes.size() != this->avgCompletionTimes.size()){
		throw exception("Mismatched sizes on wait and completion times!");
	}
	if (this->avgUrgentWaitTimes.size() != this->avgUrgentCompletionTimes.size()){
		throw exception("Mismatched sizes on wait and completion times!");
	}

	this->keyfiguresfile << run << "  " << this->avgWaitTimes[run] << " " << this->avgCompletionTimes[run] << " ";
	this->keyfiguresfile << this->avgUrgentWaitTimes[run] << " " << this->avgUrgentCompletionTimes[run] << " " << this->doctorWorkload[run] << endl;

	if (run < 10){
		cout << "-----------------------------------" << endl;
		cout << "Results for run: " << run << endl;
		cout << "Average Emergency Waiting Time is: ";
		cout << this->avgWaitTimes[run] << " minutes" << endl;
		cout << "Average Emergency Completion Time is: ";
		cout << this->avgCompletionTimes[run] << " minutes" << endl;
		cout << "Average Urgent Emergency Waiting Time is: ";
		cout << this->avgUrgentWaitTimes[run] << " minutes" << endl;
		cout << "Average Urgent Emergency Completion Time is: ";
		cout << this->avgUrgentCompletionTimes[run] << " minutes" << endl;
		cout << "Doctor Workload: " << this->doctorWorkload[run] * 100 << " %" << endl;
		if (run == 9) cout << endl << "More runs in progress.." << endl;
	}
	else{
		float progress = (float)run/(float)NUM_OF_RUNS;
		int barWidth = 50;

		std::cout << "[";
		int pos = barWidth * progress;
		for (int i = 0; i < barWidth; ++i) {
			if (i < pos) std::cout << "=";
			else if (i == pos) std::cout << ">";
			else std::cout << " ";
		}
		std::cout << "] " << int(progress * 100.0) << "% " << run << "/" << NUM_OF_RUNS << "\r";
		std::cout.flush();
	}
	
}

void FirstAidSimulation::calculateAndOutputCompleteAnalysis(){
	double sumAvgWaitTime = 0;
	double sumAvgCompletionTime = 0;
	double sumAvgUrgentWaitTime = 0;
	double sumAvgUrgentCompletionTime = 0;
	double sumAvgDoctorWorkload = 0;
	for (int i = 0; i < NUM_OF_RUNS; i++){
		sumAvgWaitTime += this->avgWaitTimes[i];
		sumAvgCompletionTime += this->avgCompletionTimes[i];
		sumAvgUrgentWaitTime += this->avgUrgentWaitTimes[i];
		sumAvgUrgentCompletionTime += this->avgUrgentCompletionTimes[i];
		sumAvgDoctorWorkload += this->doctorWorkload[i];
	}


	cout << "[=========================================================] 100%" << endl;
	cout << "Complete Analysis using " << FirstAidSimulation::STRATEGY_STRING << " Strategy" << endl;
	cout << "---------------------------------" << endl;
	cout << "Results for all runs" << endl;
	cout << "Average Emergency Waiting Time is: ";
	cout << sumAvgWaitTime / (double) NUM_OF_RUNS << " minutes" << endl;
	cout << "Average Emergency Completion Time is: ";
	cout << sumAvgCompletionTime / (double) NUM_OF_RUNS << " minutes" << endl;
	cout << "Average Urgent Emergency Waiting Time is: ";
	cout << sumAvgUrgentWaitTime / (double) NUM_OF_RUNS << " minutes" << endl;
	cout << "Average Urgent Emergency Completion Time is: ";
	cout << sumAvgUrgentCompletionTime / (double) NUM_OF_RUNS << " minutes" << endl;
	cout << "Doctor Workload: " << sumAvgDoctorWorkload / (double) NUM_OF_RUNS * 100 << " %" << endl;
}

void FirstAidSimulation::runSimulation(){

	cout << "Running simulations..." << endl;

	//Einlesen und Verarbeiten der Eingabedaten(Dauer Simulation, Urgent Ratio, Einwohner, Distanzmatrix):
	this->loadSimulationDataFromFile();
	SimRandom::setUpDiscreteDist(this->SimDataPtr->getPopulationList());
	SIM_DURATION = this->SimDataPtr->getSimDuration();
	SimRandom::setExpDistribution(this->SimDataPtr->getInterarrivalTime());

	//Durchlaufen der Simulationsexperimente
	for (int i = 0; i < NUM_OF_RUNS; i++){
		//Generieren der Eventliste.
		generateEmergencies(i);
		
		while (true){
			this->doctorPtr->doNextMove();
			if (this->SimTimePtr->getTime() > SIM_DURATION) break;
		}
		calculateKeyFigures();
		outputKeyFigures(i);
		
		//Simulation für nächste Wiederholung neuinitialisieren
		this->resetSimulation(i);
	}

	calculateAndOutputCompleteAnalysis();
	exec("gnuplot -p firstaid.gnu");

}