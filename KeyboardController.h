#ifndef KEYBOARD_CONTROLLER_H
#define KEYBOARD_CONTROLLER_H

#include <Arduino.h>

void initKeyboardController();
void scanKeyboard();
bool getKeyStatus(uint8_t key);
bool getKeyClick(uint8_t key);
char getNumberStatus();
char getNumberClick();
bool anyKeyClick(uint8_t first, uint8_t last);

#endif
