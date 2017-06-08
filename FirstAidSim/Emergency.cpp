#include "Emergency.h"

Emergency::Emergency(){
	this->startTime = Emergency::INIT_TO_INVALID;
	this->setDistrict(Emergency::INIT_TO_INVALID);
	this->setUrgent(Emergency::INIT_TO_INVALID);
	this->setStatus(Emergency::NOT_SET);
	this->setDoctorArrivalTime(Emergency::NOT_SET);
	this->setCompletionTime(Emergency::NOT_SET);
}

Emergency::~Emergency(){
}