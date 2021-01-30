#include "FarmGame.h"

static const uint8_t faces[60] = { 3, 15, 15, 3, 15, 15, 15, 15, 15, 1, 1, 15, 1, 15, 15, 1, 15, 15, 15, 15, 3, 15, 15, 3, 15, 15, 15, 15,
		15, 15, 15, 15, 1, 1, 1, 1, 15, 15, 15, 15, 3, 15, 15, 3, 15, 15, 15, 15, 15, 15, 15, 15, 1, 15, 15, 1, 15, 1, 1, 15 };
static const uint8_t goods[100] = { 15, 15, 15, 15, 15, 0, 0, 15, 15, 0, 0, 15, 15, 0, 0, 15, 15, 15, 15, 15, 0, 0, 15, 0, 0, 15, 15, 15,
		15, 15, 15, 15, 15, 5, 5, 15, 15, 15, 15, 15, 3, 3, 3, 3, 3, 0, 0, 3, 3, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 3, 3,
		0, 0, 3, 3, 0, 0, 3, 3, 3, 3, 3, 15, 15, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 15, 15, 0 };
static const uint8_t tierBild[100] = { 0, 0, 0, 3, 0, 0, 0, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5,
		5, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 15, 15, 15, 15, 0, 0, 15, 0, 0, 0, 0, 7, 0, 0, 7, 7, 7, 7, 7, 7,
		7, 7, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 5, 5, 0, 0, 0, 5, 0, 0 };
static const uint8_t werbung[140] = { 0, 0, 0, 0, 15, 15, 15, 15, 15, 0, 0, 15, 15, 15, 15, 15, 15, 0, 0, 15, 15, 0, 15, 0, 0, 15, 0, 15,
		15, 0, 15, 0, 0, 15, 0, 15, 15, 0, 15, 0, 15, 0, 0, 0, 15, 15, 15, 15, 0, 0, 0, 15, 15, 15, 15, 15, 15, 0, 0, 0, 0, 15, 15, 0, 15, 0,
		0, 15, 15, 0, 0, 15, 0, 15, 15, 0, 2, 2, 2, 2, 0, 15, 15, 0, 0, 15, 15, 0, 15, 15, 15, 15, 15, 2, 2, 15, 15, 15, 15, 15, 0, 15, 15, 0,
		15, 2, 2, 15, 2, 15, 15, 2, 15, 2, 2, 15, 0, 15, 15, 0, 15, 15, 15, 15, 15, 2, 2, 15, 15, 2, 2, 15, 15, 2, 2, 15, 15, 2, 15, 15 };

FarmGame::FarmGame(){
	tierPosX = (char*) malloc(MAX_TIER);
	tierPosY = (char*) malloc(MAX_TIER);
	tierType = (char*) malloc(MAX_TIER);
	if (tierPosX == NULL || tierPosY == NULL || tierType == NULL) Serial.println("malloc failed");
	for (int i = 0; i < MAX_TIER; i++)
		tierType[i] = -1;
	totalTiere = 0;
}

void FarmGame::play(){
	clear(0);

	switch (globalState) {
		case RUNNING:
			showFarm();
			break;
		case GELD:
			char text[10];
			sprintf(text, "%9d", money);
			showScroller(text, (millis() / 200) % 40, true);
			break;
		case STATUS:
			if (((millis() - statusStart) / 2000) % 2)
				switch (daytime) {
					case 0:
						drawFade(0, 1, 2, 1, 1);
						set(0, 2, 15);
						break;
					case 1:
						drawFade(1, 2, 3, 1, 1);
						set(0, 1, 15);
						break;
					case 2:
						drawFade(3, 3, 3, 1, 1);
						set(1, 0, 15);
						break;
					case 3:
						drawFade(1, 2, 3, 1, 1);
						set(2, 1, 15);
						break;
					case 4:
						drawFade(0, 1, 2, 1, 1);
						set(3, 2, 8);
						break;
					case 5:
						drawFade(0, 0, 0, 1, 1);
						break;
				}
			else
				copy(faces + 20 + happiness * 20);
			break;
		case WERBUNG:
			copy(werbung + werbungPointer * 20);
			if (millis() - werbungStart >= werbungPointer * 1000 + 1000) {
				werbungStart = millis();
				werbungPointer++;
				if (werbungPointer == 7) {
					globalState = RUNNING;
					tone(2500, 255, 0, -1);
					switch (daytime) {
						case 0:
							money += 500;
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
			if (state == FARM) {
				copy(goods + (kaufState - STALL) * 20);
				if (millis() - blinkKaufen < 300) for (char i = 0; i < 20; i++)
					getScreen()[i] += 10 - (millis() - blinkKaufen) / 30;
			}
			if (state == MARKT) {
				copy(tierBild + kaufState * 20);
				if (millis() - blinkKaufen < 300) for (char i = 0; i < 20; i++)
					getScreen()[i] += 10 - (millis() - blinkKaufen) / 30;
			}
			break;
	}

	switch (getNumberClick()) {
		case 1:
			if (globalState == KAUFEN) {
				globalState = RUNNING;
				posX = 1;
				posY = 1;
			} else {
				if (getScreen()[6] < 4) {
					posY--;
					noise(rand() % 1700 + 200, 170, 0, -4);
					logPosition();
				}
				if ((state == FARM || state == WEIDE || state == MARKT) && posX == 4 && posY == 7 && hasHaus) enter(HAUS, 5, 8);
				if ((state == FARM || state == WEIDE || state == MARKT) && posX == 9 && posY == 8) {
					totalTiere = 0;
					initTiere(cows, KUH, 1, 1, 6 + hasStall, 6 + hasStall);
					initTiere(sheep, SCHAF, 1, 1, 6 + hasStall, 6 + hasStall);
					enter(STALL, 5, 7 + hasStall);
				}
				if ((state == FARM || state == WEIDE || state == MARKT) && posX == -2 && posY == 6) {
					totalTiere = 0;
					initTiere(pigs, SCHWEIN, 1, 1, 6 + hasSchweine, 6 + hasSchweine);
					enter(SCHWEINE, 5, 7 + hasSchweine);
				}
				if ((state == FARM || state == WEIDE || state == MARKT) && posX == 9 && posY == 2) {
					totalTiere = 0;
					initTiere(chicken, HUHN, 1, 1, 6 + hasChicken, 6 + hasChicken);
					enter(CHICKEN, 5, 7 + hasChicken);
				}
				if ((state == FARM || state == WEIDE || state == MARKT) && posX == 4 && posY == 40) {	// easter egg portal
					posX = 1;
					posY = 1;
				}
				if (state == HAUS && posX >= 7 && posY == 1) {
					totalTiere = 0;
					initTiere(birds, VOGEL, 3 - hasCage, 3, 1 + hasCage, 1 + hasCage);
					state = HAUS_OG;
				}
				if (state == MARKT && posY == 13) enter(FARM, posX, posY);
			}
			break;
		case 3:
			if (globalState == KAUFEN) {
				kaufState--;
				if (state == FARM && kaufState < STALL) kaufState = SCHWEINE;
				if (state == MARKT && kaufState < KUH) kaufState = VOGEL;
			} else {
				if (getScreen()[9] < 4) {
					posX--;
					logPosition();
					noise(rand() % 1700 + 200, 170, 0, -4);
				}
				if (state == WEIDE && posX == 14) enter(FARM, posX, posY);
			}
			break;
		case 5:
			switch (globalState) {
				case KAUFEN:
					kaufState++;
					if (state == FARM && kaufState > SCHWEINE) kaufState = STALL;
					if (state == MARKT && kaufState > VOGEL) kaufState = KUH;
					break;
				case STATUS:
					globalState = WERBUNG;
					werbungStart = millis();
					werbungPointer = 0;
				case RUNNING:
					if (getScreen()[11] < 4) {
						posX++;
						logPosition();
						noise(rand() % 1700 + 200, 170, 0, -4);
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
				if (getScreen()[14] < 4) {
					posY++;
					logPosition();
					noise(rand() % 1700 + 200, 170, 0, -4);
				}
				if (state == HAUS && posX == 5 && posY == 9) enter(FARM, 4, 8);
				if (state == STALL && posX == 5 && posY == 8 + hasStall) enter(FARM, 9, 9);
				if (state == SCHWEINE && posX == 5 && posY == 8 + hasStall) enter(FARM, -2, 7);
				if (state == CHICKEN && posX == 5 && posY == 8 + hasChicken) enter(FARM, 9, 3);
				if (state == HAUS_OG && posX >= 7 && posY == 4) {
					state = HAUS;
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
					if (state == FARM) {
						globalState = KAUFEN;
						kaufState = STALL;
					}
					if (state == MARKT) {
						globalState = KAUFEN;
						kaufState = KUH;
					}
					break;
				case STATUS:
					globalState = WERBUNG;
					werbungStart = millis();
					werbungPointer = 0;
					break;
				case KAUFEN:
					if (state == FARM) {
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
					}
					if (state == MARKT) {
						switch (kaufState) {
							case KUH:
								if (money >= 20) {
									cows++;
									money -= 20;
									blinkKaufen = millis();
								}
								break;
							case SCHAF:
								if (money >= 20) {
									sheep++;
									money -= 20;
									blinkKaufen = millis();
								}
								break;
							case SCHWEIN:
								if (money >= 30) {
									pigs++;
									money -= 30;
									blinkKaufen = millis();
								}
								break;
							case HUHN:
								if (money >= 5) {
									chicken++;
									money -= 5;
									blinkKaufen = millis();
								}
								break;
							case VOGEL:
								if (money >= 10) {
									birds++;
									money -= 10;
									blinkKaufen = millis();
								}
								break;
						}
					}
			}
			break;
	}

	flipBuffer();
}

void FarmGame::showFarm(){
	switch (state) {
		case FARM:
		case WEIDE:
		case MARKT:
			// rahmen
			drawRectangle(rx(hasSchweine ? -5 : 0), ry(hasChicken ? -4 : 0), hasSchweine ? 19 : 14, hasChicken ? 17 : 13, 15);
			// durchgaenge
			set(rx(4), ry(12), 0);
			set(rx(13), ry(7), 0);
			// haus
			if (hasHaus) {
				drawBox(rx(2), ry(3), 5, 5, 10);
				drawLine(rx(3), ry(2), 1, 0, 3, 10);
				set(rx(4), ry(1), 10);
				set(rx(4), ry(7), 2);		// eingang
			}
			// stall
			drawRectangle(rx(8), ry(4), 4, 5, 11);
			set(rx(9), ry(8), 2);		// eingang
			// markt
			drawLine(rx(3), ry(12), 0, 1, 3, 15);
			drawLine(rx(5), ry(12), 0, 1, 3, 15);
			drawRectangle(rx(1), ry(14), 7, 6, 15);
			set(rx(4), ry(14), 0);
			drawBox(rx(2), ry(17), 2, 2, 2);				// staende
			drawBox(rx(5), ry(15), 2, 2, 2);
			// weide
			drawLine(rx(13), ry(6), 1, 0, 3, 15);
			drawLine(rx(13), ry(8), 1, 0, 3, 15);
			drawRectangle(rx(15), ry(4), 6, 7, 15);
			set(rx(15), ry(7), 0);
			drawBox(rx(16), ry(5), 4, 5, 3);
			// schweine
			if (hasSchweine) {
				drawRectangle(rx(-3), ry(2), 3, 5, 15);
				set(rx(-2), ry(6), 2);		// eingang
			}
			// chicken
			if (hasChicken) {
				drawRectangle(rx(8), ry(-2), 4, 5, 15);
				drawBox(rx(9), ry(-1), 2, 3, 1);
				set(rx(9), ry(2), 2);		// eingang
			}
			// easter egg
			drawBox(rx(0), ry(36), 9, 8, 1);
			set(rx(3), ry(40), 15);		// portal zum haus
			set(rx(4), ry(39), 15);
			set(rx(5), ry(40), 15);
			set(rx(4), ry(40), 3);
			if (posX < -90) {							//outer border
				drawLine(rx(-94), 0, 0, 1, 5, 1);
				drawLine(rx(-95), 0, 0, 1, 5, 2);
				drawLine(rx(-96), 0, 0, 1, 5, 3);
				drawLine(rx(-97), 0, 0, 1, 5, 4);
				drawLine(rx(-98), 0, 0, 1, 5, 5);
				drawLine(rx(-99), 0, 0, 1, 5, 6);
			}
			if (posX > 90) {
				drawLine(rx(94), 0, 0, 1, 5, 1);
				drawLine(rx(95), 0, 0, 1, 5, 2);
				drawLine(rx(96), 0, 0, 1, 5, 3);
				drawLine(rx(97), 0, 0, 1, 5, 4);
				drawLine(rx(98), 0, 0, 1, 5, 5);
				drawLine(rx(99), 0, 0, 1, 5, 6);
			}
			if (posY < -90) {
				drawLine(0, ry(-94), 1, 0, 4, 1);
				drawLine(0, ry(-95), 1, 0, 4, 2);
				drawLine(0, ry(-96), 1, 0, 4, 3);
				drawLine(0, ry(-97), 1, 0, 4, 4);
				drawLine(0, ry(-98), 1, 0, 4, 5);
				drawLine(0, ry(-99), 1, 0, 4, 6);
			}
			if (posY > 90) {
				drawLine(0, ry(94), 1, 0, 4, 1);
				drawLine(0, ry(95), 1, 0, 4, 2);
				drawLine(0, ry(96), 1, 0, 4, 3);
				drawLine(0, ry(97), 1, 0, 4, 4);
				drawLine(0, ry(98), 1, 0, 4, 5);
				drawLine(0, ry(99), 1, 0, 4, 6);
			}
			if (state == WEIDE) tierSound(WEIDE);
			break;
		case STALL:
			drawRectangle(rx(0), ry(0), 9 + hasStall, 9 + hasStall, 15);
			set(2, 2, (millis() / 100) % 3 + 1);		//player
			drawTiere();
			set(rx(5), ry(8 + hasStall), 2);
			break;
		case SCHWEINE:
			drawRectangle(rx(0), ry(0), 9 + hasSchweine, 9 + hasSchweine, 15);
			set(2, 2, (millis() / 100) % 3 + 1);		//player
			drawTiere();
			set(rx(5), ry(8 + hasSchweine), 2);
			break;
		case CHICKEN:
			drawRectangle(rx(0), ry(0), 9 + hasChicken, 9 + hasChicken, 15);
			set(2, 2, (millis() / 100) % 3 + 1);		//player
			drawTiere();
			set(rx(5), ry(8 + hasChicken), 2);
			break;
		case HAUS:
			drawRectangle(rx(0), ry(0), 10, 10, 15);
			// bett
			drawLine(rx(2), ry(2), 0, 1, 3, 15);
			drawLine(rx(2), ry(3), 1, 0, 3, 15);
			set(rx(4), ry(4), 15);
			// treppe
			drawDottedLine(rx(7), ry(1), 0, 1, 5, 2, 1);
			drawDottedLine(rx(8), ry(1), 0, 1, 5, 2, 1);
			drawLine(rx(6), ry(1), 0, 1, 5, 7);
			// ausgang
			set(rx(5), ry(9), 2);
			break;
		case HAUS_OG:
			drawRectangle(rx(0 - hasCage), ry(0), 10 + hasCage, 10 + hasCage, 15);
			// treppe
			drawDottedLine(rx(7), ry(1), 0, 1, 4, 2, 1);
			drawDottedLine(rx(8), ry(1), 0, 1, 4, 2, 1);
			drawLine(rx(6), ry(2), 0, 1, 4, 7);
			drawLine(rx(6), ry(5), 1, 0, 3, 7);
			// vogelkaefig
			if (hasCage) {
				drawRectangle(rx(2 - hasCage), ry(2), 3 + hasCage, 3 + hasCage, 2);
				set(rx(2 - hasCage), ry(5 + hasCage), 2);
				set(rx(4), ry(5 + hasCage), 2);
				drawTiere();
			}
			break;
	}
	set(2, 2, (millis() / 100) % 3 + 1);		//player
}

void FarmGame::enter(char newState, int startPosX, int startPosY){
	state = newState;
	posX = startPosX;
	posY = startPosY;
}

void FarmGame::initTiere(uint16_t count, char type, int x, int y, int dx, int dy){
	if (tierPosX == NULL) return;
	for (uint16_t i = 0; i < count; i++) {
		tierType[totalTiere + i] = type;
		tierPosX[totalTiere + i] = rand() % dx + x;
		tierPosY[totalTiere + i] = rand() % dy + y;
	}
	totalTiere += count;
}

void FarmGame::drawTiere(){
	if (tierPosX == NULL) return;
	for (uint16_t i = 0; i < totalTiere; i++) {
		if (rx(tierPosX[i] >= 0 && rx(tierPosX[i] < 4) && ry(tierPosY[i]) >= 0 && ry(tierPosY[i]) < 5)) switch (rand() % 300) {
			case 0:
				if (rx(tierPosX[i]) > 0 && getScreen()[ry(tierPosY[i] * 4 + rx(tierPosX[i]) - 1)] == 0) tierPosX[i]--;
				break;
			case 1:
				if (rx(tierPosX[i]) < 3 && getScreen()[ry(tierPosY[i] * 4 + rx(tierPosX[i]) + 1)] == 0) tierPosX[i]++;
				break;
			case 2:
				if (ry(tierPosY[i]) > 0 && getScreen()[ry(tierPosY[i] * 4 + rx(tierPosX[i]) - 4)] == 0) tierPosY[i]--;
				break;
			case 3:
				if (ry(tierPosY[i]) < 4 && getScreen()[ry(tierPosY[i] * 4 + rx(tierPosX[i]) + 4)] == 0) tierPosY[i]++;
				break;
		}
		switch (tierType[i]) {
			case KUH:
				set(rx(tierPosX[i]), ry(tierPosY[i]), 12);
				tierSound(KUH);
				break;
			case SCHAF:
				set(rx(tierPosX[i]), ry(tierPosY[i]), 2);
				tierSound(SCHAF);
				break;
			case SCHWEIN:
				set(rx(tierPosX[i]), ry(tierPosY[i]), 12);
				tierSound(SCHWEIN);
				break;
			case HUHN:
				set(rx(tierPosX[i]), ry(tierPosY[i]), 2);
				tierSound(HUHN);
				break;
			case VOGEL:
				set(rx(tierPosX[i]), ry(tierPosY[i]), 2);
				tierSound(VOGEL);
				break;
		}
	}
}

void FarmGame::logPosition(){
	Serial.print(posX);
	Serial.print("/");
	Serial.println(posY);
}

void FarmGame::tierSound(char tierType){
	if (rand() % 1400 == 0) switch (tierType) {
		case KUH:
			tone(500, 255, 10, -2);
			break;
		case SCHAF:
			noise(5000, 255, -20, -2);
			break;
		case SCHWEIN:
			tone(300, 500, 3, -1);
			break;
		case HUHN:
			tone(500, 255, 30, -10);
			break;
		case VOGEL:
			tone(500, 1000, 300, -2);
			break;
		case WEIDE:								// grillen
			tone(500, 500, 2000, -1);
			break;
	}
}
