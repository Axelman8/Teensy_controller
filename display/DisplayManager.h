#pragma once

#include <Arduino.h>
#include <AxeFxControl.h>
#include "BaseDisplay.h"  
#include "../config/ConfigManager.h"

class DisplayManager {
public:
  DisplayManager();
  ~DisplayManager();
  
  void begin();
  void update();
  
  // Creëer displays op basis van configuratie
  void createDisplays(const DisplayConfig* configs, uint8_t count);
  
  // Update functies
  void updatePresetInfo(AxePreset preset);
  void updateEffectStatus(AxeEffect effect);
  void updateTuner(const char* note, byte string, byte fineTune);
  void updateTunerStatus(bool enabled);
  
private:
  BaseScreen** _displays;
  uint8_t _displayCount;
  bool _needsUpdate;
  bool _tunerActive;
  
  // Tuner data
  char _tunerNote[4];
  byte _tunerString;
  byte _tunerFineTune;
};
