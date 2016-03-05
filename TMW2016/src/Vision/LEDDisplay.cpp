/*
 * LEDDisplay.cpp
 */

#include "LEDDisplay.h"

static float map(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

LEDDisplay::LEDDisplay() {
	widthOutput.reset(new AnalogOutput(0));
	translationOutput.reset(new AnalogOutput(1));
}

LEDDisplay::~LEDDisplay() {
}

void LEDDisplay::Update(const VisionData &vd) {
	if (vd.HasData()) {
		float widthVoltage = map(vd.width, MIN_WIDTH, MAX_WIDTH, MIN_OUT, MAX_OUT);
		float translationVoltage = map(vd.xposition, MIN_X, MAX_X, MIN_OUT, MAX_OUT);
		std::cout << "LEDDisplay W: " << vd.width << " -> " << widthVoltage << "\t"
				  << " X: " << vd.xposition << " -> " << translationVoltage << "\n";
		widthOutput->SetVoltage(widthVoltage);
		translationOutput->SetVoltage(translationVoltage);
	}
}

