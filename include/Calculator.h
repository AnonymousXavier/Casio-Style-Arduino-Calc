#include "KeypadManager.h"
#include "LCDManager.h"

class Calculator {
private:
  String number1Buffer;
  String number2Buffer;

  bool number1Defined = false;
  bool number2Started = false;
  char sign;
  KeyPadManager keyPad = KeyPadManager();
  LCDManager display = LCDManager();

  float result = 0.0;

  void handle_keypresses();

  void update_display();

  void update_char_buffers(char key);

  void compute();

  void update_display_after_computing();

public:
  void setup();

  void update();
};