#ifndef CRABSPEED_H_
#define CRABSPEED_H_

#include "PIDOutput.h"

class CrabSpeed : public PIDOutput
{
public:
	CrabSpeed();
	 void PIDWrite(float Output);
	float Get();
	
private:
	float output;
};

#endif
