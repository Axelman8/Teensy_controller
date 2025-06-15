#include "DisplayManager.h"
#include ".../Screen.h"
#include ".../ST7735Screen.h"
#include ".../ST7789Screen.h"

DisplayManager::DisplayManager(ConfigManager* configManager) :
  _configManager(configManager),
  _displayCount(0),
  _needsUpdate(false),
  _tunerActive(false)
{
  // Initialiseer display pointers
  for (uint8_t i = 0; i < MAX_DISPLAYS; i++) {
    _displays[i] = nullptr;
  }
  
  // Initialiseer preset info
  _currentPreset.presetNumber = 0;
  _currentPreset.sceneNumber = 0;
  strcpy(_currentPreset.name, "");
  
  // Initialiseer tuner info
  _currentTuner.enabled = false;
  _currentTuner.note = ' ';
  _currentTuner.cents = 0;
}

DisplayManager::~DisplayManager() {
  destroyDisplays();
}

void DisplayManager::begin() {
  // Maak displays aan op basis van configuratie
  createDisplays();
  
  // Initialiseer alle displays
  for (uint8_t i = 0; i < _displayCount; i++) {
    if (_displays[i] != nullptr) {
      _displays[i]->begin();
      _displays[i]->clear();
    }
  }
  
  // Markeer voor initiële update
  _needsUpdate = true;
}

void DisplayManager::update() {
  // Update displays alleen als nodig
  if (_needsUpdate) {
    for (uint8_t i = 0; i < _displayCount; i++) {
      if (_displays[i] != nullptr) {
        // Kies wat te renderen op basis van status
        if (_tunerActive) {
          renderTuner(_displays[i]);
        } else {
          renderPresetInfo(_displays[i]);
          renderEffectStatus(_displays[i]);
        }
        
        // Update het display
        _displays[i]->update();
      }
    }
    
    // Reset update flag
    _needsUpdate = false;
  }
}

void DisplayManager::updatePresetInfo(const AxePreset& preset) {
  // Update huidige preset info
  _currentPreset = preset;
  
  // Markeer voor update
  _needsUpdate = true;
}

void DisplayManager::updateEffectStatus(const AxeEffect& effect) {
  // Update effect status in huidige preset
  for (uint8_t i = 0; i < MAX_EFFECTS; i++) {
    if (_currentPreset.effects[i].effectId == effect.effectId) {
      _currentPreset.effects[i].bypassed = effect.bypassed;
      break;
    }
  }
  
  // Markeer voor update
  _needsUpdate = true;
}

void DisplayManager::updateTuner(const AxeTuner& tuner) {
  // Update tuner info
  _currentTuner = tuner;
  
  // Update tuner status
  _tunerActive = tuner.enabled;
  
  // Markeer voor update
  _needsUpdate = true;
}

void DisplayManager::createDisplays() {
  // Haal display configuratie op
  DisplayConfig* displayConfigs = _configManager->getDisplayConfigs();
  _displayCount = _configManager->getDisplayCount();
  
  // Maak displays aan op basis van configuratie
  for (uint8_t i = 0; i < _displayCount && i < MAX_DISPLAYS; i++) {
    DisplayConfig* config = &displayConfigs[i];
    
    // Maak het juiste type display aan
    switch (config->type) {
      case DISPLAY_TYPE_ST7735:
        _displays[i] = new ST7735Screen(config->csPin, config->dcPin, config->rstPin);
        break;
        
      case DISPLAY_TYPE_ST7789:
        _displays[i] = new ST7789Screen(config->csPin, config->dcPin, config->rstPin);
        break;
        
      default:
        // Onbekend display type
        _displays[i] = nullptr;
        break;
    }
  }
}

void DisplayManager::destroyDisplays() {
  // Verwijder alle display objecten
  for (uint8_t i = 0; i < _displayCount; i++) {
    if (_displays[i] != nullptr) {
      delete _displays[i];
      _displays[i] = nullptr;
    }
  }
  
  _displayCount = 0;
}

void DisplayManager::renderPresetInfo(Screen* display) {
  // Teken preset informatie op het display
  char buffer[32];
  
  // Teken preset nummer en naam
  sprintf(buffer, "Preset: %d", _currentPreset.presetNumber);
  display->drawText(5, 10, buffer, 0xFFFF);
  display->drawText(5, 30, _currentPreset.name, 0xFFFF);
  
  // Teken scene nummer
  sprintf(buffer, "Scene: %d", _currentPreset.sceneNumber);
  display->drawText(5, 50, buffer, 0xFFFF);
}

void DisplayManager::renderEffectStatus(Screen* display) {
  // Teken effect status op het display
  int y = 70;
  
  // Teken status van elk effect
  for (uint8_t i = 0; i < MAX_EFFECTS; i++) {
    if (_currentPreset.effects[i].effectId != 0) {
      // Bepaal kleur op basis van bypass status
      uint16_t color = _currentPreset.effects[i].bypassed ? 0x7BEF : 0x07E0;
      
      // Teken effect naam en status
      display->drawText(5, y, _currentPreset.effects[i].name, color);
      y += 15;
      
      // Beperk aantal effecten dat getoond wordt
      if (y > 150) break;
    }
  }
}

void DisplayManager::renderTuner(Screen* display) {
  // Teken tuner informatie op het display
  char buffer[32];
  
  // Teken tuner titel
  display->drawText(40, 20, "TUNER", 0xFFFF);
  
  // Teken noot
  char noteStr[2] = {_currentTuner.note, '\0'};
  display->drawText(60, 50, noteStr, 0xFFFF);
  
  // Teken cents indicator
  sprintf(buffer, "%+d cents", _currentTuner.cents);
  display->drawText(30, 80, buffer, 0xFFFF);
  
  // Teken tuner meter
  int meterWidth = 100;
  int meterHeight = 20;
  int meterX = (display->width() - meterWidth) / 2;
  int meterY = 110;
  
  // Teken meter achtergrond
  display->drawRect(meterX, meterY, meterWidth, meterHeight, 0xFFFF);
  
  // Teken indicator
  int indicatorPos = meterWidth / 2 + (_currentTuner.cents * meterWidth / 100);
  indicatorPos = constrain(indicatorPos, 0, meterWidth);
  
  // Bepaal kleur op basis van hoe dicht bij 0 cents
  uint16_t color;
  if (abs(_currentTuner.cents) < 5) {
    color = 0x07E0;  // Groen
  } else if (abs(_currentTuner.cents) < 15) {
    color = 0xFFE0;  // Geel
  } else {
    color = 0xF800;  // Rood
  }
  
  // Teken indicator
  display->fillRect(meterX + indicatorPos - 2, meterY, 4, meterHeight, color);
  
  // Teken middenlijn
  display->drawLine(meterX + meterWidth / 2, meterY, meterX + meterWidth / 2, meterY + meterHeight, 0xFFFF);
}
