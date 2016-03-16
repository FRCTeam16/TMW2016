/*
 * SearchForGoal.h
 */

#ifndef SRC_AUTONOMOUS_SEARCHFORGOAL_H_
#define SRC_AUTONOMOUS_SEARCHFORGOAL_H_

#include "Step.h"
#include "World.h"

class SearchForGoal : public Step {
public:
	SearchForGoal(float timeout_, float speed_) :
		timeout(timeout_), speed(speed_) {}
	virtual ~SearchForGoal() {}
	bool operator()(World *world) override;
private:
	const float timeout;
	const float speed;
	float startTime = -1;
};

#endif /* SRC_AUTONOMOUS_SEARCHFORGOAL_H_ */
