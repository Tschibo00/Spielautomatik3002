#include "FarmGame.h"

FarmGame::FarmGame(DisplayController *dc, KeyboardController *kc, SoundController *sc){
	this->dc = dc;
	this->kc = kc;
	this->sc = sc;
}

void FarmGame::play(){

	dc->clear(0);

	switch (globalState) {
		case RUNNING:
			showFarm();
			break;
		case GELD:
			char text[10];
			sprintf(text, "%9d", money);
			dc->showScroller(text, (millis() / 200) % 40, true);
			break;
		case STATUS:
			break;
	}

	switch (kc->getNumberClick()) {
		case 1:
			if (dc->screen[6] < 3) posY--;
			if ((state == FARM || state == WEIDE || state == MARKT) && posX == 4 && posY == 7) enter(HAUS, 5, 9);
			if ((state == FARM || state == WEIDE || state == MARKT) && posX == 9 && posY == 8) enter(STALL, 5, 9);
			if (state == HAUS && posX >= 7 && posY == 1) {
				state = HAUS_OG;
				sc->tone(300, 350, 3, -1);
			}
			if (state == MARKT && posY == 13) enter(FARM, posX, posY);
			break;
		case 3:
			if (dc->screen[9] < 3) posX--;
			if (state == WEIDE && posX == 14) enter(FARM, posX, posY);
			break;
		case 5:
			if (dc->screen[11] < 3) posX++;
			if (state == FARM && posX == 14) enter(WEIDE, posX, posY);
			break;
		case 7:
			if (dc->screen[14] < 3) posY++;
			if (state == HAUS && posX == 5 && posY == 9) enter(FARM, 4, 7);
			if (state == STALL && posX == 5 && posY == 9) enter(FARM, 9, 8);
			if (state == HAUS_OG && posX >= 7 && posY == 4) {
				state = HAUS;
				sc->tone(2500, 350, -8, -1);
			}
			if (state == FARM && posY == 13) enter(MARKT, posX, posY);
			break;
		case 6:
			if (globalState != STATUS)
				globalState = STATUS;
			else
				globalState = RUNNING;
			break;
		case 8:
			if (globalState != GELD)
				globalState = GELD;
			else
				globalState = RUNNING;
			break;
	}

	dc->flipBuffer();
}

void FarmGame::showFarm(){
	switch (state) {
		case FARM:
		case WEIDE:
		case MARKT:
			// rahmen
			dc->drawRectangle(rx(0), ry(0), 14, 13, 15);
			// durchgaenge
			dc->set(rx(4), ry(12), 0);
			dc->set(rx(13), ry(7), 0);
			// haus
			dc->drawBox(rx(2), ry(3), 5, 5, 10);
			dc->drawLine(rx(3), ry(2), 1, 0, 3, 10);
			dc->set(rx(4), ry(1), 10);
			dc->set(rx(4), ry(7), 2);		// eingang
			// stall
			dc->drawRectangle(rx(8), ry(4), 4, 5, 11);
			dc->set(rx(9), ry(8), 2);		// eingang
			// markt
			dc->drawLine(rx(3), ry(12), 0, 1, 3, 15);
			dc->drawLine(rx(5), ry(12), 0, 1, 3, 15);
			dc->drawRectangle(rx(1), ry(14), 7, 6, 15);
			dc->set(rx(4), ry(14), 0);
			dc->drawBox(rx(2), ry(17), 2, 2, 2);				// staende
			dc->drawBox(rx(5), ry(15), 2, 2, 2);
			// weide
			dc->drawLine(rx(13), ry(6), 1, 0, 3, 15);
			dc->drawLine(rx(13), ry(8), 1, 0, 3, 15);
			dc->drawRectangle(rx(15), ry(4), 6, 7, 15);
			dc->set(rx(15), ry(7), 0);
			dc->drawBox(rx(16), ry(5), 4, 5, 1);
			break;
		case STALL:
			dc->drawRectangle(rx(0), ry(0), 10, 10, 15);
			dc->drawDottedLine(rx(2), ry(2), 0, 1, 7, 1, 2);
			dc->drawDottedLine(rx(2), ry(2), 1, 0, 6, 1, 2);
			dc->drawDottedLine(rx(7), ry(2), 0, 1, 7, 2, 1);
			dc->set(rx(5), ry(9), 2);
			break;
		case HAUS:
			dc->drawRectangle(rx(0), ry(0), 10, 10, 15);
			// bett
			dc->drawLine(rx(2), ry(2), 0, 1, 3, 15);
			dc->drawLine(rx(2), ry(3), 1, 0, 3, 15);
			dc->set(rx(4), ry(4), 15);
			// treppe
			dc->drawDottedLine(rx(7), ry(1), 0, 1, 5, 2, 1);
			dc->drawDottedLine(rx(8), ry(1), 0, 1, 5, 2, 1);
			dc->drawLine(rx(6), ry(1), 0, 1, 5, 7);
			// ausgang
			dc->set(rx(5), ry(9), 2);
			break;
		case HAUS_OG:
			dc->drawRectangle(rx(0), ry(0), 10, 10, 15);
			// treppe
			dc->drawDottedLine(rx(7), ry(1), 0, 1, 4, 2, 1);
			dc->drawDottedLine(rx(8), ry(1), 0, 1, 4, 2, 1);
			dc->drawLine(rx(6), ry(2), 0, 1, 4, 7);
			dc->drawLine(rx(6), ry(5), 1, 0, 3, 7);
			// vogelkaefig

			break;
		case CAGE:
			break;
		case CHICKEN:
			break;
	}
	dc->set(2, 2, (millis() / 100) % 3 + 1);		//player
}

void FarmGame::enter(char newState, int startPosX, int startPosY){
	state = newState;
	posX = startPosX;
	posY = startPosY;
	sc->noise(10000, 600, 30, -4);
}
