#pragma once

#include <Arduino.h>
#include <AxeFxControl.h>
#include "BaseScreen.h"

#define MAX_DISPLAYS 4

class DisplayManager {
public:
  DisplayManager();
  ~DisplayManager();
  
  void begin();
  void update();
  
  // Creëer displays op basis van configuratie
  void createDisplays(const DisplayConfig* configs, uint8_t count);
  
  // Update functies die direct met AxeFxControl klassen werken
  void updatePresetInfo(AxePreset preset);
  void updateEffectStatus(AxeEffect effect);
  void updateTuner(const char* note, byte string, byte fineTune);
  void updateTunerStatus(bool enabled);
  void updateLooperStatus(const MyAxeLooper& looper);
  
private:
  BaseScreen** _displays;
  uint8_t _displayCount;
  bool _needsUpdate;
  
  // Tuner status
  bool _tunerActive;
  char _tunerNote[4];
  byte _tunerString;
  byte _tunerFineTune;
};
