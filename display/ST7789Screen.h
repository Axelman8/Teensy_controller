#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <AxeFxControl.h>
#include "BaseDisplay.h"

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240

class ST7789Screen : public BaseScreen {
public:
  ST7789Screen(uint8_t csPin, uint8_t dcPin, uint8_t rstPin);
  virtual ~ST7789Screen();
  
  void begin() override;
  void update() override;
  
  // Update functies
  void updatePresetInfo(AxePreset preset) override;
  void updateEffectStatus(AxeEffect effect) override;
  void updateTuner(const char* note, byte string, byte fineTune) override;
  void showTuner(bool enabled, const char* note, byte string, byte fineTune) override;
  virtual void updateLooperStatus(AxeLooper looper) override;
  
private:
  // Implementatie details
  Adafruit_ST7789* _display;
  uint8_t _csPin, _dcPin, _rstPin;
  bool _needsFullRedraw;
  uint8_t _contentType; // 0 = preset, 1 = effect, 2 = tuner
  AxeLooper _looper;
  
  // Preset info
  uint16_t _presetNumber;
  char _presetName[32];
  uint8_t _sceneNumber;
  char _sceneName[32];
  
  // Effect info
  uint8_t _effectId;
  bool _effectBypassed;
  
  // Tuner info
  bool _tunerEnabled;
  char _tunerNote[4];
  byte _tunerString;
  byte _tunerFineTune;
  
  // Helper functies
  void select();
  void drawPresetScreen();
  void drawEffectScreen();
  void drawTunerScreen();
  void drawLooperScreen();


};
