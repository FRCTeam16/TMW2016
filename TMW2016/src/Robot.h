// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#ifndef _ROBOT_H
#define _ROBOT_H

// #define NDEBUG // Define to disabled assertions

#include "WPILib.h"
#include "Commands/Command.h"
#include "RobotMap.h"
#include "LiveWindow/LiveWindow.h"
#include "RAWCConstants.h"
#include "AutoManager.h"
#include "BSButton.h"
#include "Util/DataLogger.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
#include "Commands/AutonomousCommand.h"
#include "Subsystems/Arm.h"
#include "Subsystems/DriveBase.h"


    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
#include "OI.h"

class Robot : public IterativeRobot {
public:
	std::unique_ptr<Command> autonomousCommand;
	static std::unique_ptr<OI> oi;
	LiveWindow *lw = LiveWindow::GetInstance();
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
    static std::shared_ptr<DriveBase> driveBase;
    static std::shared_ptr<Arm> arm;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	virtual void RobotInit();
	virtual void DisabledInit();
	virtual void DisabledPeriodic();
	virtual void AutonomousInit();
	virtual void AutonomousPeriodic();
	virtual void TeleopInit();
	virtual void TeleopPeriodic();
	virtual void TestPeriodic();
	static PowerDistributionPanel *pdp;

private:
	double FLOffset;
	double FROffset;
	double RLOffset;
	double RROffset;
	bool prevTrigger;
	Preferences* prefs;
	RAWCConstants* File;
	void SystemSMDB();
	void DriveBaseSMDB();
	std::unique_ptr<AutoManager> automan;
	std::shared_ptr<DataLogger> dataLogger;
	bool dartOpen;
	bool dartSpeed;
	bool shooterRun;
	bool tankRun;
	bool firing;
	double fireTime;
	bool lowFiring;
	double lowFireTime;
	float shooterSpeed;
	float feederSpeed;
	float beaterBarSpeed;

	void LogData();
	int logCounter = 0;			// simple counting to control log frequency
	const int LOG_FREQ = 50;

};
#endif
