#ifndef SimRandom_h
#define SimRandom_h

#include <random>
#include <chrono>
#include <thread>
#include <map>
#include <math.h> 

using namespace std;

class SimRandom{
public:
	inline static void setUpDiscreteDist(vector<int> population);
	inline static void setUrgentRatio(double ratio);

	inline static int getNextEmergencyTime();
	inline static int getNextEmergencyDistrict();
	inline static int getActualTravelTime(int time);
	inline static int getCareDuration(bool urgentEmergency);
	inline static bool isEmergencyUrgent();
	

private:
	static int seed;
	static double urgentRatio;
	static mt19937 generator;
	static exponential_distribution<double> exp_distribution;
	static discrete_distribution<int> disc_distribution;
	static uniform_int_distribution<int> urgent_uni_distribution;
	static uniform_int_distribution<int> nonUrgent_uni_distribution;
	static map<int, uniform_int_distribution<int>> map_dist_distribution;
	static uniform_real_distribution<double> real_distribution;

};

inline void SimRandom::setUpDiscreteDist(vector<int> population){
	int i = 0;
	disc_distribution = discrete_distribution<int>(population.size(),
		0.0, // dummy!
		1.0, // dummy!
		[&population, &i](double)
	{
		return population[i++];
	});
}

inline void SimRandom::setUrgentRatio(double ratio){
	urgentRatio = ratio;
}

inline int SimRandom::getNextEmergencyTime(){ 
	return exp_distribution(generator);
}

inline int SimRandom::getNextEmergencyDistrict(){
	return disc_distribution(generator);
}

inline int SimRandom::getActualTravelTime(int time){
	int rand;
	map<int, uniform_int_distribution<int>>::iterator it = map_dist_distribution.find(time);
	if (it == map_dist_distribution.end()){
		map_dist_distribution[time] = uniform_int_distribution<int>(ceil(time*0.5), ceil(time*1.5));
		rand = map_dist_distribution[time](generator);
	}
	else {
		rand = map_dist_distribution[time](generator);
	}
	return rand;
}

inline int SimRandom::getCareDuration(bool urgentEmergency){
	int rand;
	if (urgentEmergency){
		rand = urgent_uni_distribution(generator);
	}
	else{
		rand = nonUrgent_uni_distribution(generator);
	}
	return rand;
}

inline bool SimRandom::isEmergencyUrgent(){
	bool isUrgent;
	if (real_distribution(generator) < urgentRatio) isUrgent = true; 
	else isUrgent = false;
	return isUrgent;
}



#endif