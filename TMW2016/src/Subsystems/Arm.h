// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#ifndef ARM_H
#define ARM_H


#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "../RAWCConstants.h"

/**
 *
 *
 * @author ExampleAuthor
 */
class Arm: public Subsystem {
private:
	// It's desirable that everything possible is private except
	// for methods that implement subsystem capabilities
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	std::shared_ptr<CANTalon> dartLeft;
	std::shared_ptr<CANTalon> dartRight;
	std::shared_ptr<Compressor> comp;
	std::shared_ptr<Solenoid> climbLeft;
	std::shared_ptr<Solenoid> climbRight;
	std::shared_ptr<CANTalon> beaterBar;
	std::shared_ptr<CANTalon> feederWheel;
	std::shared_ptr<CANTalon> shooterWheel;
	std::shared_ptr<Solenoid> fire;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	RAWCConstants* File;
	float dartOffset;
	float dartMaxForward;
	float dartMaxReverse;
	float shooterSpeed;
	float feederSpeed;
	bool shooterRun;
	bool firing;
	double fireTime;
	bool lowFiring;
	double lowFireTime;
	const int highShooter = 630;
	const int lowShooter = 720;
	const int pickup = 940;
	const int travel = 838;

public:
	Arm();
	void InitDefaultCommand();
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS
	void Darter(float speed);
	void DartPosition(int pos);
	void DartOpenLoop(float speed);
	void DartSpeedControl(float speed);
	void DartSetToCurrent();
	int GetCurrentDartDifference();
	int GetCorrectedDartDifference();
	void SetDartOffset(int offset);
	void ClimbExtend();
	void ClimbRetract();
	void RunShooter(bool run);
	void ToggleShooterRun();
	void Fire();
	void LowFire();
	void BeaterBar(float speed);
	void ShooterHigh();
	void ShooterLow();
	void PickupPosition();
	void TravelPosition();
	void SetShooterSpeed(float ShooterSpeed, float FeederSpeed);
	void ShooterManager();
	void FireManager();
	void DartManager();
	void SMDB();
};

#endif
