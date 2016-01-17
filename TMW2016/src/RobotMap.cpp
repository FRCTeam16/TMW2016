// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "RobotMap.h"
#include "LiveWindow/LiveWindow.h"


// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=ALLOCATION
std::shared_ptr<SpeedController> RobotMap::driveBaseFrontLeftDrive;
std::shared_ptr<SpeedController> RobotMap::driveBaseFrontRightDrive;
std::shared_ptr<SpeedController> RobotMap::driveBaseRearLeftDrive;
std::shared_ptr<SpeedController> RobotMap::driveBaseRearRightDrive;
std::shared_ptr<AnalogInput> RobotMap::driveBaseFrontLeftPos;
std::shared_ptr<CANTalon> RobotMap::driveBaseFrontLeftSteer;
std::shared_ptr<PIDController> RobotMap::driveBaseFrontLeft;
std::shared_ptr<AnalogInput> RobotMap::driveBaseFrontRightPos;
std::shared_ptr<CANTalon> RobotMap::driveBaseFrontRightSteer;
std::shared_ptr<PIDController> RobotMap::driveBaseFrontRight;
std::shared_ptr<AnalogInput> RobotMap::driveBaseRearLeftPos;
std::shared_ptr<CANTalon> RobotMap::driveBaseRearLeftSteer;
std::shared_ptr<PIDController> RobotMap::driveBaseRearLeft;
std::shared_ptr<AnalogInput> RobotMap::driveBaseRearRightPos;
std::shared_ptr<CANTalon> RobotMap::driveBaseRearRightSteer;
std::shared_ptr<PIDController> RobotMap::driveBaseRearRight;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=ALLOCATION

void RobotMap::init() {
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
    LiveWindow *lw = LiveWindow::GetInstance();

    driveBaseFrontLeftDrive.reset(new Talon(0));
    lw->AddActuator("DriveBase", "FrontLeftDrive", (Talon&) driveBaseFrontLeftDrive);
    
    driveBaseFrontRightDrive.reset(new Talon(1));
    lw->AddActuator("DriveBase", "FrontRightDrive", (Talon&) driveBaseFrontRightDrive);
    
    driveBaseRearLeftDrive.reset(new Talon(2));
    lw->AddActuator("DriveBase", "RearLeftDrive", (Talon&) driveBaseRearLeftDrive);
    
    driveBaseRearRightDrive.reset(new Talon(3));
    lw->AddActuator("DriveBase", "RearRightDrive", (Talon&) driveBaseRearRightDrive);
    
    driveBaseFrontLeftPos.reset(new AnalogInput(0));
    lw->AddSensor("DriveBase", "FrontLeftPos", driveBaseFrontLeftPos);
    
    driveBaseFrontLeftSteer.reset(new CANTalon(6));
    lw->AddActuator("DriveBase", "FrontLeftSteer", driveBaseFrontLeftSteer);
    
    driveBaseFrontLeft.reset(new PIDController(0.1, 0.0, 0.0,/* F: 0.0, */ driveBaseFrontLeftPos.get(), driveBaseFrontLeftSteer.get(), 0.02));
    lw->AddActuator("DriveBase", "FrontLeft", driveBaseFrontLeft);
    driveBaseFrontLeft->SetContinuous(true); driveBaseFrontLeft->SetAbsoluteTolerance(0.2); 
        driveBaseFrontLeft->SetInputRange(0.0, 5.0);
        driveBaseFrontLeft->SetOutputRange(-0.75, 0.75);
    driveBaseFrontRightPos.reset(new AnalogInput(1));
    lw->AddSensor("DriveBase", "FrontRightPos", driveBaseFrontRightPos);
    
    driveBaseFrontRightSteer.reset(new CANTalon(7));
    lw->AddActuator("DriveBase", "FrontRightSteer", driveBaseFrontRightSteer);
    
    driveBaseFrontRight.reset(new PIDController(0.1, 0.0, 0.0,/* F: 0.0, */ driveBaseFrontRightPos.get(), driveBaseFrontRightSteer.get(), 0.02));
    lw->AddActuator("DriveBase", "FrontRight", driveBaseFrontRight);
    driveBaseFrontRight->SetContinuous(true); driveBaseFrontRight->SetAbsoluteTolerance(0.2); 
        driveBaseFrontRight->SetInputRange(0.0, 5.0);
        driveBaseFrontRight->SetOutputRange(-0.75, 0.75);
    driveBaseRearLeftPos.reset(new AnalogInput(2));
    lw->AddSensor("DriveBase", "RearLeftPos", driveBaseRearLeftPos);
    
    driveBaseRearLeftSteer.reset(new CANTalon(8));
    lw->AddActuator("DriveBase", "RearLeftSteer", driveBaseRearLeftSteer);
    
    driveBaseRearLeft.reset(new PIDController(0.1, 0.0, 0.0,/* F: 0.0, */ driveBaseRearLeftPos.get(), driveBaseRearLeftSteer.get(), 0.02));
    lw->AddActuator("DriveBase", "RearLeft", driveBaseRearLeft);
    driveBaseRearLeft->SetContinuous(true); driveBaseRearLeft->SetAbsoluteTolerance(0.2); 
        driveBaseRearLeft->SetInputRange(0.0, 5.0);
        driveBaseRearLeft->SetOutputRange(-0.75, 0.75);
    driveBaseRearRightPos.reset(new AnalogInput(3));
    lw->AddSensor("DriveBase", "RearRightPos", driveBaseRearRightPos);
    
    driveBaseRearRightSteer.reset(new CANTalon(9));
    lw->AddActuator("DriveBase", "RearRightSteer", driveBaseRearRightSteer);
    
    driveBaseRearRight.reset(new PIDController(0.1, 0.0, 0.0,/* F: 0.0, */ driveBaseRearRightPos.get(), driveBaseRearRightSteer.get(), 0.02));
    lw->AddActuator("DriveBase", "RearRight", driveBaseRearRight);
    driveBaseRearRight->SetContinuous(true); driveBaseRearRight->SetAbsoluteTolerance(0.2); 
        driveBaseRearRight->SetInputRange(0.0, 5.0);
        driveBaseRearRight->SetOutputRange(-0.75, 0.75);


    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
}
