#include "DisplayController.h"
#include "avr/pgmspace.h"
#include "font.h"

int ledLatchPin[4] = { A0, A1, A2, A3 };
int ledMatrixPin[5] = { 2, 13, 4, 3, 6 };
char _displayInternal[20];
uint8_t displayWait = 1;
uint8_t currentRow = 0;
char palette[5] = { 0, 1, 3, 7, 15 };
uint8_t screen[20];

void initDisplayController(){
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

void displayShow(){
	uint8_t i;

	if (displayWait == 0) for (i = 0; i < 4; i++)
		digitalWrite(ledLatchPin[i], HIGH);

	for (i = 0; i < 5; i++) {
		digitalWrite(ledMatrixPin[i], _displayInternal[i * 4 + currentRow] > displayWait);
	}
	if (displayWait == 0) digitalWrite(ledLatchPin[currentRow], LOW);

	displayWait++;
	if (displayWait == 15) {
		displayWait = 0;
		currentRow++;
		if (currentRow > 3) currentRow = 0;
	}
}

void flipBuffer(){
	for (uint8_t i = 0; i < 20; i++)
		_displayInternal[i] = screen[i];
}

void clear(uint8_t color){
	for (uint8_t i = 0; i < 20; i++)
		screen[i] = color;
}

void copy(const uint8_t *source){
	copy((const char*) source);
}

void copy(const char *source){
	for (uint8_t i = 0; i < 20; i++)
		if ((source[i] >= 0) && (source[i] < 16)) screen[i] = source[i];
}

void showDigit(char row, char number){
	bool negative = (number < 0);
	if (number < 0) number = -number;
	for (char i = 0; i < 4; i++) {
		screen[i + row * 4] = (number & 1) * (negative ? 2 : 15);
		number = number >> 1;
	}
}

void showNumber(int number){
	for (char i = 0; i < 5; i++) {
		showDigit(4 - i, number % 10);
		number /= 10;
	}
}

void showScroller(const char *text, int scrollPos, bool smooth){
	int textOffset = scrollPos / 4;
	int charOffset = smooth ? 3 - (scrollPos % 4) : (scrollPos % 4 == 3 ? -10 : 3);
	clear(0);
	showCharacter(text[textOffset], charOffset - 3);
	showCharacter(text[textOffset + 1], charOffset + 1);
}

void showCharacter(unsigned char c, int xOffset){
	uint8_t b;
	char x, y;
	unsigned char cc = 0;
	if ((c >= 97) && (c <= 122)) cc = c - 86;
	if ((c >= 65) && (c <= 90)) cc = c - 54;
	if ((c >= 48) && (c <= 57)) cc = c - 47;
	for (y = 0; y < 5; y++) {
		b = pgm_read_byte(font + cc * 5 + y);
		for (x = 3; x >= 0; x--) {
			set(x + xOffset, y, (b & 1) * 15);
			b = b >> 1;
		}
	}
}

void showCharacter(unsigned char c){
	showCharacter(c, 0);
}

void drawLine(int x, int y, int dx, int dy, int steps, char color){
	drawDottedLine(x, y, dx, dy, steps, color, color);
}
void drawDottedLine(int x, int y, int dx, int dy, int steps, char color1, char color2){
	for (int i = 0; i < steps; i++) {
		set(x, y, i % 2 ? color1 : color2);
		x += dx;
		y += dy;
	}
}
void drawRectangle(int x, int y, int width, int height, char color){
	drawLine(x, y, 1, 0, width, color);
	drawLine(x, y + height - 1, 1, 0, width, color);
	drawLine(x, y + 1, 0, 1, height - 2, color);
	drawLine(x + width - 1, y + 1, 0, 1, height - 2, color);
}
void drawBox(int x, int y, int width, int height, char color){
	for (int yy = y; yy < y + height; yy++)
		for (int xx = x; xx < x + width; xx++)
			set(xx, yy, color);
}
void drawFade(char c0, char c1, char c2, char c3, char c4){
	for (char i = 0; i < 4; i++) {
		screen[i] = c0;
		screen[i + 4] = c1;
		screen[i + 8] = c2;
		screen[i + 12] = c3;
		screen[i + 16] = c4;
	}
}
void set(int x, int y, char color){
	if ((x >= 0) && (x < 4) && (y >= 0) && (y < 5)) screen[y * 4 + x] = color;
}
char get(int x, int y){
	return screen[y * 4 + x];
}
char*getPalette(){return palette;}
uint8_t*getScreen(){return screen;}
