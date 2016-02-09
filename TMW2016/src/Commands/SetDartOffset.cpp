// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "SetDartOffset.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

SetDartOffset::SetDartOffset(): Command() {
        // Use requires() here to declare subsystem dependencies
    // eg. requires(chassis);
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	File = RAWCConstants::getInstance();
	SetRunWhenDisabled(true);

}
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

// Called just before this Command runs the first time
void SetDartOffset::Initialize() {
	File = RAWCConstants::getInstance();
	if(DriverStation::GetInstance().IsDisabled()){
		File->insertKeyAndValue("DartOffset", Robot::arm->GetCurrentDartDifference());
		File->save();
		Robot::arm->SetDartOffset(Robot::arm->GetCurrentDartDifference());
		SetTimeout(1);
	}
	else {
		SetTimeout(0);
	}

}

// Called repeatedly when this Command is scheduled to run
void SetDartOffset::Execute() {

}

// Make this return true when this Command no longer needs to run execute()
bool SetDartOffset::IsFinished() {
	return IsTimedOut();
}

// Called once after isFinished returns true
void SetDartOffset::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void SetDartOffset::Interrupted() {

}
