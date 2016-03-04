/*
 * DataLogger.cpp
 */

#include "DataLogger.h"
#include <ctime>
#include <iostream>
#include <string>
#include <chrono>

typedef std::pair<string, std::function<float()>> HeaderFnPair;

DataLogger::DataLogger(DataLoggerMode dataLoggerMode,
					  std::shared_ptr<DriveBase> driveBase_,
					  std::shared_ptr<Arm> arm_,
					  OI *oi_,
					  VisionServer *visionServer_) :
		driveBase(driveBase_), arm(arm_), oi(oi_), visionServer(visionServer_)
{
	std::string basename;
	switch(dataLoggerMode) {
	case kAutonomous:
		basename = "autonomous";
		break;
	case kTeleop:
		basename = "teleop";
		break;
	default:
		basename = "unknown";
	}
	std::time_t time = std::time(nullptr);
	char buf[80];
	std::strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", std::localtime(&time));
	std::string filename = "/home/lvuser/" + basename  + "-datalog-" + buf + ".csv";

	outstream.open(filename);
	if (outstream.fail()) {
		cout << "ERROR Opening Log File [" << buf << "] : " << strerror(errno) << '\n';
	}

	cout << "Created and opened output stream\n";

	// IMU Data Structures
	imu_data.push_back( HeaderFnPair("Yaw",[this](){return driveBase->imu->GetYaw();}) );
	imu_data.push_back( HeaderFnPair("Pitch",[this](){return driveBase->imu->GetPitch();}) );
	imu_data.push_back( HeaderFnPair("Roll",[this](){return driveBase->imu->GetRoll();}) );
	imu_data.push_back( HeaderFnPair("Altitude",[this](){return driveBase->imu->GetAltitude();}) );
	imu_data.push_back( HeaderFnPair("Compass",[this](){return driveBase->imu->GetCompassHeading();}) );
	imu_data.push_back( HeaderFnPair("Angle",[this](){return driveBase->imu->GetAngle();}) );
	imu_data.push_back( HeaderFnPair("DisplacementX",[this](){return driveBase->imu->GetDisplacementX();}) );
	imu_data.push_back( HeaderFnPair("DisplacementY",[this](){return driveBase->imu->GetDisplacementY();}) );
	imu_data.push_back( HeaderFnPair("DisplacementZ",[this](){return driveBase->imu->GetDisplacementZ();}) );

	// Wheel Structures to be logged
	wheels.push_back(Wheel(driveBase->frontLeftPos, driveBase->frontLeftDrive, driveBase->frontLeft, driveBase->frontLeftDrive));
	wheels.push_back(Wheel(driveBase->frontRightPos, driveBase->frontRightDrive, driveBase->frontRight, driveBase->frontRightDrive));
	wheels.push_back(Wheel(driveBase->rearLeftPos, driveBase->rearLeftDrive, driveBase->rearLeft, driveBase->rearLeftDrive));
	wheels.push_back(Wheel(driveBase->rearRightPos, driveBase->rearRightDrive, driveBase->rearRight, driveBase->rearRightDrive));


	// --------------------------------------------------------------------- //
	// Write Headers
	// --------------------------------------------------------------------- //
	outstream << "Time";

	// IMU Headers
	std::for_each(imu_data.begin(), imu_data.end(), [this](HeaderFnPair &p){ outstream << ',' << p.first; });

	// Wheel Headers
	outstream	<< ",FL Pos,FL Steer Volt,FL Steer Current,FL Drive Volt,FL Drive Current,FL Error"
				<< ",FR Pos,FR Steer Volt,FR Steer Current,FR Drive Volt,FR Drive Current,FR Error"
				<< ",RL Pos,RL Steer Volt,RL Steer Current,RL Drive Volt,RL Drive Current,RL Error"
				<< ",RR Pos,RR Steer Volt,RR Steer Current,RR Drive Volt,RR Drive Current,RR Error";

	// Tank Headers
	outstream 	<< ",TankLeft Volt,TankLeft Current"
				<< ",TankRight Volt,TankRight Current";

	// Arm Headers
	std::vector<std::string> armHeaders = arm->GetHeaders();
	std::for_each(armHeaders.begin(), armHeaders.end(),
			[this](std::string &str){ this->outstream << ',' << str; });

	// Vision Headers
	outstream << ",vision.x,vision.y,vision.angle,vision.width";

	// Finish header
	outstream	<< '\n';
	outstream.flush();
	initialized = true;
	cout << "DataLogger::DataLogger completed init\n";
}

DataLogger::~DataLogger() {
	if (outstream) {
		outstream.flush();
		outstream.close();
	}
}

void DataLogger::Log() {
	if (!initialized) {
		return;
	}
	// Time Header
//	std::time_t t = std::time(nullptr);
//	outstream << t;
	long ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	outstream << ms;

	// IMU Headers
	std::for_each(imu_data.begin(), imu_data.end(), [this](HeaderFnPair &p){ outstream << ',' << p.second();});

	// Wheels
	std::for_each(wheels.begin(), wheels.end(), [this](Wheel &wheel){LogWheel(wheel);});

	// Tank
	outstream	<< ',' << driveBase->tankLeft->GetOutputVoltage()
				<< ',' << driveBase->tankLeft->GetOutputCurrent()
				<< ',' << driveBase->tankRight->GetOutputVoltage()
				<< ',' << driveBase->tankRight->GetOutputCurrent();

	// Arm
	arm->Log(outstream);

	// Vision Headers
	const VisionData v = visionServer->GetVisionData();
	outstream	<< ',' << v.xposition
				<< ',' << v.yposition
				<< ',' << v.tilt_angle
				<< ',' << v.width;


	outstream << '\n';
	outstream.flush();
}

inline void DataLogger::LogWheel(const Wheel &wheel) {
	outstream	<< ',' << wheel.position->GetValue()
				<< ',' << wheel.steer->GetOutputVoltage()
				<< ',' << wheel.steer->GetOutputCurrent()
				<< ',' << wheel.drive->GetOutputVoltage()
				<< ',' << wheel.drive->GetOutputCurrent()
				<< ',' << wheel.pid->GetError();
}
