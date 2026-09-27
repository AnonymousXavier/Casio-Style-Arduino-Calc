#pragma once
#include "Keypad.h"

class KeyPadManager {
private:
  static const byte ROWS = 4;
  static const byte COLS = 4;

  char keys[ROWS][COLS] = {{'1', '2', '3', '+'},
                           {'4', '5', '6', '-'},
                           {'7', '8', '9', '*'},
                           {'$', '0', '=', '/'}};

  byte rowPins[ROWS] = {6, 7, 8, 9};
  byte colPins[COLS] = {10, 11, 12, 13};

  Keypad calcPad;

public:
  // ONLY declarations here. No curly braces allowed!
  KeyPadManager();
  char get_pressed_key();
};