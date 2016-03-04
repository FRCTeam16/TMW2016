#include "DualMaxBoticsI2CXL.h"

#include "WPILib.h"
#include <string>

DualMaxBoticsI2CXL::DualMaxBoticsI2CXL(I2C::Port port, uint8_t address1, uint8_t address2, float frequency) {
  enabled_ = true;
  running_ = true;
  isDead_ = false;
  address_[0] = address1;
  address_[1] = address2;
//  address1_ = address1;
//  address2_ = address2;
  frequency_ = frequency;
//  sensor1_ = new I2C(port, address_[0]);
//  sensor2_ = new I2C(port, address_[1]);
  sensor_[0] = new I2C(port, address_[0]);
  sensor_[1] = new I2C(port, address_[1]);
  task_ = new Task("DualMaxBoticsI2CXL", (FUNCPTR)DualMaxBoticsI2CXL::Run, this);
}

DualMaxBoticsI2CXL::~DualMaxBoticsI2CXL(){
  delete task_;
}

void DualMaxBoticsI2CXL::Run(DualMaxBoticsI2CXL* task) {
  while (task->running_) {
    if (task->enabled_) {
    	for (uint i=0; i < sizeof(address_); i++) {
    	task->sensor_[i]->Write((unsigned char)INITREADING,0);
    	Wait(task->frequency_);
    	task->sensor_[i]->ReadOnly(2,task->distance_[i]);
    	}
    }
    else
      Wait(1.0);   // 50 ms wait period while task is 'paused'
  }

  task->isDead_ = true; // Falling off the edge of the earth...
}

void DualMaxBoticsI2CXL::Start() {
  enabled_ = true;
}

void DualMaxBoticsI2CXL::Pause() {
  enabled_ = false;
}

void DualMaxBoticsI2CXL::Terminate() {
  running_ = false;

  // Above told the task to exit on the next loop around.
  // That could take 2ms or 50ms based on whether it's in pause or run and how long
  // the actual Run() routine takes too. So we have to wait until we're really terminated here.
  while (!isDead_) {
    Wait(0.02); // Wait until we're really dead on that task.
  }
}

int DualMaxBoticsI2CXL::GetDistance(int address) {
	int distance = 0;
	for (uint i=0; i < sizeof(address_); i++) {
		if(address_[i] == address)
			distance = distance_[i][1] + distance_[i][0]*256;
	}
	return distance;
}
int DualMaxBoticsI2CXL::GetFilteredDistance(int lowfilter, int highfilter) {
	int distance = 0;
	for (uint i=0; i < sizeof(address_); i++) {
		int tempdistance = distance_[i][1] + distance_[i][0]*256;
		if(tempdistance > lowfilter && tempdistance < highfilter)
			distance = tempdistance;
	}
	return distance;

}
int DualMaxBoticsI2CXL::GetFilteredError(int lowfilter, int highfilter, int setpoint) {
	int distance = 0;
	for (uint i=0; i < sizeof(address_); i++) {
		int tempdistance = distance_[i][1] + distance_[i][0]*256;
		if(tempdistance > lowfilter && tempdistance < highfilter)
			distance = tempdistance;
	}
	return setpoint - distance;

}

double DualMaxBoticsI2CXL::PIDGet() {
	return (double)GetFilteredDistance(30, 150);
}
