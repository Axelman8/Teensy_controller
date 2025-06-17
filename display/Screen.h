#pragma once

#include <Arduino.h>
#include "../axefx/AxeFxTypes.h"

class Screen {
public:
  virtual ~Screen() {}
  
  virtual void begin() = 0;
  virtual void update() = 0;
  virtual void select() = 0;
  
  // Update methodes voor verschillende content types
  virtual void updatePresetInfo(const MyAxePreset& preset) = 0;
  virtual void updateEffectStatus(const MyAxeEffect& effect) = 0;
  virtual void showTuner(const MyAxeTuner& tuner) = 0;
};
