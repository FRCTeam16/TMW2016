/*
 * Step.h
 */

#ifndef SRC_AUTONOMOUS_STEP_H_
#define SRC_AUTONOMOUS_STEP_H_

#include "../Subsystems/DriveBase.h"
#include "World.h"
#include <vector>

struct CrabInfo {
	float twist		= 0.0;
	float yspeed	= 0.0;
	float xspeed	= 0.0;
	bool gyro = true;
	bool lock = false;

	void Stop() {
		twist  = 0.0;
		yspeed = 0.0;
		xspeed = 0.0;
	}

	void Update(const float twist_, const float yspeed_, const float xspeed_, const bool gyro_) {
		twist = twist_;
		yspeed = yspeed_;
		xspeed = xspeed_;
		gyro = gyro_;
	}
};

// --------------------------------------------------------------------------//


class Step {
public:
	Step() {};
	virtual ~Step() {};
	virtual bool operator()(World *world) = 0;
	const CrabInfo* GetCrabInfo() { return crab.get(); }
protected:
	std::unique_ptr<CrabInfo> crab { new CrabInfo() };
	std::shared_ptr<DriveBase> driveBase;
};


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

class LockWheels : public Step {
public:
	bool operator()(World *world) override;
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

// --------------------------------------------------------------------------//


class ShootBall : public Step {
public:
	ShootBall(bool waitForShooterWheels_= true) : waitForShooterWheels(waitForShooterWheels_) {};
	bool operator()(World *world) override;
private:
	const bool waitForShooterWheels;
	const float fireWait = 1.0;
	double startTime = -1;
	bool hasFired = false;

};

// --------------------------------------------------------------------------//

class SetArmPosition : public Step {
public:
	enum struct Position { Custom, Pickup, Travel, ShooterLow, ShooterHigh };
	SetArmPosition(Position pos_, bool wait_):
		position(pos_), wait(wait_), customTarget(-1) {}
	SetArmPosition(int targetPosition_, bool wait_):
			position(Position::Custom), wait(wait_), customTarget(targetPosition_) {}
	bool operator()(World *world) override;
private:
	const Position position;
	const bool wait;
	const int customTarget;
	const int MAX_LOOPS = 500;	// 10 seconds
	int loopCounter = 0;
	bool running = false;
};

#endif /* SRC_AUTONOMOUS_STEP_H_ */
