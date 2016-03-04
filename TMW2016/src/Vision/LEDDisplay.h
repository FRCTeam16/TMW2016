/*
 * LEDDisplay.h
 */

#ifndef SRC_VISION_LEDDISPLAY_H_
#define SRC_VISION_LEDDISPLAY_H_

#include "WPILib.h"
#include "VisionData.h"


/**
 * Interface to LED display for displaying vision related information
 * to human drivers.
 *
 * @see http://www.pdocs.kauailabs.com/navx-mxp/examples/mxp-io-expansion/
 */
class LEDDisplay {
public:
	LEDDisplay();
	virtual ~LEDDisplay();
	void Update(const VisionData &vd);
private:
	std::unique_ptr<AnalogOutput> widthOutput;
	std::unique_ptr<AnalogOutput> translationOutput;
	const long MIN_WIDTH = 0;
	const long MAX_WIDTH = 320;
	const long MIN_X = -160;
	const long MAX_X = 160;
	const long MIN_OUT = 0;
	const long MAX_OUT = 5;
};

#endif /* SRC_VISION_LEDDISPLAY_H_ */