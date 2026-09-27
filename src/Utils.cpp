// In a file called Utils.h
#include <Utils.h>

bool Utils::is_sign(char key) {
  char valid_signs[4] = {'*', '/', '+', '-'};
  for (const char &sign : valid_signs) {
    if (sign == key)
      return true;
  }
  return false;
}
