#ifndef CRABSPEED_H_
#define CRABSPEED_H_

#include "PIDOutput.h"

class CrabSpeed : public PIDOutput
{
public:
	CrabSpeed();
	virtual ~CrabSpeed() {};
	void PIDWrite(float Output) override;
	float Get();
	
private:
	float output;
};

#endif
