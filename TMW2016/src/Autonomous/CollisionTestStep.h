/*
 * CollisionTestStep.h
 *
 *  Created on: Mar 11, 2016
 *      Author: Programmer
 */

#ifndef SRC_AUTONOMOUS_COLLISIONTESTSTEP_H_
#define SRC_AUTONOMOUS_COLLISIONTESTSTEP_H_

#include "World.h"
#include "../Robot.h"


class CollisionTestStep : public Step {
public:
	CollisionTestStep() {}
	virtual ~CollisionTestStep() {}
	bool operator()(World* world) override;
private:
	float last_accel_x;
	float last_accel_y;
	const float COLLISION_THRESHOLD_DELTA_G = 1.0f;
};

#endif /* SRC_AUTONOMOUS_COLLISIONTESTSTEP_H_ */
