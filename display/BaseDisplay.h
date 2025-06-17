#pragma once

#include <Arduino.h>
#include <AxeFxControl.h>

class BaseScreen {
public:
  virtual ~BaseScreen() {}
  
  virtual void begin() = 0;
  virtual void update() = 0;
  
  // Update functies
  virtual void updatePresetInfo(AxePreset preset) = 0;
  virtual void updateEffectStatus(AxeEffect effect) = 0;
  virtual void updateTuner(const char* note, byte string, byte fineTune) = 0;
  virtual void showTuner(bool enabled, const char* note, byte string, byte fineTune) = 0;
};
