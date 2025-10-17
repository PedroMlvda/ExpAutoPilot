#pragma once
#include <U8g2lib.h>
#include "types.h"

class Display {
public:
  Display();
  void begin();

  // Rendu principal avec un seul message en bas
  void renderMainFrame(const AppState& s, const char* bottomMsg);

  // Écrans de démarrage (anglais)
  void renderStartup(bool gpsOk, bool controlsOk, const char* infoMsg);
  void renderStartupProgress(uint8_t percent);

private:
  U8G2_SSD1309_128X64_NONAME2_F_HW_I2C u8g2_;
  void drawGrid_();
  void drawStatusLine_(const char* label, bool ok, uint8_t y);
};
