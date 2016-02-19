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

#include "WPILib.h"


static const std::string AUTO_POSITION = "Auto Position";
static const std::string AUTO_DEFENSE = "Auto Defense";

AutoManager::AutoManager():
		world(new World()),
		driveBase(Robot::driveBase),
		position(new SendableChooser()),
		defense(new SendableChooser()) {
	//
	// Initialize strategies and commands
	//
	cout << "AutoManager::AutoManager start..";
	strategies.push_back(std::unique_ptr<Strategy>{new OuterworkAndShootStrategy(new NoOpStrategy())});
	strategies.push_back(std::unique_ptr<Strategy>{new OuterworkAndShootStrategy(new LowBarStrategy(0.2))});
	strategies.push_back(std::unique_ptr<Strategy>{new OuterworkAndShootStrategy(new RoughTerrainStrategy())});

	Strategy *noop = strategies[0].get();
	Strategy *lowbar = strategies[1].get();
	Strategy *roughTerrain = strategies[2].get();

	// Map Defenses to Strategies
	strategyLookup.insert(std::make_pair(LowBar, lowbar));
	strategyLookup.insert(std::make_pair(Portcullis, noop));
	strategyLookup.insert(std::make_pair(ChevalDeFrise, noop));
	strategyLookup.insert(std::make_pair(Moat, noop));
	strategyLookup.insert(std::make_pair(Ramparts, noop));
	strategyLookup.insert(std::make_pair(Drawbridge, noop));
	strategyLookup.insert(std::make_pair(SallyPort, noop));
	strategyLookup.insert(std::make_pair(RockWall, roughTerrain));
	strategyLookup.insert(std::make_pair(RoughTerrain, roughTerrain));
	currentStrategy = noop;

	//
	// Initialize Sendable Objects and Dashboard
	//
	defense->AddDefault("1: LowBar", (void*) LowBar);
	defense->AddObject("A: Portcullis", (void*) Portcullis);
	defense->AddObject("A: ChevalDeFrise", (void*) ChevalDeFrise);
	defense->AddObject("B: Moat", (void*) Moat);
	defense->AddObject("B: Ramparts", (void*) Ramparts);
	defense->AddObject("C: Drawbridge", (void*) Drawbridge);
	defense->AddObject("C: SallyPort", (void*) SallyPort);
	defense->AddObject("D: RockWall", (void*) RockWall);
	defense->AddObject("D: RoughTerrain", (void*) RoughTerrain);

	position->AddDefault("1", (void*) 1);
	position->AddObject("2", (void*) 2);
	position->AddObject("3", (void*) 3);
	position->AddObject("4", (void*) 4);
	position->AddObject("5", (void*) 5);

	SmartDashboard::PutData(AUTO_POSITION, position.get());
	SmartDashboard::PutData(AUTO_DEFENSE, defense.get());
	cout << "AutoManager::AutoManager complete";
}


void AutoManager::Init() {
	cout << "AutoManager::Init\n";
	// Read state of world information from driver station
	int startingDefenseIdx = (int) defense->GetSelected();
	const int startingPosition = (int)(position->GetSelected());

	cout << "Starting Defense Idx: " << startingDefenseIdx << '\n';
	cout << "Starting Position   : " << startingPosition << '\n';
	outerworks startingDefense = static_cast<outerworks>(startingDefenseIdx);

	auto iterator = strategyLookup.find(startingDefense);
	if (iterator != strategyLookup.end()) {
		currentStrategy = iterator->second;
		currentStrategy->Init();
	} else {
		// ERROR
		assert(false && "No strategy selected in dash board");
	}

	// Initialize world state with initial information
	world->Init(startingPosition);

	// Initialize sensors
	driveBase->imu->ZeroYaw();
	cout << "AutoManager::Init COMPLETE\n";

}

void AutoManager::Periodic() {
	assert(currentStrategy != nullptr && "No strategy currently set");
	world->Update();
	currentStrategy->Run(world.get());
}
