/*
 * DataLogger.cpp
 */

#include "DataLogger.h"
#include <ctime>
#include <iostream>
#include <string>

typedef std::pair<string, std::function<float()>> HeaderFnPair;

DataLogger::DataLogger(std::shared_ptr<DriveBase> driveBase_, std::shared_ptr<Arm> arm_, OI *oi_) :
		driveBase(driveBase_), arm(arm_), oi(oi_)
{
	std::time_t time = std::time(nullptr);
	char buf[80];
	std::strftime(buf, sizeof(buf), "/home/lvuser/datalog-%Y%m%d%H%M%S.csv", std::localtime(&time));
	outstream.open(fileName);
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
	wheels.push_back(Wheel(driveBase->frontLeftPos, driveBase->frontLeftDrive, driveBase->frontLeft));
	wheels.push_back(Wheel(driveBase->frontRightPos, driveBase->frontRightDrive, driveBase->frontRight));
	wheels.push_back(Wheel(driveBase->rearRightPos, driveBase->rearRightDrive, driveBase->rearRight));
	wheels.push_back(Wheel(driveBase->rearRightPos, driveBase->rearRightDrive, driveBase->rearRight));

	// --------------------------------------------------------------------- //
	// Write Headers
	// --------------------------------------------------------------------- //
	outstream << "Time";

	// IMU Headers
	std::for_each(imu_data.begin(), imu_data.end(), [this](HeaderFnPair &p){ outstream << ',' << p.first; });

	// Wheel Headers
	outstream	<< "FL Volt,FL Value,"
				<< "FR Volt,FR Value,"
				<< "RL Volt,RL Value,"
				<< "RR Volt,RR Value";

	// Arm Headers
	std::vector<std::string> armHeaders = arm->GetHeaders();
	std::for_each(armHeaders.begin(), armHeaders.end(),
			[this](std::string &str){ this->outstream << ',' << str; });
	outstream	<< '\n';
	outstream.flush();

	cout << "DataLogger::DataLogger completed init\n";
}

DataLogger::~DataLogger() {
	if (outstream) {
		outstream.flush();
		outstream.close();
	}
}

void DataLogger::Log() {
	// Time Header
	std::time_t t = std::time(nullptr);
	outstream << t << ',';

	// IMU Headers
	std::for_each(imu_data.begin(), imu_data.end(), [this](HeaderFnPair &p){ outstream << ',' << p.second();});

	// Wheels
	std::for_each(wheels.begin(), wheels.end(), [this](Wheel &wheel){LogWheel(wheel);});

	// Arm
	arm->Log(outstream);


	outstream << '\n';
	outstream.flush();
	cout << "DataLogger::Log finished\n";
}

inline void DataLogger::LogWheel(const Wheel &wheel) {
	outstream << wheel.position->GetVoltage() << ',' << wheel.position->GetValue();
}
