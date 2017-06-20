#ifndef Doctor_h
#define Doctor_h

#include <list>
#include <memory>
#include "SimTime.h"
#include "EmergencyList.h"
#include "SimData.h"
#include "SimRandom.h"
#include "QueueStrategy.h"

using namespace std;

class GroupController;

class Doctor {
public:
	Doctor();
	virtual ~Doctor();
	enum MoveState;
	void doNextMove();
	void setMoveState(Doctor::MoveState movestate);
	void setCurrPosition(int pos);
	void setDestPosition(int pos);
	inline int getCurrPosition();
	inline int getDestPosition();
	inline int getMoveState();
	
	enum MoveState {
		INVALID_MOVE_STATE = -1,
		IS_AT_STATION,
		MOVE_TO_PATIENT,
		HELP_PATIENT,
		CALL_STATION,
		MOVE_TO_STATION,
	};

private:
	int currPosition;
	int destPosition;
	SimData* simDataPtr;
	EmergencyList* emergencyListPtr;
	MoveState moveState;
	QueueStrategy qs = QueueStrategy(DistrictStrategy);
	inline int genTravelTime(int index, int currPos);
	int getFastestWay(int emergencyEventNumber, int travelTime);
};

inline void Doctor::setMoveState(const Doctor::MoveState moveState) {
	this->moveState = moveState;
}

inline void Doctor::setCurrPosition(const int pos) {
	this->currPosition = pos;
}

inline int Doctor::getCurrPosition() {
	return this->currPosition;
}

inline int Doctor::getMoveState() {
	return this->moveState;
}

inline void Doctor::setDestPosition(int pos) {
	this->destPosition = pos;
}

inline int Doctor::getDestPosition() {
	return this->destPosition;
}

inline int Doctor::genTravelTime(int index, int currPos) {
	int destinationDistrict = emergencyListPtr->getEmergencyDistrictAt(index);
	int travelTime = SimRandom::getActualTravelTime(simDataPtr->getDistance(currPos, destinationDistrict));
	return travelTime;
}


#endif