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
	//Parameter: population: Vektor Anzahl der Bewohner geordnet nach Stadtbezirken
	//Konfiguriert die Diskrete Verteilung
	inline static void setUrgentRatio(double ratio);
	//Parameter: ratio: Verhältnis von lebensbedrohlichen Notfällen
	//Konfiguriert die diskrete Gleichverteilung zur bestimmung von lebensbedrohlichen Notfällen

	inline static int getNextEmergencyTime();
	//Return: Gibt die Zeit bis zum nächsten Notfall an
	//Diese Zahl bleibt für den Arzt unbekannt.
	inline static int getNextEmergencyDistrict();
	//Return: Gibt den Stadtbezirk des nächsten Notfalls aus
	inline static int getActualTravelTime(int time);
	//Parameter: time: Die millere Fahrtzeit
	//Return: Die tatsächliche Fahrtzeit im Intervall [time-50%;time+50%]
	inline static int getCareDuration(bool urgentEmergency);
	//Parameter: urgentEmergency: handelt es sich um ein lebensbedrohlicher Notfall?
	//Return: Die Behandlungszeit für den Arzt am Notfallort
	//        Bei lebensbedrohlichen Notfällen im Intervall [30;90]
	//        Bei normalen Notfällen im Intervall [10;20]
	inline static bool isEmergencyUrgent();
	//Return: Handelt es sich um ein lebensbedrohlicher Notfall?
	

private:
	static int seed;
	//Saat für die Zufallszahlengenertoren
	static double urgentRatio;
	//Verhältniss von lebensbedrohlichen Notfällen
	static mt19937 generator;
	//Der Pseudozufallszahlengenerator Mersenne-Twister
	//Mersenne-Twister: extrem lange Periodendauer; schneller Algorithmus; Bits der Ausgebesequenz sind gleichverteilt
	static exponential_distribution<double> exp_distribution;
	//Exponitionelle Verteilung; Benötigt für das randomiserte Auftretten der Notfälle
	static discrete_distribution<int> disc_distribution;
	//Diskrete Verteilung; Benötigt für die Verteilung der Notfälle auf die Stadtbezirke entsprechend der Bevölkerungszahl
	static uniform_int_distribution<int> urgent_uni_distribution;
	//Diskrete Gleichverteilung; Benötigt für die Versorgungsdauer der lebensbedrohlichen Notfälle
	static uniform_int_distribution<int> nonUrgent_uni_distribution;
	//Diskrete Gleichverteilung; Benötigt für die Versorgungsdauer der normalen Notfälle
	static map<int, uniform_int_distribution<int>> map_dist_distribution;
	//Sammulung von Gleichverteilung; Benötigt für tatsächliche Fahrtzeiten
	static uniform_real_distribution<double> real_distribution;
	//Stetige Gleichverteilung; Benötigt für die Unterscheidung normale und lebensbedrohliche Notfälle

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