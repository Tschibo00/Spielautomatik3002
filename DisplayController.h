#ifndef DISPLAY_CONTROLLER
#define DISPLAY_CONTROLLER

#include <Arduino.h>

void initDisplayController();
void displayShow();
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
void set(int x, int y, char color);
char get(int x, int y);
void strike();
char* getPalette();
uint8_t* getScreen();

#endif
