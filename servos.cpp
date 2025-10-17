#include "servos.h"
#include "config.h"
#include "utils.h"

void Servos::begin(uint8_t pinPitch, uint8_t /*pinRoll*/) {
  pitch_.attach(pinPitch);
  pitch_.write(SERVO_PITCH_NTR);
}

void Servos::writePitch(int pos) {
  pitch_.write(clampInt(pos, SERVO_PITCH_MIN, SERVO_PITCH_MAX));
}
