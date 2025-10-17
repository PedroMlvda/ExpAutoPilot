#include "startup.h"
#include "input.h"
#include "config.h"

void Startup::run(AppState& s, Display& ui, GpsService& gps) {
  unsigned long lastUi = 0;
  // Wait for both OK
  for (;;) {
    gps.feed(s);
    bool gpsOk = gps.isOk(s);
    bool controlsOk = allControlsReleased();

    unsigned long now = millis();
    if (now - lastUi >= 200) {
      lastUi = now;
      const char* msg = (!gpsOk && !controlsOk) ? "Waiting: GPS & buttons" :
                        (!gpsOk)                ? "Waiting for GPS..."     :
                        (!controlsOk)           ? "Release all buttons"    : "Ready";
      ui.renderStartup(gpsOk, controlsOk, msg);
    }
    if (gpsOk && controlsOk) break;
  }

  // Progress bar 5 s
  const uint16_t total = STARTUP_PROGRESS_MS;
  unsigned long t0 = millis();
  for (;;) {
    gps.feed(s);
    unsigned long now = millis();
    uint16_t elapsed = (now - t0 >= total) ? total : (uint16_t)(now - t0);
    uint8_t pct = (uint8_t)((elapsed * 100UL) / total);
    ui.renderStartupProgress(pct);
    if (elapsed >= total) break;
    delay(30);
  }
}
