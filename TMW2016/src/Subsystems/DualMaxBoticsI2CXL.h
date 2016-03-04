#ifndef DualMaxBoticsI2CXL_H_
#define DualMaxBoticsI2CXL_H_
#define INITREADING 0x51

#include "WPILib.h"
#include "PIDSource.h"

class DualMaxBoticsI2CXL: public SensorBase, public PIDSource {
 public:
  DualMaxBoticsI2CXL(I2C::Port port, uint8_t address1, uint8_t address2, float frequency);
  virtual ~DualMaxBoticsI2CXL();
  static void Run(DualMaxBoticsI2CXL* task);
  void Start();
  void Pause();
  void Terminate();
  int GetDistance(int address);
  int GetFilteredDistance(int lowfilter, int highfilter);
  int GetFilteredError(int lowfilter, int highfilter, int setpoint);
  double PIDGet();
 private:
//  uint8_t address1_;
 // uint8_t address2_;
  uint8_t address_[2];
  unsigned char distance_[2][2];
  bool enabled_;
  bool running_;
  bool isDead_;
//  unsigned char distance1_[2] = {0,0};
 // unsigned char distance2_[2] = {0,0};
  float frequency_;
  Task* task_;
//  I2C* sensor1_;
//  I2C* sensor2_;
  I2C* sensor_[2];
};

#endif  // DualMaxBoticsI2CXL_H_
