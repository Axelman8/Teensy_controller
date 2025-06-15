#include "ConfigManager.h"

ConfigManager::ConfigManager() : _sdInitialized(false) {
  // Initialiseer arrays met standaardwaarden
  for (int i = 0; i < MAX_BUTTON_CONFIGS; i++) {
    _buttonConfigs[i].buttonId = i;
    _buttonConfigs[i].type = 0;
    _buttonConfigs[i].value = 0;
    _buttonConfigs[i].holdType = 0;
    _buttonConfigs[i].holdValue = 0;
    strcpy(_buttonConfigs[i].label, "Button");
    _buttonConfigs[i].color = 0xFFFF;  // Wit
  }
  
  for (int i = 0; i < MAX_SCREEN_CONFIGS; i++) {
    _screenConfigs[i].screenId = i;
    _screenConfigs[i].type = 0;
    _screenConfigs[i].layout = 0;
    _screenConfigs[i].color = 0xFFFF;  // Wit
    _screenConfigs[i].backgroundColor = 0x0000;  // Zwart
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
  StaticJsonDocument<CONFIG_JSON_SIZE> doc;
  
  // Parse de JSON
  DeserializationError error = deserializeJson(doc, configFile);
  configFile.close();
  
  if (error) return false;
  
  // Parse button configuraties
  JsonArray buttons = doc["buttons"];
  for (JsonObject button : buttons) {
    ButtonConfig config;
    parseButtonConfig(button, config);
    updateButtonConfig(config);
  }
  
  // Parse scherm configuraties
  JsonArray screens = doc["screens"];
  for (JsonObject screen : screens) {
    ScreenConfig config;
    parseScreenConfig(screen, config);
    updateScreenConfig(config);
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
  StaticJsonDocument<CONFIG_JSON_SIZE> doc;
  
  // Maak arrays voor buttons en schermen
  JsonArray buttons = doc.createNestedArray("buttons");
  JsonArray screens = doc.createNestedArray("screens");
  
  // Voeg button configuraties toe
  for (int i = 0; i < MAX_BUTTON_CONFIGS; i++) {
    JsonObject button = buttons.createNestedObject();
    button["id"] = _buttonConfigs[i].buttonId;
    button["type"] = _buttonConfigs[i].type;
    button["value"] = _buttonConfigs[i].value;
    button["holdType"] = _buttonConfigs[i].holdType;
    button["holdValue"] = _buttonConfigs[i].holdValue;
    button["label"] = _buttonConfigs[i].label;
    button["color"] = _buttonConfigs[i].color;
  }
  
  // Voeg scherm configuraties toe
  for (int i = 0; i < MAX_SCREEN_CONFIGS; i++) {
    JsonObject screen = screens.createNestedObject();
    screen["id"] = _screenConfigs[i].screenId;
    screen["type"] = _screenConfigs[i].type;
    screen["layout"] = _screenConfigs[i].layout;
    screen["color"] = _screenConfigs[i].color;
    screen["backgroundColor"] = _screenConfigs[i].backgroundColor;
  }
  
  // Schrijf de JSON naar het bestand
  if (serializeJson(doc, configFile) == 0) {
    configFile.close();
    return false;
  }
  
  configFile.close();
  return true;
}

void ConfigManager::parseButtonConfig(JsonObject& json, ButtonConfig& config) {
  config.buttonId = json["id"] | 0;
  config.type = json["type"] | 0;
  config.value = json["value"] | 0;
  config.holdType = json["holdType"] | 0;
  config.holdValue = json["holdValue"] | 0;
  
  const char* label = json["label"] | "Button";
  strncpy(config.label, label, sizeof(config.label) - 1);
  config.label[sizeof(config.label) - 1] = '\0';  // Zorg voor null-terminatie
  
  config.color = json["color"] | 0xFFFF;
}

void ConfigManager::parseScreenConfig(JsonObject& json, ScreenConfig& config) {
  config.screenId = json["id"] | 0;
  config.type = json["type"] | 0;
  config.layout = json["layout"] | 0;
  config.color = json["color"] | 0xFFFF;
  config.backgroundColor = json["backgroundColor"] | 0x0000;
}

ButtonConfig* ConfigManager::getButtonConfig(uint8_t buttonId) {
  if (buttonId < MAX_BUTTON_CONFIGS) {
    return &_buttonConfigs[buttonId];
  }
  return nullptr;
}

ScreenConfig* ConfigManager::getScreenConfig(uint8_t screenId) {
  if (screenId < MAX_SCREEN_CONFIGS) {
    return &_screenConfigs[screenId];
  }
  return nullptr;
}

void ConfigManager::updateButtonConfig(const ButtonConfig& config) {
  uint8_t buttonId = config.buttonId;
  if (buttonId < MAX_BUTTON_CONFIGS) {
    _buttonConfigs[buttonId] = config;
  }
}

void ConfigManager::updateScreenConfig(const ScreenConfig& config) {
  uint8_t screenId = config.screenId;
  if (screenId < MAX_SCREEN_CONFIGS) {
    _screenConfigs[screenId] = config;
  }
}

void ConfigManager::setDefaultConfig() {
  // Stel standaard button configuraties in
  for (int i = 0; i < MAX_BUTTON_CONFIGS; i++) {
    _buttonConfigs[i].buttonId = i;
    _buttonConfigs[i].type = 0;  // Preset type
    _buttonConfigs[i].value = i;  // Preset nummer = button ID
    _buttonConfigs[i].holdType = 1;  // Scene type bij ingedrukt houden
    _buttonConfigs[i].holdValue = 0;  // Scene 0
    sprintf(_buttonConfigs[i].label, "Preset %d", i);
    _buttonConfigs[i].color = 0xFFFF;  // Wit
  }
  
  // Stel standaard scherm configuraties in
  for (int i = 0; i < MAX_SCREEN_CONFIGS; i++) {
    _screenConfigs[i].screenId = i;
    _screenConfigs[i].type = 0;  // Preset info
    _screenConfigs[i].layout = 0;  // Standaard layout
    _screenConfigs[i].color = 0xFFFF;  // Wit
    _screenConfigs[i].backgroundColor = 0x0000;  // Zwart
  }
}
