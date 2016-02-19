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
	if(!prefs->ContainsKey("TwistP"))
		prefs->PutFloat("TwistP",.035);
	if(!prefs->ContainsKey("TwistI"))
		prefs->PutFloat("TwistI",0);
	if(!prefs->ContainsKey("TwistD"))
		prefs->PutFloat("TwistD",0.02);
	File = RAWCConstants::getInstance();
	driveBase->SetOffsets(File->getValueForKey("FLOff"), File->getValueForKey("FROff"), File->getValueForKey("RLOff"), File->getValueForKey("RROff"));
	dartOpen = false;
	dartSpeed = false;
	shooterRun = false;
	tankRun = false;
	firing = false;
	fireTime = 0;
	lowFiring = false;
	lowFireTime = 0;
	shooterSpeed = 0;
	feederSpeed = 0;
	beaterBarSpeed = 0;
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
	driveBase->DriveControlTwist->SetPID(prefs->GetFloat("TwistP"),prefs->GetFloat("TwistI"),prefs->GetFloat("TwistD"));
}

void Robot::TeleopPeriodic() {
	Scheduler::GetInstance()->Run();

	driveBase->SMDB();
	arm->SMDB();

	if(oi->DL1->Pressed())
		driveBase->Lock();
	else if(oi->DL4->Pressed()) {
		driveBase->DriveControlTwist->SetSetpoint(90);
		driveBase->Crab(driveBase->CrabSpeedTwist->Get(),-oi->getJoystickY(),oi->getJoystickX(),true);
	}
	else if(oi->DL5->Pressed()) {
		driveBase->DriveControlTwist->SetSetpoint(-90);
		driveBase->Crab(driveBase->CrabSpeedTwist->Get(),-oi->getJoystickY(),oi->getJoystickX(),true);
	}
	else if(oi->DL3->Pressed()) {
		driveBase->DriveControlTwist->SetSetpoint(0);
		driveBase->Crab(driveBase->CrabSpeedTwist->Get(),-oi->getJoystickY(),oi->getJoystickX(),true);
	}
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
		shooterSpeed = prefs->GetFloat("ShooterSpeed");
		feederSpeed = prefs->GetFloat("FeederSpeed");
	}

	if(oi->GPY->RisingEdge()) {
		arm->DartPosition(630);
		shooterRun = true;
		tankRun = false;
		shooterSpeed = prefs->GetFloat("ShooterSpeed");
		feederSpeed = prefs->GetFloat("FeederSpeed");
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
		shooterSpeed = prefs->GetFloat("ShooterSpeed");
		feederSpeed = prefs->GetFloat("FeederSpeed");

	}

	if(shooterRun && (oi->GPRT->RisingEdge() || oi->DR1->RisingEdge())) {
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

	if(oi->DR2->RisingEdge()) {
		lowFiring = true;
		arm->Fire(true);
		shooterRun = true;
		shooterSpeed = -13500;
		feederSpeed = 1.0;
		beaterBarSpeed = 1.0;
		lowFireTime = GetClock();
	}

	if(lowFiring && ((lowFireTime + 1 < GetClock())) ) {
		lowFiring = false;
		arm->Fire(false);
		shooterRun = false;
	}


	if (!lowFiring) {
		beaterBarSpeed = oi->getGamepad()->GetRawAxis(1);
	}

	arm->RunShooter(shooterRun, shooterSpeed,feederSpeed);

	arm->BeaterBar(beaterBarSpeed);

	arm->DartMonitor();

}

void Robot::TestPeriodic() {
	lw->Run();
}

START_ROBOT_CLASS(Robot);

