/*
 * World.h
 */

#ifndef SRC_AUTONOMOUS_WORLD_H_
#define SRC_AUTONOMOUS_WORLD_H_

#include "WPILib.h"
#include "../Subsystems/DriveBase.h"
#include "../Subsystems/Arm.h"
#include "AHRS.h"
#include "../Vision/VisionData.h"

typedef enum outerworks {LowBar, Portcullis, ChevalDeFrise, Moat, Ramparts, Drawbridge, SallyPort, RockWall, RoughTerrain, Debug, Noop} outerworks;


class World {
public:
	World() : timer(new Timer()) {};
	virtual ~World() {};
	void Init(const int& startingPosition_, const int& targetGoal_, outerworks startingDefense_);
	void Update(const VisionData &visionData);
	long GetRunCounter() const;					// number of times Run() is called
	double GetClock() const;					// time elapsed since Init() in seconds
	int GetStartingPosition() const;			// starting position of robot
	int GetTargetGoal() const;					// target goal number
	void SetTargetGoal(const int targetGoal_);
	outerworks GetStartingDefense() const;		// starting outerworks defense
	const VisionData& GetVisionData() const;	// vision information
private:
	long runCounter = 0;					// number of times periodic is called
	int startingPosition = -1;				// our starting location
	int targetGoal = -1;					// target goal
	outerworks startingDefense = Noop;		// starting outerworks type
	std::unique_ptr<Timer> timer;			// tracks time in autonomous world
	VisionData visionData;					// tracks vision data
};

#endif /* SRC_AUTONOMOUS_WORLD_H_ */
