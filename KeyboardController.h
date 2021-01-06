#ifndef KEYBOARD_CONTROLLER
#define KEYBOARD_CONTROLLER

#include <Arduino.h>

class KeyboardController {
private:
	int keyLatchPin[3] = { 9, 8, 7 };
	int keyMatrixPin[4] = { 10, 12, A4, A5 };
	bool keyStatus[12];
	bool keyLocked[12];

public:
	KeyboardController();
	void scanKeyboard();
	bool getKeyStatus(uint8_t key);
	bool getKeyClick(uint8_t key);
	char getNumberStatus();
	char getNumberClick();
	bool anyKeyClick(uint8_t first, uint8_t last);
};
#endif
