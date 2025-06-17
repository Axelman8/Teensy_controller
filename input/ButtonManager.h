#pragma once

#include <Arduino.h>
#include <functional>
#include "../config/ConfigManager.h"

// Button event types
#define BUTTON_PRESSED  0
#define BUTTON_RELEASED 1
#define BUTTON_HELD     2

// Debounce tijd in milliseconden
#define DEBOUNCE_TIME 50

// Tijd voor lang indrukken in milliseconden
#define HOLD_TIME 1000

// Maximum aantal buttons
#define MAX_BUTTONS 16

struct ButtonEvent {
  uint8_t type;  // BUTTON_PRESSED, BUTTON_RELEASED, BUTTON_HELD
  uint32_t time; // Tijdstip van event in millis()
};

// Callback type definitie met std::function
using ButtonEventCallback = std::function<void(uint8_t buttonId, ButtonEvent event)>;

class ButtonManager {
public:
  ButtonManager(ConfigManager* configManager);
  
  void begin();
  void update();
  
  // Callback registratie met std::function
  void registerButtonEventCallback(ButtonEventCallback callback);
  
private:
  void readButton(uint8_t buttonId);
  
  ConfigManager* _configManager;
  
  // Button pins
  uint8_t _buttonPins[MAX_BUTTONS];
  
  // Button state tracking
  bool _buttonStates[MAX_BUTTONS];
  bool _lastButtonStates[MAX_BUTTONS];
  uint32_t _lastDebounceTime[MAX_BUTTONS];
  bool _buttonHeld[MAX_BUTTONS];
  uint32_t _buttonPressTime[MAX_BUTTONS];
  
  // Callback met std::function
  ButtonEventCallback _buttonEventCallback;
};
