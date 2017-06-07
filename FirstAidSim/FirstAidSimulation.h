#ifndef FirstAidSimulation_h
#define FirstAidSimulation_h

#include <string>
#include <vector>
#include "SimData.h"
#include "SimTime.h"
#include "EmergencyList.h"

using namespace std;

class FirstAidSimulation{
public:
	FirstAidSimulation();
	virtual ~FirstAidSimulation();

	void resetSimulation();
	void runSimulation();
	inline void setSimulationDataFile(const string& simulationDataFile);
	void loadSimulationDataFromFile();
	void parseLine(const string& currLine, int& population, vector<int>& distance) const;
	void generateEmergencies();
	void queueEmergencies();

	static const int DEBUG_MODE = 1;
	static int SIM_DURATION;


private:
	string simulationDataFile;
	SimData* SimDataPtr;
	SimTime* SimTimePtr;
	EmergencyList* EmergencyListPtr;
};

inline void FirstAidSimulation::setSimulationDataFile(const string& simulationDataFile){
	this->simulationDataFile = simulationDataFile;
}


#endif