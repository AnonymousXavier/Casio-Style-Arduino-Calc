#include "LCDManager.h"
#include "LiquidCrystal_I2C.h"

LCDManager::LCDManager() : lcd(0x27, 16, 2) {}

void LCDManager::write_to_top(const String &message) {
  // If we haven't hit the limit, just append it!
  if (message.length() <= TOP_BUFFER_SIZE) {
    String blanks;
    if (message.length() < 16) {
      for (int i = 0; i < 16 - message.length(); i++) {
        blanks += " ";
        ;
      }
    }
    topBuffer = message + blanks;
  } else {
    topBuffer = message.substring(0, TOP_BUFFER_SIZE - 1);
  }
}

void LCDManager::display_top() {
  lcd.setCursor(0, 0); // Always start at the top left

  if (topBuffer.length() > 16) {
    if (topBufferDisplayIndex > static_cast<int>(topBuffer.length()) - 16) {
      topBufferDisplayIndex = topBuffer.length() - 16;
    }

    lcd.print(
        topBuffer.substring(topBufferDisplayIndex, topBufferDisplayIndex + 16));
  } else {
    // If it's 16 characters or less, just print the whole thing
    lcd.print(topBuffer);
  }
}

void LCDManager::write_to_bottom(const String &message) {
  if (message.length() <= BOTTOM_BUFFER_SIZE) {
    bottomBuffer = message;
  }
}

void LCDManager::display_bottom() {
  lcd.setCursor(0, 1); // Move to the first column of the second row

  // Calculate how many blank spaces we need to push the text to the right
  int emptySpaces = 16 - bottomBuffer.length();

  // Print the empty spaces first
  for (int i = 0; i < emptySpaces; i++) {
    lcd.print(" ");
  }

  // Print the actual result
  lcd.print(bottomBuffer);
}

void LCDManager::clear_displays() {
  clear_buffer(topBuffer);
  clear_buffer(bottomBuffer);
  topBufferDisplayIndex = 0; // Reset scroll position!
  lcd.clear(); // Built-in command that wipes the physical screen instantly
}

// Update this to draw both rows!
void LCDManager::update_lcd_display() {
  display_top();
  display_bottom();
}

void LCDManager::clear_buffer(String &buffer) { buffer = ""; }

void LCDManager::clear_top_display() {
  lcd.setCursor(0, 0);
  lcd.print("                ");
  clear_buffer(topBuffer);
}

void LCDManager::setup_lcd() {
  pinMode(LEFT_BTN_PIN, INPUT);
  pinMode(RIGHT_BTN_PIN, INPUT);

  topBuffer.reserve(TOP_BUFFER_SIZE);
  bottomBuffer.reserve(BOTTOM_BUFFER_SIZE);

  lcd.init();
  lcd.backlight();
}

void LCDManager::handle_button_presses() {
  bool leftPressed = digitalRead(LEFT_BTN_PIN) == HIGH;
  bool rightPressed = digitalRead(RIGHT_BTN_PIN) == HIGH;

  // If ANY button is currently held down
  if (leftPressed || rightPressed) {

    // Only act if this is a fresh press
    if (either_button_released) {
      either_button_released = false; // Lock it until released

      if (leftPressed) {
        topBufferDisplayIndex--;
        if (topBufferDisplayIndex < 0)
          topBufferDisplayIndex = 0;
      } else if (rightPressed) {
        topBufferDisplayIndex++;
        // The update_lcd() function will naturally cap the maximum index
      }
    }

  } else {
    // Neither button is pressed, safe to reset the lock
    either_button_released = true;
  }
}

void LCDManager::update_lcd() {
  handle_button_presses();
  update_lcd_display();
}