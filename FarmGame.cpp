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
			if (((millis() - statusStart) / 2000) % 2)
				switch (daytime) {
					case 0:
						dc->drawFade(0, 1, 2, 1, 1);
						dc->set(0, 2, 15);
						break;
					case 1:
						dc->drawFade(1, 2, 3, 1, 1);
						dc->set(0, 1, 15);
						break;
					case 2:
						dc->drawFade(3, 3, 3, 1, 1);
						dc->set(1, 0, 15);
						break;
					case 3:
						dc->drawFade(1, 2, 3, 1, 1);
						dc->set(2, 1, 15);
						break;
					case 4:
						dc->drawFade(0, 1, 2, 1, 1);
						dc->set(3, 2, 8);
						break;
					case 5:
						dc->drawFade(0, 0, 0, 1, 1);
						break;
				}
			else
				dc->copy(faces + 20 + happiness * 20);
			break;
		case KAUFEN:
			dc->copy(goods + (kaufState - STALL) * 20);
			if (millis() - blinkKaufen < 300) for (char i = 0; i < 20; i++)
				dc->screen[i] += 10 - (millis() - blinkKaufen) / 30;
			break;
	}

	switch (kc->getNumberClick()) {
		case 1:
			if (globalState == KAUFEN) {
				globalState = RUNNING;
				posX = 1;
				posY = 1;
			} else {
				if (dc->screen[6] < 4) {
					posY--;
					sc->noise(rand() % 1700 + 200, 170, 0, -4);
				}
				if ((state == FARM || state == WEIDE || state == MARKT) && posX == 4 && posY == 7 && hasHaus) enter(HAUS, 5, 9);
				if ((state == FARM || state == WEIDE || state == MARKT) && posX == 9 && posY == 8) enter(STALL, 5, 9);
				if (state == HAUS && posX >= 7 && posY == 1) {
					state = HAUS_OG;
//				sc->tone(300, 350, 3, -1);
				}
				if (state == MARKT && posY == 13) enter(FARM, posX, posY);
			}
			break;
		case 3:
			if (globalState == KAUFEN) {
				kaufState--;
				if (kaufState < STALL) kaufState = SCHWEINE;
			} else {
				if (dc->screen[9] < 4) {
					posX--;
					sc->noise(rand() % 1700 + 200, 170, 0, -4);
				}
				if (state == WEIDE && posX == 14) enter(FARM, posX, posY);
			}
			break;
		case 5:
			if (globalState == KAUFEN) {
				kaufState++;
				if (kaufState > SCHWEINE) kaufState = STALL;
			} else {
				if (dc->screen[11] < 4) {
					posX++;
					sc->noise(rand() % 1700 + 200, 170, 0, -4);
				}
				if (state == FARM && posX == 14) enter(WEIDE, posX, posY);
			}
			break;
		case 7:
			if (globalState == KAUFEN) {
				globalState = RUNNING;
				posX = 1;
				posY = 1;
			} else {
				if (dc->screen[14] < 4) {
					posY++;
					sc->noise(rand() % 1700 + 200, 170, 0, -4);
				}
				if (state == HAUS && posX == 5 && posY == 9) enter(FARM, 4, 7);
				if (state == STALL && posX == 5 && posY == 8 + hasStall) enter(FARM, 9, 8);
				if (state == HAUS_OG && posX >= 7 && posY == 4) {
					state = HAUS;
//				sc->tone(2500, 350, -8, -1);
				}
				if (state == FARM && posY == 13) enter(MARKT, posX, posY);
			}
			break;
		case 6:
			if (globalState != STATUS) {
				globalState = STATUS;
				statusStart = millis();
			} else
				globalState = RUNNING;
			break;
		case 8:
			if (globalState != GELD)
				globalState = GELD;
			else
				globalState = RUNNING;
			break;
		case 4:
			if (globalState != KAUFEN)
				globalState = KAUFEN;
			else {
				switch (kaufState) {
					case STALL:
						if (money >= 500) {
							hasStall++;
							money -= 500;
							blinkKaufen = millis();
						}
						break;
					case HAUS:
						if (money >= 500) {
							hasHaus++;
							money -= 500;
							blinkKaufen = millis();
						}
						break;
					case CAGE:
						if (money >= 100) {
							hasCage++;
							money -= 100;
							blinkKaufen = millis();
						}
						break;
					case CHICKEN:
						if (money >= 50) {
							hasChicken++;
							money -= 50;
							blinkKaufen = millis();
						}
						break;
					case SCHWEINE:
						if (money >= 200) {
							hasSchweine++;
							money -= 200;
							blinkKaufen = millis();
						}
						break;
				}
				// kaufen vom ausgewaehlten
			}
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
			dc->drawRectangle(rx(hasSchweine ? -5 : 0), ry(hasChicken ? -4 : 0), hasSchweine ? 19 : 14, hasChicken ? 17 : 13, 15);
			// durchgaenge
			dc->set(rx(4), ry(12), 0);
			dc->set(rx(13), ry(7), 0);
			// haus
			if (hasHaus) {
				dc->drawBox(rx(2), ry(3), 5, 5, 10);
				dc->drawLine(rx(3), ry(2), 1, 0, 3, 10);
				dc->set(rx(4), ry(1), 10);
				dc->set(rx(4), ry(7), 2);		// eingang
			}
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
			dc->drawBox(rx(16), ry(5), 4, 5, 3);
			// schweine
			if (hasSchweine) {
				dc->drawRectangle(rx(-3), ry(2), 3, 5, 15);
				dc->set(rx(-2), ry(6), 2);		// eingang
			}
			// chicken
			if (hasChicken) {
				dc->drawRectangle(rx(8), ry(-2), 4, 5, 15);
				dc->drawBox(rx(9), ry(-1), 2, 3, 1);
				dc->set(rx(9), ry(2), 2);		// eingang
			}
			break;
		case STALL:
			dc->drawRectangle(rx(0), ry(0), 9 + hasStall, 9 + hasStall, 15);
			dc->drawDottedLine(rx(2), ry(2), 0, 1, 6 + hasStall, 1, 3);
			dc->drawDottedLine(rx(2), ry(2), 1, 0, 5 + hasStall, 1, 3);
			dc->drawDottedLine(rx(6 + hasStall), ry(2), 0, 1, 6 + hasStall, 3, 1);
			dc->set(rx(5), ry(8 + hasStall), 2);
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
			dc->drawRectangle(rx(0 - hasCage), ry(0), 10 + hasCage, 10 + hasCage, 15);
			// treppe
			dc->drawDottedLine(rx(7), ry(1), 0, 1, 4, 2, 1);
			dc->drawDottedLine(rx(8), ry(1), 0, 1, 4, 2, 1);
			dc->drawLine(rx(6), ry(2), 0, 1, 4, 7);
			dc->drawLine(rx(6), ry(5), 1, 0, 3, 7);
			// vogelkaefig
			if (hasCage) {
				dc->drawRectangle(rx(2 - hasCage), ry(2), 3 + hasCage, 3 + hasCage, 2);
				dc->set(rx(2 - hasCage), ry(5 + hasCage), 2);
				dc->set(rx(4), ry(5 + hasCage), 2);
			}
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
//	sc->noise(10000, 600, 30, -4);
}
