#pragma once
#include "types.h"
#include "display.h"
#include "gps_service.h"

class Startup {
public:
  void run(AppState& s, Display& ui, GpsService& gps);
};
