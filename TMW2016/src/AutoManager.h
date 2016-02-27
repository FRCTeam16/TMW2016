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

class AutoManager {
public:
	AutoManager(const VisionServer *visionServer);
	virtual ~AutoManager() {};
	void Init();
	void Periodic();
private:
	std::unique_ptr<SendableChooser> position;
	std::unique_ptr<SendableChooser> defense;
	std::unique_ptr<SendableChooser> target;
	std::unique_ptr<World> world;
	typedef enum outerworks {LowBar, Portcullis, ChevalDeFrise, Moat, Ramparts, Drawbridge, SallyPort, RockWall, RoughTerrain} outerworks;
	std::string outerworksLookup[];
	std::map<outerworks, std::shared_ptr<Strategy>> strategyLookup;
	Strategy* currentStrategy;
	const VisionServer* visionServer;
	std::shared_ptr<DriveBase> driveBase;
};

#endif /* AUTOMANAGER_H_ */
