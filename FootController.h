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
  ~FootController();
  
  void begin();
  void update();
  
  // Getters
  AxeFxManager* getAxeFxManager() { return _axeFxManager; }
  
private:
  ConfigManager* _configManager;
  ButtonManager* _buttonManager;
  AxeFxManager* _axeFxManager;
  DisplayManager* _displayManager;
  
  bool _tunerEnabled = false;
  
  // Event handlers
  void onButtonEvent(uint8_t buttonId, ButtonEvent event);
  void onPresetChange(AxePreset preset);
  void onEffectBypass(AxeEffect effect);
  void onTunerData(const char* note, byte string, byte fineTune);
  void onTunerStatus(bool enabled);
  void onLooperStatus(AxeLooper looper);
  
  // Statische callback functies
  static void onPresetChangeStatic(AxePreset preset);
  static void onEffectBypassStatic(AxeEffect effect);
  static void onTunerDataStatic(const char* note, byte string, byte fineTune);
  static void onTunerStatusStatic(bool enabled);
  static void onLooperStatusStatic(AxeLooper looper);
};

// Globale pointer voor statische callbacks
extern FootController* g_footController;
