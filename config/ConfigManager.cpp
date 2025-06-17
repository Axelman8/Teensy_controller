#include <SD.h>
#include <ArduinoJson.h>
#include "../config/ConfigManager.h"


ConfigManager::ConfigManager() : _sdInitialized(false), _displayCount(0) {
  // Initialiseer arrays met standaardwaarden
  for (int i = 0; i < MAX_BUTTONS; i++) {
    _buttonConfigs[i].pin = 22 + i; // Start vanaf pin 22
    _buttonConfigs[i].type = BUTTON_TYPE_NONE;
    _buttonConfigs[i].value = 0;
    _buttonConfigs[i].holdType = BUTTON_TYPE_NONE;
    _buttonConfigs[i].holdValue = 0;
  }
  
  for (int i = 0; i < MAX_DISPLAYS; i++) {
    _displayConfigs[i].type = 0;
    _displayConfigs[i].csPin = 10 + i;
    _displayConfigs[i].dcPin = 9;
    _displayConfigs[i].rstPin = 8;
    _displayConfigs[i].width = 160;
    _displayConfigs[i].height = 128;
  }
}

void ConfigManager::begin() {
  // Initialiseer SD kaart
  _sdInitialized = SD.begin(BUILTIN_SDCARD);
  
  if (_sdInitialized) {
    // Probeer configuratie te laden
    if (!loadConfig()) {
      // Als laden mislukt, gebruik standaardconfiguratie
      setDefaultConfig();
      // En probeer deze op te slaan
      saveConfig();
    }
  } else {
    // Als SD initialisatie mislukt, gebruik standaardconfiguratie
    setDefaultConfig();
  }
}

bool ConfigManager::loadConfig(const char* filename) {
  if (!_sdInitialized) return false;
  
  // Open het configuratiebestand
  File configFile = SD.open(filename, FILE_READ);
  if (!configFile) return false;
  
  // Alloceer een tijdelijke buffer voor de JSON data
  JsonDocument doc;  // Gebruik JsonDocument in plaats van StaticJsonDocument
  
  // Deserialize de JSON data
  DeserializationError error = deserializeJson(doc, configFile);
  configFile.close();
  
  if (error) {
    Serial.print("JSON parsing failed: ");
    Serial.println(error.c_str());
    return false;
  }
  
  // Verwerk button configuraties
  JsonArray buttons = doc["buttons"].as<JsonArray>();  // Gebruik .as<JsonArray>()
  
  for (JsonObject button : buttons) {
    uint8_t id = button["id"] | 0;
    if (id < MAX_BUTTONS) {
      _buttonConfigs[id].pin = button["pin"] | (22 + id);
      _buttonConfigs[id].type = button["type"] | BUTTON_TYPE_NONE;
      _buttonConfigs[id].value = button["value"] | 0;
      _buttonConfigs[id].holdType = button["holdType"] | BUTTON_TYPE_NONE;
      _buttonConfigs[id].holdValue = button["holdValue"] | 0;
    }
  }
  
  // Verwerk display configuraties
  _displayCount = 0;
  JsonArray displays = doc["displays"].as<JsonArray>();  // Gebruik .as<JsonArray>()
  for (JsonObject display : displays) {
    _displayConfigs[_displayCount].type = display["type"] | 0;
    _displayConfigs[_displayCount].csPin = display["csPin"] | (10 + _displayCount);
    _displayConfigs[_displayCount].dcPin = display["dcPin"] | 9;
    _displayConfigs[_displayCount].rstPin = display["rstPin"] | 8;
    _displayConfigs[_displayCount].width = display["width"] | 160;
    _displayConfigs[_displayCount].height = display["height"] | 128;
    _displayCount++;
    
    if (_displayCount >= MAX_DISPLAYS) break;
  }
  
  return true;
}

bool ConfigManager::saveConfig(const char* filename) {
  if (!_sdInitialized) return false;
  
  // Verwijder het bestaande bestand als het bestaat
  if (SD.exists(filename)) {
    SD.remove(filename);
  }
  
  // Open het bestand voor schrijven
  File configFile = SD.open(filename, FILE_WRITE);
  if (!configFile) return false;
  
  // Maak een JSON document
  JsonDocument doc;  // Gebruik JsonDocument in plaats van StaticJsonDocument
  
  // Maak arrays voor buttons en displays
  JsonArray buttons = doc["buttons"].to<JsonArray>();
  JsonArray displays = doc["displays"].to<JsonArray>();
  
  // Voeg button configuraties toe
  for (int i = 0; i < MAX_BUTTONS; i++) {
    JsonObject button = buttons.add<JsonObject>();  // Gebruik add<JsonObject>() in plaats van createNestedObject()
    button["id"] = i;
    button["pin"] = _buttonConfigs[i].pin;
    button["type"] = _buttonConfigs[i].type;
    button["value"] = _buttonConfigs[i].value;
    button["holdType"] = _buttonConfigs[i].holdType;
    button["holdValue"] = _buttonConfigs[i].holdValue;
  }
  
  // Voeg display configuraties toe
  for (int i = 0; i < _displayCount; i++) {
    JsonObject display = displays.add<JsonObject>();  // Gebruik add<JsonObject>() in plaats van createNestedObject()
    display["type"] = _displayConfigs[i].type;
    display["csPin"] = _displayConfigs[i].csPin;
    display["dcPin"] = _displayConfigs[i].dcPin;
    display["rstPin"] = _displayConfigs[i].rstPin;
    display["width"] = _displayConfigs[i].width;
    display["height"] = _displayConfigs[i].height;
  }
  
  // Schrijf de JSON naar het bestand
  if (serializeJson(doc, configFile) == 0) {
    configFile.close();
    return false;
  }
  
  configFile.close();
  return true;
}


void ConfigManager::setDefaultConfig() {
  // Stel standaard button configuraties in
  _buttonConfigs[0].type = BUTTON_TYPE_PRESET;
  _buttonConfigs[0].value = 0;  // Preset 0
  
  _buttonConfigs[1].type = BUTTON_TYPE_PRESET;
  _buttonConfigs[1].value = 1;  // Preset 1
  
  _buttonConfigs[2].type = BUTTON_TYPE_SCENE;
  _buttonConfigs[2].value = 0;  // Scene 0
  
  _buttonConfigs[3].type = BUTTON_TYPE_SCENE;
  _buttonConfigs[3].value = 1;  // Scene 1
  
  _buttonConfigs[4].type = BUTTON_TYPE_EFFECT;
  _buttonConfigs[4].value = 58; // Drive 1 (ID 58)
  
  _buttonConfigs[5].type = BUTTON_TYPE_EFFECT;
  _buttonConfigs[5].value = 59; // Drive 2 (ID 59)
  
  _buttonConfigs[6].type = BUTTON_TYPE_TUNER;
  _buttonConfigs[6].value = 1;  // Tuner aan
  _buttonConfigs[6].holdType = BUTTON_TYPE_TUNER;
  _buttonConfigs[6].holdValue = 0; // Tuner uit
  
  _buttonConfigs[7].type = BUTTON_TYPE_LOOPER;
  _buttonConfigs[7].value = 1;  // Looper play
  
  // Stel standaard display configuraties in
  _displayCount = 1;
  _displayConfigs[0].type = 0;  // ST7735
  _displayConfigs[0].csPin = 10;
  _displayConfigs[0].dcPin = 9;
  _displayConfigs[0].rstPin = 8;
  _displayConfigs[0].width = 160;
  _displayConfigs[0].height = 128;
}

ButtonConfig* ConfigManager::getButtonConfig(uint8_t buttonId) {
  if (buttonId < MAX_BUTTONS) {
    return &_buttonConfigs[buttonId];
  }
  return nullptr;
}

DisplayConfig* ConfigManager::getDisplayConfig(uint8_t displayId) {
  if (displayId < _displayCount) {
    return &_displayConfigs[displayId];
  }
  return nullptr;
}
