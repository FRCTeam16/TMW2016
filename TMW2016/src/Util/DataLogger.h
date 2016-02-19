/*
 * DataLogger.h
 */

#ifndef SRC_UTIL_DATALOGGER_H_
#define SRC_UTIL_DATALOGGER_H_

#include "WPILib.h"
#include "../Subsystems/Arm.h"
#include "../Subsystems/DriveBase.h"
#include "../OI.h"
#include <fstream>
#include <vector>

typedef struct Wheel {
	const std::shared_ptr<AnalogInput> position;
	const std::shared_ptr<CANTalon> steer;
	const std::shared_ptr<PIDController> pid;
	const std::shared_ptr<CANTalon> drive;

	Wheel(const std::shared_ptr<AnalogInput> &position_,
		  const std::shared_ptr<CANTalon> &steer_,
	      const std::shared_ptr<PIDController> &pid_,
		  const std::shared_ptr<CANTalon> &drive_) :
	    	  position(position_), steer(steer_), pid(pid_), drive(drive_) {}
} Wheel;

enum DataLoggerMode { kAutonomous, kTeleop };

class DataLogger {
public:
	DataLogger(DataLoggerMode dataLoggerMode,
			   std::shared_ptr<DriveBase> driveBase_,
			   std::shared_ptr<Arm> arm_,
			   OI *oi_);
	virtual ~DataLogger();
	void Log();
protected:
private:
	std::vector<std::pair<string, std::function<float()>>> imu_data;
	std::vector<Wheel> wheels;
	void LogWheel(const Wheel &wheel);
	std::shared_ptr<DriveBase> driveBase;
	std::shared_ptr<Arm> arm;
	OI* oi;
	std::ofstream outstream;
	bool initialized;
};



#endif /* SRC_UTIL_DATALOGGER_H_ */
