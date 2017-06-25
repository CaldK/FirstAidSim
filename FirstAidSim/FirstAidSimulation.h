#ifndef FirstAidSimulation_h
#define FirstAidSimulation_h


#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include "SimData.h"
#include "SimTime.h"
#include "QueueStrategy.h"
#include "EmergencyList.h"
#include "Doctor.h"


using namespace std;

class FirstAidSimulation{
public:
	FirstAidSimulation();
	virtual ~FirstAidSimulation();

	void resetSimulation(int run);
	void runSimulation();
	inline void setSimulationDataFile(const string& simulationDataFile);
	void loadSimulationDataFromFile();
	void parseLine(const string& currLine, int& population, vector<int>& distance) const;
	void generateEmergencies(int run);
	void calculateKeyFigures();
	void outputKeyFigures(int run);
	void calculateAndOutputCompleteAnalysis();

	ofstream keyfiguresfile;
	QueueStrategy qs = QueueStrategy(DistrictStrategy);

	static const int DEBUG_MODE = 1;
	static int NUM_OF_RUNS;
	static int SIM_DURATION;
	static function<int(int)> STRATEGY;
	static string STRATEGY_STRING;


private:
	string simulationDataFile;
	SimData* SimDataPtr;
	SimTime* SimTimePtr;
	EmergencyList* EmergencyListPtr;
	Doctor* doctorPtr;
	

	vector<double> avgWaitTimes;
	vector<double> avgCompletionTimes;
	vector<double> avgUrgentWaitTimes;
	vector<double> avgUrgentCompletionTimes;
	vector<double> doctorWorkload;
};

inline void FirstAidSimulation::setSimulationDataFile(const string& simulationDataFile){
	this->simulationDataFile = simulationDataFile;
}


#endif