#ifndef EmergencyList_h
#define EmergencyList_h

#include <vector>
#include "Emergency.h"

using namespace std;

class EmergencyList{
public:
	virtual ~EmergencyList();

	inline static EmergencyList* getInstance();

	inline void addEmergency(const Emergency& Emergency);
	inline Emergency getEmergency(const int index);

private:
	EmergencyList();
	static EmergencyList* instancePtr;
	vector<Emergency> emergencyList;
};

inline EmergencyList* EmergencyList::getInstance(){
	if (!instancePtr){
		instancePtr = new EmergencyList;
	}
	return instancePtr;
}

inline void EmergencyList::addEmergency(const Emergency& Emergency){
	this->emergencyList.push_back(Emergency);
}

inline Emergency EmergencyList::getEmergency(const int index){
	return emergencyList.at(index);
}
#endif