/*
 * CollisionDetector.h
 */

#ifndef SRC_UTIL_COLLISIONDETECTOR_H_
#define SRC_UTIL_COLLISIONDETECTOR_H_

#include "AHRS.h"

class CollisionDetector {
public:
	CollisionDetector(AHRS* imu, double threshold_=1.0);
	virtual ~CollisionDetector();
	bool Detect();
private:
	AHRS *imu;
	const double threshold;
	double last_accel_x = 0.0;
	double last_accel_y = 0.0;
};

#endif /* SRC_UTIL_COLLISIONDETECTOR_H_ */
