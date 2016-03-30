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

		float translationVoltage = MapX(goal.xposition);
		float widthVoltage = MapWidth(goal.width, goal.xposition);

//		std::cout << "LEDDisplay W: " << goal.width << " -> " << widthVoltage << "\t"
//				  << " X: " << goal.xposition << " -> " << translationVoltage << "\n";
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

float LEDDisplay::MapX(const int xposition) const {
	//  <--/ -50 - 50 \-->

	if (xposition < -50) {
		return 0.0;
	} else if (xposition > 50) {
		return 5.0;
	} else {
		return map(xposition, -50, 50, 0, 5);
	}
}

float LEDDisplay::MapWidth(const int width, const int x) const {
	if (x > -50 && x < 50) {
		// on target
		if (width > 50 && width < 60) {
			// good width
			return 5.0;
		} else if (width <= 50) {
			return map(width, 35, 50, 0, 5.0);
		} else if (width >= 60 ){
			return map(width, 60, 75, 5.0, 0);
		}
	} else {
		// not on target
		if (x < -50) {
			return map(x, -100, -50, 0, 5.0);
		} else if (x > 50) {
			return map(x, 50, 100, 5.0, 0.0);
		}
	}
	return 0;
}

