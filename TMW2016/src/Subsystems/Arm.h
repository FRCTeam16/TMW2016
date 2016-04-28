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
#include "../Util/DataLoggable.h"
#include "../Util/SafetyChecker.h"

/**
 *
 *
 * @author ExampleAuthor
 */
class Arm: public Subsystem, DataLoggable {
	friend class SafetyChecker;
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
	float dartLeftMaxForward;
	float dartLeftMaxReverse;
	float dartRightMaxForward;
	float dartRightMaxReverse;
	float dartMaxForwardDefault;
	float dartMaxReverseDefault;
	const int dartMaxLimit = 100;
	int dartOutputLimit;
	int dartRightHotCount;
	int dartLeftHotCount;
	int dartCoolCount;
	float shooterSpeed;
	float feederSpeed;
	bool shooterRun;
	bool firing;
	double fireTime;
	bool lowFiring;
	double lowFireTime;
	bool wallshotDartEnabled = false;

	bool dartOutputUnlimited;

	int highShooter = 720;					// from prefs
	int lowShooter = 570;					// from prefs
	int pickup = 910;				// 4/5/2016 - 933 		4/20/2016 moving from 920
	int travel = 838;
	const int extendPosition = 300;
	const int extendLimit = 550;

public:
	Arm();
	void InitDefaultCommand();
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS
	void Darter(float speed);
	void DartPosition(int pos);
	int GetDartPosition() const;
	void DartOpenLoop(float speed);
	void DartSpeedControl(float speed);
	void DartSetToCurrent();
	int GetCurrentDartDifference();
	int GetCorrectedDartDifference();
	void SetDartOffset(int offset);
	void UnlimitDartOutput(bool unlimit);
	bool DartInPosition() const;
	void IncDart();
	void DecDart();
	void IncLowDart();
	void DecLowDart();
	void SetWallshotDart(const bool enabled);
	bool GetWallshotDart() const;
	void ClimbExtend();
	void ClimbRetract();
	void RunShooter(bool run);
	void ToggleShooterRun();
	bool Fire();
	void LowFire();
	void BeaterBar(float speed);
	void ShooterHigh();
	void ShooterLow();
	void PickupPosition();
	void TravelPosition();
	void SetShooterSpeed(float ShooterSpeed, float FeederSpeed);
	void ShooterManager();
	bool ShooterWheelsAtSpeed() const;
	void FireManager();
	void DartManager();
	void SMDB();

	// DataLoggable
	std::vector<std::string> GetHeaders() override;
	void Log(std::ofstream &outstream) override;
};

#endif
