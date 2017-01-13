/*
 * AlignWithWall.h
 */

#ifndef SRC_AUTONOMOUS_COLLIDEWITHWALL_H_
#define SRC_AUTONOMOUS_COLLIDEWITHWALL_H_

#include "Step.h"
#include "../Util/CollisionDetector.h"

class CollideWithWall : public Step {
public:
	CollideWithWall(float frontAngleDegrees_, float xSpeed_, float ySpeed_, float jerkThreshold = 1.0, float timeout_ = 1.5);
	~CollideWithWall();
	bool operator ()(World* world);
private:
	std::unique_ptr<CollisionDetector> collisionDetector;
	const float YAW_THRESHOLD = 5.0;
	const float yawDegrees;
	const float xSpeed;
	const float ySpeed;
	const float JERK_THRESHOLD;
	const float TIMEOUT;
	double startTime = -1;
	double collideStartTime = -1;
	bool frontInPosition = false;

	bool collisionDetected = false;
};

#endif /* SRC_AUTONOMOUS_COLLIDEWITHWALL_H_ */
