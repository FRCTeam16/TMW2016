/*
 * VisionServer.h
 */

#ifndef SRC_VISION_VISIONSERVER_H_
#define SRC_VISION_VISIONSERVER_H_

#include "VisionData.h"
#include "VisionDataParser.h"
#include "WPILib.h"

using frc::SmartDashboard;

class VisionServer {
public:
	VisionServer(const int port);
	virtual ~VisionServer();
	VisionData GetVisionData() const;
	void SMDB();
	void SetClientConnected(bool connected);
	bool GetClientConnected() const;
private:
	std::unique_ptr<Task> server;
	std::unique_ptr<VisionDataParser> parser;
	bool clientConnected = false;
};


#endif /* SRC_VISION_VISIONSERVER_H_ */
