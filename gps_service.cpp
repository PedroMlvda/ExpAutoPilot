#include "gps_service.h"
#include <Arduino.h>
#include <math.h>
#include "config.h"

void GpsService::begin(unsigned long baud) { Serial1.begin(baud); }

void GpsService::feed(AppState& s) {
  while (Serial1.available() > 0) {
    if (gps_.encode(Serial1.read())) s.lastGpsDataMs = millis();
  }

  // --- ALTITUDE: EMA + quantification ---
  if (gps_.altitude.isValid()) {
    float altFt = gps_.altitude.feet();
    if (!s.altEmaInit) {
      s.altEma = altFt;
      s.altEmaInit = true;
    } else {
      s.altEma = ALT_EMA_ALPHA * altFt + (1.0f - ALT_EMA_ALPHA) * s.altEma;
    }
    int quant = (int) lroundf(s.altEma / (float)ALT_QUANT_FT) * ALT_QUANT_FT;
    s.currentAlt = quant;
  }

  // --- CAP: direct depuis TinyGPS++ ---
  if (gps_.course.isValid()) {
    int hdg = (int) lround(gps_.course.deg()); // 0..359
    if (hdg >= 360) hdg -= 360;
    if (hdg < 0)    hdg += 360;
    s.currentHdg = hdg;
  }
}

bool GpsService::isOk(const AppState& s) const {
  unsigned long now = millis();
  bool recent = (now - s.lastGpsDataMs) < 2000;
  bool chars  = gps_.charsProcessed() > 120;
  return recent && chars;
}
