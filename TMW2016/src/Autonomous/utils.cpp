/*
 * utils.cpp
 */

#include "utils.h"
#include <iostream>
#include <cmath>

namespace utils {

static const float PI_3 = M_PI / 3;

float CalculateDriveAngle(const int pos, const int goal, outerworks defense) {
	//all distances in inches
	float dShoot = 144;  //distance from goal for shot
	float dxGoalFromWall = 141; //distance in x of the goal from the left wall
	float dxGoal = 7.5; // additional distance in x after the start of the goal
	float dyGoal = 13; // additional distance in y after the start of the goal
	float dxDefense = 24 + (pos-1)*53; //distance in x of the final robot position after crossing the defense
	float dx = 0;
	float dy = 0;

	float dyDefense = 140; //distance in y of the final robot position after crossing the defense (not 135)
	switch(defense) {
	case LowBar:
		dyDefense = 145;
		break;
	case RockWall:
	case Ramparts:
		dyDefense = 146;
		break;
	case RoughTerrain:
		dyDefense = 137;
		break;
	case Moat:
		dyDefense = 142;
		break;
	default:
		// use default value
		break;
	}

	if(goal == 1 || goal == 6) {
		dxGoal = 7.5;
		dyGoal = 13;
		dx = dxGoalFromWall + dxGoal - dShoot*sin(PI_3) - dxDefense;
		dy = dyDefense - (dyGoal+dShoot*cos(PI_3));
	}
	if (goal == 2 || goal == 7) {
		dxGoal = 30;
		dyGoal = 26;
		dx = dxGoalFromWall + dxGoal - dxDefense;
		dy = dyDefense - (dyGoal + dShoot);
	}
	if(goal == 3) {
		dxGoal = 52.5;
		dyGoal = 13;
		dx = dxGoalFromWall + dxGoal + dShoot*sin(PI_3) - dxDefense;
		dy = dyDefense - (dyGoal+dShoot*cos(PI_3));
	}
	if(goal == 4) {
		dxGoal = 7.5;
		dyGoal = 13;
		dx = dxGoalFromWall + dxGoal - dShoot - dxDefense;
		dy = dyDefense - dyGoal;
	}
	if(goal == 5) {
		dxGoal = 52.5;
		dyGoal = 13;
		dx = dxGoalFromWall + dxGoal + dShoot - dxDefense;
		dy = dyDefense - dyGoal;
	}
	std::cout	<< "dx: " << dx
				<< " dy: " << dy
				<< " atan2(dx,dy) = " << atan2(dx,dy)
				<< std::endl;
	return atan2(dx,dy);
}

}
