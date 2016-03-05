#include "MaxBoticsI2CXL.h"

#include "WPILib.h"
#include <string>

MaxBoticsI2CXL::MaxBoticsI2CXL(I2C::Port port, uint8_t address, float frequency) {
  enabled_ = true;
  running_ = true;
  isDead_ = false;
  address_ = address;
  frequency_ = frequency;
  sensor_ = new I2C(port, address);
  task_ = new Task("MaxBoticsI2CXL", (FUNCPTR)MaxBoticsI2CXL::Run, Task::kDefaultPriority);
}

MaxBoticsI2CXL::~MaxBoticsI2CXL(){
  delete task_;
}

void MaxBoticsI2CXL::Run(MaxBoticsI2CXL* task) {
  while (task->running_) {
    if (task->enabled_) {
    	task->sensor_->Write((unsigned char)INITREADING,0);
    	Wait(task->frequency_);
    	task->sensor_->ReadOnly(2,task->distance_);
    }
    else
      Wait(1.0);   // 50 ms wait period while task is 'paused'
  }

  task->isDead_ = true; // Falling off the edge of the earth...
}

void MaxBoticsI2CXL::Start() {
  enabled_ = true;
}

void MaxBoticsI2CXL::Pause() {
  enabled_ = false;
}

void MaxBoticsI2CXL::Terminate() {
  running_ = false;

  // Above told the task to exit on the next loop around.
  // That could take 2ms or 50ms based on whether it's in pause or run and how long
  // the actual Run() routine takes too. So we have to wait until we're really terminated here.
  while (!isDead_) {
    Wait(0.02); // Wait until we're really dead on that task.
  }
}
int MaxBoticsI2CXL::GetDistance() {
	return distance_[1] + distance_[0]*256;
}
