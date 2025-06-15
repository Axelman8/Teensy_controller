/* 
 * Axe-Fx Controller
 * 
 * Een MIDI controller voor Fractal Audio Axe-Fx gitaarprocessors
 * met ondersteuning voor meerdere displays en configureerbare knoppen.
 * 
 * Ontwikkeld voor Teensy 4.1
 */

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <MIDI.h>
#include "config/PinDefinitions.h"
#include "config/ConfigManager.h"
#include "input/ButtonManager.h"
#include "display/DisplayManager.h"
#include "axefx/AxeFxManager.h"

// Globale objecten
ConfigManager configManager;
ButtonManager buttonManager(&configManager);
DisplayManager displayManager(&configManager);
AxeFxManager axeFxManager;

// Callback functies voor Axe-Fx events
void onPresetChange(const AxePreset& preset) {
  // Update displays met nieuwe preset informatie
  displayManager.updatePresetInfo(preset);
  
  // Debug output
  Serial.print("Preset changed: ");
  Serial.print(preset.presetNumber);
  Serial.print(" - ");
  Serial.println(preset.name);
}

void onEffectBypass(const AxeEffect& effect) {
  // Update displays met nieuwe effect status
  displayManager.updateEffectStatus(effect);
  
  // Debug output
  Serial.print("Effect ");
  Serial.print(effect.effectId);
  Serial.print(" (");
  Serial.print(effect.name);
  Serial.print(") ");
  Serial.println(effect.bypassed ? "bypassed" : "active");
}

void onTunerData(const AxeTuner& tuner) {
  // Update displays met tuner informatie
  displayManager.updateTuner(tuner);
  
  // Debug output
  if (tuner.enabled) {
    Serial.print("Tuner: ");
    Serial.print(tuner.note);
    Serial.print(" ");
    Serial.print(tuner.cents);
    Serial.println(" cents");
  }
}

// Callback functie voor knop events
void onButtonEvent(uint8_t buttonId, ButtonEvent event) {
  // Verwerk knop events
  if (event.type == BUTTON_PRESSED) {
    Serial.print("Button ");
    Serial.print(buttonId);
    Serial.println(" pressed");
    
    // Haal de knop configuratie op
    ButtonConfig* config = configManager.getButtonConfig(buttonId);
    
    // Voer actie uit op basis van knop type
    switch (config->type) {
      case BUTTON_TYPE_PRESET:
        // Preset selectie
        axeFxManager.sendPresetChange(config->value);
        break;
        
      case BUTTON_TYPE_SCENE:
        // Scene selectie
        axeFxManager.sendSceneChange(config->value);
        break;
        
      case BUTTON_TYPE_EFFECT:
        // Effect bypass toggle
        {
          AxeEffect effect = axeFxManager.getEffect(config->value);
          axeFxManager.sendEffectBypass(config->value, !effect.bypassed);
        }
        break;
        
      case BUTTON_TYPE_TUNER:
        // Tuner toggle
        {
          AxeTuner tuner = axeFxManager.getTuner();
          axeFxManager.sendTunerToggle(!tuner.enabled);
        }
        break;
        
      case BUTTON_TYPE_LOOPER:
        // Looper commando
        axeFxManager.sendLooperCommand(config->value, 127);
        break;
        
      default:
        // Onbekend knop type
        Serial.println("Unknown button type");
        break;
    }
  }
  else if (event.type == BUTTON_RELEASED) {
    Serial.print("Button ");
    Serial.print(buttonId);
    Serial.println(" released");
    
    // Verwerk knop release events indien nodig
    ButtonConfig* config = configManager.getButtonConfig(buttonId);
    
    // Voor sommige looper commando's moeten we een "off" bericht sturen bij release
    if (config->type == BUTTON_TYPE_LOOPER) {
      // Stuur waarde 0 voor looper commando's die momentary zijn
      if (config->value == AXE_CC_LOOPER_RECORD || 
          config->value == AXE_CC_LOOPER_PLAY) {
        axeFxManager.sendLooperCommand(config->value, 0);
      }
    }
  }
}

void setup() {
  // Initialiseer seriële communicatie voor debugging
  Serial.begin(115200);
  Serial.println("Axe-Fx Controller starting...");
  
  // Initialiseer SPI voor displays
  SPI.begin();
  
  // Initialiseer SD kaart
  if (SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialized");
  } else {
    Serial.println("SD card initialization failed");
  }
  
  // Laad configuratie
  configManager.begin();
  Serial.println("Configuration loaded");
  
  // Initialiseer displays
  displayManager.begin();
  Serial.println("Displays initialized");
  
  // Initialiseer knoppen
  buttonManager.begin();
  buttonManager.registerButtonEventCallback(onButtonEvent);
  Serial.println("Buttons initialized");
  
  // Initialiseer Axe-Fx communicatie
  axeFxManager.begin();
  axeFxManager.registerPresetChangeCallback(onPresetChange);
  axeFxManager.registerEffectBypassCallback(onEffectBypass);
  axeFxManager.registerTunerDataCallback(onTunerData);
  Serial.println("Axe-Fx communication initialized");
  
  // Toon welkomstbericht
  Serial.println("Axe-Fx Controller ready");
}

void loop() {
  // Update knoppen
  buttonManager.update();
  
  // Update Axe-Fx communicatie
  axeFxManager.update();
  
  // Update displays
  displayManager.update();
  
  // Kleine vertraging om CPU-gebruik te verminderen
  delay(10);
}
