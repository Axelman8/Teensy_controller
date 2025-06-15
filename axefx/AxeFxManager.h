#ifndef AXE_FX_MANAGER_H
#define AXE_FX_MANAGER_H

#include <Arduino.h>
#include <MIDI.h>
#include "AxeFxTypes.h"

// MIDI kanaal voor Axe-Fx
#define AXE_FX_MIDI_CHANNEL 1

// Maximaal aantal effecten
#define MAX_EFFECTS 12

// Voorwaartse declaratie van MIDI interface
#if defined(TEENSYDUINO)
extern midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> MIDI;
#endif

class AxeFxManager {
public:
  AxeFxManager();
  
  // Initialisatie
  void begin();
  
  // Update (verwerk MIDI berichten)
  void update();
  
  // Commando's naar Axe-Fx sturen
  void sendPresetChange(uint16_t presetNumber);
  void sendSceneChange(uint8_t sceneNumber);
  void sendEffectBypass(uint8_t effectId, bool bypassed);
  void sendTunerToggle(bool enabled);
  void sendLooperCommand(uint8_t command, uint8_t value);
  
  // Getters voor huidige status
  AxePreset getCurrentPreset();
  AxeEffect getEffect(uint8_t effectId);
  AxeTuner getTuner();
  AxeLooper getLooper();
  
  // Callback registratie
  typedef void (*PresetChangeCallback)(const AxePreset&);
  typedef void (*EffectBypassCallback)(const AxeEffect&);
  typedef void (*TunerDataCallback)(const AxeTuner&);
  typedef void (*LooperStatusCallback)(const AxeLooper&);
  
  void registerPresetChangeCallback(PresetChangeCallback callback);
  void registerEffectBypassCallback(EffectBypassCallback callback);
  void registerTunerDataCallback(TunerDataCallback callback);
  void registerLooperStatusCallback(LooperStatusCallback callback);

private:
  // Huidige status
  AxePreset _currentPreset;
  AxeEffect _effects[MAX_EFFECTS];
  AxeTuner _tuner;
  AxeLooper _looper;
  
  // Callbacks
  PresetChangeCallback _presetChangeCallback;
  EffectBypassCallback _effectBypassCallback;
  TunerDataCallback _tunerDataCallback;
  LooperStatusCallback _looperStatusCallback;
  
  // MIDI bericht verwerking
  void handleProgramChange(byte channel, byte program);
  void handleControlChange(byte channel, byte control, byte value);
  void handleSysEx(byte* array, unsigned size);
  
  // Helper methoden
  void initializeEffects();
  uint8_t getEffectIdFromCC(byte control);
};

#endif // AXE_FX_MANAGER_H
