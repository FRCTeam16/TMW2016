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
#include <fstream>
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
    dartLeft->SetControlMode(CANTalon::kPosition);
    dartLeft->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
    dartLeft->ConfigLimitMode(CANSpeedController::kLimitMode_SoftPositionLimits);
    dartLeft->ConfigSoftPositionLimits(972,0);
    dartLeft->ConfigPeakOutputVoltage(dartMaxForward, dartMaxReverse);
//    dartLeft->SetPID(100,0,0);
    CANSpeedController *cspL = dynamic_cast<CANSpeedController*>(dartLeft.get());
    cspL->SetPID(30,0,0);

    dartRight->SetFeedbackDevice(CANTalon::AnalogPot);
    dartRight->SetControlMode(CANTalon::kPosition);
    dartRight->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
    dartRight->ConfigLimitMode(CANSpeedController::kLimitMode_SoftPositionLimits);
    dartRight->ConfigSoftPositionLimits(972 + dartOffset,0);
    dartRight->ConfigPeakOutputVoltage(dartMaxForward, dartMaxReverse);
//    dartRight->SetPID(100,0,0);
    CANSpeedController *cspR = dynamic_cast<CANSpeedController*>(dartRight.get());
	cspR->SetPID(30,0,0);


    shooterWheel->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);
    shooterWheel->SetFeedbackDevice(CANTalon::EncRising);
	shooterWheel->SetControlMode(CANTalon::kSpeed);
	shooterWheel->ConfigEncoderCodesPerRev(3);
	shooterWheel->SetVoltageRampRate(24);

	shooterWheel->SetP(18);
	shooterWheel->SetI(0);
	shooterWheel->SetD(0);
	shooterWheel->SetF(13);

	feederWheel->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);
	beaterBar->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);

	shooterSpeed = 0;
	feederSpeed = 0;
	shooterRun = false;

	firing = false;
	fireTime = 0;
	lowFiring = false;
	lowFireTime = 0;

	dartOutputUnlimited = false;

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

bool Arm::DartInPosition() const {
	return (dartLeft->Get() == dartLeft->GetSetpoint()) &&
			(dartRight->Get() == dartRight->GetSetpoint());
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

int Arm::GetCurrentDartDifference() {
	return dartRight->GetPosition() - dartLeft->GetPosition();
}

int Arm::GetCorrectedDartDifference() {
	return (dartRight->GetPosition() - dartOffset) - dartLeft->GetPosition();
}

void Arm::SetDartOffset(int offset) {
	dartOffset = offset;
}

void Arm::UnlimitDartOutput(bool unlimit) {

	dartOutputUnlimited = unlimit;

	if(unlimit)
		comp->Stop();
	else
		comp->Start();
}

void Arm::ClimbExtend() {
	DartPosition(extendLimit-10);
	if(dartLeft->GetPosition() < extendLimit) {
		climbLeft->Set(true);
		climbRight->Set(true);
	}
}

void Arm::ClimbRetract() {
	climbLeft->Set(false);
	climbRight->Set(false);
}


void Arm::RunShooter(bool run) {
	shooterRun = run;
}

void Arm::ToggleShooterRun() {
	shooterRun = !shooterRun;
}

void Arm::Fire() {
	if(shooterRun) {
	firing = true;
	fireTime = GetClock();
	fire->Set(true);
	}
}

void Arm::LowFire() {
	lowFiring = true;
	lowFireTime = GetClock();
	shooterRun = true;
	fire->Set(true);
}

void Arm::BeaterBar(float speed) {
	if(!lowFiring) {
		beaterBar->Set(speed);
	}
	else {
		beaterBar->Set(1.0);
	}

}


void Arm::ShooterHigh() {
	DartPosition(highShooter);
	shooterRun = true;
}
void Arm::ShooterLow() {
	DartPosition(lowShooter);
	shooterRun = true;

}
void Arm::PickupPosition() {
	DartPosition(pickup);
	shooterRun = false;
}
void Arm::TravelPosition() {
	DartPosition(travel);

}

void Arm::SetShooterSpeed(float ShooterSpeed, float FeederSpeed) {
	shooterSpeed = ShooterSpeed;
	feederSpeed = FeederSpeed;
}
void Arm::ShooterManager() {
	if(shooterRun) {
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

void Arm::FireManager() {
	if(firing && ((fireTime + 1) < GetClock())) {
		firing = false;
		fire->Set(false);
		shooterRun = false;
//		DartPosition(838);
	}

	if(lowFiring && ((lowFireTime + 1 < GetClock()))) {
		lowFiring = false;
		fire->Set(false);
		shooterRun = false;
	}
}

void Arm::DartManager() {
	if(dartOutputUnlimited) {
		dartLeft->ConfigPeakOutputVoltage(12, -12);
		dartRight->ConfigPeakOutputVoltage(12, -12);
	}
	else {
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
}


void Arm::SMDB() {
	SmartDashboard::PutNumber("DartOffset", dartOffset);
	SmartDashboard::PutNumber("Shooter Speed", shooterWheel->GetSpeed());
	SmartDashboard::PutNumber("Shooter Amps", shooterWheel->GetOutputCurrent());
	SmartDashboard::PutNumber("Feeder Amps", feederWheel->GetOutputCurrent());
	SmartDashboard::PutNumber("DartPosition", dartLeft->GetPosition());
	SmartDashboard::PutNumber("CorrectedDartDifference", GetCorrectedDartDifference());
}

std::vector<std::string> Arm::GetHeaders() {
	return std::vector<std::string> {
		"Left Dart Position", "Left Dart Volt", "Left Dart Current",
		"Right Dart Position","Right Dart Volt", "Right Dart Current",
		"Shooter Wheel Speed", "Shooter Wheel Volt", "Shooter Wheel Current"
		"Feeder Wheel Speed", "Feeder Wheel Volt", "Feeder Wheel Current",
		"Beater Wheel Speed", "Beater Wheel Volt", "Beater Wheel Current",
		"Fire Status",
		"Climb Left Status",
		"Climb Right Status"
	};
}

void Arm::Log(std::ofstream &outstream) {
		outstream	<< ',' << dartLeft->GetPosition()
					<< ',' << dartLeft->GetOutputVoltage()
					<< ',' << dartLeft->GetOutputCurrent()
					<< ',' << dartRight->GetPosition()
					<< ',' << dartRight->GetOutputVoltage()
					<< ',' << dartRight->GetOutputCurrent()
					<< ',' << shooterWheel->GetSpeed()
					<< ',' << shooterWheel->GetOutputVoltage()
					<< ',' << shooterWheel->GetOutputCurrent()
					<< ',' << feederWheel->GetSpeed()
					<< ',' << feederWheel->GetOutputVoltage()
					<< ',' << feederWheel->GetOutputCurrent()
					<< ',' << beaterBar->GetSpeed()
					<< ',' << beaterBar->GetOutputVoltage()
					<< ',' << beaterBar->GetOutputCurrent()
					<< ',' << fire->Get()
					<< ',' << climbLeft->Get()
					<< ',' << climbRight->Get();
}

