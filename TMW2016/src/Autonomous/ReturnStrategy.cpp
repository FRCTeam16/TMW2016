/*
 * ReturnStrategy.cpp
 */

#include "ReturnStrategy.h"
#include "AlignWithGoal.h"
#include "SearchForGoal.h"
#include "CollideWithWall.h"
#include "TraverseObstacleSteps.h"



ReturnStrategy::ReturnStrategy() {
	// TODO Auto-generated constructor stub

}

ReturnStrategy::~ReturnStrategy() {
	// TODO Auto-generated destructor stub
}

void ReturnStrategy::Init(World* world) {
	const int position = world->GetReturnPosition();
	const bool ctypeReturn = world->GetCtypeReturn();
	cout << "ReturnStrategy::Init(" << position << ", ctype=" << ctypeReturn << ")\n";
	switch (position) {
	case 0:
		break;
	case 1:
		if (world->GetStartingPosition() == 1) {
			steps.push_back(std::unique_ptr<Step>(new SetArmPosition(SetArmPosition::Position::Pickup, false)));
			steps.push_back(std::unique_ptr<Step>(new ControlBeaterBar(false)));
			steps.push_back(std::unique_ptr<Step>(new CollideWithWall(0.0, -0.3, -0.5, 0.8)));
			steps.push_back(std::unique_ptr<Step>(new TraverseObstacleWithGyroAndSonarLockingValues(-0.5, -2.0, 5, false, 0.0)));
		} else {
			steps.push_back(std::unique_ptr<Step>(new SetArmPosition(SetArmPosition::Position::Pickup, false)));
			steps.push_back(std::unique_ptr<Step>(new ControlBeaterBar(false)));
			steps.push_back(std::unique_ptr<Step>(new TimedCrab(1.0, 0.0, 0.3, -0.6)));
			steps.push_back(std::unique_ptr<Step>(new CollideWithWall(0.0, -0.5, -0.0, 7)));
			steps.push_back(std::unique_ptr<Step>(new TraverseObstacleWithGyroAndSonarLockingValues(-0.5, -2.0, 5, false, 0.0)));
		}
		break;
	default:
		steps.push_back(std::unique_ptr<Step>(new AlignWithGoalAndShoot(5.0, 0.3, true)));
		steps.push_back(std::unique_ptr<Step>(new SetArmPosition(SetArmPosition::Position::Travel, false)));
		steps.push_back(std::unique_ptr<Step>(new Turn(180.0)));
		steps.push_back(std::unique_ptr<Step>(new SetArmPosition(SetArmPosition::Position::Travel, true)));
//		steps.push_back(std::unique_ptr<Step>(new TraverseObstacleWithGyroAndSonarLockingValues(-0.75, 180.0, 5, false, 0.0)));
		if (ctypeReturn) {
			steps.push_back(std::unique_ptr<Step>(new TimedCrab(2.0, 180.0, -0.75, 0.0)));
		} else {
			steps.push_back(std::unique_ptr<Step>(new TraverseObstacleWithGyroAndSonar(-0.75, 180.0)));
		}
	}
	steps.push_back(std::unique_ptr<Step>(new LockWheels()));
	cout << "Initialized " << steps.size() << " steps\n";
}

