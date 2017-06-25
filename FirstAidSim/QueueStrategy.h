#ifndef QueueStrategy_h
#define QueueStrategy_h

#include "EmergencyList.h"
#include <functional>

using namespace std;

class QueueStrategy{
public:
	template<typename F>
	QueueStrategy(F strategy) : strategy(move(strategy)) { }
	inline int getNextEmergency(int district);
private:
	EmergencyList* EmergencyListPtr;
	function<int(int)> strategy;
};

int FIFOStrategy(int district);
int DistrictStrategy(int district);


inline int QueueStrategy::getNextEmergency(int district) { return strategy(district); }

#endif