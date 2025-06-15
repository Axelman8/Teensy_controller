#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

#include <Arduino.h>
#include "../config/ConfigManager.h"
#include "../config/PinDefinitions.h"

// Constanten
#define MAX_BUTTONS 18
#define MAX_BUTTON_EVENTS 32
#define BUTTON_HOLD_TIME 1000  // 1 seconde voor een hold event

// Button event types
enum ButtonEventType {
  BUTTON_PRESSED,
  BUTTON_RELEASED,
  BUTTON_HELD
};

// Button event struct
struct ButtonEvent {
  uint8_t buttonId;
  ButtonEventType type;
  unsigned long timestamp;
};

class ButtonManager {
public:
  // Constructor met ConfigManager
  ButtonManager(ConfigManager* configManager);
  
  // Constructor zonder parameters (voor backward compatibility)
  ButtonManager();
  
  // Callback type definitie
  typedef void (*ButtonEventCallback)(uint8_t buttonId, ButtonEvent event);
  
  void begin();
  void update();
  
  bool hasEvents();
  ButtonEvent getNextEvent();
  
  bool isButtonPressed(uint8_t buttonId);
  bool isButtonHeld(uint8_t buttonId);
  unsigned long getButtonHoldTime(uint8_t buttonId);
  
  // Methode om callback te registreren
  void registerButtonEventCallback(ButtonEventCallback callback);
  
private:
  ConfigManager* _configManager;
  
  // Button state tracking
  bool _buttonState[MAX_BUTTONS];
  bool _lastButtonState[MAX_BUTTONS];
  unsigned long _buttonPressTime[MAX_BUTTONS];
  bool _buttonHoldProcessed[MAX_BUTTONS];
  unsigned long _lastDebounceTime[MAX_BUTTONS];
  uint8_t _buttonPins[MAX_BUTTONS];
  unsigned long _debounceDelay;
  
  // Event queue
  ButtonEvent _eventQueue[MAX_BUTTON_EVENTS];
  uint8_t _queueHead;
  uint8_t _queueTail;
  uint8_t _queueSize;
  
  // Callback pointer
  ButtonEventCallback _eventCallback;
  
  // Helper methods
  void readButton(uint8_t buttonId);
  void addEventToQueue(uint8_t buttonId, ButtonEventType type);
};

#endif // BUTTON_MANAGER_H
