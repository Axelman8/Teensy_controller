#include "AxeFxManager.h"

// Static instance voor callbacks
AxeFxManager* AxeFxManager::_instance = nullptr;

AxeFxManager::AxeFxManager() {
  // Initialisatie
}

void AxeFxManager::begin() {
  // Sla de instance op voor gebruik in callbacks
  _instance = this;
  
  // Start de communicatie met de Axe-Fx
  _axe.begin(Serial1);
  
  // Registreer callbacks
  _axe.registerPresetChangeCallback(presetChangeCallback);
  _axe.registerEffectFilterCallback([](const PresetNumber, const AxeEffect effect) {
    return true; // Accepteer alle effecten
  });
  _axe.registerTunerDataCallback(tunerDataCallback);
  _axe.registerTunerStatusCallback(tunerStatusCallback);
  _axe.registerLooperStatusCallback(looperStatusCallback);
  
  // Start de Axe-Fx communicatie
  _axe.requestPresetDetails();
  _axe.refresh();
}

void AxeFxManager::update() {
  _axe.update();
}

// Registreer callbacks van de FootController
void AxeFxManager::registerPresetChangeCallback(void (*callback)(AxePreset)) {
  _presetChangeCallback = callback;
}

void AxeFxManager::registerEffectBypassCallback(void (*callback)(AxeEffect)) {
  _effectBypassCallback = callback;
}

void AxeFxManager::registerTunerDataCallback(void (*callback)(const char*, byte, byte)) {
  _tunerDataCallback = callback;
}

void AxeFxManager::registerTunerStatusCallback(void (*callback)(bool)) {
  _tunerStatusCallback = callback;
}

void AxeFxManager::registerLooperStatusCallback(void (*callback)(AxeLooper)) {
  _looperStatusCallback = callback;
}

// Commando's naar de Axe-Fx
void AxeFxManager::sendPresetChange(uint16_t preset) {
  _axe.sendPresetChange(preset);
}

void AxeFxManager::sendSceneChange(uint8_t scene) {
  _axe.sendSceneChange(scene);
}

void AxeFxManager::sendEffectBypass(uint8_t effectId, bool bypassed) {
  if (bypassed) {
    _axe.disableEffect(effectId);
  } else {
    _axe.enableEffect(effectId);
  }
}

void AxeFxManager::sendTunerToggle(bool enabled) {
  if (enabled) {
    _axe.enableTuner();
  } else {
    _axe.disableTuner();
  }
}

void AxeFxManager::sendLooperCommand(uint8_t command, uint8_t value) {
  // Gebruik de juiste constanten uit de AxeFxControl bibliotheek
  // Dit is een voorbeeld, controleer de juiste constanten in de bibliotheek
  byte data[2] = {command, value};
  _axe.sendCommand(0x0D, data, 2); // 0x0D is een voorbeeld, controleer de juiste waarde
}

// Getters
AxePreset AxeFxManager::getCurrentPreset() {
  return _axe.getCurrentPreset();
}

AxeEffect* AxeFxManager::getEffectById(uint8_t effectId) {
  return _axe.getCurrentPreset().getEffectById(effectId);
}

// Static callback functies voor de AxeSystem
void AxeFxManager::presetChangeCallback(AxePreset preset) {
  if (_instance && _instance->_presetChangeCallback) {
    _instance->_presetChangeCallback(preset);
  }
}

void AxeFxManager::effectBypassCallback(AxeEffect effect) {
  if (_instance && _instance->_effectBypassCallback) {
    _instance->_effectBypassCallback(effect);
  }
}

void AxeFxManager::tunerDataCallback(const char* note, byte string, byte fineTune) {
  if (_instance && _instance->_tunerDataCallback) {
    _instance->_tunerDataCallback(note, string, fineTune);
  }
}

void AxeFxManager::tunerStatusCallback(bool enabled) {
  if (_instance && _instance->_tunerStatusCallback) {
    _instance->_tunerStatusCallback(enabled);
  }
}

void AxeFxManager::looperStatusCallback(AxeLooper looper) {
  if (_instance && _instance->_looperStatusCallback) {
    _instance->_looperStatusCallback(looper);
  }
}
