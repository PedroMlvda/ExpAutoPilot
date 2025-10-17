#pragma once
#include <TinyGPSPlus.h>
#include "types.h"

class GpsService {
public:
  void begin(unsigned long baud);
  void feed(AppState& s);
  bool isOk(const AppState& s) const;
  TinyGPSPlus& raw() { return gps_; }
private:
  TinyGPSPlus gps_;
};
