#include "DisplayController.h"
#include "avr/pgmspace.h"
#include "font.h"

DisplayController::DisplayController() {
	uint8_t i;
	for (i = 0; i < 4; i++) {
		pinMode(ledLatchPin[i], OUTPUT);
	}
	for (i = 0; i < 5; i++) {
		pinMode(ledMatrixPin[i], OUTPUT);
	}
	for (i = 0; i < 20; i++) {
		_displayInternal[i] = 0;
		screen[i] = 0;
	}
}

void DisplayController::show() {
	uint8_t i;

	if (displayWait == 0)
		for (i = 0; i < 4; i++)
			digitalWrite(ledLatchPin[i], HIGH);

	for (i = 0; i < 5; i++) {
		digitalWrite(ledMatrixPin[i], _displayInternal[i * 4 + currentRow] > displayWait);
	}
	if (displayWait == 0)
		digitalWrite(ledLatchPin[currentRow], LOW);

	displayWait++;
	if (displayWait == 15) {
		displayWait = 0;
		currentRow++;
		if (currentRow > 3)
			currentRow = 0;
	}
}

void DisplayController::flipBuffer() {
	for (uint8_t i = 0; i < 20; i++)
		_displayInternal[i] = screen[i];
}

void DisplayController::clear(uint8_t color) {
	for (uint8_t i = 0; i < 20; i++)
		screen[i] = color;
}

void DisplayController::copy(const uint8_t *source) {
	copy((const char*) source);
}

void DisplayController::copy(const char *source) {
	for (uint8_t i = 0; i < 20; i++)
		if ((source[i] >= 0) && (source[i] < 16))
			screen[i] = source[i];
}

void DisplayController::showDigit(char row, char number) {
	bool negative = (number < 0);
	if (number < 0)
		number = -number;
	for (char i = 0; i < 4; i++) {
		screen[i + row * 4] = (number & 1) * (negative ? 2 : 15);
		number = number >> 1;
	}
}

void DisplayController::showNumber(int number) {
	for (char i = 0; i < 5; i++) {
		showDigit(4 - i, number % 10);
		number /= 10;
	}
}

void DisplayController::showScroller(const char *text, int scrollPos, bool smooth) {
	int textOffset = scrollPos / 4;
	int charOffset = smooth?3 - (scrollPos % 4):(scrollPos%4==3?-10:3);
	clear(0);
	showCharacter(text[textOffset], charOffset - 3);
	showCharacter(text[textOffset + 1], charOffset + 1);
}

void DisplayController::showCharacter(unsigned char c, int xOffset) {
	uint8_t b;
	char x, y;
	unsigned char cc = 0;
	if ((c >= 97) && (c <= 122))
		cc = c - 86;
	if ((c >= 65) && (c <= 90))
		cc = c - 54;
	if ((c >= 48) && (c <= 57))
		cc = c - 47;
	for (y = 0; y < 5; y++) {
		b = pgm_read_byte(font + cc * 5 + y);
		for (x = 3; x >= 0; x--) {
			set(x + xOffset, y, (b & 1) * 15);
			b = b >> 1;
		}
	}
}

void DisplayController::showCharacter(unsigned char c) {
	showCharacter(c, 0);
}
