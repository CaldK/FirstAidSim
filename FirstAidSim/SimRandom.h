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
	mt19937 generator(seed);
	exponential_distribution<double> distribution(1 / 50.f);
	return distribution(generator);
}

inline int SimRandom::getNextEmergencyDistrict(vector<int> population){
	int i = 0;
	auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
	mt19937 generator(seed);
	discrete_distribution<int> distribution(population.size(),
		0.0, // dummy!
		1.0, // dummy!
		[&population, &i](double)
	{
		return population[i++];
	});
	return distribution(generator);
}

inline int SimRandom::getActualTravelTime(int time){
	auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
	mt19937 generator(seed);
	uniform_int_distribution<int> distribution(time*0.5, time*1.5);
	return distribution(generator);
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
	mt19937 generator(seed);
	uniform_int_distribution<int>distribution(a, b);
	return distribution(generator);
}



#endif