#pragma once
#include "types.h"

class PitchController {
public:
  void reset(AppState& s);
  void step(AppState& s, float dtSec);
};
