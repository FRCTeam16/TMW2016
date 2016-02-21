// RobotBuilder Version: 1.5
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#ifndef DRIVEBASE_H
#define DRIVEBASE_H
#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "AHRS.h"
#include "CrabSpeed.h"


class DriveBase: public Subsystem {
private:

	float gyroreference;

	bool GyroZeroFlag;
	float GyroZeroTime;

	//Steering Functions
	void LeftTurn4Wheels();	//Calculates a left hand turn
	void RightTurn4Wheels();	//Calculates a right hand turn


	//DriveDirection
	bool driveFront;

	//Steering Variables
	double radian; // input steering angle in radians
	double A;	//A is the ratio of X to turn harder
	double W;	//W is the distance form the mid-point of one back wheel to the mid-point of the other back wheel
	double X;	//X is the distance form the mid-point of the back wheels to the mid-point of the front wheels
	double Y;	//Y is the distance from the mid-point of one front wheel to the mid-point of the other font wheel

	double FL;	//FL, distance from Front Left Wheel to the center of rotation
	double FR;	//FR, distance from Front Right Wheel to the center of rotation
	double RL;	//RL, distance from Rear Left Wheel to the center of rotation
	double RR;	//RR, distance from Rear Right Wheel to the center of rotation

	double Z;	//Z, distance form A * X to center of rotation

	double thetaRC;	//Angle used to calculate all other angles

	double thetaFL;	//thetaFL, angle from Front Left Wheel to the center of rotation
	double thetaFR;	//thetaFR, angle from Front Right Wheel to the center of rotation
	double thetaRL;	//thetaRL, angle from Rear Left Wheel to the center of rotation
	double thetaRR;	//thetaRR, angle from Rear Right Wheel to the center of rotation
	double FRRatio;	//Ratio of Speed of Front Right wheel
	double FLRatio;	//Ratio of Speed of Front Left wheel
	double RRRatio;	//Ratio of Speed of Rear Right wheel
	double RLRatio;	//Ratio of Speed of Rear Left wheel

	double FLOffset;
	double FROffset;
	double RLOffset;
	double RROffset;

	//Crab Variables
	double AP;
	double BP;
	double CP;
	double DP;
	double radius; //distance from center to each wheel
	double robotangle; //current robot angle from Gyro

	//Inversion Variables
	int FLInv;
	int FRInv;
	int RLInv;
	int RRInv;

	//Counting Turns
	int FLTurns;
	int FRTurns;
	int RLTurns;
	int RRTurns;

	int MaxTurns;

	//Previous wheel volts
	float pFLVolt;
	float pFRVolt;
	float pRLVolt;
	float pRRVolt;

	//Current wheel volts
	float cFLVolt;
	float cFRVolt;
	float cRLVolt;
	float cRRVolt;

	double CorrectSteerSetpoint(double setpoint);
	void SetSteerSetpoint(float setpoint, std::shared_ptr<AnalogInput> actual, float offset, std::shared_ptr<PIDController> PIDCon, std::shared_ptr<CANTalon> steer, int turns, int &inv, bool UseShortcut);
	void SetDriveSpeed(float FLSpeed, float FRSpeed, float RLSpeed, float RRSpeed);
	void SetSteering(float FLSetPoint, float FRSetPoint, float RLSetPoint, float RRSetPoint, bool UseShortcut);
	void TurnsCheck(float previous, float current, int &turns);
public:
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	std::shared_ptr<AnalogInput> frontLeftPos;
	std::shared_ptr<CANTalon> frontLeftSteer;
	std::shared_ptr<PIDController> frontLeft;
	std::shared_ptr<AnalogInput> frontRightPos;
	std::shared_ptr<CANTalon> frontRightSteer;
	std::shared_ptr<PIDController> frontRight;
	std::shared_ptr<AnalogInput> rearLeftPos;
	std::shared_ptr<CANTalon> rearLeftSteer;
	std::shared_ptr<PIDController> rearLeft;
	std::shared_ptr<AnalogInput> rearRightPos;
	std::shared_ptr<CANTalon> rearRightSteer;
	std::shared_ptr<PIDController> rearRight;
	std::shared_ptr<CANTalon> tankLeft;
	std::shared_ptr<CANTalon> tankRight;
	std::shared_ptr<CANTalon> frontLeftDrive;
	std::shared_ptr<CANTalon> frontRightDrive;
	std::shared_ptr<CANTalon> rearLeftDrive;
	std::shared_ptr<CANTalon> rearRightDrive;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	DriveBase();
	void InitDefaultCommand();
	void SetWheelbase(float w, float x, float y);
	void SetOffsets(double FLOff, double FROff, double RLOff, double RROff);
	void ToggleFrontBack();
	void Steer(float radian, float speed, float a);
	void Crab(float twist, float y, float x, bool UseGyro);
	void Lock();
	void CheckForTurns();
	void UndoTurns();
	void UndoTurns(std::shared_ptr<PIDController> PID, std::shared_ptr<CANTalon> can, int turns, double offset);
	int GetFLTurns();
	int GetFRTurns();
	int GetRLTurns();
	int GetRRTurns();
	void SetFLTurns(int val);
	void SetFRTurns(int val);
	void SetRLTurns(int val);
	void SetRRTurns(int val);
	PIDController *DriveControlUS;
	std::unique_ptr<CrabSpeed> CrabSpeedTwist;
	std::unique_ptr<PIDController> DriveControlTwist;
	AHRS *imu;
	void TestDrive(float first);
	void SMDB();

};

#endif
