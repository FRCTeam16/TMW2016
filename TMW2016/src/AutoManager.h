/*
 * AutoManager.h
 *
 *  Created on: Feb 4, 2016
 *      Author: Programmer
 */

#ifndef AUTOMANAGER_H_
#define AUTOMANAGER_H_

#include <map>
#include <vector>
#include "WPILib.h"

#include "Autonomous/Strategy.h"
#include "Autonomous/World.h"
#include "Vision/VisionServer.h"

static const std::string AUTO_DELAY = "Auto Delay";
static const std::string AUTO_C_TYPE_RETURN = "Auto C-Type Return";
static const std::string AUTO_TWO_BALL_PREVENT = "STC";


class AutoManager {
public:
	AutoManager(const VisionServer *visionServer);
	virtual ~AutoManager() {};
	void Init(World *world);
	void Periodic(World *world);

	std::unique_ptr<SendableChooser> position;
	std::unique_ptr<SendableChooser> defense;
	std::unique_ptr<SendableChooser> target;
	std::unique_ptr<SendableChooser> returnPosition;
private:
	std::map<outerworks, std::shared_ptr<Strategy>> strategyLookup;
	Strategy* currentStrategy;
	const VisionServer* visionServer;
	std::shared_ptr<DriveBase> driveBase;
};

#endif /* AUTOMANAGER_H_ */
