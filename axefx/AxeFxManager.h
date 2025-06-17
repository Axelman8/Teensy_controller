#pragma once

#include <Arduino.h>
#include <AxeFxControl.h>

class AxeFxManager {
public:
  AxeFxManager();
  
  void begin();
  void update();
  
  // Registreer callbacks
  void registerPresetChangeCallback(void (*callback)(AxePreset));
  void registerEffectBypassCallback(void (*callback)(AxeEffect));
  void registerTunerDataCallback(void (*callback)(const char*, byte, byte));
  void registerTunerStatusCallback(void (*callback)(bool));
  void registerLooperStatusCallback(void (*callback)(AxeLooper));
  
  // Commando's naar de Axe-Fx
  void sendPresetChange(uint16_t preset);
  void sendSceneChange(uint8_t scene);
  void sendEffectBypass(uint8_t effectId, bool bypassed);
  void sendTunerToggle(bool enabled);
  void sendLooperCommand(uint8_t command, uint8_t value);
  
  // Getters
  AxePreset getCurrentPreset();
  AxeEffect* getEffect(uint8_t effectId);
  
private:
  AxeSystem _axe;
  
  // Static instance voor callbacks
  static AxeFxManager* _instance;
  
  // Static callback functies voor de AxeSystem
  static void presetChangeCallback(AxePreset preset);
  static void effectBypassCallback(AxeEffect effect);
  static void tunerDataCallback(const char* note, byte string, byte fineTune);
  static void tunerStatusCallback(bool enabled);
  static void looperStatusCallback(AxeLooper looper);
  
  // Callback functies
  void (*_presetChangeCallback)(AxePreset) = nullptr;
  void (*_effectBypassCallback)(AxeEffect) = nullptr;
  void (*_tunerDataCallback)(const char*, byte, byte) = nullptr;
  void (*_tunerStatusCallback)(bool) = nullptr;
  void (*_looperStatusCallback)(AxeLooper) = nullptr;
};
