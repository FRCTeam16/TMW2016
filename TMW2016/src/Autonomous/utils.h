/*
 * utils.h
 */

#ifndef SRC_AUTONOMOUS_UTILS_H_
#define SRC_AUTONOMOUS_UTILS_H_

#include "World.h"

namespace utils {
float CalculateDriveAngle(const int pos, const int goal, outerworks defense);
void DebugDisplayCalculateDriveAngle();
}



#endif /* SRC_AUTONOMOUS_UTILS_H_ */
