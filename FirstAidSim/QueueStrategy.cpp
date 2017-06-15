#include "QueueStrategy.h"

int FIFOStrategy(int district){
	EmergencyList* EmergencyListPtr = EmergencyList::getInstance();
	int numEmergencies = EmergencyListPtr->getEmergencyListSize();
	int index = -1;
	for (int i = 0; i < numEmergencies; i++){
		Emergency tmp_e = EmergencyListPtr->getEmergency(i);
		if (tmp_e.getStatus() == Emergency::QUEUED_WAITING){
			if (index == -1) index = i;
			if (tmp_e.getUrgent()) return i;
		}
	}
	return index;
}

int DistrictStrategy(int district){
	EmergencyList* EmergencyListPtr = EmergencyList::getInstance();
	int numEmergencies = EmergencyListPtr->getEmergencyListSize();
	int index = -1;
	bool noDistrictMatch = true;
	for (int i = 0; i < numEmergencies; i++){
		Emergency tmp_e = EmergencyListPtr->getEmergency(i);
		if (tmp_e.getStatus() == Emergency::QUEUED_WAITING){
			if (index == -1) index = i;
			if (noDistrictMatch && tmp_e.getDistrict() == district){
				index = i;
				noDistrictMatch = false;
			}
			if (tmp_e.getUrgent()) return i;
		}
	}
	return index;
}