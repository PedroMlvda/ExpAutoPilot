#pragma once
#include <Arduino.h>

struct PidState {
  float error = 0.0f;
  float errorPrev = 0.0f;
  float integral = 0.0f;
  float derivative = 0.0f;
};

struct AppState {
  // Mesures
  int   currentAlt = 0;   // ft (EMA + quantification)
  int   currentHdg = 0;   // deg 0..359

  // EMA altitude (interne)
  float altEma = 0.0f;    // pieds
  bool  altEmaInit = false;

  // Modes / consignes
  bool lockAlt = false;   // Alt lock actif
  int  lockedAlt = 0;     // ft
  int  lockedHdg = 0;     // deg (0 => pas de lock; utiliser 360 pour Nord verrouillé)

  // Servos
  int servoPitchPos = 90;
  int servoRollPos  = 90;

  // GPS
  unsigned long lastGpsDataMs = 0; // ms

  // PID
  PidState pidPitch;
};
