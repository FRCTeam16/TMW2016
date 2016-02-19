/*
 * SimpleDriveForwardStrategy.cpp
 */

#include "SimpleDriveForwardStrategy.h"
#include <iostream>

SimpleDriveForwardStrategy::~SimpleDriveForwardStrategy() {
}

void SimpleDriveForwardStrategy::Init() {
	cout << "SimpleDriveForwardStrategy::Init\n";
//	steps.push_back(std::unique_ptr<Step>(new ForwardWithArm(5.0)));
//	steps.push_back(std::unique_ptr<Step>(new ForwardWithArmAndRoll()));
//	steps.push_back(std::unique_ptr<Step>(new TimedCrab(3, 60.0, 0.30)));
//	steps.push_back(std::unique_ptr<Step>(new Turn(45.0)));
//	steps.push_back(std::unique_ptr<Step>(new AlignToZero()));
	steps.push_back(std::unique_ptr<Step>(new SetArmPosition(710)));	// TODO: Read preferences?
//	steps.push_back(std::unique_ptr<Step>(new ShootBall()));
	cout << "SimpleDriveForwardStrategy::Init with " << steps.size() << " steps\n";
}

