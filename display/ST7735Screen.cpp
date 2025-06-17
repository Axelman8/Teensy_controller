#include "ST7735Screen.h"

ST7735Screen::ST7735Screen(uint8_t csPin, uint8_t dcPin, uint8_t rstPin)
  : _csPin(csPin), _dcPin(dcPin), _rstPin(rstPin),
    _contentType(0), _needsFullRedraw(true),
    _presetNumber(0), _sceneNumber(0),
    _effectId(0), _effectBypassed(false) {
  
  // Initialiseer display
  _display = new Adafruit_ST7735(_csPin, _dcPin, _rstPin);
  
  // Initialiseer preset naam
  memset(_presetName, 0, sizeof(_presetName));
}

ST7735Screen::~ST7735Screen() {
  delete _display;
}

void ST7735Screen::begin() {
  // Initialiseer ST7735 display
  _display->initR(INITR_BLACKTAB);
  _display->setRotation(0);
  _display->fillScreen(ST7735_BLACK);
  
  // Teken welkomstscherm
  _display->setTextSize(1);
  _display->setTextColor(ST7735_WHITE);
  _display->setCursor(20, 20);
  _display->print("Axe-Fx Controller");
  _display->setCursor(20, 40);
  _display->print("Initializing...");
  
  _needsFullRedraw = true;
}

void ST7735Screen::select() {
  // Niets nodig voor ST7735, CS pin wordt automatisch beheerd
}

void ST7735Screen::update() {
  select();
  
  if (_needsFullRedraw) {
    // Teken het juiste scherm op basis van content type
    switch (_contentType) {
      case 0:  // Preset info
        drawPresetScreen();
        break;
      case 1:  // Effect info
        drawEffectScreen();
        break;
      case 2:  // Tuner
        drawTunerScreen();
        break;
    }
    
    _needsFullRedraw = false;
  }
}

void ST7735Screen::updatePresetInfo(AxePreset preset) {
  _contentType = 0; // Preset info
  _presetNumber = preset.getPresetNumber();
  strncpy(_presetName, preset.getPresetName(), sizeof(_presetName) - 1);
  _presetName[sizeof(_presetName) - 1] = '\0';
  _sceneNumber = preset.getSceneNumber();
  strncpy(_sceneName, preset.getSceneName(), sizeof(_sceneName) - 1);
  _sceneName[sizeof(_sceneName) - 1] = '\0';
  
  _needsFullRedraw = true;
}

void ST7735Screen::updateEffectStatus(AxeEffect effect) {
  _contentType = 1; // Effect info
  _effectId = effect.getEffectId();
  _effectBypassed = effect.isBypassed();
  
  _needsFullRedraw = true;
}

void ST7735Screen::updateTuner(const char* note, byte string, byte fineTune) {
  if (_tunerEnabled) {
    strncpy(_tunerNote, note, sizeof(_tunerNote) - 1);
    _tunerNote[sizeof(_tunerNote) - 1] = '\0';
    _tunerString = string;
    _tunerFineTune = fineTune;
    
    _needsFullRedraw = true;
  }
}

void ST7735Screen::showTuner(bool enabled, const char* note, byte string, byte fineTune) {
  _tunerEnabled = enabled;
  
  if (enabled) {
    _contentType = 2; // Tuner info
    strncpy(_tunerNote, note, sizeof(_tunerNote) - 1);
    _tunerNote[sizeof(_tunerNote) - 1] = '\0';
    _tunerString = string;
    _tunerFineTune = fineTune;
  } else {
    _contentType = 0; // Terug naar preset info
  }
  
  _needsFullRedraw = true;
}


void ST7735Screen::drawPresetScreen() {
  _display->fillScreen(ST7735_BLACK);
  
  // Teken preset header
  _display->setTextSize(1);
  _display->setTextColor(ST7735_WHITE);
  _display->setCursor(5, 5);
  _display->print("Preset: ");
  _display->print(_presetNumber);
  
  // Teken preset naam
  _display->setCursor(5, 20);
  _display->print(_presetName);
  
  // Teken scene nummer
  _display->setCursor(5, 40);
  _display->print("Scene: ");
  _display->print(_sceneNumber);
  
  // Teken kader rond het scherm
  _display->drawRect(0, 0, ST7735_SCREEN_WIDTH, ST7735_SCREEN_HEIGHT, ST7735_BLUE);
}

void ST7735Screen::drawEffectScreen() {
  _display->fillScreen(ST7735_BLACK);
  
  // Teken effect header
  _display->setTextSize(1);
  _display->setTextColor(ST7735_WHITE);
  _display->setCursor(5, 5);
  _display->print("Effect ID: ");
  _display->print(_effectId);
  
  // Teken effect status
  _display->setCursor(5, 25);
  if (_effectBypassed) {
    _display->setTextColor(ST7735_RED);
    _display->print("BYPASSED");
  } else {
    _display->setTextColor(ST7735_GREEN);
    _display->print("ACTIVE");
  }
  
  // Teken kader rond het scherm
  _display->drawRect(0, 0, ST7735_SCREEN_WIDTH, ST7735_SCREEN_HEIGHT, ST7735_YELLOW);
}

void ST7735Screen::drawTunerScreen() {
  _display->fillScreen(ST7735_BLACK);
  
  if (_tuner.engaged) {
    // Teken tuner header
    _display->setTextSize(1);
    _display->setTextColor(ST7735_WHITE);
    _display->setCursor(5, 5);
    _display->print("TUNER");
    
    // Teken noot
    _display->setTextSize(3);
    _display->setCursor(40, 30);
    _display->print(_tuner.note);
    
    // Teken indicator voor stemming
    int16_t x = ST7735_SCREEN_WIDTH / 2;
    int16_t y = 80;
    int16_t width = 100;
    int16_t height = 10;
    
    // Teken middenlijn
    _display->drawFastVLine(x, y - 5, height + 10, ST7735_WHITE);
    
    // Teken balk
    _display->drawRect(x - width/2, y, width, height, ST7735_WHITE);
    
    // Teken indicator
    int16_t offset = map(_tuner.fineTune, -50, 50, -width/2, width/2);
    _display->fillRect(x + offset - 2, y - 5, 4, height + 10, 
                      abs(offset) < 3 ? ST7735_GREEN : ST7735_RED);
  } else {
    // Toon bericht dat tuner uit staat
    _display->setTextSize(1);
    _display->setTextColor(ST7735_WHITE);
    _display->setCursor(20, 50);
    _display->print("Tuner not active");
  }
  
  // Teken kader rond het scherm
  _display->drawRect(0, 0, ST7735_SCREEN_WIDTH, ST7735_SCREEN_HEIGHT, ST7735_GREEN);
}