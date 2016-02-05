// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "Robot.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INITIALIZATION
std::shared_ptr<DriveBase> Robot::driveBase;
std::shared_ptr<Arm> Robot::arm;
std::unique_ptr<OI> Robot::oi;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INITIALIZATION

void Robot::RobotInit() {
	RobotMap::init();
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
    driveBase.reset(new DriveBase());
    arm.reset(new Arm());

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
	// This MUST be here. If the OI creates Commands (which it very likely
	// will), constructing it during the construction of CommandBase (from
	// which commands extend), subsystems are not guaranteed to be
	// yet. Thus, their requires() statements may grab null pointers. Bad
	// news. Don't move it.
	oi.reset(new OI());

	// instantiate the command used for the autonomous period
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=AUTONOMOUS
	autonomousCommand.reset(new AutonomousCommand());

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=AUTONOMOUS
	driveBase->SetWheelbase(22.5/2, 21.75, 22.5/2);

	File = RAWCConstants::getInstance();
	driveBase->SetOffsets(File->getValueForKey("FLOff"), File->getValueForKey("FROff"), File->getValueForKey("RLOff"), File->getValueForKey("RROff"));
	dartOpen = false;
  }

/**
 * This function is called when the disabled button is hit.
 * You can use it to reset subsystems before shutting down.
 */
void Robot::DisabledInit(){

}

void Robot::DisabledPeriodic() {
	Scheduler::GetInstance()->Run();
	driveBase->SMDB();

}

void Robot::AutonomousInit() {
	if (autonomousCommand.get() != nullptr)
		autonomousCommand->Start();
}

void Robot::AutonomousPeriodic() {
	Scheduler::GetInstance()->Run();
}

void Robot::TeleopInit() {
	// This makes sure that the autonomous stops running when
	// teleop starts running. If you want the autonomous to
	// continue until interrupted by another command, remove
	// these lines or comment it out.
	if (autonomousCommand.get() != nullptr)
		autonomousCommand->Cancel();
}

void Robot::TeleopPeriodic() {
	Scheduler::GetInstance()->Run();

	//Resets gyro to zero when crab starts
	if (!prevTrigger && oi->getDriverRight()->GetRawButton(12)) {
		driveBase->imu->ZeroYaw();
	}

	prevTrigger = oi->getDriverRight()->GetRawButton(1);


//	prevTrigger = oi->getDriverRight()->GetRawButton(1);
	if(oi->getDriverLeft()->GetRawButton(1))
		driveBase->Lock();

	else if(oi->getDriverRight()->GetRawButton(1))
	{
		driveBase->Crab(0,-oi->getJoystickY(),0,true);
	}
	else if (oi->getDriverLeft()->GetRawButton(2))
	{
		driveBase->Steer(oi->getLeftJoystickXRadians(),oi->getJoystickY(),.5);
	}
//		else if (oi->getDriverLeft()->GetRawButton(3)) {
//			driveBase->Steer(3.14159,oi->getDriverLeft()->GetX(),2.1);
//		}
	else
	{
		driveBase->Crab(oi->getJoystickTwist(),-oi->getJoystickY(),oi->getJoystickX(),true);
	}

	driveBase->tankLeft->Set(-oi->getDriverRight()->GetRawAxis(1));
	driveBase->tankRight->Set(-oi->getDriverRight()->GetRawAxis(1));

	driveBase->SMDB();


	if(oi->getGamepad()->GetRawButton(5)) {
		arm->DartPosition(500);
	}

	if(oi->getGamepad()->GetRawButton(6)) {
		arm->DartPosition(800);
	}


	if(fabs(oi->getGamepad()->GetRawAxis(5)) > .05) {
		arm->DartOpenLoop(oi->getGamepad()->GetRawAxis(5));
		dartOpen = true;
	}
	else if(dartOpen) {
		arm->DartSetToCurrent();
		dartOpen = false;
	}
}

void Robot::TestPeriodic() {
	lw->Run();
}

START_ROBOT_CLASS(Robot);

