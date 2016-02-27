/*
 * VisionData.h
 */

#ifndef SRC_VISION_VISIONDATA_H_
#define SRC_VISION_VISIONDATA_H_

#include <cmath>
#include <ostream>

/**
 * Store Vision Data.
 *	center of image is 0,0
 *	320 x 240 camera resolution
 *
 *	Do we need acquisition time?
 */
struct VisionData {
	friend class DataLogger;
    float xposition;        // horizontal center of goal in pixels
    float yposition;        // vertical center of goal in pixels
    float tilt_angle;       // angle of goal in degrees
    float width;            // width of goal in pixels

    VisionData() :  xposition(NAN),
                    yposition(NAN),
                    tilt_angle(NAN),
                    width(NAN) {}

    VisionData(float x_, float y_, float angle_, float width_) :
                    xposition(x_),
                    yposition(y_),
                    tilt_angle(angle_),
                    width(width_) {}
    ~VisionData() {}
    bool HasData() const {
        return !isnan(xposition) || !isnan(yposition) || !isnan(tilt_angle) || !isnan(width);
    }
};


#endif /* SRC_VISION_VISIONDATA_H_ */
