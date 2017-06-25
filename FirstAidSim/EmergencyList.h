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
	inline int getEmergencyListSize() const;


	inline int getEmergencyStartTimeAt(const int index) const;
	inline int getEmergencyDistrictAt(const int index) const;
	inline int getEmergencyCareDurationAt(const int index) const;
	inline bool getEmergencyUrgentAt(const int index) const;
	inline int getEmergencyDoctorArrivalTimeAt(const int index) const;
	inline int getEmergencyCompletionTimeAt(const int index) const;
	inline Emergency::Status getEmergencyStatusAt(const int index) const;

	inline void setEmergencyStatusAt(const int index, const Emergency::Status status);
	inline void setEmergencyDoctorArrivalTimeAt(const int index, const int time);
	inline void setEmergencyCompletionTimeAt(const int index, const int time);

	void resetEmergencyList();

private:
	EmergencyList();
	//Singleton Instanzpointer
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

inline int EmergencyList::getEmergencyListSize() const{
	return this->emergencyList.size();
}

inline int EmergencyList::getEmergencyStartTimeAt(const int index) const{
	return emergencyList.at(index).getStartTime();
}

inline int EmergencyList::getEmergencyDistrictAt(const int index) const{
	return emergencyList.at(index).getDistrict();
}

inline int EmergencyList::getEmergencyCareDurationAt(const int index) const{
	return emergencyList.at(index).getCareDurationTime();
}

inline bool EmergencyList::getEmergencyUrgentAt(const int index) const{
	return emergencyList.at(index).getUrgent();
}

inline void EmergencyList::setEmergencyStatusAt(const int index, const Emergency::Status status){
	this->emergencyList.at(index).setStatus(status);
}

inline int EmergencyList::getEmergencyDoctorArrivalTimeAt(const int index) const{
	return this->emergencyList.at(index).getDoctorArrivalTime();
}
inline int EmergencyList::getEmergencyCompletionTimeAt(const int index) const{
	return this->emergencyList.at(index).getCompletionTime();
}
inline Emergency::Status EmergencyList::getEmergencyStatusAt(const int index) const{
	return this->emergencyList.at(index).getStatus();
}
inline void EmergencyList::setEmergencyDoctorArrivalTimeAt(const int index, const int time){
	this->emergencyList.at(index).setDoctorArrivalTime(time);
}
inline void EmergencyList::setEmergencyCompletionTimeAt(const int index, const int time){
	this->emergencyList.at(index).setCompletionTime(time);
}

#endif