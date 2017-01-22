/*
 * Delay.cpp
 */

#include "Delay.h"
#include "../AutoManager.h"

Delay::Delay() {
}

Delay::Delay(float delayTime_) : delayTime(delayTime_) {}

Delay::~Delay() {}

bool Delay::operator ()(World* world) {
	const double currentTime = world->GetClock();
	if (startTime < 0) {
		cout << "Delay::Delay(" << delayTime << ")\n";
		startTime = currentTime;
		if (delayTime < 0) {
			delayTime = SmartDashboard::GetNumber(AUTO_DELAY, 0.0);
			cout << "\tInitialized from Dashboard: " << delayTime << '\n';
		}
	}
	return ((currentTime - startTime) >= delayTime);
}

