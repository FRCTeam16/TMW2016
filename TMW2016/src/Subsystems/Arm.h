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
//	Preferences prefs;
	RAWCConstants* File;
	float dartOffset;

public:
	Arm();
	void InitDefaultCommand();
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS
	void Darter(float speed);
	void DartPosition(int pos);
	void DartOpenLoop(float speed);
	void DartSetToCurrent();
	int GetCurrentDartDifference();
	void SetDartOffset(int offset);
	void ClimbExtend();
	void ClimbRetract();
	void RunShooter(bool run, float speed);
	void Fire(bool value);
	void BeaterBar(float speed);
	void SMDB();
};

#endif
