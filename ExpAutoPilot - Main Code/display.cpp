#include "display.h"
#include "utils.h"

Display::Display()
: u8g2_(U8G2_R0, /*reset=*/U8X8_PIN_NONE) {}

void Display::begin() {
  u8g2_.begin();
  u8g2_.setFontPosTop();
  u8g2_.setFontDirection(0);
}

void Display::drawGrid_() {
  u8g2_.drawLine(66, 0, 66, 52);
  u8g2_.drawLine(0, 17, 128, 17);
  u8g2_.drawLine(0, 36, 128, 36);
  u8g2_.drawLine(0, 52, 128, 52);
}

void Display::drawStatusLine_(const char* label, bool ok, uint8_t y) {
  const uint8_t RIGHT_MARGIN = 12; // décale OK/NO vers la gauche
  u8g2_.setFont(u8g2_font_6x10_tf);
  // Libellé à gauche
  u8g2_.setCursor(0, y);
  u8g2_.print(label);
  // Valeur à droite avec marge
  const char* s = ok ? "OK" : "NO";
  uint8_t w = u8g2_.getStrWidth(s);
  int16_t xr = 128 - RIGHT_MARGIN - w;
  if (xr < 0) xr = 0;
  u8g2_.setCursor(xr, y);
  u8g2_.print(s);
}

void Display::renderMainFrame(const AppState& s, const char* bottomMsg) {
  u8g2_.clearBuffer();
  drawGrid_();

  // ====== Ligne 1 : Alt & Hdg courants ======
  u8g2_.setFont(u8g2_font_10x20_tf);
  u8g2_.setCursor(0,0);  u8g2_.print(padRightInt(s.currentAlt,5));
  u8g2_.setFont(u8g2_font_6x10_tf);
  u8g2_.setCursor(50,0); u8g2_.print("ft");
  u8g2_.setFont(u8g2_font_10x20_tf);
  u8g2_.setCursor(80,0); u8g2_.print(padRightInt(s.currentHdg,3));
  u8g2_.setFont(u8g2_font_unifont_t_symbols);
  u8g2_.drawGlyph(110,0,0x00B0);

  // ====== Ligne 2 : Alt lock (gauche) + Hdg lock (droite, aligné dynamiquement) ======
  // Altitude verrouillée : s.lockAlt pilote l'affichage immédiat
  if (s.lockAlt) {
    u8g2_.setFont(u8g2_font_10x20_tf);
    u8g2_.setCursor(0,20); u8g2_.print(padRightInt(s.lockedAlt,5));
    // Cadenas gauche
    u8g2_.drawBox(52, 27, 9, 6);
    u8g2_.drawCircle(56, 25, 3, U8G2_DRAW_ALL);
  } else {
    u8g2_.setFont(u8g2_font_5x7_tf);
    u8g2_.setCursor(0,23); u8g2_.print("AltLock: No");
  }

  // Paramètres généraux d'alignement à droite
  const uint8_t SCREEN_W = 128;
  const uint8_t RIGHT_PAD = 2; // réduit la largeur effective d'écran de 2 px (marge)
  const int rightEdge = SCREEN_W - RIGHT_PAD;

  // Cap verrouillé (à droite) — alignement dynamique et marge droite
  if (s.lockedHdg != 0) {
    // 1) nombre en 10x20
    u8g2_.setFont(u8g2_font_10x20_tf);
    int hdgDisp = (s.lockedHdg == 360) ? 360 : s.lockedHdg; // 1..360
    char num[8];
    snprintf(num, sizeof(num), "%3d", hdgDisp);
    uint8_t wNum = u8g2_.getStrWidth(num);

    // 2) symbole degré (police unifont)
    u8g2_.setFont(u8g2_font_unifont_t_symbols);
    uint8_t wDeg = u8g2_.getUTF8Width("\xC2\xB0");
    if (wDeg == 0) wDeg = u8g2_.getMaxCharWidth();

    // 3) cadenas (primitives)
    const uint8_t lockW = 9;   // largeur du cadenas
    const uint8_t gap   = 2;   // espace entre ° et cadenas

    // xStart = bord droit effectif - (nombre + ° + gap + cadenas)
    int xStart = rightEdge - (int)(wNum + wDeg + gap + lockW);
    if (xStart < 66) xStart = 66; // ne pas empiéter sur la grille centrale

    //  Rendu aligné à droite
    u8g2_.setFont(u8g2_font_10x20_tf);
    u8g2_.setCursor(xStart, 20); u8g2_.print(num);

    u8g2_.setFont(u8g2_font_unifont_t_symbols);
    u8g2_.drawGlyph(xStart + wNum, 20, 0x00B0);

    // Cadenas à la fin
    int lockX = xStart + wNum + wDeg + gap;
    u8g2_.drawBox(lockX, 27, 9, 6);
    u8g2_.drawCircle(lockX + 4, 25, 3, U8G2_DRAW_ALL);
  } else {
    // "HdgLock: No" aligné à droite avec marge
    u8g2_.setFont(u8g2_font_5x7_tf);
    const char* txt = "HdgLock: No";
    uint8_t wNo = u8g2_.getStrWidth(txt);
    int16_t xNo = rightEdge - wNo;
    if (xNo < 66) xNo = 66;
    u8g2_.setCursor(xNo, 23);
    u8g2_.print(txt);
  }

  // ====== Ligne 3 : Servos ======
  u8g2_.setFont(u8g2_font_5x7_tf);
  u8g2_.setCursor(0,40); u8g2_.print("Pitch:");
  u8g2_.setFont(u8g2_font_6x10_tf);
  u8g2_.print(padRightInt(s.servoPitchPos,3));
  u8g2_.setFont(u8g2_font_unifont_t_symbols);
  u8g2_.drawGlyph(53,39,0x00B0);
  u8g2_.setFont(u8g2_font_6x10_tf);
  u8g2_.setCursor(71,40); u8g2_.print("Roll:");
  u8g2_.print(padRightInt(s.servoRollPos,3));
  u8g2_.setFont(u8g2_font_unifont_t_symbols);
  u8g2_.drawGlyph(120,39,0x00B0);

  // ====== Ligne 4 : message bas ======
  u8g2_.setFont(u8g2_font_6x10_tf);
  u8g2_.setCursor(0,54);
  u8g2_.print(bottomMsg);

  u8g2_.sendBuffer();
}

void Display::renderStartup(bool gpsOk, bool controlsOk, const char* infoMsg) {
  u8g2_.clearBuffer();
  u8g2_.setFont(u8g2_font_6x10_tf);

  const char* title = "STARTUP";
  uint8_t tw = u8g2_.getStrWidth(title);
  int16_t tx = (128 - tw) / 2; if (tx < 0) tx = 0;
  u8g2_.setCursor(tx,0); u8g2_.print(title);
  u8g2_.drawLine(0,12,128,12);

  drawStatusLine_("GPS",      gpsOk,     20);
  drawStatusLine_("Controls", controlsOk, 32);

  u8g2_.drawLine(0,44,128,44);
  u8g2_.setCursor(0,54); u8g2_.print(infoMsg);

  u8g2_.sendBuffer();
}

void Display::renderStartupProgress(uint8_t percent) {
  if (percent > 100) percent = 100;
  u8g2_.clearBuffer();

  const char* title = "STARTUP";
  u8g2_.setFont(u8g2_font_6x10_tf);
  uint8_t tw = u8g2_.getStrWidth(title);
  int16_t tx = (128 - tw)/2; if (tx < 0) tx = 0;
  u8g2_.setCursor(tx,0); u8g2_.print(title);
  u8g2_.drawLine(0,12,128,12);

  drawStatusLine_("GPS",      true, 20);
  drawStatusLine_("Controls", true, 32);

  u8g2_.drawLine(0,44,128,44);

  const uint8_t x=0,y=54,w=128,h=10;
  u8g2_.drawFrame(x,y,w,h);
  uint8_t fill = (w-2)*percent/100;
  u8g2_.drawBox(x+1,y+1,fill,h-2);

  u8g2_.sendBuffer();
}
