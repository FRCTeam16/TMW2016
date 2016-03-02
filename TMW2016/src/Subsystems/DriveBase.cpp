// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.




#include "DriveBase.h"
#include "../RobotMap.h"
// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

DriveBase::DriveBase() : Subsystem("DriveBase") {
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
    frontLeftPos = RobotMap::driveBaseFrontLeftPos;
    frontLeftSteer = RobotMap::driveBaseFrontLeftSteer;
    frontLeft = RobotMap::driveBaseFrontLeft;
    frontRightPos = RobotMap::driveBaseFrontRightPos;
    frontRightSteer = RobotMap::driveBaseFrontRightSteer;
    frontRight = RobotMap::driveBaseFrontRight;
    rearLeftPos = RobotMap::driveBaseRearLeftPos;
    rearLeftSteer = RobotMap::driveBaseRearLeftSteer;
    rearLeft = RobotMap::driveBaseRearLeft;
    rearRightPos = RobotMap::driveBaseRearRightPos;
    rearRightSteer = RobotMap::driveBaseRearRightSteer;
    rearRight = RobotMap::driveBaseRearRight;
    tankLeft = RobotMap::driveBaseTankLeft;
    tankRight = RobotMap::driveBaseTankRight;
    frontLeftDrive = RobotMap::driveBaseFrontLeftDrive;
    frontRightDrive = RobotMap::driveBaseFrontRightDrive;
    rearLeftDrive = RobotMap::driveBaseRearLeftDrive;
    rearRightDrive = RobotMap::driveBaseRearRightDrive;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
    imu = new AHRS(SPI::Port::kMXP);
	frontLeft->Enable();
	frontRight->Enable();
	rearLeft->Enable();
	rearRight->Enable();
	FLOffset = 0;
	FROffset = 0;
	RLOffset = 0;
	RROffset = 0;
	FLInv = 1;
	FRInv = 1;
	RRInv = 1;
	RLInv = 1;
	FLTurns = 0;
	FRTurns = 0;
	RLTurns = 0;
	RRTurns = 0;
	MaxTurns = 1000;
	pFLVolt = 0;
	pFRVolt = 0;
	pRLVolt = 0;
	pRRVolt = 0;
	driveFront = true;

	Y=0;
	robotangle=0;
	A=0;
	AP=0;
	BP=0;
	cFRVolt=0;
	cFLVolt=0;
	CP=0;
	cRLVolt=0;
	cRRVolt=0;
	DP=0;
	FL=0;
	FLRatio=0;
	FRRatio=0;
	gyroreference=0;
	GyroZeroFlag=0;
	GyroZeroTime=0;
	radian=0;
	radius=0;
	FR=0;
	RL=0;
	RLRatio=0;
	RR=0;
	RRRatio=0;
	thetaFL=0;
	thetaRL=0;
	thetaFR=0;
	thetaRR=0;
	thetaRC=0;
	W=0;
	X=0;
	Z=0;

	FLHotCount = 0;
	FRHotCount = 0;
	RLHotCount = 0;
	RRHotCount = 0;
	driveLimit = 1.0;
	coolCount = 0;


	CrabSpeedTwist.reset(new CrabSpeed());
	DriveControlTwist.reset(new PIDController(.01, 0, .05, imu, CrabSpeedTwist.get(), 0.02));
	DriveControlTwist->SetContinuous(true);
	DriveControlTwist->SetAbsoluteTolerance(2.0);
	DriveControlTwist->Enable();
	DriveControlTwist->SetOutputRange(-.5, .5);
	DriveControlTwist->SetInputRange(-180, 180);

	frontLeftSteer->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	frontLeftDrive->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);
	frontRightSteer->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	frontRightDrive->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);
	rearLeftSteer->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	rearLeftDrive->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);
	rearRightSteer->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	rearRightDrive->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);
}

void DriveBase::InitDefaultCommand() {
    // Set the default command for a subsystem here.
    // SetDefaultCommand(new MySpecialCommand());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND


    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND
}


// Put methods for controlling this subsystem
// here. Call these from Commands.

void DriveBase::SetWheelbase(float w, float x, float y) {
	W=w;
	X=x;
	Y=y;
}
void DriveBase::SetOffsets(double FLOff, double FROff, double RLOff, double RROff) {
	FLOffset = FLOff;
	FROffset = FROff;
	RLOffset = RLOff;
	RROffset = RROff;
}
void DriveBase::ToggleFrontBack(){
	driveFront = !driveFront;
}
void DriveBase::Crab(float twist, float y, float x, bool UseGyro) {

	robotangle = imu->GetYaw()*M_PI/180;

	float FWD = y;
	float STR = x;
	if(UseGyro)
	{
		FWD = y*cos(robotangle) + x*sin(robotangle);
		STR = -y*sin(robotangle) + x*cos(robotangle);
	}

	radius = sqrt(pow(2*Y,2)+pow(X,2));

	AP = STR - twist*X/radius;
	BP = STR + twist*X/radius;
	CP = FWD - twist*2*Y/radius;
	DP = FWD + twist*2*Y/radius;

	float FLSetPoint = 2.5;
	float FRSetPoint = 2.5;
	float RLSetPoint = 2.5;
	float RRSetPoint = 2.5;

	if(DP != 0 || BP != 0)
		FLSetPoint = (2.5 + 2.5/M_PI*atan2(BP,DP));
	if(BP != 0 || CP != 0)
		FRSetPoint = (2.5 + 2.5/M_PI*atan2(BP,CP));
	if(AP != 0 || DP != 0)
		RLSetPoint = (2.5 + 2.5/M_PI*atan2(AP,DP));
	if(AP != 0 || CP != 0)
		RRSetPoint = (2.5 + 2.5/M_PI*atan2(AP,CP));


	//SetSteerSetpoint(FLSetPoint, FRSetPoint, RLSetPoint, RRSetPoint, true);
	SetSteering(FLSetPoint, FRSetPoint, RLSetPoint, RRSetPoint, true);
	FL = sqrt(pow(BP,2)+pow(DP,2));
	FR = sqrt(pow(BP,2)+pow(CP,2));
	RL = sqrt(pow(AP,2)+pow(DP,2));
	RR = sqrt(pow(AP,2)+pow(CP,2));


	//Solve for fastest wheel speed
	double speedarray[] = {fabs(FL), fabs(FR), fabs(RL), fabs(RR)};

	 int length = 4;
     double maxspeed = speedarray[0];
     for(int i = 1; i < length; i++)
     {
          if(speedarray[i] > maxspeed)
                maxspeed = speedarray[i];
     }

	//Set ratios based on maximum wheel speed
    if(maxspeed > 1 || maxspeed < -1)
    {
		FLRatio = FL/maxspeed;
		FRRatio = FR/maxspeed;
		RLRatio = RL/maxspeed;
		RRRatio = RR/maxspeed;
    }
    else
    {
		FLRatio = FL;
		FRRatio = FR;
		RLRatio = RL;
		RRRatio = RR;
    }

	//Set drive speeds
    SetDriveSpeed(FLRatio, -FRRatio, RLRatio, -RRRatio);

}
void DriveBase::Steer(float radian, float speed, float a) {

	std::cout << "Radian:" << radian << "|| Speed:" << speed << std::endl;

	A=a;

	thetaRC = M_PI - radian;  //convert steering angle to rear center wheel angle

	if(thetaRC != M_PI / 2)	//If we are not driving straight forward...
	{
		if(thetaRC < M_PI / 2)	//Right Turn
		{
			RightTurn4Wheels();
		}
		else if(thetaRC > M_PI / 2)	//Left Turn
		{
			LeftTurn4Wheels();
		}
	}
	else	//thetaRC = M_PI / 2
	{
		thetaFL = M_PI / 2;
		thetaFR = M_PI / 2;
		thetaRL = M_PI / 2;
		thetaRR = M_PI / 2;
		FLRatio = 1;
		FRRatio = 1;
		RLRatio = 1;
		RRRatio = 1;
	}
	//Solve for fastest wheel speed
	double speedarray[] = {fabs(FL), fabs(FR), fabs(RL), fabs(RR)};

	 int length = 4;
     double maxspeed = speedarray[0];
     for(int i = 1; i < length; i++)
     {
          if(speedarray[i] > maxspeed)
                maxspeed = speedarray[i];
     }

	//Set ratios based on maximum wheel speed
	FLRatio = FL/maxspeed;
	FRRatio = FR/maxspeed;
	RLRatio = RL/maxspeed;
	RRRatio = RR/maxspeed;

	//Set drive speeds
	SetDriveSpeed(-FLRatio*speed, FRRatio*speed, -RLRatio*speed, RRRatio*speed);

	//Set Steering PID Setpoints
	float FLSetPoint = (1.25 + 2.5/M_PI*thetaFL);
	float FRSetPoint = (1.25 + 2.5/M_PI*thetaFR);
	float RLSetPoint = (1.25 + 2.5/M_PI*thetaRL);
	float RRSetPoint = (1.25 + 2.5/M_PI*thetaRR);

	SetSteering(FLSetPoint, FRSetPoint, RLSetPoint, RRSetPoint, true);
	}
void DriveBase::LeftTurn4Wheels()
{
	Z = ((A * X) * tan(M_PI - thetaRC));				//find turning radius

	//calculate angles based on turning radius
	thetaRL = M_PI - atan((Z - W) / (A * X));
	thetaRR = M_PI - atan((Z + W) / (A * X));
	thetaFR = M_PI / 2;
	thetaFL = M_PI / 2;

	if(A != 1) //not turning about front wheels
	{
		thetaFL = atan((Z - Y) / ((1 - A) * X));	//These are identical for right and left turns
		thetaFR = atan((Z + Y) / ((1 - A) * X));	//These are identical for right and left turns
	}
	//Solve each wheel turning radii (wheel speed)
	FL = (Z - Y) / sin(thetaFL);
	FR = (Z + Y) / sin(thetaFR);
	RL = (Z - W) / sin(M_PI - thetaRL);
	RR = (Z + W) / sin(M_PI - thetaRR);
}
void DriveBase::RightTurn4Wheels()
{
	Z = ((A * X) * tan(thetaRC));				//find turning radius

	//calculate angles based on turning radius
	thetaRL = atan((Z + W) / (A * X));
	thetaRR = atan((Z - W) / (A * X));
	thetaFR = M_PI / 2;
	thetaFL = M_PI / 2;


	if(A != 1)  //not turning about front wheels
	{
		thetaFR = M_PI - atan((Z - Y) / ((1 - A) * X));	//These are identical for right and left turns
		thetaFL = M_PI - atan((Z + Y) / ((1 - A) * X));	//These are identical for right and left turns
	}

	//Solve each wheel turning radii (wheel speed)
	FL = (Z + Y) / sin(M_PI - thetaFL);
	FR = (Z - Y) / sin(M_PI - thetaFR);
	RL = (Z + W) / sin(thetaRL);
	RR = (Z - W) / sin(thetaRR);
}
double DriveBase::CorrectSteerSetpoint(double setpoint) {
	//Used to correct steering setpoints to within the 0 to 5 V scale

	if (setpoint < 0)
	{
		return setpoint + 5;
	}
	else if (setpoint > 5)
	{
		return setpoint - 5;
	}
	else if (setpoint == 5)
	{
		return 0;
	}
	else
	{
		return setpoint;
	}
}
void DriveBase::SetSteerSetpoint(float setpoint, std::shared_ptr<AnalogInput> actual, float offset, std::shared_ptr<PIDController> PIDCon, std::shared_ptr<CANTalon> steer, int turns, int &inv, bool UseShortcut) {

	float volt = actual->GetVoltage();

	if(turns >= MaxTurns){
		PIDCon->Disable();
		steer->Set(-1);
	}
	else if (turns <= -MaxTurns){
		PIDCon->Disable();
		steer->Set(1);
	}

	else if(fabs(CorrectSteerSetpoint(setpoint + offset) - volt) < 1.25 || fabs(CorrectSteerSetpoint(setpoint + offset) - volt) > 3.75)
	{
		PIDCon->Enable();
		if((turns + 1 == MaxTurns && CorrectSteerSetpoint(volt - offset) - CorrectSteerSetpoint(setpoint) > 2.5) || (turns - 1 == -MaxTurns && CorrectSteerSetpoint(volt - offset) - CorrectSteerSetpoint(setpoint) < -2.5))
		{
			PIDCon->SetSetpoint(CorrectSteerSetpoint(setpoint + offset-2.5));
			inv = -1;
		}
		else
		{
			PIDCon->SetSetpoint(CorrectSteerSetpoint(setpoint + offset));
			inv = 1;
		}
	}
	else
	{
		PIDCon->Enable();
		if((turns + 1 == MaxTurns && CorrectSteerSetpoint(volt - offset) - CorrectSteerSetpoint(setpoint - 2.5) > 2.5) || (turns - 1 == -MaxTurns && CorrectSteerSetpoint(volt - offset) - CorrectSteerSetpoint(setpoint - 2.5) < -2.5))
		{
			PIDCon->SetSetpoint(CorrectSteerSetpoint(setpoint + offset));
			inv = 1;
		}
		else
		{
			PIDCon->SetSetpoint(CorrectSteerSetpoint(setpoint + offset - 2.5));
			inv = -1;
		}
	}

}
void DriveBase::SetSteering(float FLSetPoint, float FRSetPoint, float RLSetPoint, float RRSetPoint, bool UseShortcut) {
	if(driveFront) {
		SetSteerSetpoint(FLSetPoint, frontLeftPos, FLOffset, frontLeft, frontLeftSteer, FLTurns, FLInv, UseShortcut);
		SetSteerSetpoint(FRSetPoint, frontRightPos, FROffset, frontRight, frontRightSteer, FRTurns, FRInv, UseShortcut);
		SetSteerSetpoint(RLSetPoint, rearLeftPos, RLOffset, rearLeft, rearLeftSteer, RLTurns, RLInv, UseShortcut);
		SetSteerSetpoint(RRSetPoint, rearRightPos, RROffset, rearRight, rearRightSteer, RRTurns, RRInv, UseShortcut);
	}
	else
	{
		SetSteerSetpoint(RRSetPoint, frontLeftPos, FLOffset, frontLeft, frontLeftSteer, FLTurns, FLInv, UseShortcut);
		SetSteerSetpoint(RLSetPoint, frontRightPos, FROffset, frontRight, frontRightSteer, FRTurns, FRInv, UseShortcut);
		SetSteerSetpoint(FRSetPoint, rearLeftPos, RLOffset, rearLeft, rearLeftSteer, RLTurns, RLInv, UseShortcut);
		SetSteerSetpoint(FLSetPoint, rearRightPos, RROffset, rearRight, rearRightSteer, RRTurns, RRInv, UseShortcut);
	}
}
void DriveBase::CheckForTurns() {
	cFLVolt = frontLeftPos->GetVoltage();
	cFRVolt = frontRightPos->GetVoltage();
	cRLVolt = rearLeftPos->GetVoltage();
	cRRVolt = rearRightPos->GetVoltage();
	TurnsCheck(CorrectSteerSetpoint(pFLVolt - FLOffset), CorrectSteerSetpoint(cFLVolt- FLOffset), FLTurns);
	TurnsCheck(CorrectSteerSetpoint(pFRVolt - FROffset), CorrectSteerSetpoint(cFRVolt - FROffset), FRTurns);
	TurnsCheck(CorrectSteerSetpoint(pRLVolt - RLOffset), CorrectSteerSetpoint(cRLVolt - RLOffset), RLTurns);
	TurnsCheck(CorrectSteerSetpoint(pRRVolt - RROffset), CorrectSteerSetpoint(cRRVolt - RROffset), RRTurns);
	/*SmartDashboard::PutNumber("pFL", CorrectSteerSetpoint(pFLVolt - FLOffset));
	SmartDashboard::PutNumber("cFL", CorrectSteerSetpoint(cFLVolt - FLOffset));
	SmartDashboard::PutNumber("Prev-Curr", CorrectSteerSetpoint(pFLVolt - FLOffset) - CorrectSteerSetpoint(cFLVolt - FLOffset));*/
	pFLVolt = cFLVolt;
	pFRVolt = cFRVolt;
	pRLVolt = cRLVolt;
	pRRVolt = cRRVolt;
	/*SmartDashboard::PutNumber("FLTurns", FLTurns);
	SmartDashboard::PutNumber("FRTurns", FRTurns);
	SmartDashboard::PutNumber("RLTurns", RLTurns);
	SmartDashboard::PutNumber("RRTurns", RRTurns);*/
}
void DriveBase::TurnsCheck(float previous, float current, int& turns) {
	if(previous - current < -3.5)
		turns--;

	else if(previous - current > 3.5)
		turns++;
}
void DriveBase::UndoTurns() {
	UndoTurns(frontLeft, frontLeftSteer, FLTurns, FLOffset);
	UndoTurns(frontRight, frontRightSteer, FRTurns, FROffset);
	UndoTurns(rearLeft, rearLeftSteer, RLTurns, RLOffset);
	UndoTurns(rearRight, rearRightSteer, RRTurns, RROffset);
}
void DriveBase::UndoTurns(std::shared_ptr<PIDController> PID, std::shared_ptr<CANTalon> can, int turns, double offset){
	if(turns < 0) {
		PID->Disable();
		can->Set(1);
	}
	else if(turns > 0) {
		PID->Disable();
		can->Set(-1);
	}
	else{
		PID->Enable();
		PID->SetSetpoint(2.5 + offset);
	}
}

int DriveBase::GetFLTurns() {
	return FLTurns;
}
int DriveBase::GetFRTurns() {
	return FRTurns;
}
int DriveBase::GetRLTurns() {
	return RLTurns;
}
int DriveBase::GetRRTurns() {
	return RRTurns;
}

void DriveBase::SetFLTurns(int val) {
	FLTurns = val;
}
void DriveBase::SetFRTurns(int val) {
	FRTurns = val;
}
void DriveBase::SetRLTurns(int val) {
	RLTurns = val;
}
void DriveBase::SetRRTurns(int val) {
	RRTurns = val;
}

void DriveBase::SetDriveSpeed(float FLSpeed, float FRSpeed, float RLSpeed, float RRSpeed) {
//applies inversion variables defined in SetSteerSetPoint function
	float driveOutLimit = 50;
	int hotCountLimit = 100;
	int coolCountLimit = 1000;
	frontLeftDrive->GetOutputCurrent() > driveOutLimit ? FLHotCount++ : FLHotCount =0;
	frontRightDrive->GetOutputCurrent() > driveOutLimit ? FRHotCount++ : FRHotCount =0;
	rearLeftDrive->GetOutputCurrent() > driveOutLimit ? RLHotCount++ : RLHotCount =0;
	rearRightDrive->GetOutputCurrent() > driveOutLimit ? RRHotCount++ : RRHotCount =0;

	if ( FLHotCount == 0 && FRHotCount == 0 && RLHotCount == 0 && RRHotCount == 0) {
		coolCount++;
	}
	else {
		coolCount = 0;
	}

	if (FLHotCount > hotCountLimit || FRHotCount > hotCountLimit || RLHotCount > hotCountLimit || RRHotCount > hotCountLimit) {
		driveLimit = .5;
	}

	if(coolCount > coolCountLimit) {
		driveLimit = 1.0;
	}

	if(driveFront) {
		frontLeftDrive->Set(FLSpeed*FLInv*driveLimit);
		frontRightDrive->Set(FRSpeed*FRInv*driveLimit);
		rearLeftDrive->Set(RLSpeed*RLInv*driveLimit);
		rearRightDrive->Set(RRSpeed*RRInv*driveLimit);
	}
	else {
		frontLeftDrive->Set(RRSpeed*FLInv*driveLimit);
		frontRightDrive->Set(RLSpeed*FRInv*driveLimit);
		rearLeftDrive->Set(FRSpeed*RLInv*driveLimit);
		rearRightDrive->Set(FLSpeed*RRInv*driveLimit);
	}

	std::cout << "DriveLimit:" << driveLimit << " HotCounts:" << FLHotCount << "|" << FRHotCount << "|" << RLHotCount << "|" << RRHotCount << " CoolCount:" << coolCount << std::endl;
}

void DriveBase::RunTanks(float speed) {
	float driveOutLimit = 50;
	int hotCountLimit = 100;
	int coolCountLimit = 1000;
	tankLeft->GetOutputCurrent() > driveOutLimit ? tankLeftHotCount++ : tankLeftHotCount =0;
	tankRight->GetOutputCurrent() > driveOutLimit ? tankRightHotCount++ : tankRightHotCount =0;

	tankLeftHotCount == 0 && tankRightHotCount == 0 ? tankCoolCount++ : tankCoolCount = 0;

	if (tankLeftHotCount > hotCountLimit || tankRightHotCount > hotCountLimit) {
		tankDriveLimit = .5;
	}

	if(coolCount > coolCountLimit) {
		tankDriveLimit = 1.0;
	}

	tankLeft->Set(speed*tankDriveLimit);
	tankRight->Set(speed*tankDriveLimit);
}
void DriveBase::Lock() //locks wheels to prevent robot movement
{
	SetSteering(2.0, 0.75, 3.25, 4.5, true);
	SetDriveSpeed(0,0,0,0);
}

void DriveBase::TestDrive(float first) {
	frontRight->SetSetpoint(first*5);
	frontLeft->SetSetpoint(first*5);
	rearLeft->SetSetpoint(first*5);
	rearRight->SetSetpoint(first*5);
}

void DriveBase::SMDB() {
	SmartDashboard::PutNumber("Yaw", imu->GetYaw());
	SmartDashboard::PutNumber("Angle", imu->GetAngle());
	SmartDashboard::PutNumber("FrontLeftDriveCurrent", frontLeftDrive->GetOutputCurrent());
	SmartDashboard::PutNumber("FrontRightDriveCurrent", frontRightDrive->GetOutputCurrent());
	SmartDashboard::PutNumber("RearLeftDriveCurrent", rearLeftDrive->GetOutputCurrent());
	SmartDashboard::PutNumber("RearRightDriveCurrent", rearRightDrive->GetOutputCurrent());
	SmartDashboard::PutNumber("LeftTankDriveCurrent", tankLeft->GetOutputCurrent());
	SmartDashboard::PutNumber("RightTankDriveCurrent", tankRight->GetOutputCurrent());
}
