#ifndef SimTime_h
#define SimTime_h

class SimTime{
public:
	virtual ~SimTime();
	inline static SimTime* getInstance();
	inline int getTime() const;
	inline void incrementTime();
	inline void setTime(const int time);

private:
	SimTime();

	static SimTime* instancePtr;
	int time;
};

inline SimTime* SimTime::getInstance(){
	if (!instancePtr){
		instancePtr = new SimTime;
	}
	return instancePtr;
}

inline int SimTime::getTime() const{
	return time;
}

inline void SimTime::incrementTime(){
	this->time++;
}

inline void SimTime::setTime(const int time){
	this->time = time;
}

#endif SimTime_h