/*
 * AlignGoalUsingPID.cpp
 */

#include "AlignGoalUsingPID.h"
#include "../Robot.h"
#include <iostream>

AlignGoalUsingPID::AlignGoalUsingPID(const float speed_, const bool teleop_mode_) :
	speed(speed_), teleop_mode(teleop_mode_), xAdapter(new VisionPIDAdapter()), yAdapter(new VisionPIDAdapter())
{
	pidX.reset(new PIDController(0.006, 0, 0.005, xAdapter.get(), xAdapter.get(), 0.2));
	pidX->SetSetpoint(GOAL_CENTER_OFFSET);
	pidX->SetContinuous(false);
	pidX->SetOutputRange(-speed, speed);	// output motor speeds
	pidX->SetInputRange(-160, 160);			// size of input image
	pidX->Enable();
	pidY.reset(new PIDController(0.01, 0, 0.0, yAdapter.get(), yAdapter.get(), 0.2));
	pidY->SetSetpoint(Y_TARGET);
	pidY->SetContinuous(false);
	pidY->SetOutputRange(-speed, speed);	// output motor speeds
	pidY->SetInputRange(0, 320);			// size of input image
	pidY->Enable();
	GOAL_CENTER_OFFSET = -3;

}

AlignGoalUsingPID::~AlignGoalUsingPID() {

}

bool AlignGoalUsingPID::operator ()(World* world) {
	cout << "AlignGoalUsingPID()\n";
	crab->lock = false;
	const float currentTime = world->GetClock();
	if (startTime < 0) {
		startTime = currentTime;
		GOAL_CENTER_OFFSET = Preferences::GetInstance()->GetInt("VisionCenterOffset", -11);
		pidX->SetSetpoint(GOAL_CENTER_OFFSET);
	}

	if ((currentTime - startTime) > timeout) {
		std::cout << "*** TIMEOUT ***\n";
		crab->lock = true;
		return false;
	}

	const VisionData vd = world->GetVisionData();
	int lastX = goal.xposition;
	goal = vd.GetGoal(world->GetTargetGoal());
	if (goal.HasData()) {
		noGoalCounter = 0;
		xAdapter->Update(goal.xposition);
		yAdapter->Update(goal.height);
		kickCounter = (goal.xposition == lastX) ? kickCounter + 1 : 0;
	} else {
		if (noGoalCounter++ > MAX_NO_GOAL_SCANS) {
			std::cout << "*** NO GOAL ***\n";
			if (!teleop_mode) {
				crab->Update(lastCrab.twist, lastCrab.xspeed, lastCrab.yspeed, lastCrab.gyro);
				// TODO: This is probably incorrect
			} else {
				// No goal, allow driver to operate robot
				crab->Update(Robot::oi->getJoystickTwist(),-Robot::oi->getJoystickY(),Robot::oi->getJoystickX(),true);
				return false;
			}
		}
	}

	float current_twist = fabs(Robot::driveBase->imu->GetYaw());
	float delta_twist = last_twist < 0 ? current_twist : fabs(current_twist - last_twist);
	if (delta_twist > 180.0) {
		delta_twist = 360.0 - current_twist;
	}
	last_twist = current_twist;

	const bool in_twist = delta_twist > TWIST_THRESHOLD;
	const int maxX = goal.xposition + X_THRESHOLD;
	const int minX = goal.xposition - X_THRESHOLD;
	if (GOAL_CENTER_OFFSET > minX && GOAL_CENTER_OFFSET < maxX) {
		std::cout << "!!! ====================== Goal Aligned  ====================== !!! " << fired << "\n";
		if (!fired && !in_twist) {
			cout << "***********************-=====> FIRING\n";
			fired = Robot::arm->Fire();
		} else {
			cout << "!!! Not firing, fired = " << fired << "  in_twist = " << in_twist << '\n';
		}
	}


	const int targetGoal = world->GetTargetGoal();
	float targetAngle = 0.0;
	float setpointAngle = 0.0;
	if (targetGoal == 1 || targetGoal == 6) {	// FIXME: Target Goal ID Handling
		setpointAngle = 60.0;
	} else if (targetGoal == 2 || targetGoal == 7) {
		//setpointAngle = teleop_mode ? 12.0 : 0.0;
		setpointAngle = 0.0;
	} else if (targetGoal == 3) {
		//setpointAngle = teleop_mode ? -3.0 : -60.0;
		setpointAngle = -60.0;
	}
	targetAngle = setpointAngle-90;
	Robot::driveBase->DriveControlTwist->SetSetpoint(setpointAngle);
	const float radians = targetAngle * M_PI / 180.0;
	float setpoint_radians = setpointAngle * M_PI / 180.0;
	float magnitude = xAdapter->GetOutputValue();
	if (fired) {
		magnitude = 0.001;
	} else if (kickCounter > 0) {
		magnitude += 0.005 * kickCounter;
	}
	float x = magnitude * sin(radians);
	float y = magnitude * cos(radians);

	if (teleop_mode) {
		// Calculate vector normal to our drive vector of joystick input vector
		float theta_j = Robot::oi->getDriverRight()->GetDirectionRadians();
		float theta_m = radians - theta_j;
		float joystick_magnitude = Robot::oi->getDriverRight()->GetMagnitude();
		float M = joystick_magnitude * sin(theta_m);
		x += M * sin(setpoint_radians + M_PI);
		y += M * cos(setpoint_radians + M_PI);
	} else {
		// Autonomous y adjustment
		if (!fired) {
			float yMagnitude = yAdapter->GetOutputValue();
			x += yMagnitude * sin(setpoint_radians + M_PI);
			y += yMagnitude * cos(setpoint_radians + M_PI);
		}
	}

	std::cout << "\tGoal X: " << goal.xposition
			<< "  Target Angle: " << targetAngle
			<< "  Setpoint Angle: " << setpointAngle
			<< "  Magnitude: " << magnitude
			<< "  Kick Cntr: " << kickCounter
			<< "  X: " << x
			<< "  Y: " << y
			<< "  DELTA TWIST: " << delta_twist << '\n';
	crab->Update(Robot::driveBase->CrabSpeedTwist->Get(), y, x, true);		// no twist, don't use Robot::driveBase->CrabSpeedTwist->Get()
	lastCrab.beater_bar = crab.get()->beater_bar;
	lastCrab.gyro = crab.get()->gyro;
	lastCrab.lock = crab.get()->lock;
	lastCrab.twist = crab.get()->twist;
	lastCrab.xspeed = crab.get()->xspeed;
	lastCrab.yspeed = crab.get()->yspeed;
	return false;
}
