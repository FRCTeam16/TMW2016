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

		float widthVoltage = map(goal.width, MIN_WIDTH, MAX_WIDTH, MIN_OUT, MAX_OUT);
		float translationVoltage = map(goal.xposition, MIN_X, MAX_X, MIN_OUT, MAX_OUT);
//		std::cout << "LEDDisplay W: " << vd.width << " -> " << widthVoltage << "\t"
//				  << " X: " << vd.xposition << " -> " << translationVoltage << "\n";
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

void LEDDisplay::Update(float xposition, float width) {
	float translationVoltage = map(xposition, -1.0, 1.0, MIN_OUT, MAX_OUT);
	float widthVoltage = MapWidth(width);
//			std::cout << "LEDDisplay W: " << width << " -> " << widthVoltage << "\t"
//					  << " X: " << xposition << " -> " << translationVoltage << "\n";
	widthOutput->SetVoltage(widthVoltage);
	translationOutput->SetVoltage(translationVoltage);
}

float LEDDisplay::MapWidth(const int width) const {
	if (width >= 70) {
		// too close
		return 0.0;
	} else if (width <= 40) {
		// too far
		return 0.0;
	} else if (width <= 50) {
		return map(width, 40, 55, 5/0.3, 5.0);
	} else if (width >= 60 ){
		return map(width, 55, 70, 5.0, 5/0.3);
	} else {
		// on target
		return 5.0;
	}
}

