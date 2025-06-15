#include "ButtonManager.h"

// Nieuwe constructor die ConfigManager accepteert
ButtonManager::ButtonManager(ConfigManager* configManager) :
  _configManager(configManager),
  _queueHead(0),
  _queueTail(0),
  _queueSize(0),
  _debounceDelay(20),  // 20ms debounce tijd
  _eventCallback(nullptr)  // Initialiseer de callback pointer
{
  // Initialiseer arrays
  for (int i = 0; i < MAX_BUTTONS; i++) {
    _buttonState[i] = false;
    _lastButtonState[i] = false;
    _buttonPressTime[i] = 0;
    _buttonHoldProcessed[i] = false;
    _lastDebounceTime[i] = 0;
  }
  
  // Stel button pins in
  _buttonPins[0] = BUTTON1_PIN;
  _buttonPins[1] = BUTTON2_PIN;
  _buttonPins[2] = BUTTON3_PIN;
  _buttonPins[3] = BUTTON4_PIN;
  _buttonPins[4] = BUTTON5_PIN;
  _buttonPins[5] = BUTTON6_PIN;
  _buttonPins[6] = BUTTON7_PIN;
  _buttonPins[7] = BUTTON8_PIN;
  _buttonPins[8] = BUTTON9_PIN;
  _buttonPins[9] = BUTTON10_PIN;
  _buttonPins[10] = BUTTON11_PIN;
  _buttonPins[11] = BUTTON12_PIN;
  _buttonPins[12] = BUTTON13_PIN;
  _buttonPins[13] = BUTTON14_PIN;
  _buttonPins[14] = BUTTON15_PIN;
  _buttonPins[15] = BUTTON16_PIN;
  _buttonPins[16] = BUTTON17_PIN;
  _buttonPins[17] = BUTTON18_PIN;
}

// Behoud de originele constructor voor backward compatibility
ButtonManager::ButtonManager() :
  _configManager(nullptr),
  _queueHead(0),
  _queueTail(0),
  _queueSize(0),
  _debounceDelay(20),  // 20ms debounce tijd
  _eventCallback(nullptr)  // Initialiseer de callback pointer
{
  // Initialiseer arrays
  for (int i = 0; i < MAX_BUTTONS; i++) {
    _buttonState[i] = false;
    _lastButtonState[i] = false;
    _buttonPressTime[i] = 0;
    _buttonHoldProcessed[i] = false;
    _lastDebounceTime[i] = 0;
  }
  
  // Stel button pins in
  _buttonPins[0] = BUTTON1_PIN;
  _buttonPins[1] = BUTTON2_PIN;
  _buttonPins[2] = BUTTON3_PIN;
  _buttonPins[3] = BUTTON4_PIN;
  _buttonPins[4] = BUTTON5_PIN;
  _buttonPins[5] = BUTTON6_PIN;
  _buttonPins[6] = BUTTON7_PIN;
  _buttonPins[7] = BUTTON8_PIN;
  _buttonPins[8] = BUTTON9_PIN;
  _buttonPins[9] = BUTTON10_PIN;
  _buttonPins[10] = BUTTON11_PIN;
  _buttonPins[11] = BUTTON12_PIN;
  _buttonPins[12] = BUTTON13_PIN;
  _buttonPins[13] = BUTTON14_PIN;
  _buttonPins[14] = BUTTON15_PIN;
  _buttonPins[15] = BUTTON16_PIN;
  _buttonPins[16] = BUTTON17_PIN;
  _buttonPins[17] = BUTTON18_PIN;
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
  bool reading = !digitalReadFast(_buttonPins[buttonId]);
  
  // Check of de reading is veranderd door ruis of een echte druk
  if (reading != _lastButtonState[buttonId]) {
    // Reset de debounce timer
    _lastDebounceTime[buttonId] = millis();
  }
  
  // Als er genoeg tijd is verstreken sinds de laatste verandering
  if ((millis() - _lastDebounceTime[buttonId]) > _debounceDelay) {
    // Als de button staat is veranderd
    if (reading != _buttonState[buttonId]) {
      _buttonState[buttonId] = reading;
      
      // Als de button nu ingedrukt is
      if (_buttonState[buttonId]) {
        _buttonPressTime[buttonId] = millis();
        _buttonHoldProcessed[buttonId] = false;
        addEventToQueue(buttonId, BUTTON_PRESSED);
      } 
      // Als de button is losgelaten
      else {
        addEventToQueue(buttonId, BUTTON_RELEASED);
      }
    }
  }
  
  // Check voor hold events
  if (_buttonState[buttonId] && !_buttonHoldProcessed[buttonId]) {
    if ((millis() - _buttonPressTime[buttonId]) > BUTTON_HOLD_TIME) {
      addEventToQueue(buttonId, BUTTON_HELD);
      _buttonHoldProcessed[buttonId] = true;
    }
  }
  
  // Sla de laatste button staat op
  _lastButtonState[buttonId] = reading;
}

void ButtonManager::addEventToQueue(uint8_t buttonId, ButtonEventType type) {
  // Voeg alleen toe als er ruimte is
  if (_queueSize < MAX_BUTTON_EVENTS) {
    ButtonEvent event;
    event.buttonId = buttonId;
    event.type = type;
    event.timestamp = millis();
    
    _eventQueue[_queueTail] = event;
    _queueTail = (_queueTail + 1) % MAX_BUTTON_EVENTS;
    _queueSize++;
    
    // Als er een callback is geregistreerd, roep deze aan
    if (_eventCallback != nullptr) {
      _eventCallback(buttonId, event);
    }
  }
}

bool ButtonManager::hasEvents() {
  return _queueSize > 0;
}

ButtonEvent ButtonManager::getNextEvent() {
  ButtonEvent event;
  
  if (_queueSize > 0) {
    event = _eventQueue[_queueHead];
    _queueHead = (_queueHead + 1) % MAX_BUTTON_EVENTS;
    _queueSize--;
  }
  
  return event;
}

bool ButtonManager::isButtonPressed(uint8_t buttonId) {
  if (buttonId < MAX_BUTTONS) {
    return _buttonState[buttonId];
  }
  return false;
}

bool ButtonManager::isButtonHeld(uint8_t buttonId) {
  if (buttonId < MAX_BUTTONS) {
    return _buttonState[buttonId] && 
           ((millis() - _buttonPressTime[buttonId]) > BUTTON_HOLD_TIME);
  }
  return false;
}

unsigned long ButtonManager::getButtonHoldTime(uint8_t buttonId) {
  if (buttonId < MAX_BUTTONS && _buttonState[buttonId]) {
    return millis() - _buttonPressTime[buttonId];
  }
  return 0;
}

// Nieuwe methode voor het registreren van de callback
void ButtonManager::registerButtonEventCallback(ButtonEventCallback callback) {
  _eventCallback = callback;
}
