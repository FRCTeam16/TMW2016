// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.




#include "Arm.h"
#include "../RobotMap.h"
// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

Arm::Arm() : Subsystem("Arm") {
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
    dartLeft = RobotMap::armDartLeft;
    dartRight = RobotMap::armDartRight;
    comp = RobotMap::armComp;
    climbLeft = RobotMap::armClimbLeft;
    climbRight = RobotMap::armClimbRight;
    beaterBar = RobotMap::armBeaterBar;
    feederWheel = RobotMap::armFeederWheel;
    shooterWheel = RobotMap::armShooterWheel;
    fire = RobotMap::armFire;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS

    dartMaxForward = 5;
    dartMaxReverse = -12;
	File = RAWCConstants::getInstance();
	dartOffset = File->getValueForKey("DartOffset");
    dartLeft->SetFeedbackDevice(CANTalon::AnalogPot);
    dartRight->SetFeedbackDevice(CANTalon::AnalogPot);
    dartLeft->SetControlMode(CANTalon::kPosition);
    dartRight->SetControlMode(CANTalon::kPosition);
    dartLeft->ConfigSoftPositionLimits(972,0);
    dartLeft->ConfigSoftPositionLimits(972 + dartOffset,0);
    dartLeft->ConfigPeakOutputVoltage(dartMaxForward, dartMaxReverse);
    dartRight->ConfigPeakOutputVoltage(dartMaxForward, dartMaxReverse);
	shooterWheel->SetFeedbackDevice(CANTalon::EncRising);
	shooterWheel->SetControlMode(CANTalon::kSpeed);
	shooterWheel->ConfigEncoderCodesPerRev(3);

	shooterWheel->SetP(15);
	shooterWheel->SetI(0);
	shooterWheel->SetD(0);
	shooterWheel->SetF(12);

}


void Arm::InitDefaultCommand() {
    // Set the default command for a subsystem here.
    // SetDefaultCommand(new MySpecialCommand());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND


    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND
}


// Put methods for controlling this subsystem
// here. Call these from Commands.

void Arm::Darter(float speed) {
	dartLeft->Set(speed);
	dartRight->Set(speed);
}

void Arm::DartPosition(int pos) {
	dartLeft->SetControlMode(CANTalon::kPosition);
	dartRight->SetControlMode(CANTalon::kPosition);
	dartLeft->Set(pos);
	dartRight->Set(pos+dartOffset);
}

void Arm::DartSetToCurrent() {
	DartPosition(dartLeft->GetPosition());

}

void Arm::DartOpenLoop(float speed) {
	dartLeft->SetControlMode(CANTalon::kPercentVbus);
	dartRight->SetControlMode(CANTalon::kPercentVbus);

	dartLeft->Set(speed);
	dartRight->Set(speed);
}

void Arm::DartSpeedControl(float speed) {
	dartLeft->SetControlMode(CANTalon::kPercentVbus);
	dartRight->SetControlMode(CANTalon::kPosition);

	dartLeft->Set(speed);
	dartRight->Set(dartLeft->GetPosition()+dartOffset);
}

void Arm::DartMonitor() {
	if(GetCorrectedDartDifference() > 5) { //right dart is lower than left
		if(dartLeft->Get() > 0) { //going down
			dartRight->ConfigPeakOutputVoltage(2,dartMaxReverse);
		}
		if(dartLeft->Get() < 0) { //going up
			dartLeft->ConfigPeakOutputVoltage(dartMaxForward,-1);
		}
	}

	else if(GetCorrectedDartDifference() < -5) { //left dart lower than right
		if(dartLeft->Get() > 0) { //going down
			dartLeft->ConfigPeakOutputVoltage(2,dartMaxReverse);
		}
		if(dartLeft->Get() < 0) { //going up
			dartRight->ConfigPeakOutputVoltage(dartMaxForward,-1);
		}
	}
	else {
		dartLeft->ConfigPeakOutputVoltage(dartMaxForward,dartMaxReverse);
		dartRight->ConfigPeakOutputVoltage(dartMaxForward,dartMaxReverse);
	}
}

int Arm::GetCurrentDartDifference() {
	return dartRight->GetPosition() - dartLeft->GetPosition();
}

int Arm::GetCorrectedDartDifference() {
	return (dartRight->GetPosition() - dartOffset) - dartLeft->GetPosition();
}

void Arm::SetDartOffset(int offset) {
	dartOffset = offset;
}

void Arm::ClimbExtend() {
	climbLeft->Set(true);
	climbRight->Set(true);
}

void Arm::ClimbRetract() {
	climbLeft->Set(false);
	climbRight->Set(false);
}

void Arm::RunShooter(bool run, float shooterSpeed, float feederSpeed) {
	if(run) {
		shooterWheel->Set(shooterSpeed);
		feederWheel->Set(feederSpeed);
		comp->Stop();
	}
	else {
		shooterWheel->Set(0);
		feederWheel->Set(0);
		comp->Start();
		}
}

void Arm::Fire(bool value) {
	fire->Set(value);
}

void Arm::BeaterBar(float speed) {
	beaterBar->Set(speed);
}

void Arm::SMDB() {
	SmartDashboard::PutNumber("DartOffset", dartOffset);
	SmartDashboard::PutNumber("Shooter Speed", shooterWheel->GetSpeed());
	SmartDashboard::PutNumber("Shooter Amps", shooterWheel->GetOutputCurrent());
	SmartDashboard::PutNumber("Feeder Amps", feederWheel->GetOutputCurrent());
	SmartDashboard::PutNumber("DartPosition", dartLeft->GetPosition());
}
