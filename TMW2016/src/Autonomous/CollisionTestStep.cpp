/*
 * CollisionTestStep.cpp
 *
 *  Created on: Mar 11, 2016
 *      Author: Programmer
 */

#include "CollisionTestStep.h"
#include <iostream>


bool CollisionTestStep::operator ()(World* world) {
	// @see http://www.pdocs.kauailabs.com/navx-mxp/examples/collision-detection/
	double current_accel_x = Robot::driveBase->imu->GetWorldLinearAccelX();
	double current_jerk_x = current_accel_x - last_accel_x;
	last_accel_x = current_accel_x;

	double current_accel_y = Robot::driveBase->imu->GetWorldLinearAccelY();
	double current_jerk_y = current_accel_y - last_accel_y;
	last_accel_y = current_accel_y;

	std::cout << "Last X: " << last_accel_x << '\n'
			  << "Cur  X: " << current_accel_x << "\n"
			  << "Last Y: " << last_accel_y << '\n'
			  << "Cur  Y: " << current_accel_y << '\n';


	std::cout << "jerk: x->" << current_jerk_x << " y-> " << current_jerk_y << "\n";

	return ((fabs(current_jerk_x) > COLLISION_THRESHOLD_DELTA_G) ||
			(fabs(current_jerk_y) > COLLISION_THRESHOLD_DELTA_G));

	return false;
}
