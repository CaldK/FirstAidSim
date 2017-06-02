#ifndef SimData_h
#define SimData_h

#include <vector>

using namespace std;

class SimData{
public:
	virtual ~SimData();
	inline static SimData* getInstance();

	inline int getPopulation(int district) const;
	inline vector<int> getPopulationList() const;
	inline int getDistance(int start, int end) const;
	inline int getSimDuration() const;

	inline void addPopulation(const int population);
	inline void addDistance(const vector<int> distance);
	inline void setSimDuration(const int simDuration);

private:
	SimData();
	static SimData* instancePtr;
	int numDistricts;
	int totalPopulation;
	int simDuration;
	vector<int> population;
	vector<vector<int>> distanceMatrix;
};

inline SimData* SimData::getInstance(){
	if (!instancePtr){
		instancePtr = new SimData;
	}
	return instancePtr;
}

inline int SimData::getPopulation(int district) const{
	int population;
	if (district < this->numDistricts){
		population = this->population[district];
	} 
	else population = -1;
	return population;
}

inline vector<int> SimData::getPopulationList() const{
	return this->population;
}

inline int SimData::getDistance(int start, int end) const{
	int distance;
	if (start < this->numDistricts && end < this->numDistricts){
		distance = this->distanceMatrix[start][end];
	}
	else distance = -1;
	return distance;
}

inline int SimData::getSimDuration() const{
	return this->simDuration;
}

inline void SimData::addPopulation(const int population){
	this->population.push_back(population);
	this->totalPopulation += population;
	this->numDistricts++;
}

inline void SimData::addDistance(const vector<int> distance){
	this->distanceMatrix.push_back(distance);
}

inline void SimData::setSimDuration(const int simDuration){
	this->simDuration = simDuration;
}


#endif