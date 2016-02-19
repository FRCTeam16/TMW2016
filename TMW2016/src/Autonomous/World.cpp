/*
 * World.cpp
 */

#include "World.h"
#include <cassert>

void World::Init(const int& startingPosition_) {
	cout << "World::Init\n";
	startingPosition = startingPosition_;
	timer->Start();
	cout << "World::Init complete\n";
}


void World::Update() {
	cout << "World::Update\n";
	runCounter++;
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


