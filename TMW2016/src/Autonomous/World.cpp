/*
 * World.cpp
 */

#include "World.h"
#include <cassert>

void World::Init(const int& startingPosition_, const int& targetGoal_, outerworks startingDefense_, int returnPosition_, bool ctypeReturn_) {
	cout << "World::Init\n";
	startingPosition = startingPosition_;
	targetGoal = targetGoal_;
	startingDefense = startingDefense_;
	returnPosition = returnPosition_;
	ctypeReturn = ctypeReturn_;
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

void World::SetTargetGoal(const int targetGoal_) {
	targetGoal = targetGoal_;
}

outerworks World::GetStartingDefense() const {
	return startingDefense;
}

int World::GetReturnPosition() const {
	return returnPosition;
}

bool World::GetCtypeReturn() const {
	return ctypeReturn;
}
