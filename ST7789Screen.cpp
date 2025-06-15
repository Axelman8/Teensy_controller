#include "ST7789Screen.h"

ST7789Screen::ST7789Screen(uint8_t csPin, uint8_t dcPin, uint8_t rstPin) 
  : Screen(csPin, dcPin, rstPin),
    _display(nullptr),
    _contentType(0),
    _needsFullRedraw(true),
    _presetNumber(0),
    _sceneNumber(0),
    _effectId(0),
    _effectBypassed(false),
    _frameBuffer(nullptr)
{
  _presetName[0] = '\0';
  _tuner.note[0] = '\0';
  _tuner.cents = 0;
  _tuner.enabled = false;
}

ST7789Screen::~ST7789Screen() {
  if (_frameBuffer) {
    delete[] _frameBuffer;
  }
  
  if (_display) {
    delete _display;
  }
}

void ST7789Screen::begin() {
  _display = new Adafruit_ST7789(_csPin, _dcPin, _rstPin);
  _display->init(SCREEN_WIDTH, SCREEN_HEIGHT);
  _display->setRotation(2);  // Rotatie aanpassen indien nodig
  _display->fillScreen(ST77XX_BLACK);
  
  // Alloceer frame buffer als nodig
  if (!_frameBuffer) {
    _frameBuffer = new uint16_t[SCREEN_WIDTH * SCREEN_HEIGHT];
  }
  
  _needsFullRedraw = true;
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
    }
    
    _needsFullRedraw = false;
  }
  
  deselect();
}

void ST7789Screen::clear() {
  select();
  _display->fillScreen(ST77XX_BLACK);
  deselect();
  _needsFullRedraw = true;
}

void ST7789Screen::drawText(int16_t x, int16_t y, const char* text, uint16_t color) {
  _display->setCursor(x, y);
  _display->setTextColor(color);
  _display->print(text);
}

void ST7789Screen::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  _display->drawRect(x, y, w, h, color);
}

void ST7789Screen::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  _display->fillRect(x, y, w, h, color);
}

void ST7789Screen::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
  _display->drawLine(x0, y0, x1, y1, color);
}

void ST7789Screen::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
  _display->drawCircle(x0, y0, r, color);
}

void ST7789Screen::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
  _display->fillCircle(x0, y0, r, color);
}

bool ST7789Screen::showsPresetInfo() {
  return _contentType == 0;
}

bool ST7789Screen::showsEffectInfo(uint8_t effectId) {
  return _contentType == 1 && _effectId == effectId;
}

void ST7789Screen::updatePresetInfo(const AxePreset& preset) {
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

void ST7789Screen::updateEffectStatus(const AxeEffect& effect) {
  if (_effectId != effect.effectId || _effectBypassed != effect.bypassed) {
    _effectId = effect.effectId;
    _effectBypassed = effect.bypassed;
    
    _contentType = 1;
    _needsFullRedraw = true;
  }
}

void ST7789Screen::showTuner(const AxeTuner& tuner) {
  if (strcmp(_tuner.note, tuner.note) != 0 || _tuner.cents != tuner.cents || _tuner.enabled != tuner.enabled) {
    strncpy(_tuner.note, tuner.note, sizeof(_tuner.note) - 1);
    _tuner.note[sizeof(_tuner.note) - 1] = '\0';
    _tuner.cents = tuner.cents;
    _tuner.enabled = tuner.enabled;
    
    _contentType = 2;
    _needsFullRedraw = true;
  }
}

void ST7789Screen::setRotation(uint8_t rotation) {
  _display->setRotation(rotation);
  _needsFullRedraw = true;
}

void ST7789Screen::setTextSize(uint8_t size) {
  _display->setTextSize(size);
}

void ST7789Screen::setTextColor(uint16_t color) {
  _display->setTextColor(color);
}

void ST7789Screen::setCursor(uint16_t x, uint16_t y) {
  _display->setCursor(x, y);
}

void ST7789Screen::print(const char* text) {
  _display->print(text);
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
  _display->setCursor(10, 40);
  _display->print("Scene: ");
  _display->print(_sceneNumber);
  
  // Teken preset naam
  _display->setTextSize(1);
  _display->setCursor(10, 70);
  _display->print(_presetName);
  
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
  
  if (_tuner.enabled) {
    // Teken tuner header
    _display->setTextSize(2);
    _display->setTextColor(ST77XX_WHITE);
    _display->setCursor(10, 10);
    _display->print("Tuner");
    
    // Teken noot in groot formaat
    _display->setTextSize(4);
    _display->setCursor(SCREEN_WIDTH / 2 - 30, 50);
    _display->print(_tuner.note);
    
    // Teken tuner meter
    int centerX = SCREEN_WIDTH / 2;
    int centerY = 150;
    int meterWidth = 180;
    int meterHeight = 20;
    
    // Teken basis meter
    _display->drawRect(centerX - meterWidth/2, centerY - meterHeight/2, 
                      meterWidth, meterHeight, ST77XX_WHITE);
    
    // Teken center marker
    _display->drawLine(centerX, centerY - meterHeight/2 - 5,
                      centerX, centerY + meterHeight/2 + 5, ST77XX_WHITE);
    
    // Teken indicator op basis van cents
    int indicatorPos = centerX + (_tuner.cents * meterWidth / 100);
    indicatorPos = constrain(indicatorPos, centerX - meterWidth/2 + 5, centerX + meterWidth/2 - 5);
    
    // Kies kleur op basis van hoe dicht bij 0 cents
    uint16_t indicatorColor;
    if (abs(_tuner.cents) < 5) {
      indicatorColor = ST77XX_GREEN;
    } else if (abs(_tuner.cents) < 15) {
      indicatorColor = ST77XX_YELLOW;
    } else {
      indicatorColor = ST77XX_RED;
    }
    
    // Teken indicator
    _display->fillCircle(indicatorPos, centerY, 10, indicatorColor);
    
    // Teken cents waarde
    _display->setTextSize(1);
    _display->setCursor(centerX + 50, centerY + 30);
    _display->print(_tuner.cents);
    _display->print(" cents");
  } else {
    // Toon bericht dat tuner niet actief is
    _display->setTextSize(2);
    _display->setTextColor(ST77XX_WHITE);
    _display->setCursor(20, SCREEN_HEIGHT / 2 - 10);
    _display->print("Tuner not active");
  }
}
