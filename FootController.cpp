#include "FootController.h"

FootController::FootController() {
  _configManager = new ConfigManager();
  _buttonManager = new ButtonManager();
  _displayManager = new DisplayManager();
  _axeFxManager = new AxeFxManager();
}

void FootController::begin() {
  // Initialiseer componenten
  _configManager->begin();
  _buttonManager->begin();
  _displayManager->begin();
  
  // Configureer button pins
  for (int i = 0; i < _configManager->getButtonCount(); i++) {
    ButtonConfig* config = _configManager->getButtonConfig(i);
    _buttonManager->configureButton(config->buttonId, config->pin);
  }
  
  // Configureer displays
  _displayManager->createDisplays(_configManager->getDisplayConfigs(), _configManager->getDisplayCount());
  
  // Registreer callbacks
  _buttonManager->registerButtonEventCallback([this](const ButtonEvent& event) {
    this->onButtonEvent(event);
  });
  
  // Registreer callbacks voor AxeFxManager
  _axeFxManager->registerPresetChangeCallback([this](AxePreset preset) {
    this->onPresetChange(preset);
  });
  
  _axeFxManager->registerEffectBypassCallback([this](AxeEffect effect) {
    this->onEffectBypass(effect);
  });
  
  _axeFxManager->registerTunerDataCallback([this](const char* note, byte string, byte fineTune) {
    this->onTunerData(note, string, fineTune);
  });
  
  _axeFxManager->registerTunerStatusCallback([this](bool enabled) {
    this->onTunerStatus(enabled);
  });
  
  _axeFxManager->registerLooperStatusCallback([this](AxeLooper looper) {
    this->onLooperStatus(looper);
  });
  
  // Start AxeFxManager
  _axeFxManager->begin();
}

void FootController::update() {
  // Update componenten
  _buttonManager->update();
  _axeFxManager->update();
  _displayManager->update();
}

void FootController::onButtonEvent(const ButtonEvent& event) {
  // Verwerk button events
  ButtonConfig* config = _configManager->getButtonConfig(event.buttonId);
  
  if (event.type == BUTTON_PRESSED) {
    // Verwerk korte druk
    switch (config->type) {
      case BUTTON_TYPE_PRESET:
        _axeFxManager->sendPresetChange(config->value);
        break;
        
      case BUTTON_TYPE_SCENE:
        _axeFxManager->sendSceneChange(config->value);
        break;
        
      case BUTTON_TYPE_EFFECT:
        _axeFxManager->sendEffectBypass(config->value, false); // false = enable effect
        break;
        
      case BUTTON_TYPE_TUNER:
        _axeFxManager->sendTunerToggle(true);
        break;
        
      case BUTTON_TYPE_LOOPER:
        _axeFxManager->sendLooperCommand(config->value, 127); // 127 = aan
        break;
    }
  }
  else if (event.type == BUTTON_RELEASED) {
    // Verwerk loslaten (indien nodig)
    if (config->holdType == BUTTON_TYPE_LOOPER && config->holdValue == 0) { // 0 = RECORD
      // Stop met opnemen als de record-knop wordt losgelaten
      _axeFxManager->sendLooperCommand(0, 0); // 0 = uit
    }
  }
}

void FootController::onPresetChange(AxePreset preset) {
  // Update display met nieuwe preset info
  _displayManager->updatePresetInfo(preset);
}

void FootController::onEffectBypass(AxeEffect effect) {
  // Update display met nieuwe effect status
  _displayManager->updateEffectStatus(effect);
}

void FootController::onTunerData(const char* note, byte string, byte fineTune) {
  // Update display met tuner data
  _displayManager->updateTuner(note, string, fineTune);
}

void FootController::onTunerStatus(bool enabled) {
  // Update display met tuner status
  _displayManager->updateTunerStatus(enabled);
}

void FootController::onLooperStatus(AxeLooper looper) {
  // Implementeer indien nodig
  // Bijvoorbeeld: update display met looper status
}
