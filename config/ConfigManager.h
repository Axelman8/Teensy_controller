#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>
#include <SPI.h>

// Maximale grootte van de JSON configuratie
#define CONFIG_JSON_SIZE 8192

// Configuratie structuur voor buttons
struct ButtonConfig {
  uint8_t buttonId;
  uint8_t type;  // 0 = preset, 1 = scene, 2 = effect, 3 = looper, etc.
  uint16_t value;  // preset nummer, scene nummer, effect ID, etc.
  uint8_t holdType;
  uint16_t holdValue;
  char label[20];
  uint16_t color;
};

// Configuratie structuur voor schermen
struct ScreenConfig {
  uint8_t screenId;
  uint8_t type;  // 0 = preset, 1 = scene, 2 = effect, etc.
  uint8_t layout;
  uint16_t color;
  uint16_t backgroundColor;
};

// Maximaal aantal button configuraties
#define MAX_BUTTON_CONFIGS 18

// Maximaal aantal scherm configuraties
#define MAX_SCREEN_CONFIGS 16

class ConfigManager {
public:
  ConfigManager();
  
  // Initialisatie
  void begin();
  
  // Laad configuratie van SD kaart
  bool loadConfig(const char* filename = "/config.json");
  
  // Sla configuratie op naar SD kaart
  bool saveConfig(const char* filename = "/config.json");
  
  // Krijg button configuratie
  ButtonConfig* getButtonConfig(uint8_t buttonId);
  
  // Krijg scherm configuratie
  ScreenConfig* getScreenConfig(uint8_t screenId);
  
  // Update button configuratie
  void updateButtonConfig(const ButtonConfig& config);
  
  // Update scherm configuratie
  void updateScreenConfig(const ScreenConfig& config);
  
private:
  // Configuratie arrays
  ButtonConfig _buttonConfigs[MAX_BUTTON_CONFIGS];
  ScreenConfig _screenConfigs[MAX_SCREEN_CONFIGS];
  
  // SD kaart helpers
  bool _sdInitialized;
  
  // JSON parsing helpers
  void parseButtonConfig(JsonObject& json, ButtonConfig& config);
  void parseScreenConfig(JsonObject& json, ScreenConfig& config);
  
  // Standaard configuratie
  void setDefaultConfig();
};

#endif // CONFIG_MANAGER_H
