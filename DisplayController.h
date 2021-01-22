#ifndef DISPLAY_CONTROLLER
#define DISPLAY_CONTROLLER

#include <Arduino.h>

class DisplayController {
	private:
		int ledLatchPin[4] = { A0, A1, A2, A3 };
		int ledMatrixPin[5] = { 2, 13, 4, 3, 6 };
		char _displayInternal[20];
		uint8_t displayWait = 1;
		uint8_t currentRow = 0;

	public:
		// 15, 7, 3, 1 as available brightness, row-wise
		char palette[5] = { 0, 1, 3, 7, 15 };
		uint8_t screen[20];
		DisplayController();
		void show();
		void flipBuffer();
		void clear(uint8_t color);
		void copy(const uint8_t *source);
		void copy(const char *source);
		void showDigit(char row, char number);
		void showNumber(int number);
		void showCharacter(unsigned char c);
		void showCharacter(unsigned char c, int xOffset);
		void showScroller(const char *text, int scrollPos, bool smooth);
		void drawLine(int x, int y, int dx, int dy, int steps, char color);
		void drawDottedLine(int x, int y, int dx, int dy, int steps, char color1, char color2);
		void drawRectangle(int x, int y, int width, int height, char color);
		void drawBox(int x, int y, int width, int height, char color);
		void drawFade(char c0, char c1, char c2, char c3, char c4);

		void set(int x, int y, char color){
			if ((x >= 0) && (x < 4) && (y >= 0) && (y < 5)) screen[y * 4 + x] = color;
		}
		char get(int x, int y){
			return screen[y * 4 + x];
		}
};
#endif
