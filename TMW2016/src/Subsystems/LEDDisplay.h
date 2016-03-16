/*
 * LEDDisplay.h
 */

#ifndef SRC_SUBSYSTEMS_LEDDISPLAY_H_
#define SRC_SUBSYSTEMS_LEDDISPLAY_H_

#include "WPILib.h"
#include "../Vision/VisionData.h"


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
	void Update(float xposition, float width);
private:
	std::unique_ptr<AnalogOutput> widthOutput;
	std::unique_ptr<AnalogOutput> translationOutput;
	std::unique_ptr<DigitalOutput> allianceOutput;
	std::unique_ptr<DigitalOutput> shooterStatus;

	const long MIN_WIDTH = 0;
	const long MAX_WIDTH = 320;
	const long MIN_X = -160;
	const long MAX_X = 160;
	const long MIN_OUT = 0;
	const long MAX_OUT = 5;

	float MapWidth(const int width, const int xposition) const;
	float MapX(const int xposition) const;
};

#endif /* SRC_SUBSYSTEMS_LEDDISPLAY_H_ */
