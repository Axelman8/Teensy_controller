
#include "FootController.h"

// Initialiseer statische pointer
FootController* FootController::instance = nullptr;

// Statische callback functies
void onPresetChange(const AxePreset& preset) {
  if (FootController::instance) {
    FootController::instance->handlePresetChange(preset);
  }
}

void onEffectBypass(const AxeEffect& effect) {
  if (FootController::instance) {
    FootController::instance->handleEffectBypass(effect);
  }
}

void onTunerData(const AxeTuner& tuner) {
  if (FootController::instance) {
    FootController::instance->handleTunerData(tuner);
  }
}

FootController::FootController() :
  configManager(),
  displayManager(&configManager),
  buttonManager(),
  axeFxManager(),
  initialized(false),
  lastUpdateTime(0)
{
  // Sla pointer naar deze instantie op
  instance = this;
}

void FootController::begin() {
  // Initialiseer alle componenten
  configManager.begin();
  buttonManager.begin();
  displayManager.begin();
  axeFxManager.begin();
  
  // Registreer callbacks voor Axe-Fx events
  axeFxManager.registerPresetChangeCallback(onPresetChange);
  axeFxManager.registerEffectBypassCallback(onEffectBypass);
  axeFxManager.registerTunerDataCallback(onTunerData);
  
  initialized = true;
}

void FootController::update() {
  if (!initialized) return;
  
  // Bereken delta tijd sinds laatste update
  unsigned long currentTime = millis();
  lastUpdateTime = currentTime;
  
  // Update alle componenten
  buttonManager.update();
  axeFxManager.update();
  
  // Verwerk button events
  handleButtonEvents();
  
  // Update displays
  displayManager.update();
}

void FootController::handleButtonEvents() {
  // Verwerk alle button events in de queue
  if (buttonManager.hasEvents()) {
    ButtonEvent event = buttonManager.getNextEvent();
    
    // Verwerk event op basis van type
    switch (event.type) {
      case BUTTON_PRESSED:
        // Button is ingedrukt
        // Implementeer je eigen logica hier
        break;
        
      case BUTTON_RELEASED:
        // Button is losgelaten
        // Implementeer je eigen logica hier
        break;
        
      case BUTTON_HELD:
        // Button is ingedrukt gehouden
        // Implementeer je eigen logica hier
        break;
    }
  }
}

void FootController::updateDisplays() {
  // Update displays indien nodig
  displayManager.update();
}

void FootController::processAxeFxCallbacks() {
  // De callbacks worden automatisch aangeroepen door de AxeFxManager
  // wanneer er nieuwe informatie binnenkomt
}

// Callback handlers
void FootController::handlePresetChange(const AxePreset& preset) {
  displayManager.updatePresetInfo(preset);
}

void FootController::handleEffectBypass(const AxeEffect& effect) {
  displayManager.updateEffectStatus(effect);
}

void FootController::handleTunerData(const AxeTuner& tuner) {
  displayManager.updateTuner(tuner);
}
