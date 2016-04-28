/*
 * LEDDisplay.cpp
 */

#include "../Subsystems/LEDDisplay.h"

static float map(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

LEDDisplay::LEDDisplay() {
	widthOutput.reset(new AnalogOutput(0));
	translationOutput.reset(new AnalogOutput(1));
	blueAlliance.reset(new DigitalOutput(11));
	redAlliance.reset(new DigitalOutput(12));
}

LEDDisplay::~LEDDisplay() {
}

void LEDDisplay::Update(const VisionData &vd) {
	if (vd.HasData()) {
		const int invalidGoal = -1;
		GoalInfo goal = vd.GetGoal(invalidGoal);
		const int offset = Preferences::GetInstance()->GetInt("VisionCenterOffset", -11);
		float translationVoltage = MapX(goal.xposition, offset);
		float widthVoltage = MapWidth(goal.width, goal.xposition, offset);
		widthOutput->SetVoltage(widthVoltage);
		translationOutput->SetVoltage(translationVoltage);
	} else {
		// No data
		widthOutput->SetVoltage(0);
		translationOutput->SetVoltage(0);
	}

	blueAlliance->Set(DriverStation::GetInstance().GetAlliance() == DriverStation::kBlue);
	redAlliance->Set(DriverStation::GetInstance().GetAlliance() == DriverStation::kRed);
	}

float LEDDisplay::MapX(const int xposition, const int offset) const {
	//  <--/ -50 - 50 \-->
	const int LOWER = -50 + offset;
	const int UPPER = 50 + offset;

	if (xposition < LOWER) {
		return 0.0;
	} else if (xposition > UPPER) {
		return 5.0;
	} else {
		return map(xposition, LOWER, UPPER, 0, 5);
	}
}

float LEDDisplay::MapWidth(const int width, const int x, const int offset) const {
	const int LOWER = -50 + offset;
	const int UPPER = 50 + offset;
	const int LOWER_WIDTH = 50;
	const int UPPER_WIDTH = 60;

	if (x > LOWER && x < UPPER) {
		// on target
		if (width > LOWER_WIDTH && width < UPPER_WIDTH) {
			// good width
			return 5.0;
		} else if (width <= LOWER_WIDTH) {
			return map(width, 35, LOWER_WIDTH, 0, 5.0);
		} else if (width >= UPPER_WIDTH ){
			return map(width, UPPER_WIDTH, 75, 5.0, 0);
		}
	} else {
		// not on target
		if (x < LOWER) {
			return map(x, -100 + offset, LOWER, 0, 5.0);
		} else if (x > 50) {
			return map(x, UPPER, 100 + offset, 5.0, 0.0);
		}
	}
	return 0;
}

