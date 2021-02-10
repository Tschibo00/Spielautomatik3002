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
	if (millis() > stopTimeDisplay) {
		clear(0);

		switch (globalState) {
			case RUNNING:
				showFarm();
				break;
			case GELD:
				char text[10];
				sprintf(text, "%-9ld", money);
				showScroller(text, ((millis() - statusStart) / 200) % 40, true);
				break;
			case STATUS:
				if (((millis() - statusStart) / 2000) % 2)
					drawDaytime();
				else
					copy(faces + 20 + happiness * 20);
				break;
			case WERBUNG:
				copy(werbung + werbungPointer * 20);
				if (millis() - werbungStart >= werbungPointer * 1000 + 1000) {
					werbungStart = millis();
					werbungPointer++;
					if (werbungPointer == 7) {
						enter(RUNNING, state, posX, posY);
						tone(2500, 255, 0, -1);
						switch (daytime) {
							case 0:
								money += 500;
								break;
							case 1:
								if (hasSchweine)
									pigs += 20;
								else
									cows += 20;
								break;
							case 2:
								money += 3000;
								break;
							case 3:
								if (hasSchweine) pigs += 20;
								break;
							case 4:
								money += 700;
								break;
						}
					}
				}
				break;
			case KAUFEN:
			case VERKAUFEN:
				if (state == FARM) {
					copy(goods + (kaufState - STALL) * 20);
					if (millis() - blinkKaufen < 300) for (char i = 0; i < 20; i++)
						getScreen()[i] += 10 - (millis() - blinkKaufen) / 30;
				}
				if (onMarktFutter() || onMarktTiere()) {
					copy(tierBild + kaufState * 20);
					if (millis() - blinkKaufen < 300) for (char i = 0; i < 20; i++)
						getScreen()[i] += 10 - (millis() - blinkKaufen) / 30;
				}
				break;
		}
	}

	switch (getNumberClick()) {
		case 1:
			if (globalState == KAUFEN || globalState == VERKAUFEN) {
				if (state == FARM)
					enter(RUNNING, state, 1, 1);
				else
					enter(RUNNING, state, posX, posY);
			} else {
				if (getScreen()[6] < 4) {
					posY--;
					noise(random(1700) + 200, 170, 0, -4);
					logPosition();
				}
				if ((state == FARM) && posX == 4 && posY == 7 && hasHaus) enter(globalState, HAUS, 5, 8);
				if ((state == FARM) && posX == 9 && posY == 8) {
					totalTiere = 0;
					initTiere(cows, KUH, 1, 1, 6 + hasStall, 6 + hasStall);
					initTiere(sheep, SCHAF, 1, 1, 6 + hasStall, 6 + hasStall);
					enter(globalState, STALL, 5, 7 + hasStall);
				}
				if ((state == FARM) && posX == -2 && posY == 6) {
					totalTiere = 0;
					initTiere(pigs, SCHWEIN, 1, 1, 6 + hasSchweine, 6 + hasSchweine);
					enter(globalState, SCHWEINE, 5, 7 + hasSchweine);
				}
				if ((state == FARM) && posX == 9 && posY == 2) {
					totalTiere = 0;
					initTiere(chicken, HUHN, 1, 1, 6 + hasChicken, 6 + hasChicken);
					enter(globalState, CHICKEN, 5, 7 + hasChicken);
				}
				if ((state == FARM || state == WEIDE || state == MARKT) && posX == 4 && posY == 40) {	// easter egg portal
					posX = 1;
					posY = 1;
				}
				if (state == HAUS && posX >= 7 && posY == 1) {
					totalTiere = 0;
					initTiere(birds, VOGEL, 3 - hasCage, 3, 1 + hasCage, 1 + hasCage);
					enter(globalState, HAUS_OG, posX, posY);
				}
				if (state == MARKT && posY == 13) enter(globalState, FARM, posX, posY);
			}
			break;
		case 3:
			if (globalState == KAUFEN || globalState == VERKAUFEN) {
				kaufState--;
				if (state == FARM && kaufState < STALL) kaufState = SCHWEINE;
				if ((onMarktFutter() || onMarktTiere()) && kaufState < KUH) kaufState = VOGEL;
			} else {
				if (getScreen()[9] < 4) {
					posX--;
					logPosition();
					noise(random(1700) + 200, 170, 0, -4);
				}
				if (state == WEIDE && posX == 14) enter(globalState, FARM, posX, posY);
			}
			break;
		case 5:
			switch (globalState) {
				case KAUFEN:
				case VERKAUFEN:
					kaufState++;
					if (state == FARM && kaufState > SCHWEINE) kaufState = STALL;
					if ((onMarktFutter() || onMarktTiere()) && kaufState > VOGEL) kaufState = KUH;
					break;
				case STATUS:
					enter(WERBUNG, state, posX, posY);
					werbungStart = millis();
					werbungPointer = 0;
					break;
				case RUNNING:
					if (getScreen()[11] < 4) {
						posX++;
						logPosition();
						noise(random(1700) + 200, 170, 0, -4);
					}
					if (state == FARM && posX == 14) enter(globalState, WEIDE, posX, posY);
					break;
			}
			break;
		case 7:
			if (globalState == KAUFEN || globalState == VERKAUFEN) {
				if (state == FARM)
					enter(RUNNING, state, 1, 1);
				else
					enter(RUNNING, state, posX, posY);
			} else {
				if (getScreen()[14] < 4) {
					posY++;
					logPosition();
					noise(random(1700) + 200, 170, 0, -4);
				}
				if (state == HAUS && posX == 5 && posY == 9) enter(globalState, FARM, 4, 8);
				if (state == STALL && posX == 5 && posY == 8 + hasStall) enter(globalState, FARM, 9, 9);
				if (state == SCHWEINE && posX == 5 && posY == 8 + hasStall) enter(globalState, FARM, -2, 7);
				if (state == CHICKEN && posX == 5 && posY == 8 + hasChicken) enter(globalState, FARM, 9, 3);
				if (state == HAUS_OG && posX >= 7 && posY == 4) enter(globalState, HAUS, posX, posY);
				if (state == FARM && posY >= 13 && posX >= 2 && posX <= 6) enter(globalState, MARKT, posX, posY);
				if (state == MARKT && posY < 13) enter(globalState, FARM, posX, posY);
			}
			break;
		case 6:
			if (globalState != STATUS) {
				enter(STATUS, state, posX, posY);
				statusStart = millis();
			} else
				enter(RUNNING, state, posX, posY);
			break;
		case 8:
			if (globalState != GELD) {
				enter(GELD, state, posX, posY);
				statusStart = millis();
			} else
				enter(RUNNING, state, posX, posY);
			break;
		case 4:
			switch (globalState) {
				case RUNNING:
					if (state == FARM) {
						enter(KAUFEN, state, posX, posY);
						kaufState = STALL;
					}
					if ((onMarktFutter() || onMarktTiere()) && (daytime == 1 || daytime == 3)) {
						enter(KAUFEN, state, posX, posY);
						kaufState = KUH;
					}
					break;
				case STATUS:
					enter(WERBUNG, state, posX, posY);
					werbungStart = millis();
					werbungPointer = 0;
					break;
				case KAUFEN:
					if (state == FARM) {
						switch (kaufState) {
							case STALL:
								productKaufen(&hasStall, 500, 1);
								break;
							case HAUS:
								productKaufen(&hasHaus, 500, 1);
								break;
							case CAGE:
								productKaufen(&hasCage, 100, 1);
								break;
							case CHICKEN:
								productKaufen(&hasChicken, 50, 1);
								break;
							case SCHWEINE:
								productKaufen(&hasSchweine, 200, 1);
								break;
						}
					}
					if (onMarktTiere()) {
						bool bought = false;
						switch (kaufState) {
							case KUH:
								if (hasStall) bought = productKaufen(&cows, 20, 1);
								break;
							case SCHAF:
								if (hasStall) bought = productKaufen(&pigs, 20, 1);
								break;
							case SCHWEIN:
								if (hasSchweine) bought = productKaufen(&pigs, 30, 1);
								break;
							case HUHN:
								if (hasChicken) bought = productKaufen(&chicken, 5, 1);
								break;
							case VOGEL:
								if (hasCage) bought = productKaufen(&birds, 10, 1);
								break;
						}
						if (bought) tierSound(kaufState, true);
					}
					if (onMarktFutter()) {
						bool bought = false;
						switch (kaufState) {
							case KUH:
								if (cows) bought = productKaufen(&cowsFood, 1, cows);
								break;
							case SCHAF:
								if (sheep) bought = productKaufen(&sheepFood, 1, sheep);
								break;
							case SCHWEIN:
								if (pigs) bought = productKaufen(&pigsFood, 1, pigs);
								break;
							case HUHN:
								if (chicken) bought = productKaufen(&chickenFood, 1, chicken);
								break;
							case VOGEL:
								if (birds) bought = productKaufen(&birdsFood, 1, birds);
								break;
						}
						if (bought) tierSound(kaufState, true);
					}
					break;
				case VERKAUFEN:
					if (onMarktFutter()) {
						switch (kaufState) {
							case KUH:
								productVerkaufen(&cowsProduct, 6);
								break;
							case SCHAF:
								productVerkaufen(&sheepProduct, 6);
								break;
							case SCHWEIN:
								productVerkaufen(&pigsProduct, 10);
								break;
							case HUHN:
								productVerkaufen(&chickenProduct, 1);
								break;
						}
					}
					break;
			}
			break;
		case 9:
			if (daytime == 0 || daytime == 2 || daytime == 4) {
				if (state == STALL && cows > 0 && cowsFood >= cows && !cowsFed) {
					cowsFood -= cows;
					cowsFed = true;
				}
				if (state == STALL && sheep > 0 && sheepFood >= sheep && !sheepFed) {
					sheepFood -= sheep;
					sheepFed = true;
				}
				if (state == SCHWEINE && cows > 0 && pigsFood >= pigs && !pigsFed) {
					pigsFood -= pigs;
					pigsFed = true;
				}
				if (state == CHICKEN && chicken > 0 && chickenFood >= chicken && !chickenFed) {
					chickenFood -= chicken;
					chickenFed = true;
				}
				if (state == HAUS_OG && birds > 0 && birdsFood >= birds && !birdsFed) {
					birdsFood -= birds;
					birdsFed = true;
				}
			}
			break;
		case 11:
			if (state == MARKT && onMarktFutter() && (daytime == 1 || daytime == 3)) {
				enter(VERKAUFEN, state, posX, posY);
				kaufState = KUH;
			}
			break;
		case 0:
			if ((daytime == 1 || daytime == 3) && cowsFed && state == STALL) cowsProduct += cows;
			if (daytime == 2 && sheepFed && state == STALL) sheepProduct += sheep;
			if (daytime == 1 && pigsFed && state == SCHWEINE) {
				pigsProduct++;
				pigs--;
			}
			if (daytime == 4 && chickenFed && state == CHICKEN) chickenProduct += chicken;
			break;
		case 10:
			increaseDayTime();
			clear(0);
			drawDaytime();
			stopTimeDisplay = millis() + 800;
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
			drawBox(rx(2), ry(17), 2, 2, 2);				// stand futter/verkaufen
			drawBox(rx(4), ry(15), 3, 3, 2);				// stand tiere
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
			if (state == WEIDE) tierSound(WEIDE, false);
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

void FarmGame::enter(char newGlobalState, char newState, int startPosX, int startPosY){
	globalState = newGlobalState;
	state = newState;
	posX = startPosX;
	posY = startPosY;
	switch (globalState) {
		case RUNNING:
			Serial.print("RUNNING ");
			break;
		case GELD:
			Serial.print("GELD ");
			break;
		case STATUS:
			Serial.print("STATUS ");
			break;
		case KAUFEN:
			Serial.print("KAUFEN ");
			break;
		case WERBUNG:
			Serial.print("WERBUNG ");
			break;
		case VERKAUFEN:
			Serial.print("VERKAUFEN ");
			break;
	}
	switch (state) {
		case FARM:
			Serial.print("FARM ");
			break;
		case HAUS_OG:
			Serial.print("HAUS_OG ");
			break;
		case MARKT:
			Serial.print("MARKT ");
			break;
		case STALL:
			Serial.print("STALL ");
			break;
		case HAUS:
			Serial.print("HAUS ");
			break;
		case CAGE:
			Serial.print("CAGE ");
			break;
		case CHICKEN:
			Serial.print("CHICKEN ");
			break;
		case SCHWEINE:
			Serial.print("SCHWEINE ");
			break;
		case WEIDE:
			Serial.print("WEIDE ");
			break;
	}
	logPosition();
}

void FarmGame::initTiere(uint16_t count, char type, int x, int y, int dx, int dy){
	if (tierPosX == NULL) return;
	for (uint16_t i = 0; i < count; i++) {
		tierType[totalTiere + i] = type;
		tierPosX[totalTiere + i] = random(dx) + x;
		tierPosY[totalTiere + i] = random(dy) + y;
	}
	totalTiere += count;
}

void FarmGame::drawTiere(){
	if (tierPosX == NULL) return;
	for (uint16_t i = 0; i < totalTiere; i++) {
		if (rx(tierPosX[i] >= 0 && rx(tierPosX[i] < 4) && ry(tierPosY[i]) >= 0 && ry(tierPosY[i]) < 5)) switch (random(300)) {
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
				tierSound(KUH, false);
				break;
			case SCHAF:
				set(rx(tierPosX[i]), ry(tierPosY[i]), 2);
				tierSound(SCHAF, false);
				break;
			case SCHWEIN:
				set(rx(tierPosX[i]), ry(tierPosY[i]), 12);
				tierSound(SCHWEIN, false);
				break;
			case HUHN:
				set(rx(tierPosX[i]), ry(tierPosY[i]), 2);
				tierSound(HUHN, false);
				break;
			case VOGEL:
				set(rx(tierPosX[i]), ry(tierPosY[i]), 2);
				tierSound(VOGEL, false);
				break;
		}
	}
}

void FarmGame::logPosition(){
	Serial.print(posX);
	Serial.print("/");
	Serial.println(posY);
}

void FarmGame::tierSound(char tierType, bool force){
	if ((random(1500) == 0) || force) switch (tierType) {
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

void FarmGame::increaseDayTime(){
	daytime++;
	if (daytime == 6) daytime = 0;
	if (daytime == 0 || daytime == 2 || daytime == 4) {
		cowsFed = false;
		sheepFed = false;
		pigsFed = false;
		chickenFed = false;
		birdsFed = false;
	}
}

void FarmGame::productVerkaufen(uint16_t *product, uint16_t price){
	if (*product > 0) {
		money += *product * price;
		*product = 0;
		blinkKaufen = millis();
	}
}

bool FarmGame::productKaufen(uint16_t *product, uint16_t price, uint16_t amount){
	if (money >= price * amount) {
		*product += amount;
		money -= price * amount;
		blinkKaufen = millis();
		return true;
	} else
		return false;
}

bool FarmGame::onMarktTiere(){
	if (state == MARKT && posX >= 4 && posX <= 6 && posY >= 15 && posY <= 17)
		return true;
	else
		return false;
}

bool FarmGame::onMarktFutter(){
	if (state == MARKT && posX >= 2 && posX <= 3 && posY >= 17 && posY <= 18)
		return true;
	else
		return false;
}

void FarmGame::drawDaytime(){
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

}
