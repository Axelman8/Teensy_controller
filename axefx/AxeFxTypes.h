#ifndef AXE_FX_TYPES_H
#define AXE_FX_TYPES_H

#include <Arduino.h>

// AxeFX bericht types
enum AxeFxMessageType {
  AXE_PRESET_CHANGE,
  AXE_EFFECT_BYPASS,
  AXE_SCENE_CHANGE,
  AXE_TUNER_DATA,
  AXE_LOOPER_STATUS,
  AXE_TEMPO_CHANGE,
  AXE_PARAMETER_CHANGE,
  AXE_OTHER
};

// Button types
#define BUTTON_TYPE_PRESET  0
#define BUTTON_TYPE_SCENE   1
#define BUTTON_TYPE_EFFECT  2
#define BUTTON_TYPE_TUNER   3
#define BUTTON_TYPE_LOOPER  4

// Axe-Fx CC nummers
#define AXE_CC_SCENE_SELECT    34
#define AXE_CC_TUNER           15
#define AXE_CC_LOOPER_RECORD   60
#define AXE_CC_LOOPER_PLAY     61
// Voeg andere CC nummers toe...

// Bypass CC nummers
#define AXE_CC_BYPASS_COMP     21
#define AXE_CC_BYPASS_GATE     22
#define AXE_CC_BYPASS_DRIVE    23
#define AXE_CC_BYPASS_AMP      24
#define AXE_CC_BYPASS_CAB      25
#define AXE_CC_BYPASS_REVERB   26
#define AXE_CC_BYPASS_DELAY    27
#define AXE_CC_BYPASS_CHORUS   28
#define AXE_CC_BYPASS_FLANGER  29
#define AXE_CC_BYPASS_PHASER   30
#define AXE_CC_BYPASS_PITCH    31
#define AXE_CC_BYPASS_WAH      32



// AxeFX effect structuur
struct AxeEffect {
  uint8_t effectId;
  bool bypassed;
  uint8_t channel;
  char name[20];
};

// AxeFX preset structuur
struct AxePreset {
  uint16_t presetNumber;
  char name[32];
  uint8_t sceneNumber;
  bool isEdited;
};

// AxeFX tuner data
struct AxeTuner {
  char note[3];
  int8_t cents;
  bool enabled;
};

// AxeFX looper status
struct AxeLooper {
  bool recording;
  bool playing;
  bool overdubbing;
  bool once;
  bool reverse;
  bool halfSpeed;
};

// Union voor verschillende data types
union AxeFxData {
  AxeEffect effect;
  AxePreset preset;
  AxeTuner tuner;
  AxeLooper looper;
  uint8_t tempo;
  uint8_t rawData[64];
};

// AxeFX bericht structuur
struct AxeFxMessage {
  AxeFxMessageType type;
  AxeFxData data;
  uint8_t dataSize;
};

#endif // AXE_FX_TYPES_H
