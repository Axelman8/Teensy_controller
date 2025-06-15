#ifndef ST7735_SCREEN_H
#define ST7735_SCREEN_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "Screen.h"
#include "axefx/AxeFxTypes.h"

class ST7735Screen : public Screen {
public:
  ST7735Screen(uint8_t csPin, uint8_t dcPin, uint8_t rstPin);
  virtual ~ST7735Screen();
  
  // Implementatie van Screen interface
  virtual void begin() override;
  virtual void update() override;
  virtual void clear() override;
  virtual void drawText(int16_t x, int16_t y, const char* text, uint16_t color) override;
  virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
  virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
  virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) override;
  virtual void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) override;
  virtual void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) override;
  
  // Content type queries
  virtual bool showsPresetInfo() override;
  virtual bool showsEffectInfo(uint8_t effectId) override;
  
  // Content update methoden
  virtual void updatePresetInfo(const AxePreset& preset) override;
  virtual void updateEffectStatus(const AxeEffect& effect) override;
  
private:
  Adafruit_ST7735* _display;
  uint16_t* _frameBuffer;  // Toegevoegd: frame buffer voor dubbele buffering
  uint8_t _contentType;    // 0 = preset, 1 = effect
  bool _needsFullRedraw;
  
  // Content buffers
  char _presetName[32];
  uint16_t _presetNumber;
  uint8_t _sceneNumber;
  uint8_t _effectId;
  bool _effectBypassed;
  
  // Scherm dimensies
  static const uint16_t SCREEN_WIDTH = 160;
  static const uint16_t SCREEN_HEIGHT = 128;
};

#endif // ST7735_SCREEN_H
