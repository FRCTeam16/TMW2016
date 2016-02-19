/*
 * World.h
 */

#ifndef SRC_AUTONOMOUS_WORLD_H_
#define SRC_AUTONOMOUS_WORLD_H_

#include "WPILib.h"
#include "../Subsystems/DriveBase.h"
#include "../Subsystems/Arm.h"
#include "AHRS.h"

class World {
public:
	World() : timer(new Timer()) {};
	virtual ~World() {};
	void Init(const int& startingPosition_);
	void Update();
	long GetRunCounter() const;				// number of times Run() is called
	double GetClock() const;				// time elapsed since Init() in seconds
	int GetStartingPosition() const;
private:
	long runCounter = 0;					// number of times periodic is called
	int startingPosition = -1;				// our starting location
	Timer *timer;							// tracks time in autonomous world
};

#endif /* SRC_AUTONOMOUS_WORLD_H_ */
