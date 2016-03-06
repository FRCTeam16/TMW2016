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

struct GoalInfo {
	friend class DataLogger;
	float xposition;        // horizontal center of goal in pixels
	float yposition;        // vertical center of goal in pixels
	float width;       		// width
	float height;           // height

	GoalInfo() :  xposition(NAN),
				  yposition(NAN),
				  width(NAN),
				  height(NAN) {}

	GoalInfo(float x_, float y_, float width_, float height_) :
					xposition(x_),
					yposition(y_),
					width(width_),
					height(height_) {}
	~GoalInfo() {}
	bool HasData() const {
		bool nodata = isnan(xposition) || isnan(yposition);
		return !nodata;
	}
};


struct VisionData {
	friend class DataLogger;
	GoalInfo predictedGoal;
	GoalInfo leftGoal;
	GoalInfo rightGoal;

    VisionData() {}

    VisionData(GoalInfo pred_, GoalInfo left_, GoalInfo right_) :
    				predictedGoal(pred_),
                    leftGoal(left_),
                    rightGoal(right_) {}
    ~VisionData() {}
    bool HasData() const {
    	return leftGoal.HasData() || rightGoal.HasData();
    }
};


#endif /* SRC_VISION_VISIONDATA_H_ */
