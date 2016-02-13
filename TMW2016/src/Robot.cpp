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
	prefs = Preferences::GetInstance();
	if(!prefs->ContainsKey("ShooterSpeed"))
		prefs->PutFloat("ShooterSpeed",15000);
	if(!prefs->ContainsKey("FeederSeed"))
		prefs->PutFloat("FeederSpeed", -1.0);
	File = RAWCConstants::getInstance();
	driveBase->SetOffsets(File->getValueForKey("FLOff"), File->getValueForKey("FROff"), File->getValueForKey("RLOff"), File->getValueForKey("RROff"));
	dartOpen = false;
	dartSpeed = false;
	shooterRun = false;
	tankRun = false;
	firing = false;
	fireTime = 0;
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
	arm->SMDB();

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

	driveBase->SMDB();
	arm->SMDB();

	prevTrigger = oi->getDriverRight()->GetRawButton(1);


//	prevTrigger = oi->getDriverRight()->GetRawButton(1);
	if(oi->getDriverLeft()->GetRawButton(1))
		driveBase->Lock();

	else if(oi->getDriverRight()->GetRawButton(1))
	{
		driveBase->Crab(0,-oi->getJoystickY(),0,true);
	}
//	else if (oi->getDriverLeft()->GetRawButton(2))
//	{
//		driveBase->Steer(oi->getLeftJoystickXRadians(),oi->getJoystickY(),.5);
//	}
//		else if (oi->getDriverLeft()->GetRawButton(3)) {
//			driveBase->Steer(3.14159,oi->getDriverLeft()->GetX(),2.1);
//		}
	else
	{
		driveBase->Crab(oi->getJoystickTwist(),-oi->getJoystickY(),oi->getJoystickX(),true);
	}

	if(tankRun) {
		if(fabs(driveBase->imu->GetYaw())>90) {
			driveBase->tankLeft->Set(oi->getDriverRight()->GetRawAxis(1));
			driveBase->tankRight->Set(oi->getDriverRight()->GetRawAxis(1));
		}
		else {
			driveBase->tankLeft->Set(-oi->getDriverRight()->GetRawAxis(1));
			driveBase->tankRight->Set(-oi->getDriverRight()->GetRawAxis(1));
		}
	}
	else {
		driveBase->tankLeft->Set(0);
		driveBase->tankRight->Set(0);
	}



	if(oi->GPA->RisingEdge()) {
		arm->DartPosition(940);
		shooterRun = false;
		tankRun = false;
	}

	if(oi->GPX->RisingEdge()) {
		arm->DartPosition(720);
		shooterRun = true;
		tankRun = false;
	}

	if(oi->GPY->RisingEdge()) {
		arm->DartPosition(630);
		shooterRun = true;
		tankRun = false;
		}

	if(oi->GPLT->RisingEdge()) {
		arm->DartPosition(838);
		tankRun = true;
	}

	if(fabs(oi->getGamepad()->GetRawAxis(5)) > .05) {
		arm->DartSpeedControl(oi->getGamepad()->GetRawAxis(5));
		dartSpeed = true;
	}
	else if(dartSpeed) {
		arm->DartSetToCurrent();
		dartSpeed = false;
	}

	if(oi->getGamepad()->GetPOV() == 0) {
		arm->ClimbExtend();
	}

	if(oi->getGamepad()->GetPOV() == 180) {
		arm->ClimbRetract();
	}

	if(oi->GPB->RisingEdge()) {
		shooterRun = !shooterRun;
	}

	if(shooterRun && oi->GPRT->RisingEdge()) {// || oi->driverFire->RisingEdge()) {
		firing = true;
		arm->Fire(true);
		fireTime = GetClock();
	}

	if(firing && ((fireTime + 1) < GetClock())) {
		firing = false;
		arm->Fire(false);
		shooterRun = false;
		arm->DartPosition(838);
	}

	arm->RunShooter(shooterRun, prefs->GetFloat("ShooterSpeed"),prefs->GetFloat("FeederSpeed"));

	arm->BeaterBar(oi->getGamepad()->GetRawAxis(1));

	arm->DartMonitor();

}

void Robot::TestPeriodic() {
	lw->Run();
}

START_ROBOT_CLASS(Robot);

