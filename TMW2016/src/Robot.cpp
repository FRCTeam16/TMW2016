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
#include "Autonomous/AlignWithGoal.h"

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

	// Vision PID
	if (!prefs->ContainsKey("VisionXP")) { prefs->PutFloat("VixionXP", 1.0); }
	if (!prefs->ContainsKey("VisionXI")) { prefs->PutFloat("VixionXI", 0.0); }
	if (!prefs->ContainsKey("VisionXD")) { prefs->PutFloat("VixionXD", 0.0); }

	File = RAWCConstants::getInstance();
	driveBase->SetOffsets(File->getValueForKey("FLOff"), File->getValueForKey("FROff"), File->getValueForKey("RLOff"), File->getValueForKey("RROff"));
	dartOpen = false;
	dartSpeed = false;
	tankRun = false;
	shooterSpeed = 0;
	feederSpeed = 0;
	beaterBarSpeed = 0;

	visionServer.reset(new VisionServer(5800));
	ledDisplay.reset(new LEDDisplay());
	automan.reset(new AutoManager(visionServer.get()));
	safetyChecker.reset(new SafetyChecker());
  }

/**
 * This function is called when the disabled button is hit.
 * You can use it to reset subsystems before shutting down.
 */
void Robot::DisabledInit(){
	cout << "Robot::DisabledInit";
}

void Robot::DisabledPeriodic() {
	Scheduler::GetInstance()->Run();
	driveBase->SMDB();
	arm->SMDB();
	visionServer->SMDB();
}

void Robot::AutonomousInit() {
	cout << "Robot::AutonomousInit\n";
	automan.reset(new AutoManager(visionServer.get()));
	dataLogger.reset(new DataLogger(kAutonomous, driveBase, arm, oi.get(), visionServer.get()));
	Robot::driveBase->DriveControlTwist->SetOutputRange(-0.25, 0.25);
	automan->Init();
	cout << "Robot::AutonomousInit COMPLETE\n";
}

void Robot::AutonomousPeriodic() {
	if (safetyChecker->IsFailed() || !safetyChecker->CheckLimits()) {
		safetyChecker->HaltPeriodic();
	} else {
		automan->Periodic();
	}
	LogData();
	driveBase->SMDB();
	visionServer->SMDB();
	UpdateLED();
}

void Robot::TeleopInit() {
	dataLogger.reset(new DataLogger(kTeleop, driveBase, arm, oi.get(), visionServer.get()));
	// This makes sure that the autonomous stops running when
	// teleop starts running. If you want the autonomous to
	// continue until interrupted by another command, remove
	// these lines or comment it out.
	if (autonomousCommand.get() != nullptr)
		autonomousCommand->Cancel();
	Robot::driveBase->EnableSteerPIDControllers(true);
	driveBase->DriveControlTwist->SetPID(prefs->GetFloat("TwistP"),prefs->GetFloat("TwistI"),prefs->GetFloat("TwistD"));
	driveBase->DriveControlTwist->SetOutputRange(-.5, .5);
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
			driveBase->RunTanks(oi->getDriverRight()->GetRawAxis(1));
		}
		else {
			driveBase->RunTanks(-oi->getDriverRight()->GetRawAxis(1));
		}
	}
	else {
		driveBase->RunTanks(0);
	}


/*********Climber*********/

	if(oi->getGamepad()->GetPOV() == 0) {
		arm->ClimbExtend();
	}

	if(oi->getGamepad()->GetPOV() == 180 || oi->DR4->RisingEdge()) {
		arm->ClimbRetract();
	}


/*********Shooter/Arm*********/
	if(oi->GPA->RisingEdge() || oi->DR3->RisingEdge()) {
		arm->PickupPosition();
		tankRun = false;
	}

	if(oi->GPB->RisingEdge()) {
		arm->ToggleShooterRun();
		arm->SetShooterSpeed(prefs->GetFloat("ShooterSpeed"),prefs->GetFloat("FeederSpeed"));
	}

	if(oi->GPX->RisingEdge()) {
		arm->ShooterLow();
		arm->SetShooterSpeed(prefs->GetFloat("ShooterSpeed"),prefs->GetFloat("FeederSpeed"));
		tankRun = false;
	}

	if(oi->GPY->RisingEdge()) {
		arm->ShooterHigh();
		arm->SetShooterSpeed(prefs->GetFloat("ShooterSpeed"),prefs->GetFloat("FeederSpeed"));
		tankRun = false;
	}

	if(oi->GPLT->RisingEdge()) {
		arm->TravelPosition();
		tankRun = true;
	}

	if(oi->GPRT->RisingEdge() || oi->DR1->RisingEdge()) {
		arm->Fire();
	}

	if(oi->DR2->RisingEdge()) {
		arm->LowFire();
		arm->SetShooterSpeed(0,1.0);
	}

/********Dart Open Loop*******/
	if(fabs(oi->getGamepad()->GetRawAxis(5)) > .05) {
		arm->DartSpeedControl(oi->getGamepad()->GetRawAxis(5));
		dartSpeed = true;
	}

	else if(dartSpeed) {
		arm->DartSetToCurrent();
		dartSpeed = false;
	}

	arm->UnlimitDartOutput(oi->GPBack->Pressed());


/*******Arm Managers********/
	arm->ShooterManager();
	arm->FireManager();
	arm->DartManager();


/********Beater Bar********/
	arm->BeaterBar(oi->getGamepad()->GetRawAxis(1));

/********Logging and display updates********/
	LogData();
	visionServer->SMDB();
	UpdateLED();
}

void Robot::TestPeriodic() {
	lw->Run();
	UpdateLED();
}

void Robot::LogData() {
	if (dataLogger.get() != nullptr && ++logCounter >= LOG_FREQ) {
		dataLogger->Log();
		logCounter = 0;
	}
}

void Robot::UpdateLED() {
	ledDisplay->Update(visionServer->GetVisionData());
//	ledDisplay->Update(
//				oi->getDriverLeft()->GetRawAxis(2),
//				oi->getDriverRight()->GetRawAxis(3));
}

START_ROBOT_CLASS(Robot);

