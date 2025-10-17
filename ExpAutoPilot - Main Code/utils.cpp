#include "utils.h"

int clampInt(int v, int lo, int hi) {
  return v < lo ? lo : (v > hi ? hi : v);
}

float clampFloat(float v, float lo, float hi) {
  return v < lo ? lo : (v > hi ? hi : v);
}

String padRightInt(int value, uint8_t width) {
  char buf[16];
  snprintf(buf, sizeof(buf), "%d", value);
  String s(buf);
  while ((uint8_t)s.length() < width) s = " " + s;
  return s;
}
