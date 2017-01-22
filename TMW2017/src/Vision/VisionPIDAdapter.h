/*
 * VisionPIDAdapter.h
 */

#ifndef SRC_VISION_VISIONPIDADAPTER_H_
#define SRC_VISION_VISIONPIDADAPTER_H_

#include "WPILib.h"


class VisionPIDAdapter : public PIDSource, public PIDOutput {
public:
	VisionPIDAdapter();
	virtual ~VisionPIDAdapter();
	void Update(const float value);
	double PIDGet() override;					// used as PID sensor source
	void PIDWrite(float output) override;		// used as PID sensor output
	double GetOutputValue();
private:
	double inputValue = 0;
	double outputValue = 0;
};

#endif /* SRC_VISION_VISIONPIDADAPTER_H_ */
