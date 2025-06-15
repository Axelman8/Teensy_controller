#include "ST7735Screen.h"

ST7735Screen::ST7735Screen(uint8_t csPin, uint8_t dcPin, uint8_t rstPin) 
  : Screen(csPin, dcPin, rstPin),
    _display(nullptr),
    _frameBuffer(nullptr),
    _contentType(0),
    _needsFullRedraw(true),
    _presetNumber(0),
    _sceneNumber(0),
    _effectId(0),
    _effectBypassed(false)
{
  _presetName[0] = '\0';
  
  // Alloceer frame buffer
  _frameBuffer = new uint16_t[SCREEN_WIDTH * SCREEN_HEIGHT];
}

ST7735Screen::~ST7735Screen() {
  if (_display) {
    delete _display;
  }
  
  if (_frameBuffer) {
    delete[] _frameBuffer;
  }
}

void ST7735Screen::begin() {
  _display = new Adafruit_ST7735(_csPin, _dcPin, _rstPin);
  _display->initR(INITR_BLACKTAB);  // Initialiseer ST7735 scherm
  _display->setRotation(1);  // Rotatie aanpassen indien nodig
  _display->fillScreen(ST7735_BLACK);
  
  _needsFullRedraw = true;
}

void ST7735Screen::update() {
  select();
  
  if (_needsFullRedraw) {
    // Teken het juiste scherm op basis van content type
    switch (_contentType) {
      case 0:  // Preset info
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
        
        // Teken scene info
        _display->setCursor(5, 35);
        _display->print("Scene: ");
        _display->print(_sceneNumber);
        break;
        
      case 1:  // Effect info
        _display->fillScreen(ST7735_BLACK);
        
        // Teken effect header
        _display->setTextSize(1);
        _display->setTextColor(ST7735_WHITE);
        _display->setCursor(5, 5);
        _display->print("Effect ID: ");
        _display->print(_effectId);
        
        // Teken effect status
        _display->setCursor(5, 25);
        _display->print(_effectBypassed ? "BYPASSED" : "ACTIVE");
        break;
    }
    
    _needsFullRedraw = false;
  }
  
  deselect();
}

void ST7735Screen::clear() {
  select();
  _display->fillScreen(ST7735_BLACK);
  deselect();
}

void ST7735Screen::drawText(int16_t x, int16_t y, const char* text, uint16_t color) {
  _display->setCursor(x, y);
  _display->setTextColor(color);
  _display->print(text);
}

void ST7735Screen::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  _display->drawRect(x, y, w, h, color);
}

void ST7735Screen::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  _display->fillRect(x, y, w, h, color);
}

void ST7735Screen::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
  _display->drawLine(x0, y0, x1, y1, color);
}

void ST7735Screen::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
  _display->drawCircle(x0, y0, r, color);
}

void ST7735Screen::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
  _display->fillCircle(x0, y0, r, color);
}

bool ST7735Screen::showsPresetInfo() {
  return _contentType == 0;
}

bool ST7735Screen::showsEffectInfo(uint8_t effectId) {
  return _contentType == 1 && _effectId == effectId;
}

void ST7735Screen::updatePresetInfo(const AxePreset& preset) {
  if (_presetNumber != preset.presetNumber ||
      _sceneNumber != preset.sceneNumber ||
      strcmp(_presetName, preset.name) != 0) {
    
    _presetNumber = preset.presetNumber;
    _sceneNumber = preset.sceneNumber;
    strncpy(_presetName, preset.name, sizeof(_presetName) - 1);
    _presetName[sizeof(_presetName) - 1] = '\0';
    
    _contentType = 0;
    _needsFullRedraw = true;
  }
}

void ST7735Screen::updateEffectStatus(const AxeEffect& effect) {
  if (_effectId != effect.effectId || _effectBypassed != effect.bypassed) {
    _effectId = effect.effectId;
    _effectBypassed = effect.bypassed;
    
    _contentType = 1;
    _needsFullRedraw = true;
  }
}
