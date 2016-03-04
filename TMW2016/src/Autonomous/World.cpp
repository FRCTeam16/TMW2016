/*
 * World.cpp
 */

#include "World.h"
#include <cassert>

void World::Init(const int& startingPosition_, const int& targetGoal_, outerworks startingDefense_) {
	cout << "World::Init\n";
	startingPosition = startingPosition_;
	targetGoal = targetGoal_;
	startingDefense = startingDefense_;
	timer->Start();
}


void World::Update(const VisionData &visionData_) {
	visionData = visionData_;
	runCounter++;
}

const VisionData& World::GetVisionData() const {
	return visionData;
}

long World::GetRunCounter() const {
	return runCounter;
}

double World::GetClock() const {
	return timer->Get();
}

int World::GetStartingPosition() const {
	return startingPosition;
}

int World::GetTargetGoal() const {
	return targetGoal;
}

outerworks World::GetStartingDefense() const {
	return startingDefense;
}


