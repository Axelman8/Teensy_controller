#ifndef AXE_STRUCTS_H
#define AXE_STRUCTS_H

#include <Arduino.h>

// Definitie van MyAxePreset
class MyAxePreset {
public:
  uint16_t presetNumber;
  char name[32];
  bool isValid;
  
  MyAxePreset() : presetNumber(0), isValid(false) {
    name[0] = '\0';
  }
};

// Definitie van MyAxeEffect
class MyAxeEffect {
public:
  uint8_t effectId;
  char name[32];
  bool bypassed;
  
  MyAxeEffect() : effectId(0), bypassed(true) {
    name[0] = '\0';
  }
};

// Definitie van MyAxeTuner
struct MyAxeTuner {
  bool enabled;
  char note[8];
  float frequency;
  int8_t cents;
  
  MyAxeTuner() : enabled(false), frequency(0.0f), cents(0) {
    note[0] = '\0';
  }
};

// Definitie van MyAxeLooper
struct MyAxeLooper {
  bool recording;
  bool playing;
  bool overdubbing;
  uint8_t position;
  
  MyAxeLooper() : recording(false), playing(false), overdubbing(false), position(0) {}
};

#endif // AXE_STRUCTS_H
