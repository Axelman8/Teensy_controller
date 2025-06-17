#pragma once

#include <Arduino.h>
#include "config/ConfigManager.h"
#include "input/ButtonManager.h"
#include "display/DisplayManager.h"
#include "axefx/AxeFxManager.h"
#include <AxeFxControl.h>

class FootController {
public:
  FootController();
  
  void begin();
  void update();
  
  // Callback functies voor AxeFxManager
  void onPresetChange(AxePreset preset);
  void onEffectBypass(AxeEffect effect);
  void onTunerData(const char* note, byte string, byte fineTune);
  void onTunerStatus(bool enabled);
  void onLooperStatus(AxeLooper looper);
  
  // Callback functie voor ButtonManager
  void onButtonEvent(const ButtonEvent& event);
  
private:
  ConfigManager* _configManager;
  ButtonManager* _buttonManager;
  DisplayManager* _displayManager;
  AxeFxManager* _axeFxManager;
};
