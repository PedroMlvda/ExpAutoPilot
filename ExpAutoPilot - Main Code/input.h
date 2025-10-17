#pragma once
#include <Arduino.h>
#include "config.h"

void setupInputs();
bool edgeFalling(uint8_t pin, unsigned long now);
bool allControlsReleased();
