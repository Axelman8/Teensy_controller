#include "AxeFxManager.h"

// MIDI interface definitie
#if defined(TEENSYDUINO)
midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> MIDI((HardwareSerial&)Serial1);
#else
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);
#endif

AxeFxManager::AxeFxManager() :
  _presetChangeCallback(nullptr),
  _effectBypassCallback(nullptr),
  _tunerDataCallback(nullptr),
  _looperStatusCallback(nullptr)
{
  // Initialiseer preset
  _currentPreset.presetNumber = 0;
  _currentPreset.sceneNumber = 0;
  strcpy(_currentPreset.name, "Initializing...");
  
  // Initialiseer tuner
  _tuner.note[0] = '\0';
  _tuner.cents = 0;
  _tuner.enabled = false;
  
  // Initialiseer looper
  _looper.recording = false;
  _looper.playing = false;
  _looper.overdubbing = false;
  _looper.count = 0;
  
  // Initialiseer effecten
  initializeEffects();
}

void AxeFxManager::begin() {
  // Initialiseer MIDI
  MIDI.begin(MIDI_CHANNEL_OMNI);
  
  // Stel MIDI callbacks in
  MIDI.setHandleProgramChange(std::bind(&AxeFxManager::handleProgramChange, this, std::placeholders::_1, std::placeholders::_2));
  MIDI.setHandleControlChange(std::bind(&AxeFxManager::handleControlChange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  MIDI.setHandleSystemExclusive(std::bind(&AxeFxManager::handleSysEx, this, std::placeholders::_1, std::placeholders::_2));
}

void AxeFxManager::update() {
  // Lees MIDI berichten
  MIDI.read();
}

void AxeFxManager::sendPresetChange(uint16_t presetNumber) {
  // Stuur program change naar Axe-Fx
  MIDI.sendProgramChange(presetNumber, AXE_FX_MIDI_CHANNEL);
  
  // Update lokale status
  _currentPreset.presetNumber = presetNumber;
}

void AxeFxManager::sendSceneChange(uint8_t sceneNumber) {
  // Stuur CC bericht voor scene change
  MIDI.sendControlChange(AXE_CC_SCENE_SELECT, sceneNumber, AXE_FX_MIDI_CHANNEL);
  
  // Update lokale status
  _currentPreset.sceneNumber = sceneNumber;
}

void AxeFxManager::sendEffectBypass(uint8_t effectId, bool bypassed) {
  // Bepaal de juiste CC op basis van effectId
  uint8_t cc = 0;
  
  switch (effectId) {
    case 0: cc = AXE_CC_BYPASS_COMP; break;
    case 1: cc = AXE_CC_BYPASS_GATE; break;
    case 2: cc = AXE_CC_BYPASS_DRIVE; break;
    case 3: cc = AXE_CC_BYPASS_AMP; break;
    case 4: cc = AXE_CC_BYPASS_CAB; break;
    case 5: cc = AXE_CC_BYPASS_REVERB; break;
    case 6: cc = AXE_CC_BYPASS_DELAY; break;
    case 7: cc = AXE_CC_BYPASS_CHORUS; break;
    case 8: cc = AXE_CC_BYPASS_FLANGER; break;
    case 9: cc = AXE_CC_BYPASS_PHASER; break;
    case 10: cc = AXE_CC_BYPASS_PITCH; break;
    case 11: cc = AXE_CC_BYPASS_WAH; break;
    default: return; // Ongeldig effect ID
  }
  
  // Stuur CC bericht voor effect bypass
  MIDI.sendControlChange(cc, bypassed ? 0 : 127, AXE_FX_MIDI_CHANNEL);
  
  // Update lokale status
  if (effectId < MAX_EFFECTS) {
    _effects[effectId].bypassed = bypassed;
    
    // Roep callback aan indien geregistreerd
    if (_effectBypassCallback != nullptr) {
      _effectBypassCallback(_effects[effectId]);
    }
  }
}

void AxeFxManager::sendTunerToggle(bool enabled) {
  // Stuur CC bericht voor tuner toggle
  MIDI.sendControlChange(AXE_CC_TUNER, enabled ? 127 : 0, AXE_FX_MIDI_CHANNEL);
  
  // Update lokale status
  _tuner.enabled = enabled;
  
  // Roep callback aan indien geregistreerd
  if (_tunerDataCallback != nullptr) {
    _tunerDataCallback(_tuner);
  }
}

void AxeFxManager::sendLooperCommand(uint8_t command, uint8_t value) {
  // Stuur CC bericht voor looper commando
  MIDI.sendControlChange(command, value, AXE_FX_MIDI_CHANNEL);
  
  // Update lokale status op basis van commando
  switch (command) {
    case AXE_CC_LOOPER_RECORD:
      _looper.recording = (value > 63);
      break;
    case AXE_CC_LOOPER_PLAY:
      _looper.playing = (value > 63);
      break;
    // Andere looper commando's...
  }
  
  // Roep callback aan indien geregistreerd
  if (_looperStatusCallback != nullptr) {
    _looperStatusCallback(_looper);
  }
}

AxePreset AxeFxManager::getCurrentPreset() {
  return _currentPreset;
}

AxeEffect AxeFxManager::getEffect(uint8_t effectId) {
  if (effectId < MAX_EFFECTS) {
    return _effects[effectId];
  }
  
  // Retourneer een leeg effect als fallback
  AxeEffect emptyEffect;
  emptyEffect.effectId = 0xFF;
  emptyEffect.bypassed = true;
  strcpy(emptyEffect.name, "Invalid");
  return emptyEffect;
}

AxeTuner AxeFxManager::getTuner() {
  return _tuner;
}

AxeLooper AxeFxManager::getLooper() {
  return _looper;
}

void AxeFxManager::registerPresetChangeCallback(PresetChangeCallback callback) {
  _presetChangeCallback = callback;
}

void AxeFxManager::registerEffectBypassCallback(EffectBypassCallback callback) {
  _effectBypassCallback = callback;
}

void AxeFxManager::registerTunerDataCallback(TunerDataCallback callback) {
  _tunerDataCallback = callback;
}

void AxeFxManager::registerLooperStatusCallback(LooperStatusCallback callback) {
  _looperStatusCallback = callback;
}

void AxeFxManager::handleProgramChange(byte channel, byte program) {
  // Verwerk alleen berichten op het Axe-Fx kanaal
  if (channel == AXE_FX_MIDI_CHANNEL) {
    // Update preset nummer
    _currentPreset.presetNumber = program;
    
    // Roep callback aan indien geregistreerd
    if (_presetChangeCallback != nullptr) {
      _presetChangeCallback(_currentPreset);
    }
  }
}

void AxeFxManager::handleControlChange(byte channel, byte control, byte value) {
  // Verwerk alleen berichten op het Axe-Fx kanaal
  if (channel == AXE_FX_MIDI_CHANNEL) {
    // Verwerk op basis van CC nummer
    if (control == AXE_CC_SCENE_SELECT) {
      // Scene change
      _currentPreset.sceneNumber = value;
      
      // Roep callback aan indien geregistreerd
      if (_presetChangeCallback != nullptr) {
        _presetChangeCallback(_currentPreset);
      }
    }
    else if (control == AXE_CC_TUNER) {
      // Tuner toggle
      _tuner.enabled = (value > 63);
      
      // Roep callback aan indien geregistreerd
      if (_tunerDataCallback != nullptr) {
        _tunerDataCallback(_tuner);
      }
    }
    else {
      // Controleer of het een effect bypass CC is
      uint8_t effectId = getEffectIdFromCC(control);
      if (effectId < MAX_EFFECTS) {
        // Effect bypass
        _effects[effectId].bypassed = (value < 64);
        
        // Roep callback aan indien geregistreerd
        if (_effectBypassCallback != nullptr) {
          _effectBypassCallback(_effects[effectId]);
        }
      }
    }
  }
}

void AxeFxManager::handleSysEx(byte* array, unsigned size) {
  // Verwerk SysEx berichten (voor preset naam, tuner data, etc.)
  // Dit is een vereenvoudigde implementatie
  
  // Controleer of het een Axe-Fx SysEx bericht is
  if (size > 5 && array[1] == 0x00 && array[2] == 0x01 && array[3] == 0x74) {
    // Axe-Fx SysEx bericht
    
    // Verwerk op basis van functie-ID (byte 4)
    switch (array[4]) {
      case 0x0A: // Preset naam
        if (size > 10) {
          // Kopieer preset naam (vanaf byte 6)
          memset(_currentPreset.name, 0, sizeof(_currentPreset.name));
          for (unsigned i = 0; i < sizeof(_currentPreset.name) - 1 && i + 6 < size - 1; i++) {
            _currentPreset.name[i] = array[i + 6];
          }
          
          // Roep callback aan indien geregistreerd
          if (_presetChangeCallback != nullptr) {
            _presetChangeCallback(_currentPreset);
          }
        }
        break;
        
      case 0x0D: // Tuner data
        if (size > 8) {
          // Verwerk tuner data
          char note = array[6]; // Note waarde
          int8_t cents = (int8_t)array[7]; // Cents waarde
          
          // Update tuner info
          _tuner.note[0] = note;
          _tuner.note[1] = '\0';
          _tuner.cents = cents;
          
          // Roep callback aan indien geregistreerd
          if (_tunerDataCallback != nullptr) {
            _tunerDataCallback(_tuner);
          }
        }
        break;
    }
  }
}

void AxeFxManager::initializeEffects() {
  // Initialiseer effect array met standaardwaarden
  const char* effectNames[] = {
    "Compressor", "Gate", "Drive", "Amp", "Cab", "Reverb",
    "Delay", "Chorus", "Flanger", "Phaser", "Pitch", "Wah"
  };
  
  for (int i = 0; i < MAX_EFFECTS; i++) {
    _effects[i].effectId = i;
    _effects[i].bypassed = true;
    strncpy(_effects[i].name, effectNames[i], sizeof(_effects[i].name) - 1);
    _effects[i].name[sizeof(_effects[i].name) - 1] = '\0';  // Zorg voor null-terminatie
  }
}

uint8_t AxeFxManager::getEffectIdFromCC(byte control) {
  // Vertaal CC nummer naar effect ID
  switch (control) {
    case AXE_CC_BYPASS_COMP: return 0;
    case AXE_CC_BYPASS_GATE: return 1;
    case AXE_CC_BYPASS_DRIVE: return 2;
    case AXE_CC_BYPASS_AMP: return 3;
    case AXE_CC_BYPASS_CAB: return 4;
    case AXE_CC_BYPASS_REVERB: return 5;
    case AXE_CC_BYPASS_DELAY: return 6;
    case AXE_CC_BYPASS_CHORUS: return 7;
    case AXE_CC_BYPASS_FLANGER: return 8;
    case AXE_CC_BYPASS_PHASER: return 9;
    case AXE_CC_BYPASS_PITCH: return 10;
    case AXE_CC_BYPASS_WAH: return 11;
    default: return 0xFF; // Ongeldig effect ID
  }
}
