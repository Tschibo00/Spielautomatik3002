#include "FarmGame.h"

FarmGame::FarmGame(DisplayController *dc, KeyboardController *kc, SoundController *sc){
	this->dc = dc;
	this->kc = kc;
	this->sc = sc;
	deinitTiere();
}

void FarmGame::play(){
/*
	if (tierPosX == NULL) {
		Serial.println(freeMemory());
		Serial.println(sizeof(int));
		Serial.println(sizeof(char));
		tierPosX = (char*) malloc(MAX_TIER);
		tierPosY = (char*) malloc(MAX_TIER);
		tierType = (char*) malloc(MAX_TIER);
		if (tierPosX==NULL)Serial.println("malloc failed");
	}
*/

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
		case WERBUNG:
			dc->copy(werbung + werbungPointer * 20);
			if (millis() - werbungStart >= werbungPointer * 1000 + 1000) {
				werbungStart = millis();
				werbungPointer++;
				if (werbungPointer == 7) {
					globalState = RUNNING;
					sc->tone(2500, 255, 0, -1);
					switch (daytime) {
						case 0:
							money = +500;
							break;
						case 1:
							// 20 kuehe oder schweine
							break;
						case 2:
							money += 3000;
							break;
						case 3:
							// 20 schweine
							break;
						case 4:
							money += 700;
							break;
					}
				}
			}
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
				if ((state == FARM || state == WEIDE || state == MARKT) && posX == 4 && posY == 7 && hasHaus) enter(HAUS, 5, 8);
				if ((state == FARM || state == WEIDE || state == MARKT) && posX == 9 && posY == 8) {
					initTiere(cows, KUH, 1, 1, 8 + hasStall, 8 + hasStall);
					initTiere(sheep, SCHAF, 1, 1, 8 + hasStall, 8 + hasStall);
					enter(STALL, 5, 8);
				}
				if ((state == FARM || state == WEIDE || state == MARKT) && posX == 4 && posY == 40) {	// easter egg portal
					posX = 1;
					posY = 1;
				}
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
			switch (globalState) {
				case KAUFEN:
					kaufState++;
					if (kaufState > SCHWEINE) kaufState = STALL;
					break;
				case STATUS:
					globalState = WERBUNG;
					werbungStart = millis();
					werbungPointer = 0;
				case RUNNING:
					if (dc->screen[11] < 4) {
						posX++;
						sc->noise(rand() % 1700 + 200, 170, 0, -4);
					}
					if (state == FARM && posX == 14) enter(WEIDE, posX, posY);
					break;
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
				if (state == HAUS && posX == 5 && posY == 9) enter(FARM, 4, 8);
				if (state == STALL && posX == 5 && posY == 8 + hasStall) enter(FARM, 9, 9);
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
			switch (globalState) {
				case RUNNING:
					globalState = KAUFEN;
					break;
				case STATUS:
					globalState = WERBUNG;
					werbungStart = millis();
					werbungPointer = 0;
					break;
				case KAUFEN:
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
			// easter egg
			dc->drawBox(rx(0), ry(36), 9, 8, 1);
			dc->set(rx(3), ry(40), 15);		// portal zum haus
			dc->set(rx(4), ry(39), 15);
			dc->set(rx(5), ry(40), 15);
			dc->set(rx(4), ry(40), 3);
			if (posX < -90) {							//outer border
				dc->drawLine(rx(-94), 0, 0, 1, 5, 1);
				dc->drawLine(rx(-95), 0, 0, 1, 5, 2);
				dc->drawLine(rx(-96), 0, 0, 1, 5, 3);
				dc->drawLine(rx(-97), 0, 0, 1, 5, 4);
				dc->drawLine(rx(-98), 0, 0, 1, 5, 5);
				dc->drawLine(rx(-99), 0, 0, 1, 5, 6);
			}
			if (posX > 90) {
				dc->drawLine(rx(94), 0, 0, 1, 5, 1);
				dc->drawLine(rx(95), 0, 0, 1, 5, 2);
				dc->drawLine(rx(96), 0, 0, 1, 5, 3);
				dc->drawLine(rx(97), 0, 0, 1, 5, 4);
				dc->drawLine(rx(98), 0, 0, 1, 5, 5);
				dc->drawLine(rx(99), 0, 0, 1, 5, 6);
			}
			if (posY < -90) {
				dc->drawLine(0, ry(-94), 1, 0, 4, 1);
				dc->drawLine(0, ry(-95), 1, 0, 4, 2);
				dc->drawLine(0, ry(-96), 1, 0, 4, 3);
				dc->drawLine(0, ry(-97), 1, 0, 4, 4);
				dc->drawLine(0, ry(-98), 1, 0, 4, 5);
				dc->drawLine(0, ry(-99), 1, 0, 4, 6);
			}
			if (posY > 90) {
				dc->drawLine(0, ry(94), 1, 0, 4, 1);
				dc->drawLine(0, ry(95), 1, 0, 4, 2);
				dc->drawLine(0, ry(96), 1, 0, 4, 3);
				dc->drawLine(0, ry(97), 1, 0, 4, 4);
				dc->drawLine(0, ry(98), 1, 0, 4, 5);
				dc->drawLine(0, ry(99), 1, 0, 4, 6);
			}
			break;
		case STALL:
			dc->drawRectangle(rx(0), ry(0), 9 + hasStall, 9 + hasStall, 15);
			dc->drawDottedLine(rx(3), ry(3), 0, 1, 5 + hasStall, 1, 3);
			dc->drawDottedLine(rx(3), ry(3), 1, 0, 2 + hasStall, 1, 3);
			dc->drawDottedLine(rx(5 + hasStall), ry(3), 0, 1, 5 + hasStall, 3, 1);
			drawTiere();
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

void FarmGame::initTiere(uint16_t count, char type, int x, int y, int dx, int dy){
	if (tierPosX == NULL) return;
	for (uint16_t i = 0; i < count; i++) {
		tierType[totalTiere + i] = type;
		tierPosX[totalTiere + i] = rand() % dx + x;
		tierPosY[totalTiere + i] = rand() % dy + y;
		Serial.print(tierType[totalTiere+i]);
		Serial.print(" ");
		Serial.print(tierPosX[totalTiere+i]);
		Serial.print(" ");
		Serial.print(tierPosY[totalTiere+i]);
		Serial.println();
	}
	totalTiere += count;
}

void FarmGame::drawTiere(){
	if (tierPosX == NULL) return;
	for (uint16_t i = 0; i < totalTiere; i++) {
		switch (tierType[i]) {
			case KUH:
				dc->set(rx(tierPosX[i]), ry(tierPosY[i]), 1);
				dc->set(rx(tierPosX[i]), ry(tierPosY[i] + 1), 1);
				break;
			case SCHAF:
				dc->set(rx(tierPosX[i]), ry(tierPosY[i]), 12);
				dc->set(rx(tierPosX[i]), ry(tierPosY[i] + 1), 12);
				break;
			case SCHWEIN:
				dc->set(rx(tierPosX[i]), ry(tierPosY[i]), 1);
				dc->set(rx(tierPosX[i]), ry(tierPosY[i] + 1), 1);
				dc->set(rx(tierPosX[i] + 1), ry(tierPosY[i]), 1);
				break;
			case HUHN:
				dc->set(rx(tierPosX[i]), ry(tierPosY[i]), 12);
				break;
			case VOGEL:
				dc->set(rx(tierPosX[i]), ry(tierPosY[i]), 1);
				break;
		}
	}
}

void FarmGame::deinitTiere(){
	if (tierPosX == NULL) return;
	for (int i = 0; i < MAX_TIER; i++)
		tierType[i] = -1;
	totalTiere = 0;
}
