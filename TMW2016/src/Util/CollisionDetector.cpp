/*
 * CollisionDetector.cpp
 */

#include "CollisionDetector.h"
#include <iostream>

CollisionDetector::CollisionDetector(AHRS* imu_, double threshold_ ) :
	imu(imu_), threshold(threshold_) {
}

CollisionDetector::~CollisionDetector() {
}

bool CollisionDetector::Detect() {
	// @see http://www.pdocs.kauailabs.com/navx-mxp/examples/collision-detection/
	double current_accel_x = imu->GetWorldLinearAccelX();
	double current_jerk_x = current_accel_x - last_accel_x;
	last_accel_x = current_accel_x;

	double current_accel_y = imu->GetWorldLinearAccelY();
	double current_jerk_y = current_accel_y - last_accel_y;
	last_accel_y = current_accel_y;

	std::cout << "MoveToWallShootingPosition jerk: x->" << current_jerk_x
			  << " y-> " << current_jerk_y << "\n";

	return ((fabs(current_jerk_x) > threshold) ||
			(fabs(current_jerk_y) > threshold));
}

