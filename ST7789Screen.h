#ifndef ST7789_SCREEN_H
#define ST7789_SCREEN_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "Screen.h"
#include "axefx/AxeFxTypes.h"

class ST7789Screen : public Screen {
public:
  ST7789Screen(uint8_t csPin, uint8_t dcPin, uint8_t rstPin);
  virtual ~ST7789Screen();
  
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
  
  // ST7789 specifieke methoden
  void setRotation(uint8_t rotation);
  void setTextSize(uint8_t size);
  void setTextColor(uint16_t color);
  void setCursor(uint16_t x, uint16_t y);
  void print(const char* text);
  
  // Tuner display
  void showTuner(const AxeTuner& tuner);

private:
  Adafruit_ST7789* _display;
  uint8_t _contentType;  // 0 = preset, 1 = effect, 2 = tuner
  bool _needsFullRedraw;
  
  // Content buffers
  char _presetName[32];
  uint16_t _presetNumber;
  uint8_t _sceneNumber;
  uint8_t _effectId;
  bool _effectBypassed;
  AxeTuner _tuner;
  
  // Dubbele buffer voor optimalisatie
  uint16_t* _frameBuffer;
  static const uint16_t SCREEN_WIDTH = 240;
  static const uint16_t SCREEN_HEIGHT = 240;
  
  // Helper methoden voor verschillende schermtypes
  void drawPresetScreen();
  void drawEffectScreen();
  void drawTunerScreen();
};

#endif // ST7789_SCREEN_H
