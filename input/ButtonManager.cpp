#include "ButtonManager.h"

ButtonManager::ButtonManager(ConfigManager* configManager)
  : _configManager(configManager) {
  
  // Initialiseer button pins en states
  for (int i = 0; i < MAX_BUTTONS; i++) {
    // Haal pin configuratie op uit ConfigManager
    ButtonConfig* config = _configManager->getButtonConfig(i);
    _buttonPins[i] = config->pin;
    
    // Initialiseer states
    _buttonStates[i] = false;
    _lastButtonStates[i] = false;
    _lastDebounceTime[i] = 0;
    _buttonHeld[i] = false;
    _buttonPressTime[i] = 0;
  }
}

void ButtonManager::begin() {
  // Initialiseer alle button pins als input met pull-up
  for (int i = 0; i < MAX_BUTTONS; i++) {
    pinMode(_buttonPins[i], INPUT_PULLUP);
  }
}

void ButtonManager::update() {
  // Lees alle buttons
  for (int i = 0; i < MAX_BUTTONS; i++) {
    readButton(i);
  }
}

void ButtonManager::readButton(uint8_t buttonId) {
  // Lees de huidige staat van de button (geïnverteerd omdat we pull-up gebruiken)
  bool reading = !digitalRead(_buttonPins[buttonId]);
  
  // Check of de reading is veranderd door ruis of een echte druk
  if (reading != _lastButtonStates[buttonId]) {
    // Reset de debounce timer
    _lastDebounceTime[buttonId] = millis();
  }
  
  // Als er genoeg tijd is verstreken sinds de laatste verandering
  if ((millis() - _lastDebounceTime[buttonId]) > DEBOUNCE_TIME) {
    // Als de button staat is veranderd
    if (reading != _buttonStates[buttonId]) {
      _buttonStates[buttonId] = reading;
      
      // Als de button is ingedrukt
      if (_buttonStates[buttonId]) {
        // Registreer de tijd van indrukken
        _buttonPressTime[buttonId] = millis();
        
        // Genereer BUTTON_PRESSED event
        if (_buttonEventCallback) {
          ButtonEvent event = {BUTTON_PRESSED, millis()};
          _buttonEventCallback(buttonId, event);
        }
      } 
      // Als de button is losgelaten
      else {
        // Reset held flag
        _buttonHeld[buttonId] = false;
        
        // Genereer BUTTON_RELEASED event
        if (_buttonEventCallback) {
          ButtonEvent event = {BUTTON_RELEASED, millis()};
          _buttonEventCallback(buttonId, event);
        }
      }
    }
  }
  
  // Check voor lang indrukken
  if (_buttonStates[buttonId] && !_buttonHeld[buttonId]) {
    if ((millis() - _buttonPressTime[buttonId]) > HOLD_TIME) {
      _buttonHeld[buttonId] = true;
      
      // Genereer BUTTON_HELD event
      if (_buttonEventCallback) {
        ButtonEvent event = {BUTTON_HELD, millis()};
        _buttonEventCallback(buttonId, event);
      }
    }
  }
  
  // Update last state
  _lastButtonStates[buttonId] = reading;
}

void ButtonManager::registerButtonEventCallback(ButtonEventCallback callback) {
  _buttonEventCallback = callback;
}
