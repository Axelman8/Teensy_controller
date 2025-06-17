#include "DisplayManager.h"
#include "ST7735Screen.h"
#include "ST7789Screen.h"

DisplayManager::DisplayManager() : _displays(nullptr), _displayCount(0), _needsUpdate(false), _tunerActive(false) {
  _tunerNote[0] = '\0';
  _tunerString = 0;
  _tunerFineTune = 0;
}

DisplayManager::~DisplayManager() {
  // Ruim displays op
  if (_displays != nullptr) {
    for (uint8_t i = 0; i < _displayCount; i++) {
      if (_displays[i] != nullptr) {
        delete _displays[i];
      }
    }
    delete[] _displays;
  }
}

void DisplayManager::begin() {
  // Initialisatie
}

void DisplayManager::update() {
  // Update displays alleen als nodig
  if (_needsUpdate) {
    for (uint8_t i = 0; i < _displayCount; i++) {
      if (_displays[i] != nullptr) {
        // Update het display
        _displays[i]->update();
      }
    }
    
    // Reset update flag
    _needsUpdate = false;
  }
}

void DisplayManager::createDisplays(const DisplayConfig* configs, uint8_t count) {
  // Ruim bestaande displays op
  if (_displays != nullptr) {
    for (uint8_t i = 0; i < _displayCount; i++) {
      if (_displays[i] != nullptr) {
        delete _displays[i];
      }
    }
    delete[] _displays;
  }
  
  // Maak nieuwe array voor displays
  _displayCount = count;
  _displays = new BaseScreen*[_displayCount];
  
  // Initialiseer alle displays op null
  for (uint8_t i = 0; i < _displayCount; i++) {
    _displays[i] = nullptr;
  }
  
  // Maak displays aan op basis van configuratie
  for (uint8_t i = 0; i < _displayCount; i++) {
    const DisplayConfig& config = configs[i];
    
    switch (config.type) {
      case DISPLAY_TYPE_ST7735:
        _displays[i] = new ST7735Screen(config.csPin, config.dcPin, config.rstPin);
        break;
        
      case DISPLAY_TYPE_ST7789:
        _displays[i] = new ST7789Screen(config.csPin, config.dcPin, config.rstPin);
        break;
        
      default:
        // Onbekend display type
        break;
    }
    
    // Initialiseer het display
    if (_displays[i] != nullptr) {
      _displays[i]->begin();
    }
  }
  
  // Markeer voor update
  _needsUpdate = true;
}

void DisplayManager::updatePresetInfo(AxePreset preset) {
  // Update displays met preset info
  for (uint8_t i = 0; i < _displayCount; i++) {
    if (_displays[i] != nullptr) {
      _displays[i]->updatePresetInfo(preset);
    }
  }
  
  // Markeer voor update
  _needsUpdate = true;
}

void DisplayManager::updateEffectStatus(AxeEffect effect) {
  // Update displays met effect status
  for (uint8_t i = 0; i < _displayCount; i++) {
    if (_displays[i] != nullptr) {
      _displays[i]->updateEffectStatus(effect);
    }
  }
  
  // Markeer voor update
  _needsUpdate = true;
}

void DisplayManager::updateTuner(const char* note, byte string, byte fineTune) {
  // Sla tuner data op
  strncpy(_tunerNote, note, sizeof(_tunerNote) - 1);
  _tunerNote[sizeof(_tunerNote) - 1] = '\0';
  _tunerString = string;
  _tunerFineTune = fineTune;
  
  // Update displays met tuner info als tuner actief is
  if (_tunerActive) {
    for (uint8_t i = 0; i < _displayCount; i++) {
      if (_displays[i] != nullptr) {
        _displays[i]->updateTuner(note, string, fineTune);
      }
    }
    
    // Markeer voor update
    _needsUpdate = true;
  }
}

void DisplayManager::updateTunerStatus(bool enabled) {
  // Update tuner status
  _tunerActive = enabled;
  
  // Update displays met tuner status
  for (uint8_t i = 0; i < _displayCount; i++) {
    if (_displays[i] != nullptr) {
      _displays[i]->showTuner(enabled, _tunerNote, _tunerString, _tunerFineTune);
    }
  }
  
  // Markeer voor update
  _needsUpdate = true;
}

void DisplayManager::updateLooperStatus(AxeLooper looper) {
  // Update displays met looper status
  for (uint8_t i = 0; i < _displayCount; i++) {
    if (_displays[i] != nullptr) {
      _displays[i]->updateLooperStatus(looper);
    }
  }
  
  // Markeer voor update
  _needsUpdate = true;
}
