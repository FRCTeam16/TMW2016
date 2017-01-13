#include "CrabSpeed.h"

CrabSpeed::CrabSpeed() :
output(0)
{
	
}

void CrabSpeed::PIDWrite(float Output)
{
	output = Output;
}

float CrabSpeed::Get()
{
	return output;
}
