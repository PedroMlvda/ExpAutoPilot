#pragma once
#include <Arduino.h>

int   clampInt(int v, int lo, int hi);
float clampFloat(float v, float lo, float hi);
String padRightInt(int value, uint8_t width);
