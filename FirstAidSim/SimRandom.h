#ifndef SimRandom_h
#define SimRandom_h

#include <random>
#include <chrono>
#include <thread>
#include <functional>
#include <numeric>

using namespace std;

class SimRandom{
public:
	inline static int getNextEmergencyTime();
	inline static int getNextEmergencyDistrict(vector<int> population);
	inline static int getActualTravelTime(int time);
	inline static int getCareDuration(bool urgentEmergency);

private:
	static int seed;
};

inline int SimRandom::getNextEmergencyTime(){
	auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
	auto rand = bind(exponential_distribution<int>(1/50.f), mt19937(seed));
	return rand();
}

inline int SimRandom::getNextEmergencyDistrict(vector<int> population){
	auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
	auto rand = bind(discrete_distribution<int>(population.begin(),population.end()),mt19937(seed));
	return rand();
}

inline int SimRandom::getActualTravelTime(int time){
	auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
	auto rand = bind(uniform_int_distribution<int>(time*0.5,time*1.5), mt19937(seed));
	return rand();
}

inline int SimRandom::getCareDuration(bool urgentEmergency){
	int a, b;
	if (urgentEmergency){
		a = 30; b = 90;
	}
	else{
		a = 10; b = 20;
	}
	auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
	auto rand = bind(uniform_int_distribution<int>(a,b), mt19937(seed));
	return rand();
}



#endif