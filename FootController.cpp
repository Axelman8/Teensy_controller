#include "FootController.h"

FootController::FootController() {
  _configManager = new ConfigManager();
  _buttonManager = new ButtonManager(MAX_BUTTONS);  // Geef het maximum aantal buttons door
  _axeFxManager = new AxeFxManager();
  _displayManager = new DisplayManager();
}


void FootController::begin() {
  // Initialiseer config manager
  _configManager->begin();
  
  // Initialiseer button manager
  _buttonManager->begin();
  
  // Configureer buttons op basis van config
  for (int i = 0; i < _configManager->getButtonCount(); i++) {
    ButtonConfig* config = _configManager->getButtonConfig(i);
    // Gebruik de juiste methode en parameters volgens je GitHub code
    _buttonManager->setButtonPin(i, config->pin);
  }
  
  // Initialiseer display manager
  _displayManager->begin();
  
  // Configureer displays op basis van config
  _displayManager->createDisplays(_configManager->getDisplayConfigs(), _configManager->getDisplayCount());
  
  // Initialiseer Axe-Fx manager
  _axeFxManager->begin();
  
  // Registreer callbacks
  _buttonManager->registerButtonEventCallback([this](uint8_t buttonId, ButtonEvent event) {
    this->onButtonEvent(buttonId, event);
  });
  
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

// In de onPresetChange functie:
void FootController::onPresetChange(AxePreset preset) {
  // Update displays met preset info
  _displayManager->updatePresetInfo(preset);
}

// In de onEffectBypass functie:
void FootController::onEffectBypass(AxeEffect effect) {
  // Update displays met effect status
  _displayManager->updateEffectStatus(effect);
}

// In de onTunerData functie:
void FootController::onTunerData(const char* note, byte string, byte fineTune) {
  // Update displays met tuner data
  _displayManager->updateTuner(note, string, fineTune);
}

// In de onTunerStatus functie:
void FootController::onTunerStatus(bool enabled) {
  // Update displays met tuner status
  _displayManager->updateTunerStatus(enabled);
}

// In de onLooperStatus functie:
void FootController::onLooperStatus(AxeLooper looper) {
  // Update displays met looper status
  _displayManager->updateLooperStatus(looper);
}

