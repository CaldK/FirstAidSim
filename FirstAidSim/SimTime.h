#ifndef SimTime_h
#define SimTime_h

#include "EmergencyList.h"

class SimTime{
public:
	static inline int getTime();
	static inline void addTime(int time);
	static inline void setTime(const int time);

private:
	static int time;
	static inline void queueEmergencies();
};

inline int SimTime::getTime(){
	return time;
}

inline void SimTime::addTime(int time){
	SimTime::time += time;
	SimTime::queueEmergencies();
}

inline void SimTime::setTime(const int time){
	SimTime::time = time;
}

inline void SimTime::queueEmergencies(){
	EmergencyList* EmergencyListPtr = EmergencyList::getInstance();
	int numEmergencies = EmergencyListPtr->getEmergencyListSize();
	for (int i = 0; i < numEmergencies; i++){
		if (EmergencyListPtr->getEmergencyStartTimeAt(i) <= SimTime::time){
			if(EmergencyListPtr->getEmergency(i).getStatus() == Emergency::NOT_SET) EmergencyListPtr->setEmergencyStatusAt(i, Emergency::QUEUED_WAITING);
		}
	}
}

#endif SimTime_h