#include "Arduino.h"
#include "Calculator.h"

Calculator calc;

void setup() { calc.setup(); }

void loop() { calc.update(); }