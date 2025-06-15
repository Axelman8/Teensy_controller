#ifndef PIN_DEFINITIONS_H
#define PIN_DEFINITIONS_H

// SPI pins voor Teensy 4.1
#define SPI_MOSI_PIN 11
#define SPI_MISO_PIN 12
#define SPI_SCK_PIN  13

// Display pins
#define DC_PIN  9
#define RST_PIN 8

// CS pins voor de 15 ST7735 schermen
#define CS1_PIN  0
#define CS2_PIN  1
#define CS3_PIN  2
#define CS4_PIN  3
#define CS5_PIN  4
#define CS6_PIN  5
#define CS7_PIN  6
#define CS8_PIN  7
#define CS9_PIN  24
#define CS10_PIN 25
#define CS11_PIN 26
#define CS12_PIN 27
#define CS13_PIN 28
#define CS14_PIN 29
#define CS15_PIN 30

// CS pin voor het ST7789 scherm
#define CS16_PIN 31

// Button pins
#define BUTTON1_PIN  14
#define BUTTON2_PIN  15
#define BUTTON3_PIN  16
#define BUTTON4_PIN  17
#define BUTTON5_PIN  18
#define BUTTON6_PIN  19
#define BUTTON7_PIN  20
#define BUTTON8_PIN  21
#define BUTTON9_PIN  22
#define BUTTON10_PIN 23
#define BUTTON11_PIN 32
#define BUTTON12_PIN 33
#define BUTTON13_PIN 34
#define BUTTON14_PIN 35
#define BUTTON15_PIN 36
#define BUTTON16_PIN 37
#define BUTTON17_PIN 38
#define BUTTON18_PIN 39

// Expressie pedaal pins
#define EXP_PEDAL1_PIN A0
#define EXP_PEDAL2_PIN A1

// Serial pins voor AxeFX communicatie
#define AXE_SERIAL Serial1

#define SD_CS_PIN BUILTIN_SDCARD


#endif // PIN_DEFINITIONS_H
