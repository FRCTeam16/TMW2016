/*
 * Step.h
 */

#ifndef SRC_AUTONOMOUS_STEP_H_
#define SRC_AUTONOMOUS_STEP_H_

#include "../Subsystems/DriveBase.h"
#include "World.h"
#include <vector>

class Step {
public:
	Step() {};
	virtual ~Step() {};
	virtual bool operator()(World *world) = 0;
protected:
	std::shared_ptr<DriveBase> driveBase;
};

// --------------------------------------------------------------------------//

//class ParallelStep : public Step {
//public:
//	ParallelStep(std::initializer_list<Step> stepsToAdd) : steps(stepsToAdd) {
//		cout << "Constructed ParallelStep with " << steps.size() << " steps\n";
//	}
//	bool operator()(World *world) override;
//private:
//	std::vector<Step> steps;
//};

// --------------------------------------------------------------------------//


class TimedCrab : public Step {
public:
	TimedCrab(float time_, float angle, float yspeed_, float xspeed_) :
		targetTime(time_), targetAngle(angle), ySpeed(yspeed_), xSpeed(xspeed_) {}
	bool operator()(World *world) override;
private:
	float startTime = -1;
	const float targetTime;
	const float targetAngle;
	const float ySpeed;
	const float xSpeed;
};


// --------------------------------------------------------------------------//

class ForwardWithArm : public Step {
public:
	ForwardWithArm(float time_, bool forward_ = true) : targetTime(time_), forward(forward_) {}
	bool operator()(World *world) override;
private:
	float startTime = -1;
	const float targetTime;
	bool forward;
	const float armTarget = 838;
};


class ForwardWithArmAndRoll : public Step {
public:
	ForwardWithArmAndRoll() {}
	bool operator()(World *world) override;
private:
	bool running = false;
	const float armTarget = 838;
	bool startedObstacle = false;
	bool hitNegative = false;
	int quietCount = 0;
};



class ForwardCheckRoll : public Step {
public:
	ForwardCheckRoll(float speed_ = 0.75) : speed(speed_) {}
	bool operator()(World *world) override;
private:
	const int MAX_LOOPS = 250;
	int loopCounter = 0;
	const float speed;
	bool running = false;
	bool startedObstacle = false;
	bool hitNegative = false;
	int quietCount = 0;
};

// --------------------------------------------------------------------------//



class Turn : public Step {
public:
	Turn(float angle_) : angle(angle_) {}
	bool operator()(World *world) override;
private:
	bool firstRun = true;
	const float angle;
};

// --------------------------------------------------------------------------//

class ControlShooterMotors: public Step {
public:
	ControlShooterMotors(bool enable_);
	bool operator()(World *world) override;
private:
	const bool enable;
	float shooterSpeed;
	float feederSpeed;
};

class ShootBall : public Step {
public:
	ShootBall() {};
	bool operator()(World *world) override;
private:
	const float fireWait = 1.0;
	double startTime = -1;
	bool hasFired = false;

};

// --------------------------------------------------------------------------//

class BlindSetArmPosition : public Step {
public:
	BlindSetArmPosition(int pos_) : targetPosition(pos_) {}
	bool operator()(World *world) override;
private:
	const int targetPosition;
};
class SetArmPosition : public Step {
public:
	SetArmPosition(int pos_) : running(false), targetPosition(pos_) {}
	bool operator()(World *world) override;
private:
	const int MAX_LOOPS = 500;	// 10 seconds
	int loopCounter = 0;
	bool running;
	const int targetPosition;
};

#endif /* SRC_AUTONOMOUS_STEP_H_ */
