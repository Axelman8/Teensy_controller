#include "ST7789Screen.h"
#include "../..axe/AxeStructs.h"

ST7789Screen::ST7789Screen(uint8_t csPin, uint8_t dcPin, uint8_t rstPin)
  : _csPin(csPin), _dcPin(dcPin), _rstPin(rstPin), _needsFullRedraw(true), _contentType(0),
    _presetNumber(0), _sceneNumber(0), _effectId(0), _effectBypassed(true), _tunerEnabled(false) {
  
  // Initialiseer strings
  _presetName[0] = '\0';
  _sceneName[0] = '\0';
  _tunerNote[0] = '\0';
}

ST7789Screen::~ST7789Screen() {
  if (_display != nullptr) {
    delete _display;
  }
}

void ST7789Screen::begin() {
  // Initialiseer het display
  _display = new Adafruit_ST7789(_csPin, _dcPin, _rstPin);
  _display->init(SCREEN_WIDTH, SCREEN_HEIGHT);
  _display->setRotation(2); // Rotatie aanpassen indien nodig
  _display->fillScreen(ST77XX_BLACK);
  
  // Teken een welkomstscherm
  _display->setTextSize(2);
  _display->setTextColor(ST77XX_WHITE);
  _display->setCursor(10, 10);
  _display->print("Axe-Fx Controller");
  _display->setCursor(10, 40);
  _display->print("Initializing...");
  
  _needsFullRedraw = true;
}

void ST7789Screen::select() {
  // Selecteer dit display voor updates
}

void ST7789Screen::update() {
  select();
  
  // Teken het juiste scherm op basis van content type
  if (_needsFullRedraw) {
    switch (_contentType) {
      case 0:
        drawPresetScreen();
        break;
      case 1:
        drawEffectScreen();
        break;
      case 2:
        drawTunerScreen();
        break;
      case 3:  // Looper (voeg deze toe)
        drawLooperScreen();
        break;
    }
    
    _needsFullRedraw = false;
  }
}

void ST7789Screen::updateLooperStatus(AxeLooper looper) {
    _looper = looper;
    if (_contentType == 3) { // Als looper scherm actief is
        drawLooperScreen();
    }
}

void ST7789Screen::updatePresetInfo(AxePreset preset) {
  _contentType = 0; // Preset info
  _presetNumber = preset.getPresetNumber();
  strncpy(_presetName, preset.getPresetName(), sizeof(_presetName) - 1);
  _presetName[sizeof(_presetName) - 1] = '\0';
  _sceneNumber = preset.getSceneNumber();
  strncpy(_sceneName, preset.getSceneName(), sizeof(_sceneName) - 1);
  _sceneName[sizeof(_sceneName) - 1] = '\0';
  
  _needsFullRedraw = true;
}

void ST7789Screen::updateEffectStatus(AxeEffect effect) {
  _contentType = 1; // Effect info
  _effectId = effect.getEffectId();
  _effectBypassed = effect.isBypassed();
  
  _needsFullRedraw = true;
}

void ST7789Screen::updateTuner(const char* note, byte string, byte fineTune) {
  if (_tunerEnabled) {
    strncpy(_tunerNote, note, sizeof(_tunerNote) - 1);
    _tunerNote[sizeof(_tunerNote) - 1] = '\0';
    _tunerString = string;
    _tunerFineTune = fineTune;
    
    _needsFullRedraw = true;
  }
}

void ST7789Screen::showTuner(bool enabled, const char* note, byte string, byte fineTune) {
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

void ST7789Screen::drawPresetScreen() {
  _display->fillScreen(ST77XX_BLACK);
  
  // Teken preset header
  _display->setTextSize(2);
  _display->setTextColor(ST77XX_WHITE);
  _display->setCursor(10, 10);
  _display->print("Preset: ");
  _display->print(_presetNumber);
  
  // Teken scene info
  _display->setTextSize(2);
  _display->setCursor(10, 40);
  _display->print("Scene: ");
  _display->print(_sceneNumber);
  
  // Teken preset naam
  _display->setTextSize(1);
  _display->setCursor(10, 70);
  _display->print(_presetName);
  
  // Teken scene naam
  _display->setCursor(10, 90);
  _display->print(_sceneName);
  
  // Teken kader rond het scherm
  _display->drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ST77XX_BLUE);
}

void ST7789Screen::drawEffectScreen() {
  _display->fillScreen(ST77XX_BLACK);
  
  // Teken effect header
  _display->setTextSize(2);
  _display->setTextColor(ST77XX_WHITE);
  _display->setCursor(10, 10);
  _display->print("Effect ID: ");
  _display->print(_effectId);
  
  // Teken effect status
  _display->setCursor(10, 50);
  if (_effectBypassed) {
    _display->setTextColor(ST77XX_RED);
    _display->print("BYPASSED");
  } else {
    _display->setTextColor(ST77XX_GREEN);
    _display->print("ACTIVE");
  }
  
  // Teken kader rond het scherm
  _display->drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ST77XX_YELLOW);
}

void ST7789Screen::drawTunerScreen() {
  _display->fillScreen(ST77XX_BLACK);
  
  if (_tunerEnabled) {
    // Teken tuner header
    _display->setTextSize(2);
    _display->setTextColor(ST77XX_WHITE);
    _display->setCursor(10, 10);
    _display->print("TUNER");
    
    // Teken noot
    _display->setTextSize(4);
    _display->setCursor(SCREEN_WIDTH/2 - 30, SCREEN_HEIGHT/2 - 30);
    _display->print(_tunerNote);
    
    // Teken string nummer
    _display->setTextSize(2);
    _display->setCursor(10, SCREEN_HEIGHT - 40);
    _display->print("String: ");
    _display->print(_tunerString);
    
    // Teken tuning indicator
    int barWidth = 180;
    int barHeight = 20;
    int barX = (SCREEN_WIDTH - barWidth) / 2;
    int barY = SCREEN_HEIGHT - 80;
    
    // Teken achtergrond van de balk
    _display->drawRect(barX, barY, barWidth, barHeight, ST77XX_WHITE);
    
    // Bereken positie van de indicator (0-127 naar 0-barWidth)
    int indicatorPos = map(_tunerFineTune, 0, 127, 0, barWidth);
    
    // Teken de indicator
    if (_tunerFineTune < 60) {
      _display->fillRect(barX + indicatorPos - 5, barY, 10, barHeight, ST77XX_RED);
    } else if (_tunerFineTune > 67) {
      _display->fillRect(barX + indicatorPos - 5, barY, 10, barHeight, ST77XX_RED);
    } else {
      _display->fillRect(barX + indicatorPos - 5, barY, 10, barHeight, ST77XX_GREEN);
    }
    
    // Teken middenlijn
    _display->drawLine(barX + barWidth/2, barY, barX + barWidth/2, barY + barHeight, ST77XX_BLUE);
  } else {
    // Teken "Tuner Off" bericht
    _display->setTextSize(2);
    _display->setTextColor(ST77XX_WHITE);
    _display->setCursor(10, SCREEN_HEIGHT/2 - 10);
    _display->print("Tuner Off");
  }
  
  // Teken kader rond het scherm
  _display->drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ST77XX_CYAN);
}

void ST7789Screen::drawLooperScreen() {
  _display->fillScreen(ST77XX_BLACK);
  
  // Teken looper header
  _display->setTextSize(2);
  _display->setTextColor(ST77XX_WHITE);
  _display->setCursor(10, 10);
  _display->print("LOOPER");
  
  // Teken looper status
  _display->setTextSize(1);
  _display->setCursor(10, 40);
  
  if (_looper.recording) {
    _display->setTextColor(ST77XX_RED);
    _display->print("RECORDING");
  } else if (_looper.playing) {
    _display->setTextColor(ST77XX_GREEN);
    _display->print("PLAYING");
  } else if (_looper.overdubbing) {
    _display->setTextColor(ST77XX_YELLOW);
    _display->print("OVERDUB");
  } else {
    _display->setTextColor(ST77XX_WHITE);
    _display->print("STOPPED");
  }
  
  // Teken kader rond het scherm
  _display->drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ST77XX_BLUE);
}
