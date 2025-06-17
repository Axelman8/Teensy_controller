#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <SD.h>
#include <ArduinoJson.h>

// Constanten
#define MAX_BUTTONS 16
#define MAX_SCREENS 4
#define MAX_DISPLAYS 4
#define MAX_LABEL_LENGTH 20
#define CONFIG_JSON_SIZE 4096

// Structuren op globaal niveau (niet binnen een klasse)
struct ButtonConfig {
    uint8_t buttonId;
    uint8_t type;
    uint8_t value;
    uint8_t holdType;
    uint8_t holdValue;
    char label[MAX_LABEL_LENGTH];
    uint16_t color;
    bool bypassed = false;
};

struct ScreenConfig {
    uint8_t screenId;
    uint8_t type;
    uint8_t layout;
    // andere velden
};

struct DisplayConfig {
    uint8_t type;
    uint8_t csPin;
    uint8_t dcPin;
    uint8_t rstPin;
    // andere velden
};

// ConfigManager klasse
class ConfigManager {
private:
    bool _sdInitialized;
    uint8_t _displayCount;
    
    ButtonConfig _buttonConfigs[MAX_BUTTONS];
    ScreenConfig _screenConfigs[MAX_SCREENS];
    DisplayConfig _displayConfigs[MAX_DISPLAYS];
    
    // andere private velden

public:
    ConfigManager();
    void begin();
    
    bool loadConfig(const char* filename = "/config.json");
    bool saveConfig(const char* filename = "/config.json");
    void setDefaultConfig();
    
    ButtonConfig* getButtonConfig(uint8_t buttonId);
    DisplayConfig* getDisplayConfig(uint8_t displayId);
    
    // Toegevoegde functies
    const DisplayConfig* getDisplayConfigs() const { return _displayConfigs; }
    uint8_t getDisplayCount() const { return _displayCount; }
  
  public:
    const DisplayConfig* getDisplayConfigs() const { return _displayConfigs; }
  
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

#endif // CONFIG_MANAGER_H
