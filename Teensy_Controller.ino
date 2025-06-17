#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <AxeFxControl.h>

#include "config/ConfigManager.h"
#include "config/ConfigManager.cpp"

#include "input/ButtonManager.h"
#include "input/ButtonManager.cpp"

#include "display/DisplayManager.h"
#include "display/DisplayManager.cpp"

#include "axefx/AxeFxManager.h"
#include "axefx/AxeFxManager.cpp"

#include "FootController.h"
#include "FootController.cpp"

#include "display/ST7735Screen.cpp"
#include "display/ST7789Screen.cpp"

#include "FootController.h"


// Globale pointer voor AxeFxManager callbacks
AxeFxManager* g_axeFxManager = nullptr;

// Globale controller instance
FootController* controller = nullptr;

void setup() {
  // Start seriële communicatie voor debug
// Initialisatie
  Serial8.begin(31250);  // MIDI baud rate voor Axe-Fx
  
  
  // Wacht even voor stabiliteit
  delay(500);
  
  Serial.println("Axe-Fx Controller starting...");
  
  // Start SPI voor displays en SD-kaart
  SPI.begin();
  
  // Initialiseer SD-kaart
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("SD card initialization failed!");
  } else {
    Serial.println("SD card initialized.");
  }
  
  // Maak controller aan
  controller = new FootController();
  
  
  // Sla de AxeFxManager op in de globale pointer voor callbacks
  g_axeFxManager = controller->getAxeFxManager();
  
  // Initialiseer controller
  controller->begin();
  
  Serial.println("Axe-Fx Controller ready!");
}

void loop() {
  // Update controller
  if (controller) {
    controller->update();
  }
  
  // Kleine vertraging om CPU-belasting te verminderen
  delay(10);
}