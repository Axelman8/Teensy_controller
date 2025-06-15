/*#ifndef FOOT_CONTROLLER_H
#define FOOT_CONTROLLER_H

#include <Arduino.h>
#include "config/ConfigManager.h"
#include "display/DisplayManager.h"
#include "input/ButtonManager.h"
#include "axefx/AxeFxManager.h"

class FootController {
public:
  FootController();
  
  // Initialisatie
  void begin();
  
  // Hoofdloop update
  void update();
  
private:
  // Managers voor verschillende onderdelen
  ConfigManager configManager;
  DisplayManager displayManager;
  ButtonManager buttonManager;
  AxeFxManager axeFxManager;
  
  // Interne status
  bool initialized;
  unsigned long lastUpdateTime;
  
  // Interne methoden
  void handleButtonEvents();
  void updateDisplays();
  void processAxeFxCallbacks();
};

#endif // FOOT_CONTROLLER_H*/


#ifndef FOOT_CONTROLLER_H
#define FOOT_CONTROLLER_H

#include <Arduino.h>
#include "config/ConfigManager.h"
#include "display/DisplayManager.h"
#include "input/ButtonManager.h"
#include "axefx/AxeFxManager.h"

class FootController {
public:
  FootController();
  
  // Initialisatie
  void begin();
  
  // Hoofdloop update
  void update();
  
  // Statische pointer naar huidige instantie voor callbacks
  static FootController* instance;
  
  // Callback handlers
  void handlePresetChange(const AxePreset& preset);
  void handleEffectBypass(const AxeEffect& effect);
  void handleTunerData(const AxeTuner& tuner);
  
private:
  // Managers voor verschillende onderdelen
  ConfigManager configManager;
  DisplayManager displayManager;
  ButtonManager buttonManager;
  AxeFxManager axeFxManager;
  
  // Interne status
  bool initialized;
  unsigned long lastUpdateTime;
  
  // Interne methoden
  void handleButtonEvents();
  void updateDisplays();
  void processAxeFxCallbacks();
};

// Statische callback functies
void onPresetChange(const AxePreset& preset);
void onEffectBypass(const AxeEffect& effect);
void onTunerData(const AxeTuner& tuner);

#endif // FOOT_CONTROLLER_H

