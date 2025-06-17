#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>
#include "../config/PinDefinitions.h" 
#define CONFIG_JSON_SIZE 4096  


DynamicJsonDocument doc(CONFIG_JSON_SIZE);

// Definieer button types alleen als ze nog niet gedefinieerd zijn
#ifndef BUTTON_TYPE_NONE
#define BUTTON_TYPE_NONE   0
#endif

#ifndef BUTTON_TYPE_PRESET
#define BUTTON_TYPE_PRESET 1
#endif

#ifndef BUTTON_TYPE_SCENE
#define BUTTON_TYPE_SCENE  2
#endif

#ifndef BUTTON_TYPE_EFFECT
#define BUTTON_TYPE_EFFECT 3
#endif

#ifndef BUTTON_TYPE_TUNER
#define BUTTON_TYPE_TUNER  4
#endif

#ifndef BUTTON_TYPE_LOOPER
#define BUTTON_TYPE_LOOPER 5
#endif

// Maximum aantal buttons en displays
#define MAX_BUTTONS 16
#define MAX_DISPLAYS 4

// Button configuratie
struct ButtonConfig {
    uint8_t buttonId;
    uint8_t type;
    uint8_t value;
    uint8_t holdType;
    uint8_t holdValue;
    char label[MAX_LABEL_LENGTH];
    uint16_t color;
    bool bypassed = false;


// Display configuratie
struct DisplayConfig {
  uint8_t type;      // Display type (0 = ST7735, 1 = ST7789)
  uint8_t csPin;     // CS pin
  uint8_t dcPin;     // DC pin
  uint8_t rstPin;    // RST pin
  uint16_t width;    // Scherm breedte
  uint16_t height;   // Scherm hoogte
};

class ConfigManager {

public:
  ConfigManager();
  
  void begin();
  
  // Laad en sla configuratie op
  bool loadConfig(const char* filename = "/config.json");
  bool saveConfig(const char* filename = "/config.json");
  
  // Getters voor configuraties
  ButtonConfig* getButtonConfig(uint8_t buttonId);
  DisplayConfig* getDisplayConfig(uint8_t displayId);
  
  // Getters voor aantallen
  uint8_t getButtonCount() const { return MAX_BUTTONS; }
  uint8_t getDisplayCount() const { return _displayCount; }
  
  public:
    const DisplayConfig* getDisplayConfigs() const { return _displayConfigs; }
    uint8_t getDisplayCount() const { return _displayCount; }
  
private:
  void setDefaultConfig();
  
  // SD kaart status
  bool _sdInitialized;
  
  // Button configuraties
  ButtonConfig _buttonConfigs[MAX_BUTTONS];
  
  // Display configuraties
  DisplayConfig _displayConfigs[MAX_DISPLAYS];
  uint8_t _displayCount;
};

