#ifndef MaxBoticsI2CXL_H_
#define MaxBoticsI2CXL_H_
#define INITREADING 0x51

#include "WPILib.h"

class MaxBoticsI2CXL {
 public:
  MaxBoticsI2CXL(I2C::Port port, uint8_t adress, float frequency);
  virtual ~MaxBoticsI2CXL();
  static void Run(MaxBoticsI2CXL* task);
  void Start();
  void Pause();
  void Terminate();
  int GetDistance();
 private:
  int address_;
  bool enabled_;
  bool running_;
  bool isDead_;
  unsigned char distance_[2] = {0,0};
  float frequency_;
  Task* task_;
  I2C* sensor_;
};

#endif  // MaxBoticsI2CXL_H_
