/*
 * AutoManager.cpp
 *
 *  Created on: Feb 4, 2016
 *      Author: Programmer
 */

#include <cassert>
#include <string>
#include "Robot.h"
#include "AutoManager.h"
#include "Autonomous/Strategy.h"
#include "Autonomous/OuterworkStrategies.h"
#include "Autonomous/DebugVisionStrategy.h"

#include "WPILib.h"


static const std::string AUTO_POSITION = "Auto Position";
static const std::string AUTO_DEFENSE = "Auto Defense";
static const std::string AUTO_TARGET = "Auto Target Goal";

static const std::string AUTO_INIT_CONFIG_ERROR = "Auto Config Error";

typedef std::shared_ptr<Strategy> SharedStrategyPtr;

AutoManager::AutoManager(const VisionServer *visionServer_):
		position(new SendableChooser()),
		defense(new SendableChooser()),
		target(new SendableChooser()),
		world(new World()),
		visionServer(visionServer_),
		driveBase(Robot::driveBase) {
	//
	// Initialize strategies and commands
	//
	cout << "AutoManager::AutoManager start..\n";


	std::shared_ptr<Strategy> noop { new OuterworkAndShootStrategy(new NoOpStrategy()) };
	std::shared_ptr<Strategy> lowbar { new OuterworkAndShootStrategy(new LowBarStrategy()) };
	std::shared_ptr<Strategy> roughTerrain { new OuterworkAndShootStrategy(new RoughTerrainStrategy()) };
	std::shared_ptr<Strategy> chevalDeFrise { new OuterworkAndShootStrategy(new ChevalDeFriseStrategy()) };
	std::shared_ptr<Strategy> debugVision { new DebugVisionStrategy() };

	// Map Defenses to Strategies
	strategyLookup.insert(std::make_pair(LowBar, 		lowbar));
	strategyLookup.insert(std::make_pair(Portcullis, 	noop));
	strategyLookup.insert(std::make_pair(ChevalDeFrise, chevalDeFrise));
	strategyLookup.insert(std::make_pair(Moat, 			roughTerrain));
	strategyLookup.insert(std::make_pair(Ramparts, 		roughTerrain));
	strategyLookup.insert(std::make_pair(Drawbridge, 	noop));
	strategyLookup.insert(std::make_pair(SallyPort, 	noop));
	strategyLookup.insert(std::make_pair(RockWall, 		roughTerrain));
	strategyLookup.insert(std::make_pair(RoughTerrain,	roughTerrain));
	strategyLookup.insert(std::make_pair(Debug,			debugVision));
	currentStrategy = noop.get();

	//
	// Initialize Sendable Objects and Dashboard
	//
	defense->AddDefault("1: LowBar", 		(void*) LowBar);
	defense->AddObject("A: Portcullis", 	(void*) Portcullis);
	defense->AddObject("A: ChevalDeFrise", 	(void*) ChevalDeFrise);
	defense->AddObject("B: Moat", 			(void*) Moat);
	defense->AddObject("B: Ramparts", 		(void*) Ramparts);
	defense->AddObject("C: Drawbridge", 	(void*) Drawbridge);
	defense->AddObject("C: SallyPort", 		(void*) SallyPort);
	defense->AddObject("D: RockWall", 		(void*) RockWall);
	defense->AddObject("D: RoughTerrain", 	(void*) RoughTerrain);
	defense->AddObject("X: DebugVision", 	(void*) Debug);

	position->AddDefault("1", (void*) 1);
	position->AddObject("2",  (void*) 2);
	position->AddObject("3",  (void*) 3);
	position->AddObject("4",  (void*) 4);
	position->AddObject("5",  (void*) 5);

	target->AddDefault("1", (void*) 1);
	target->AddObject("2",  (void*) 2);
	target->AddObject("3",  (void*) 3);

	SmartDashboard::PutData(AUTO_POSITION, position.get());
	SmartDashboard::PutData(AUTO_DEFENSE, defense.get());
	SmartDashboard::PutData(AUTO_TARGET, target.get());
	cout << "AutoManager::AutoManager complete\n";
}


void AutoManager::Init() {
	cout << "AutoManager::Init\n";
	// Read state of world information from driver station
	const int startingDefenseIdx = (int) defense->GetSelected();
	const int startingPosition 	 = (int) position->GetSelected();
	const int targetGoal		 = (int) target->GetSelected();

	cout << "Starting Defense Idx: " << startingDefenseIdx << '\n';
	cout << "Starting Position   : " << startingPosition << '\n';
	cout << "Starting Target     : " << targetGoal << '\n';

	// TODO: Add precondition state checks to prevent starting at 1 and shooting at 3, for example
	if ((startingPosition <= 2 && targetGoal != 1) ||
			(startingPosition >= 4 && targetGoal != 3)) {
		std::cout << "Auto init config error, invalid start and target\n";
		SmartDashboard::PutString(AUTO_INIT_CONFIG_ERROR, "Invalid starting position and target goal combination");
	}

	// Lookup strategy for starting outerwork defense
	outerworks startingDefense = static_cast<outerworks>(startingDefenseIdx);
	auto iterator = strategyLookup.find(startingDefense);
	if (iterator != strategyLookup.end()) {
		currentStrategy = iterator->second.get();
		currentStrategy->Init();
	} else {
		// ERROR
		assert(false && "No strategy selected in dash board");
	}

	// Initialize world state with initial information
	world->Init(startingPosition, targetGoal);

	// Initialize sensors
	driveBase->imu->ZeroYaw();
	cout << "AutoManager::Init COMPLETE\n";

}

void AutoManager::Periodic() {
	assert(currentStrategy != nullptr && "No strategy currently set");
	world->Update(visionServer->GetVisionData());
	currentStrategy->Run(world.get());
}
