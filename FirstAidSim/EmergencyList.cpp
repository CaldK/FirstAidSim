#include "EmergencyList.h"

EmergencyList *EmergencyList::instancePtr = 0;

EmergencyList::EmergencyList(){
}

EmergencyList::~EmergencyList(){
}

void EmergencyList::resetEmergencyList(){
	this->emergencyList.clear();
}