#include "KeypadManager.h"
#include "Keypad.h"

#include <Arduino.h>

KeyPadManager::KeyPadManager() : calcPad(makeKeymap(keys), rowPins, colPins, ROWS, COLS) {}

char KeyPadManager::get_pressed_key() {
  char key = calcPad.getKey();
  if (key) {
    return key;
  }
  return '\0';
}