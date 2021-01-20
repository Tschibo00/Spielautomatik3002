#include "FarmGame.h"

FarmGame::FarmGame(DisplayController *dc, KeyboardController *kc, SoundController *sc){
	this->dc = dc;
	this->kc = kc;
	this->sc = sc;
}

void FarmGame::play(){

	dc->clear(0);

	switch (kc->getNumberClick()) {
		case 1:
			posY--;
			break;
		case 3:
			posX--;
			break;
		case 5:
			posX++;
			break;
		case 7:
			posY++;
			break;
	}

	show();

	dc->flipBuffer();
}

void FarmGame::show(){
	switch (state) {
		case FARM:
			// rahmen
			drawLine(0 - posX, 0 - posY, 0, 1, 13, 15);
			drawLine(0 - posX, 12 - posY, 1, 0, 13, 15);
			drawLine(12 - posX, 0 - posY, 0, 1, 13, 15);
			drawLine(0 - posX, 0 - posY, 1, 0, 13, 15);
			// durchgaenge
			dc->set(4-posX, 12-posY, 0);
			dc->set(12-posX, 7-posY, 0);
			// haus
			drawBox(2 - posX, 3 - posY, 5, 5, 10);
			dc->set(3 - posX, 2 - posY, 10);
			dc->set(4 - posX, 1 - posY, 10);
			dc->set(4 - posX, 2 - posY, 10);
			dc->set(5 - posX, 2 - posY, 10);
			// stall
			drawLine(8 - posX, 4 - posY, 0, 1, 5, 11);
			drawLine(8 - posX, 4 - posY, 1, 0, 4, 11);
			drawLine(11 - posX, 4 - posY, 0, 1, 5, 11);
			drawLine(8 - posX, 8 - posY, 1, 0, 4, 11);
			break;
		case STALL:
			break;
		case WEIDE:
			break;
		case MARKT:
			break;
		case HAUS:
			break;
		case CAGE:
			break;
		case CHICKEN:
			break;
	}
}

void FarmGame::drawLine(int x, int y, int dx, int dy, int steps, char color){
	for (int i = 0; i < steps; i++) {
		dc->set(x, y, color);
		x += dx;
		y += dy;
	}
}

void FarmGame::drawBox(int x, int y, int width, int height, char color){
	for (int yy = y; yy < y + height; yy++)
		for (int xx = x; xx < x + width; xx++)
			dc->set(xx, yy, color);
}
