#include "FootController.h"

FootController::FootController() {
  _configManager = new ConfigManager();
  _buttonManager = new ButtonManager(_configManager);  // Geef ConfigManager door
  _axeFxManager = new AxeFxManager();
  _displayManager = new DisplayManager();
}


void FootController::begin() {
  // Initialiseer config manager
  _configManager->begin();
  
  // Initialiseer button manager
  _buttonManager->begin();
  
  // Initialiseer display manager
  _displayManager->begin();
  
  // Configureer displays op basis van config
  // Gebruik de juiste methode volgens je GitHub code
  _displayManager->createDisplays();
  
  // Initialiseer Axe-Fx manager
  _axeFxManager->begin();
  
  // Registreer callbacks
  // Gebruik normale functies in plaats van lambdas omdat je code geen lambdas ondersteunt
  _buttonManager->registerButtonEventCallback(std::bind(&FootController::onButtonEvent, this, std::placeholders::_1, std::placeholders::_2));
  
  _axeFxManager->registerPresetChangeCallback(onPresetChangeStatic);
  _axeFxManager->registerEffectBypassCallback(onEffectBypassStatic);
  _axeFxManager->registerTunerDataCallback(onTunerDataStatic);
  _axeFxManager->registerTunerStatusCallback(onTunerStatusStatic);
  _axeFxManager->registerLooperStatusCallback(onLooperStatusStatic);
}

// Statische callback functies
static FootController* g_footController = nullptr;

void FootController::onPresetChangeStatic(AxePreset preset) {
  if (g_footController) g_footController->onPresetChange(preset);
}

void FootController::onEffectBypassStatic(AxeEffect effect) {
  if (g_footController) g_footController->onEffectBypass(effect);
}

void FootController::onTunerDataStatic(const char* note, byte string, byte fineTune) {
  if (g_footController) g_footController->onTunerData(note, string, fineTune);
}

void FootController::onTunerStatusStatic(bool enabled) {
  if (g_footController) g_footController->onTunerStatus(enabled);
}

void FootController::onLooperStatusStatic(AxeLooper looper) {
  if (g_footController) g_footController->onLooperStatus(looper);
}

void FootController::update() {
  // Update componenten
  _buttonManager->update();
  _axeFxManager->update();
  _displayManager->update();
}

void FootController::onButtonEvent(uint8_t buttonId, ButtonEvent event) {
  ButtonConfig* config = _configManager->getButtonConfig(buttonId);
  
  if (!config) return;
  
  // Verwerk button event
  if (event.type == BUTTON_PRESSED) {
    // Verwerk indrukken
    switch (config->type) {
      case BUTTON_TYPE_PRESET:
        _axeFxManager->sendPresetChange(config->value);
        break;
        
      case BUTTON_TYPE_SCENE:
        _axeFxManager->sendSceneChange(config->value);
        break;
        
      case BUTTON_TYPE_EFFECT:
        _axeFxManager->sendEffectBypass(config->value, !config->bypassed);
        config->bypassed = !config->bypassed;
        break;
        
      case BUTTON_TYPE_TUNER:
        _axeFxManager->sendTunerToggle(!_tunerEnabled);
        _tunerEnabled = !_tunerEnabled;
        break;
        
      case BUTTON_TYPE_LOOPER:
        _axeFxManager->sendLooperCommand(config->holdValue, 127); // 127 = aan
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

