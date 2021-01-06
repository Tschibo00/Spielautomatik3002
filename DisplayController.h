#ifndef DISPLAY_CONTROLLER
#define DISPLAY_CONTROLLER

#include <Arduino.h>

class DisplayController {
private:
	int ledLatchPin[4] = { A0, A1, A2, A3 };
	int ledMatrixPin[5] = { 2, 13, 4, 5, 6 };
	char _displayInternal[20];
	uint8_t displayWait = 1;
	uint8_t currentRow = 0;

public:
	// 15, 7, 3, 1 as available brightness, row-wise
	char palette[5]={0,1,3,7,15};
	uint8_t screen[20];
	DisplayController();
	void show();
	void flipBuffer();
	void clear(uint8_t color);
	void copy(const uint8_t *source);
	void copy(const char *source);
	void showDigit(char row,char number);
	void showNumber(int number);
};
#endif
