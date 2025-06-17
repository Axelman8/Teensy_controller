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

// Button types
#define BUTTON_TYPE_NONE 0
#define BUTTON_TYPE_PRESET 1
#define BUTTON_TYPE_SCENE 2
#define BUTTON_TYPE_EFFECT 3
#define BUTTON_TYPE_TUNER 4
#define BUTTON_TYPE_LOOPER 5

// Display types
#define DISPLAY_TYPE_NONE 0
#define DISPLAY_TYPE_ST7735 1
#define DISPLAY_TYPE_ST7789 2

// Structuren
struct ButtonConfig {
    uint8_t buttonId;
    uint8_t pin;  // Fysieke pin voor de knop
    uint8_t type;
    uint8_t value;
    uint8_t holdType;
    uint8_t holdValue;
    char label[MAX_LABEL_LENGTH];
    uint16_t color;
    bool bypassed;
};

struct ScreenConfig {
    uint8_t screenId;
    uint8_t type;
    uint8_t layout;
};

struct DisplayConfig {
    uint8_t type;
    uint8_t csPin;
    uint8_t dcPin;
    uint8_t rstPin;
    uint16_t width;
    uint16_t height;
};

// ConfigManager klasse
class ConfigManager {
private:
    bool _sdInitialized;
    uint8_t _displayCount;
    
    ButtonConfig _buttonConfigs[MAX_BUTTONS];
    ScreenConfig _screenConfigs[MAX_SCREENS];
    DisplayConfig _displayConfigs[MAX_DISPLAYS];

public:
    ConfigManager();
    void begin();
    
    bool loadConfig(const char* filename = "/config.json");
    bool saveConfig(const char* filename = "/config.json");
    void setDefaultConfig();
    
    ButtonConfig* getButtonConfig(uint8_t buttonId);
    DisplayConfig* getDisplayConfig(uint8_t displayId);
    
    const DisplayConfig* getDisplayConfigs() const { return _displayConfigs; }
    uint8_t getDisplayCount() const { return _displayCount; }
};

#endif // CONFIG_MANAGER_H
