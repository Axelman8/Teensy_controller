#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include "axefx/AxeFxTypes.h"

class Screen {
public:
  // Constructor alleen declareren (niet implementeren) in de header
  Screen(uint8_t csPin, uint8_t dcPin, uint8_t rstPin);
  
  // Virtuele destructor voor polymorfisme
  virtual ~Screen() {}
  
  // Virtuele methoden die door afgeleide klassen geïmplementeerd moeten worden
  virtual void begin() = 0;
  virtual void clear() = 0;
  virtual void update() = 0;
  virtual void drawText(int16_t x, int16_t y, const char* text, uint16_t color) = 0;
  virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) = 0;
  virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) = 0;
  virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) = 0;
  virtual void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) = 0;
  virtual void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) = 0;
  
  // Content type queries
  virtual bool showsPresetInfo() = 0;
  virtual bool showsEffectInfo(uint8_t effectId) = 0;
  
  // Content update methoden
  virtual void updatePresetInfo(const AxePreset& preset) = 0;
  virtual void updateEffectStatus(const AxeEffect& effect) = 0;
  
  // Helper methoden
  void select();
  void deselect();
  
protected:
  uint8_t _csPin;
  uint8_t _dcPin;
  uint8_t _rstPin;
};

#endif // SCREEN_H