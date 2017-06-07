#ifndef Emergency_h
#define Emergency_h

using namespace std;

class Emergency{
public:
	Emergency();
	virtual ~Emergency();

	enum Status;

	inline int getStartTime() const;
	inline int getDistrict() const;
	inline bool getUrgent() const;
	inline Emergency::Status getStatus() const;
	inline int getCareDurationTime() const;

	inline void setStartTime(const int startTime);
	inline void setDistrict(const int district);
	inline void setUrgent(const bool urgent);
	inline void setStatus(const Emergency::Status status);
	inline void setCareDurationTime(const int careDurationTime);
	
	enum Status{
		NOT_SET = -1,
		QUEUED_WAITING,
		TREATMENT,
		COMPLETED
	};

	static const int INIT_TO_INVALID = -1;

private:
	int startTime;
	int district;
	bool urgent;
	Status status;
	int careDurationTime;
};

inline int Emergency::getStartTime() const{
	return this->startTime;
}
inline int Emergency::getDistrict() const{
	return this->district;
}
inline bool Emergency::getUrgent() const{
	return this->urgent;
}
inline Emergency::Status Emergency::getStatus() const{
	return this->status;
}

inline int Emergency::getCareDurationTime() const{
	return this->careDurationTime;
}

inline void Emergency::setStartTime(const int startTime){
	this->startTime = startTime;
}
inline void Emergency::setDistrict(const int district){
	this->district = district;
}
inline void Emergency::setUrgent(const bool urgent){
	this->urgent = urgent;
}
inline void Emergency::setStatus(const Emergency::Status status){
	this->status = status;
}
inline void Emergency::setCareDurationTime(const int careDurationTime){
	this->careDurationTime = careDurationTime;
}


#endif