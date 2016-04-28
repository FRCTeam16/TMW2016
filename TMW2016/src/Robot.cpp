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
	if(!prefs->ContainsKey("FeederSeed"))
		prefs->PutFloat("FeederSpeed", -1.0);
	if(!prefs->ContainsKey("TwistP"))
		prefs->PutFloat("TwistP",0.02);
	if(!prefs->ContainsKey("TwistI"))
		prefs->PutFloat("TwistI",0.0);
	if(!prefs->ContainsKey("TwistD"))
		prefs->PutFloat("TwistD",0.12);
	if(!prefs->ContainsKey("LongShooterSpeed")) { prefs->PutFloat("LongShooterSpeed", 16000); }
	if(!prefs->ContainsKey("LongShooterArm")) { prefs->PutFloat("LongShooterArm", 570); }
	if(!prefs->ContainsKey("ShortShooterSpeed")) { prefs->PutFloat("ShortShooterSpeed", 11500); }
	if(!prefs->ContainsKey("ShortShooterArm")) { prefs->PutFloat("ShortShooterArm", 720); }
	if(!prefs->ContainsKey("WallShotOffset")) { prefs->PutFloat("WallShotOffset", 0); }
	if(!prefs->ContainsKey("AutoReturnOffset2")) { prefs->PutFloat("AutoReturnOffset2", 200); }
	if(!prefs->ContainsKey("AutoReturnOffset3")) { prefs->PutFloat("AutoReturnOffset3", 92); }
	if(!prefs->ContainsKey("AutoReturnOffset4")) { prefs->PutFloat("AutoReturnOffset4", -35); }
	if(!prefs->ContainsKey("AutoReturnOffset5")) { prefs->PutFloat("AutoReturnOffset5", -131); }
	if(!prefs->ContainsKey("PickupArm")) { prefs->PutFloat("PickupArm", 910); }
	if(!prefs->ContainsKey("TravelArm")) { prefs->PutFloat("TravelArm", 838); }


	SmartDashboard::PutNumber(AUTO_DELAY, 0.0);
	SmartDashboard::PutBoolean(AUTO_C_TYPE_RETURN, false);
	SmartDashboard::PutBoolean(AUTO_TWO_BALL_PREVENT, false);

	// Vision
	if (!prefs->ContainsKey("VisionCenterOffset")) { prefs->PutInt("VisionCenterOffset", -11); }

	File = RAWCConstants::getInstance();
	driveBase->SetOffsets(File->getValueForKey("FLOff"), File->getValueForKey("FROff"), File->getValueForKey("RLOff"), File->getValueForKey("RROff"));
	dartOpen = false;
	dartSpeed = false;
	tankRun = false;
	shooterSpeed = 0;
	feederSpeed = 0;
	beaterBarSpeed = 0;

	world.reset(new World());
	visionServer.reset(new VisionServer(5800));
	ledDisplay.reset(new LEDDisplay());
	automan.reset(new AutoManager(visionServer.get()));
	safetyChecker.reset(new SafetyChecker());
	teleopAutoHighShootStrategy.reset(new TeleopAutoHighShootStrategy());
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
	UpdateLED();

	const int startingDefenseIdx = (int) automan->defense->GetSelected();
	const int startingPosition 	 = (int) automan->position->GetSelected();
	const int targetGoal		 = (int) automan->target->GetSelected();

	SmartDashboard::PutNumber("Selected Position", startingPosition);
	SmartDashboard::PutNumber("Selected Defense", startingDefenseIdx);
	SmartDashboard::PutNumber("Selected Target", targetGoal);

//	cout << "Joystick Theta: " << oi->getDriverRight()->GetDirectionRadians() * 180.0 / M_PI
//			<< "\t Joystick Magni: " << oi->getDriverRight()->GetMagnitude() << '\n';

}

void Robot::AutonomousInit() {
	cout << "Robot::AutonomousInit\n";
	automan.reset(new AutoManager(visionServer.get()));
	dataLogger.reset(new DataLogger(kAutonomous, driveBase, arm, oi.get(), visionServer.get()));
	Robot::driveBase->DriveControlTwist->SetOutputRange(-0.5, 0.5);
	automan->Init(world.get());
	cout << "Robot::AutonomousInit COMPLETE\n";
}

void Robot::AutonomousPeriodic() {
	UpdateWorld();
	if (safetyChecker->IsFailed() || !safetyChecker->CheckLimits()) {
		safetyChecker->HaltPeriodic();
	} else {
		automan->Periodic(world.get());
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
	driveBase->DriveControlTwist->SetPID(prefs->GetFloat("TwistP", 0.02),prefs->GetFloat("TwistI", 0.0),prefs->GetFloat("TwistD", 0.12));
	driveBase->DriveControlTwist->SetOutputRange(-.5, .5);
}

void Robot::TeleopPeriodic() {
	Scheduler::GetInstance()->Run();
	UpdateWorld();
	driveBase->SMDB();
	arm->SMDB();

	if (!oi->DR3->Pressed()) {
		if(oi->DL1->Pressed()) {
			driveBase->Lock();
		}
		else if(oi->DL4->Pressed()) {
			driveBase->DriveControlTwist->SetSetpoint(-120.0);
			driveBase->Crab(driveBase->CrabSpeedTwist->Get(),-oi->getJoystickY(),oi->getJoystickX(),true);
		}
		else if(oi->DL5->Pressed()) {
			driveBase->DriveControlTwist->SetSetpoint(120.0);
			driveBase->Crab(driveBase->CrabSpeedTwist->Get(),-oi->getJoystickY(),oi->getJoystickX(),true);
		}
		else if(oi->DL3->Pressed()) {
			driveBase->DriveControlTwist->SetSetpoint(0);
			driveBase->Crab(driveBase->CrabSpeedTwist->Get(),-oi->getJoystickY(),oi->getJoystickX(),true);
		}
		else if(oi->DL2->Pressed()) {
			driveBase->DriveControlTwist->SetSetpoint(180.0);
			driveBase->Crab(driveBase->CrabSpeedTwist->Get(),-oi->getJoystickY(),oi->getJoystickX(),true);
		}
		else if(oi->DR5->Pressed()) {
			driveBase->DriveControlTwist->SetSetpoint(60.0);
			driveBase->Crab(driveBase->CrabSpeedTwist->Get(),-oi->getJoystickY(),oi->getJoystickX(),true);
		}
		else if(oi->DR6->Pressed()) {
			driveBase->DriveControlTwist->SetSetpoint(-60.0);
			driveBase->Crab(driveBase->CrabSpeedTwist->Get(),-oi->getJoystickY(),oi->getJoystickX(),true);
		}
		else
		{
			driveBase->Crab(oi->getJoystickTwist(),-oi->getJoystickY(),oi->getJoystickX(),true);
		}
	} else {
		// modifier for auto high shooting enabled
		int target = -1;
		if(oi->DL1->Pressed()) {
			driveBase->Lock();
		}
		else if(oi->DL4->Pressed()) {
			target = 1;
		}
		else if(oi->DL5->Pressed()) {
			target = 2;
		}
		else if(oi->DL3->Pressed()) {
			target = 2;
		}
		else
		{
			driveBase->Crab(oi->getJoystickTwist(),-oi->getJoystickY(),oi->getJoystickX(),true);
		}

		if (target > 0) {
			world->SetTargetGoal(target);
			teleopAutoHighShootStrategy->RunPeriodic(world.get());
		} else {
			teleopAutoHighShootStrategy->Reset();
		}
	}

	if (oi->DL8->Pressed()) {
		float currentOffset = Preferences::GetInstance()->GetFloat("WallShotOffset", 0);
		Preferences::GetInstance()->PutFloat("WallShotOffset", currentOffset-1);
	}
	if (oi->DL9->Pressed()) {
		float currentOffset = Preferences::GetInstance()->GetFloat("WallShotOffset", 0);
		Preferences::GetInstance()->PutFloat("WallShotOffset", currentOffset+1);
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
	if(oi->GPA->RisingEdge() /*|| oi->DR3->RisingEdge()*/) {
		arm->SetWallshotDart(false);
		arm->PickupPosition();
		tankRun = false;
	}

	if(oi->GPB->RisingEdge()) {
		arm->SetWallshotDart(false);
		arm->SetShooterSpeed(prefs->GetFloat("ShortShooterSpeed"),prefs->GetFloat("FeederSpeed"));
		arm->ShooterLow();
		tankRun = false;
	}

	if(oi->GPX->RisingEdge()) {
		arm->SetWallshotDart(true);
		arm->SetShooterSpeed(prefs->GetFloat("LongShooterSpeed"),prefs->GetFloat("FeederSpeed"));
		arm->ShooterHigh();
		tankRun = false;
	}

	if(oi->GPY->RisingEdge()) {
		arm->SetWallshotDart(false);
		arm->SetShooterSpeed(prefs->GetFloat("LongShooterSpeed"),prefs->GetFloat("FeederSpeed"));
		arm->ShooterHigh();
		tankRun = false;
	}

	if(oi->GPLT->RisingEdge()) {
		arm->SetWallshotDart(false);
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

	if (oi->GPStart->RisingEdge()) {
		arm->ToggleShooterRun();
		arm->SetShooterSpeed(prefs->GetFloat("LongShooterSpeed"),prefs->GetFloat("FeederSpeed"));	// TODO: Needed?
	}

	if(oi->DR9->RisingEdge()) {
		arm->IncDart();
	}

	if(oi->DR11->RisingEdge()) {
		arm->DecDart();
	}

/********Dart Open Loop*******/
	if(fabs(oi->getGamepad()->GetRawAxis(5)) > .05) {
		arm->SetWallshotDart(false);
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

//	float twist = driveBase->imu->GetYaw();
//	cout << "*** DELTA TWIST: " << last_twist - twist << "\n";
//	last_twist = twist;
}



void Robot::UpdateWorld() {
	world->Update(visionServer->GetVisionData());
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

void Robot::TestCollision() {
	double current_accel_x = Robot::driveBase->imu->GetWorldLinearAccelX();
	double current_jerk_x = current_accel_x - last_accel_x;
	last_accel_x = current_accel_x;

	double current_accel_y = Robot::driveBase->imu->GetWorldLinearAccelY();
	double current_jerk_y = current_accel_y - last_accel_y;
	last_accel_y = current_accel_y;

	std::cout << "Last X: " << last_accel_x << '\n'
			  << "Cur  X: " << current_accel_x << "\n"
			  << "Last Y: " << last_accel_y << '\n'
			  << "Cur  Y: " << current_accel_y << '\n';


	std::cout << "jerk: x->" << current_jerk_x << " y-> " << current_jerk_y << "\n";
}

START_ROBOT_CLASS(Robot);

