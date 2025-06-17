#include "../display/Screen.h"

Screen::Screen(uint8_t csPin, uint8_t dcPin, uint8_t rstPin) :
  _csPin(csPin), _dcPin(dcPin), _rstPin(rstPin) 
  {
}

void Screen::select() {
  digitalWrite(_csPin, LOW);
}

void Screen::deselect() {
  digitalWrite(_csPin, HIGH);
}

