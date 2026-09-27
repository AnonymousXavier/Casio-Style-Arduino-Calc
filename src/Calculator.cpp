#include "Calculator.h"
#include "HardwareSerial.h"
#include "Utils.h"
#include "WCharacter.h"
#include "WString.h"

void Calculator::setup() {
  display.setup_lcd();
  Serial.begin(9600);
}

void Calculator::update_display_after_computing() {
  // 1. We leave the top row alone! It already says "12+5".

  // 2. Build the Casio-style bottom row: "=               17"
  String resultStr = String(result);
  String bottomRow = "=";

  // Calculate exactly how many spaces go between '=' and the result
  // 16 total columns, minus 1 for the '=', minus the length of the result.
  int spacesNeeded = 16 - 1 - resultStr.length();

  for (int i = 0; i < spacesNeeded; i++) {
    bottomRow += " ";
  }
  bottomRow += resultStr;

  // 3. Send it to the screen
  display.write_to_bottom(bottomRow);

  // 4. Prep the memory so if they press '+' next, it starts at 17
  number1Buffer = resultStr;
}

void Calculator::compute() {
  // Convert strings directly to floats
  float number1 = number1Buffer.toFloat();
  float number2 = number2Buffer.toFloat();

  switch (sign) {
  case '+':
    result = number1 + number2;
    break;
  case '-':
    result = number1 - number2;
    break;
  case '/':
    // Prevent the Arduino from crashing if divided by zero
    if (number2 == 0.0) {
      result = 0;
    } else {
      result = number1 / number2;
    }
    break;
  case '*':
    result = number1 * number2;
    break;
  }

  sign = '\0';
  number2Buffer = "";
}

void Calculator::update_char_buffers(char key) {
  // 1. Accept digits OR the decimal point
  if (isDigit(key) || key == '.') {

    if (number1Defined && sign == '\0') {
      number1Buffer = "";
      number1Defined = false;
      display.clear_displays();
    }

    if (!number1Defined) {
      // 2. Reject the key if it's a decimal and one already exists
      if (key == '.' && number1Buffer.indexOf('.') != -1) {
        return; // Break out of the function entirely
      }
      number1Buffer += key;
    } else {
      // 3. Do the same protection for number 2
      if (key == '.' && number2Buffer.indexOf('.') != -1) {
        return;
      }
      number2Buffer += key;
      number2Started = true;
    }

    Calculator::update_display();

  } else if (Utils::is_sign(key)) {

    if (!number1Defined) {
      sign = key;
      number1Defined = true;
      Calculator::update_display();
    } else {
      // THE "CHANGED MY MIND" FIX: Only compute if they actually started typing
      // number 2
      if (sign != '\0' && number2Started) {
        Calculator::compute();
        Calculator::update_display_after_computing();
      }

      sign = key;
      number2Started = false; // Reset this for the next number
      Calculator::update_display();
    }

  } else if (key == '=') {
    // Only compute if an equation actually exists
    if (number1Defined && sign != '\0' && number2Started) {
      Calculator::compute();
      Calculator::update_display_after_computing();
      number2Started = false;
    }
  }
}

void Calculator::handle_keypresses() {
  char key = keyPad.get_pressed_key();
  if (key) {
    Calculator::update_char_buffers(key);
  }
}

void Calculator::update_display() {
  // When actively typing, only show the equation on top, keep bottom clear
  String message = number1Buffer + static_cast<String>(sign) + number2Buffer;
  display.write_to_top(message);
  display.write_to_bottom(""); // Keep bottom blank while typing

  Serial.println(message);
}

void Calculator::update() {
  display.update_lcd();
  Calculator::handle_keypresses();
}