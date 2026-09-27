#pragma once
#include "Arduino.h"
#include "LiquidCrystal_I2C.h"

class LCDManager {
private:
  LiquidCrystal_I2C lcd;

  const int RIGHT_BTN_PIN = 4;
  const int LEFT_BTN_PIN = 3;

  const byte TOP_BUFFER_SIZE = 125;
  const byte BOTTOM_BUFFER_SIZE = 16;

  String topBuffer = "";
  String bottomBuffer = "";

  int topBufferWriteIndex = 0;
  int topBufferDisplayIndex = 0;
  int either_button_released = true;

public:
  LCDManager();

  void write_to_top(const String &message);

  void display_top();

  void clear_buffer(String &buffer);

  void clear_top_display();

  void setup_lcd();

  void handle_button_presses();

  void update_lcd_display();

  void update_lcd();

  // Add these under your public methods
  void write_to_bottom(const String &message);
  void display_bottom();
  void clear_displays();
};

