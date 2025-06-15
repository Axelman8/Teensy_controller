#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include "../config/ConfigManager.h"
#include "../axefx/AxeFxTypes.h"

// Forward declarations
class Screen;

// Maximum aantal displays dat ondersteund wordt
#define MAX_DISPLAYS 4

class DisplayManager {
public:
  // Constructor met ConfigManager pointer
  DisplayManager(ConfigManager* configManager);
  
  // Destructor
  ~DisplayManager();
  
  // Initialisatie
  void begin();
  
  // Update alle displays
  void update();
  
  // Update displays met Axe-Fx informatie
  void updatePresetInfo(const AxePreset& preset);
  void updateEffectStatus(const AxeEffect& effect);
  void updateTuner(const AxeTuner& tuner);
  
private:
  ConfigManager* _configManager;
  
  // Array van display pointers
  Screen* _displays[MAX_DISPLAYS];
  uint8_t _displayCount;
  
  // Huidige status informatie
  AxePreset _currentPreset;
  AxeTuner _currentTuner;
  
  // Status flags
  bool _needsUpdate;
  bool _tunerActive;
  
  // Helper methoden
  void createDisplays();
  void destroyDisplays();
  void renderPresetInfo(Screen* display);
  void renderEffectStatus(Screen* display);
  void renderTuner(Screen* display);
};

#endif // DISPLAY_MANAGER_H
