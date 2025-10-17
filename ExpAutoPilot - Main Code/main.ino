#include <Wire.h>
#include "config.h"
#include "types.h"
#include "utils.h"
#include "display.h"
#include "input.h"
#include "gps_service.h"
#include "servos.h"
#include "pid_pitch.h"
#include "startup.h"

// État et services globaux
AppState S;
Display UI;
GpsService GpsSvc;   // instance renommée pour éviter conflits
Servos SERVO;
PitchController PID;
Startup STARTUP;

unsigned long lastCtlMs = 0;
unsigned long lastUiMs  = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(); Wire.setClock(I2C_FREQ_HZ);

  setupInputs();
  UI.begin();
  GpsSvc.begin(9600);
  SERVO.begin(PIN_SERVO_PITCH, PIN_SERVO_ROLL);

  // Init état
  S.servoPitchPos = SERVO_PITCH_NTR;

  // Démarrage (écran + tests)
  STARTUP.run(S, UI, GpsSvc);

  // Écran initial (baseline)
  UI.renderMainFrame(S, "Autopilot : Ready.");
  lastCtlMs = millis();
  lastUiMs  = lastCtlMs;
}

void loop() {
  // GPS feed en continu
  GpsSvc.feed(S);

  // Gestion interrupteur Lock Altitude
  bool lockAltPressed = (digitalRead(PIN_LOCK_ALT) == LOW);
  if (lockAltPressed && !S.lockAlt) {
    S.lockAlt = true;
    S.lockedAlt = S.currentAlt; // capture immédiate
    PID.reset(S);
  } else if (!lockAltPressed && S.lockAlt) {
    S.lockAlt = false;
    S.lockedAlt = 0;
    PID.reset(S);
    S.servoPitchPos = SERVO_PITCH_NTR;
  }

  // Gestion interrupteur Lock Heading (capture & clear sur front)
  static bool prevHdgPressed = false;
  bool hdgPressed = (digitalRead(PIN_LOCK_HDG) == LOW);
  if (hdgPressed && !prevHdgPressed) {
    int hdg = S.currentHdg;           // 0..359
    if (hdg == 0) hdg = 360;          // 0 réservé pour "pas de lock"
    S.lockedHdg = hdg;
  } else if (!hdgPressed && prevHdgPressed) {
    S.lockedHdg = 0;                  // clear lock
  }
  prevHdgPressed = hdgPressed;

  // Poussoirs réactifs (à chaque tour)
  unsigned long now = millis();

  static bool firedAltPlus=false, firedAltMinus=false, firedTrimPlus=false, firedTrimMinus=false;

  if (S.lockAlt && edgeFalling(PIN_ALT_PLUS, now)) {
    if (!firedAltPlus) { S.lockedAlt = ((S.lockedAlt + 25)/50)*50 + 50; firedAltPlus = true; }
  } else if (digitalRead(PIN_ALT_PLUS)==HIGH) firedAltPlus = false;

  if (S.lockAlt && edgeFalling(PIN_ALT_MINUS, now)) {
    if (!firedAltMinus){ S.lockedAlt = ((S.lockedAlt + 25)/50)*50 - 50; firedAltMinus = true; }
  } else if (digitalRead(PIN_ALT_MINUS)==HIGH) firedAltMinus = false;

  if (hdgPressed && edgeFalling(PIN_HDG_PLUS, now)) {
    if (!firedTrimPlus) { S.servoPitchPos = clampInt(S.servoPitchPos+1, SERVO_PITCH_MIN, SERVO_PITCH_MAX); firedTrimPlus = true; }
  } else if (digitalRead(PIN_HDG_PLUS)==HIGH) firedTrimPlus = false;

  if (hdgPressed && edgeFalling(PIN_HDG_MINUS, now)) {
    if (!firedTrimMinus){ S.servoPitchPos = clampInt(S.servoPitchPos-1, SERVO_PITCH_MIN, SERVO_PITCH_MAX); firedTrimMinus = true; }
  } else if (digitalRead(PIN_HDG_MINUS)==HIGH) firedTrimMinus = false;

  // PID @ CTRL_HZ
  if (now - lastCtlMs >= (1000/CTRL_HZ)) {
    float dt = (now - lastCtlMs) * 0.001f;
    lastCtlMs = now;
    PID.step(S, dt);
    SERVO.writePitch(S.servoPitchPos);
  }

  // UI @ UI_FPS : message unique
  if (now - lastUiMs >= (1000/UI_FPS)) {
    lastUiMs = now;
    UI.renderMainFrame(S, "Autopilot : Ready.");
  }
}
