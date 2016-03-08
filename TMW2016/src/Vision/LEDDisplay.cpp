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
//	allianceOutput.reset(new DigitalOutput(0));
//	shooterWheel.reset(new DigitalOutput(1));
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

		//
		// Alliance Output
		//
//		unsigned int alliance_out_value = 0;
//		switch(DriverStation::GetInstance().GetAlliance()) {
//		case DriverStation::kBlue:
//			alliance_out_value = 1;
//			break;
//		case DriverStation::kRed:
//			alliance_out_value = 2;
//			break;
//		case DriverStation::kInvalid:
//		default:
//			break;
//		}
//		allianceOutput->Set(alliance_out_value);

		//
		// Shooter Wheel Status
		//
//		feederStatus->Set(Robot::arm->IsFeederRunning() ? 1 : 0);
	}
}

float LEDDisplay::MapX(const int xposition) const {
	//  <--75 |  |-50-50   |  | 75->

	if (xposition < -50) {
		return 0.0;
	} else if (xposition > 50) {
		return 5.0;
	} else {
		return map(xposition, -50, 50, 0, 5);
	}
}

float LEDDisplay::MapWidth(const int width, const int x) const {

	float scale = (100 - abs(x)) / 10.0;
	float maxMapped = scale;

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

