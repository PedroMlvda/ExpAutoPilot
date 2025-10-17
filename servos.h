#pragma once
#include <Servo.h>
#include "types.h"

class Servos {
public:
  void begin(uint8_t pinPitch, uint8_t pinRoll);
  void writePitch(int pos);
private:
  Servo pitch_;
};
