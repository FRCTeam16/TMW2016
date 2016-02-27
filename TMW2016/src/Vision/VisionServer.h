/*
 * VisionServer.h
 */

#ifndef SRC_VISION_VISIONSERVER_H_
#define SRC_VISION_VISIONSERVER_H_

#include "VisionData.h"
#include "VisionDataParser.h"
#include "WPILib.h"

class VisionServer {
public:
	VisionServer(const int port);
	virtual ~VisionServer();
	VisionData GetVisionData() const;
private:
	std::unique_ptr<Task> server;
	std::unique_ptr<VisionDataParser> parser;
};


#endif /* SRC_VISION_VISIONSERVER_H_ */
