/*
 * World.cpp
 */

#include "World.h"
#include <cassert>

void World::Init(const int& startingPosition_, const int& targetGoal_) {
	cout << "World::Init\n";
	startingPosition = startingPosition_;
	targetGoal = targetGoal_;
	timer->Start();
	cout << "World::Init complete\n";
}


void World::Update(const VisionData &visionData_) {
	cout << "World::Update\n";
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


