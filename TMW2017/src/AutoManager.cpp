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
#include "Autonomous/ReturnStrategy.h"

#include "WPILib.h"


static const std::string AUTO_POSITION = "Auto Position";
static const std::string AUTO_DEFENSE = "Auto Defense";
static const std::string AUTO_TARGET = "Auto Goal-";
static const std::string AUTO_RETURN = "Auto Return";

static const std::string AUTO_INIT_CONFIG_ERROR = "Auto Config Error";

typedef std::shared_ptr<Strategy> SharedStrategyPtr;

AutoManager::AutoManager(const VisionServer *visionServer_):
		position(new SendableChooser()),
		defense(new SendableChooser()),
		target(new SendableChooser()),
		returnPosition(new SendableChooser()),
		visionServer(visionServer_),
		driveBase(Robot::driveBase) {
	//
	// Initialize strategies and commands
	//
	cout << "AutoManager::AutoManager start..\n";


	std::shared_ptr<Strategy> noop 			{ new NoOpStrategy() };
	std::shared_ptr<Strategy> lowbar 		{ new OuterworkAndShootStrategy(new LowBarStrategy()) };
	std::shared_ptr<Strategy> roughTerrain 	{ new OuterworkAndShootStrategy(new RoughTerrainStrategy()) };
	std::shared_ptr<Strategy> moat 			{ new OuterworkAndShootStrategy(new MoatStrategy()) };
	std::shared_ptr<Strategy> rampart 		{ new OuterworkAndShootStrategy(new RampartStrategy()) };
	std::shared_ptr<Strategy> debugVision 	{ new DebugVisionStrategy() };
//	std::shared_ptr<Strategy> debugVision 	{ new ReturnStrategy() };

	// Map Defenses to Strategies
	strategyLookup.insert(std::make_pair(LowBar, 		lowbar));
	strategyLookup.insert(std::make_pair(Portcullis, 	noop));
	strategyLookup.insert(std::make_pair(ChevalDeFrise, noop));
	strategyLookup.insert(std::make_pair(Moat, 			moat));
	strategyLookup.insert(std::make_pair(Ramparts, 		rampart));
	strategyLookup.insert(std::make_pair(Drawbridge, 	noop));
	strategyLookup.insert(std::make_pair(SallyPort, 	noop));
	strategyLookup.insert(std::make_pair(RockWall, 		roughTerrain));
	strategyLookup.insert(std::make_pair(RoughTerrain,	roughTerrain));
	strategyLookup.insert(std::make_pair(Debug,			debugVision));
	strategyLookup.insert(std::make_pair(Noop,			noop));
	currentStrategy = noop.get();

	//
	// Initialize Sendable Objects and Dashboard
	//

	defense->AddDefault("1: LowBar", 		(void*) LowBar);
//	defense->AddObject("A: Portcullis", 	(void*) Portcullis);
//	defense->AddObject("A: ChevalDeFrise", 	(void*) ChevalDeFrise);
	defense->AddObject("B: Moat", 			(void*) Moat);
	defense->AddObject("B: Ramparts", 		(void*) Ramparts);
//	defense->AddObject("C: Drawbridge", 	(void*) Drawbridge);
//	defense->AddObject("C: SallyPort", 		(void*) SallyPort);
	defense->AddObject("D: RockWall", 		(void*) RockWall);
	defense->AddObject("D: RoughTerrain", 	(void*) RoughTerrain);
	defense->AddObject("X: DebugVision", 	(void*) Debug);
	defense->AddObject("Z: Do Nothing", 	(void*) Noop);

	position->AddDefault("1", (void*) 1);
	position->AddObject("2",  (void*) 2);
	position->AddObject("3",  (void*) 3);
	position->AddObject("4",  (void*) 4);
	position->AddObject("5",  (void*) 5);


	target->AddDefault("1 - Left", (void*) 1);
	target->AddObject("2 - Center",  (void*) 2);
//	target->AddObject("3 - Right",  (void*) 3);
	target->AddObject("4 - Left Blind",  (void*) 4);
//	target->AddObject("5 - Right Blind",  (void*) 5);
	target->AddObject("9 - Stop After Outerworks", (void*) 0);

	returnPosition->AddDefault("0 - None", (void*) 0);
	returnPosition->AddDefault("1", (void*) 1);
	returnPosition->AddObject("2",  (void*) 2);
	returnPosition->AddObject("3",  (void*) 3);
	returnPosition->AddObject("4",  (void*) 4);
	returnPosition->AddObject("5",  (void*) 5);

	SmartDashboard::PutData(AUTO_POSITION, position.get());
	SmartDashboard::PutData(AUTO_DEFENSE, defense.get());
	SmartDashboard::PutData(AUTO_TARGET, target.get());
	SmartDashboard::PutData(AUTO_RETURN, returnPosition.get());

	cout << "AutoManager::AutoManager complete\n";
}


void AutoManager::Init(World *world) {
	cout << "AutoManager::Init\n";
	// Read state of world information from driver station
	const int startingDefenseIdx = (int) defense->GetSelected();
	outerworks startingDefense 	 = static_cast<outerworks>(startingDefenseIdx);
	const int startingPosition 	 = (int) position->GetSelected();
	const int targetGoal		 = (int) target->GetSelected();
	const int returnPos			 = (int) returnPosition->GetSelected();

	SmartDashboard::PutNumber("Selected Position", startingPosition);
	SmartDashboard::PutNumber("Selected Defense", startingDefenseIdx);
	SmartDashboard::PutNumber("Selected Target", targetGoal);
	SmartDashboard::PutNumber("Selected Return", returnPos);

	const bool ctypeReturn = SmartDashboard::GetBoolean(AUTO_C_TYPE_RETURN, false);
	const bool twoBallPrevent = SmartDashboard::GetBoolean(AUTO_TWO_BALL_PREVENT, false);

	// TODO: Add precondition state checks to prevent starting at 1 and shooting at 3, for example
	SmartDashboard::PutString(AUTO_INIT_CONFIG_ERROR, "");
	if ((startingPosition <= 2 && targetGoal > 1) ||
			(startingPosition >= 4 && targetGoal != 3)) {
		std::cout << "Auto init config error, invalid start and target\n";
		SmartDashboard::PutString(AUTO_INIT_CONFIG_ERROR, "Warning: Invalid starting position and target goal combination");
	}

	// Initialize world state with initial information
	// Must occur before strategy initialization
	world->Init(startingPosition, targetGoal, startingDefense, returnPos, ctypeReturn, twoBallPrevent);

	// Lookup strategy for starting outerwork defense
	auto iterator = strategyLookup.find(startingDefense);
	if (iterator != strategyLookup.end()) {
		currentStrategy = iterator->second.get();
		currentStrategy->Init(world);
	} else {
		// ERROR
		std::string msg = "No strategy selected in dash board";
		std::cout << msg << "\n";
		SmartDashboard::PutString(AUTO_INIT_CONFIG_ERROR, msg);
		return;
	}

	//
	// Initialize sensors
	//
	driveBase->imu->ZeroYaw();
	cout << "AutoManager::Init COMPLETE\n";
}

void AutoManager::Periodic(World *world) {
	assert(currentStrategy != nullptr && "No strategy currently set");
	currentStrategy->Run(world);
}
