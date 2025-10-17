#include "pid_pitch.h"
#include "config.h"
#include "utils.h"
#include <math.h>

void PitchController::reset(AppState& s) {
  s.pidPitch = {}; // reset all PID accumulators
}

void PitchController::step(AppState& s, float dt) {
  if (!s.lockAlt) return;
  auto& p = s.pidPitch;

  p.error = (float)(s.lockedAlt - s.currentAlt);
  p.integral += p.error * dt;
  p.integral = clampFloat(p.integral, -300.0f, 300.0f);
  p.derivative = (dt > 0.0005f) ? (p.error - p.errorPrev) / dt : 0.0f;

  float out = KpPitch*p.error + KiPitch*p.integral + KdPitch*p.derivative;

  if (fabsf(p.error) < 10.0f) {
    s.servoPitchPos = SERVO_PITCH_NTR;
  } else {
    int target = SERVO_PITCH_NTR + (int)out;
    s.servoPitchPos = clampInt(target, SERVO_PITCH_MIN, SERVO_PITCH_MAX);
  }

  p.errorPrev = p.error;
}
