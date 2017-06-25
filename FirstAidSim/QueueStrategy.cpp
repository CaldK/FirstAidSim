#include "QueueStrategy.h"
#include "SimTime.h"
#include <iostream>

int FIFOStrategy(int district){
	//FIFO Strategie; Finde den der als erstes in der Liste erscheint mit Zustand QUEUED_WAITING
	//Lebensbedrohliche Notrufe haben Priorität; Prüfe restliche Liste noch Notrufen mit Zustand QUEUED_WAITING und URGENT
	//Eingabeparameter: Aktuelle Position des Arztes
	//Return: Index vom Event
	EmergencyList* EmergencyListPtr = EmergencyList::getInstance();
	int numEmergencies = EmergencyListPtr->getEmergencyListSize();
	int index = -1;
	int firstNotSet = -1;
	for (int i = 0; i < numEmergencies; i++){
		Emergency tmp_e = EmergencyListPtr->getEmergency(i);
		if (firstNotSet == -1 && tmp_e.getStatus() == Emergency::NOT_SET) firstNotSet = i;
		if (tmp_e.getStatus() == Emergency::QUEUED_WAITING){
			if (index == -1) index = i;
			if (tmp_e.getUrgent()) return i;
		}
	}
	if (index == -1) index = firstNotSet;
	return index;
}

int DistrictStrategy(int district){
	//Stadtbezirk Strategie; Finde das Event, der als erstes in der Liste erscheint mit Zustand QUEUED_WAITING
	//Notrufe im selben Stadtbezirk, in der sich der Arzt befindet haben Priorität:
	//Prüfe restliche Liste; Finde das erste Event, in Stadtbezirk district it Zustand QUEUD_WAITING
	//Lebensbedrohliche Notrufe haben Priorität; Prüfe restliche Liste noch Notrufen mit Zustand QUEUED_WAITING und URGENT
	//Eingabeparameter: Aktuelle Position des Arztes
	//Return: Index vom Event
	EmergencyList* EmergencyListPtr = EmergencyList::getInstance();
	int numEmergencies = EmergencyListPtr->getEmergencyListSize();
	int index = -1;
	int firstNotSet = -1;
	bool noDistrictMatch = true;
	for (int i = 0; i < numEmergencies; i++){
		Emergency tmp_e = EmergencyListPtr->getEmergency(i);
		if (firstNotSet == -1 && tmp_e.getStatus() == Emergency::NOT_SET) firstNotSet = i;
		if (tmp_e.getStatus() == Emergency::QUEUED_WAITING){
			if (index == -1) index = i;
			if (noDistrictMatch && tmp_e.getDistrict() == district){
				index = i;
				noDistrictMatch = false;
			}
			if (tmp_e.getUrgent()) return i;
		}
	}
	if (index == -1) index = firstNotSet;
	return index;
}