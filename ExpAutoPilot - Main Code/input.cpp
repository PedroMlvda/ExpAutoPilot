#include "input.h"

void setupInputs() {
  pinMode(PIN_LOCK_ALT, INPUT_PULLUP);
  pinMode(PIN_LOCK_HDG, INPUT_PULLUP);
  pinMode(PIN_ALT_PLUS, INPUT_PULLUP);
  pinMode(PIN_ALT_MINUS, INPUT_PULLUP);
  pinMode(PIN_HDG_PLUS, INPUT_PULLUP);
  pinMode(PIN_HDG_MINUS, INPUT_PULLUP);
}

bool edgeFalling(uint8_t pin, unsigned long now) {
  static bool prev[30];
  static unsigned long t0[30] = {0};
  bool raw = digitalRead(pin);
  if (raw != prev[pin]) { t0[pin] = now; prev[pin] = raw; }
  return (prev[pin] == LOW) && (now - t0[pin] >= DEBOUNCE_MS);
}

bool allControlsReleased() {
  return digitalRead(PIN_LOCK_ALT)==HIGH &&
         digitalRead(PIN_LOCK_HDG)==HIGH &&
         digitalRead(PIN_ALT_PLUS)==HIGH &&
         digitalRead(PIN_ALT_MINUS)==HIGH &&
         digitalRead(PIN_HDG_PLUS)==HIGH &&
         digitalRead(PIN_HDG_MINUS)==HIGH;
}
