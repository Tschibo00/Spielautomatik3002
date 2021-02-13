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
	bool canTrade = true;
	char text[10];

	if (g.cows >= 200 && g.sheep >= 200 && g.pigs >= 200 && g.birds >= 200 && g.chicken >= 200 && getHappiness() == 1) {
		victory(1, 0, 11);
		asm volatile ("jmp 0");
	}

	if (millis() > stopTimeDisplay) {
		clear(0);

		switch (globalState) {
			case RUNNING:
				showFarm();
				break;
			case GELD:
				memset(text, 0, 10);
				sprintf(text, " %-8ld", g.money);
				showScroller(text, ((millis() - statusStart) / 200) % 36, true);
				break;
			case FUTTERMENGE:
				memset(text, 0, 10);
				switch (state) {
					case STALL:
						sprintf(text, " : %d ; %d", g.cows, g.sheep);
						break;
					case CAGE:
						sprintf(text, " > %d", g.birds);
						break;
					case CHICKEN:
						sprintf(text, " = %d", g.chicken);
						break;
					case SCHWEINE:
						sprintf(text, " < %d", g.pigs);
						break;
				}
				showScroller(text, ((millis() - statusStart) / 200) % 36, true);
				break;
			case STATUS:
				if (((millis() - statusStart) / 2000) % 2)
					drawDaytime();
				else
					copy(faces + 20 + getHappiness() * 20);
				break;
			case WERBUNG:
				copy(werbung + werbungPointer * 20);
				if (millis() - werbungStart >= werbungPointer * 1000 + 1000) {
					werbungStart = millis();
					werbungPointer++;
					if (werbungPointer == 7) {
						enter(RUNNING, state, posX, posY);
						tone(2500, 255, 0, -1);
						switch (g.daytime) {
							case 0:
								g.money += 500;
								break;
							case 1:
								if (g.hasSchweine)
									g.pigs += 20;
								else
									g.cows += 20;
								break;
							case 2:
								g.money += 3000;
								break;
							case 3:
								if (g.hasSchweine) g.pigs += 20;
								break;
							case 4:
								g.money += 700;
								break;
						}
					}
				}
				break;
			case KAUFEN:
				if (state == FARM) {
					copy(goods + (kaufState - STALL) * 20);
					if (getPriceFarm(kaufState) > g.money) canTrade = false;
				}
				if (onMarktFutter()) {
					copy(tierBild + kaufState * 20);
					if (getPriceFutter(kaufState) > g.money) canTrade = false;
				}
				if (onMarktTiere()) {
					copy(tierBild + kaufState * 20);
					if (getPriceTier(kaufState) > g.money) canTrade = false;
				}
				break;
			case VERKAUFEN:
				copy(tierBild + kaufState * 20);
				Serial.println(kaufState);
				switch (kaufState) {
					case KUH:
						if (g.cowsProduct == 0) canTrade = false;
						break;
					case SCHAF:
						if (g.sheepProduct == 0) canTrade = false;
						break;
					case SCHWEIN:
						if (g.pigsProduct == 0) canTrade = false;
						break;
					case HUHN:
						if (g.chickenProduct == 0) canTrade = false;
						break;
				}
				break;
			case FUETTERN:
				if ((millis() - statusStart) < 1000) {
					copy(tierBild + kaufState * 20);
					switch (kaufState) {
						case KUH:
							if (g.cowsFood == 0) canTrade = false;
							break;
						case SCHAF:
							if (g.sheepFood == 0) canTrade = false;
							break;
						case SCHWEIN:
							if (g.pigsFood == 0) canTrade = false;
							break;
						case HUHN:
							if (g.chickenFood == 0) canTrade = false;
							break;
						case VOGEL:
							if (g.birdsFood == 0) canTrade = false;
							break;
					}
				} else {
					memset(text, 0, 10);
					switch (kaufState) {
						case KUH:
							sprintf(text, " : %d", g.cowsFood / g.cows);
							break;
						case SCHAF:
							sprintf(text, " ; %d", g.sheepFood / g.sheep);
							break;
						case SCHWEIN:
							sprintf(text, " < %d", g.pigsFood / g.pigs);
							break;
						case HUHN:
							sprintf(text, " = %d", g.chickenFood / g.chicken);
							break;
						case VOGEL:
							sprintf(text, " > %d", g.birdsFood / g.birds);
							break;
					}
					showScroller(text, ((millis() - statusStart) / 200) % 28, true);
				}
				break;
		}
	}
	if (globalState == KAUFEN || globalState == VERKAUFEN || globalState == FUETTERN) {
		if (!canTrade) strike();
		if (millis() - blinkKaufen < 300) for (char i = 0; i < 20; i++)
			getScreen()[i] += 10 - (millis() - blinkKaufen) / 30;
	}

	switch (getNumberClick()) {
		case 1:
			if (globalState == KAUFEN || globalState == VERKAUFEN || globalState == FUETTERN) {
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
				if ((state == FARM) && posX == 4 && posY == 7 && g.hasHaus) enter(globalState, HAUS, 5, 8);
				if ((state == FARM) && posX == 9 && posY == 8) {
					if (tierImStall()) {
						totalTiere = 0;
						initTiere(g.cows, KUH, 1, 1, 6 + g.hasStall, 6 + g.hasStall);
						initTiere(g.sheep, SCHAF, 1, 1, 6 + g.hasStall, 6 + g.hasStall);
					}
					enter(globalState, STALL, 5, 7 + g.hasStall);
				}
				if ((state == FARM) && posX == -2 && posY == 6) {
					totalTiere = 0;
					initTiere(g.pigs, SCHWEIN, 1, 1, 6 + g.hasSchweine, 6 + g.hasSchweine);
					enter(globalState, SCHWEINE, 5, 7 + g.hasSchweine);
				}
				if ((state == FARM) && posX == 9 && posY == 2) {
					totalTiere = 0;
					initTiere(g.chicken, HUHN, 1, 1, 6 + g.hasChicken, 6 + g.hasChicken);
					enter(globalState, CHICKEN, 5, 7 + g.hasChicken);
				}
				if ((state == FARM || state == WEIDE || state == MARKT) && posX == 4 && posY == 40) {	// easter egg portal
					posX = 1;
					posY = 1;
				}
				if (state == HAUS && posX >= 7 && posY == 1) {
					totalTiere = 0;
					initTiere(g.birds, VOGEL, 3 - g.hasCage, 3, 1 + g.hasCage, 1 + g.hasCage);
					enter(globalState, HAUS_OG, posX, posY);
				}
				if (state == MARKT && posY == 13) enter(globalState, FARM, posX, posY);
			}
			break;
		case 3:
			if (globalState == KAUFEN || globalState == VERKAUFEN || globalState == FUETTERN) {
				kaufState--;
				if (state == FARM && kaufState < STALL) kaufState = SCHWEINE;
				if ((onMarktFutter() || onMarktTiere()) && kaufState < KUH) kaufState = VOGEL;
				if (state == STALL && kaufState < KUH) kaufState = SCHAF;
				if (state == HAUS_OG || state == CAGE) kaufState = VOGEL;
				if (state == CHICKEN) kaufState = HUHN;
				if (state == SCHWEINE) kaufState = SCHWEINE;
				if (globalState == FUETTERN) statusStart = millis();
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
				case FUETTERN:
					kaufState++;
					if (state == FARM && kaufState > SCHWEINE) kaufState = STALL;
					if ((onMarktFutter() || onMarktTiere()) && kaufState > VOGEL) kaufState = KUH;
					if (globalState == VERKAUFEN && kaufState > HUHN) kaufState = KUH;
					if (state == STALL && kaufState > SCHAF) kaufState = KUH;
					if (state == HAUS_OG || state == CAGE) kaufState = VOGEL;
					if (state == CHICKEN) kaufState = HUHN;
					if (state == SCHWEINE) kaufState = SCHWEINE;
					if (globalState == FUETTERN) statusStart = millis();
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
					if (state == FARM && posX == 14) {
						if (!tierImStall()) {
							totalTiere = 0;
							initTiere(g.cows, KUH, 15, 3, 6 + g.hasStall, 6 + g.hasStall);
							initTiere(g.sheep, SCHAF, 15, 3, 6 + g.hasStall, 6 + g.hasStall);
						}
						enter(globalState, WEIDE, posX, posY);
					}
					break;
			}
			break;
		case 7:
			if (globalState == KAUFEN || globalState == VERKAUFEN || globalState == FUETTERN) {
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
				if (state == STALL && posX == 5 && posY == 8 + g.hasStall) enter(globalState, FARM, 9, 9);
				if (state == SCHWEINE && posX == 5 && posY == 8 + g.hasStall) enter(globalState, FARM, -2, 7);
				if (state == CHICKEN && posX == 5 && posY == 8 + g.hasChicken) enter(globalState, FARM, 9, 3);
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
					if ((onMarktFutter() || onMarktTiere()) && (g.daytime == 1 || g.daytime == 3)) {
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
								productKaufen(&g.hasStall, getPriceFarm(kaufState), 1);
								break;
							case HAUS:
								productKaufen(&g.hasHaus, getPriceFarm(kaufState), 1);
								break;
							case CAGE:
								if (g.hasHaus) productKaufen(&g.hasCage, getPriceFarm(kaufState), 1);
								break;
							case CHICKEN:
								productKaufen(&g.hasChicken, getPriceFarm(kaufState), 1);
								break;
							case SCHWEINE:
								productKaufen(&g.hasSchweine, getPriceFarm(kaufState), 1);
								break;
						}
						updateSaveGame();
					}
					if (onMarktTiere()) {
						bool bought = false;
						switch (kaufState) {
							case KUH:
								if (g.hasStall) bought = productKaufen(&g.cows, getPriceTier(kaufState), 1);
								break;
							case SCHAF:
								if (g.hasStall) bought = productKaufen(&g.sheep, getPriceTier(kaufState), 1);
								break;
							case SCHWEIN:
								if (g.hasSchweine) bought = productKaufen(&g.pigs, getPriceTier(kaufState), 1);
								break;
							case HUHN:
								if (g.hasChicken) bought = productKaufen(&g.chicken, getPriceTier(kaufState), 1);
								break;
							case VOGEL:
								if (g.hasCage) bought = productKaufen(&g.birds, getPriceTier(kaufState), 1);
								break;
						}
						updateSaveGame();
						if (bought) tierSound(kaufState, true);
					}
					if (onMarktFutter()) {
						bool bought = false;
						switch (kaufState) {
							case KUH:
								if (g.cows) bought = productKaufen(&g.cowsFood, getPriceFutter(kaufState), g.cows);
								break;
							case SCHAF:
								if (g.sheep) bought = productKaufen(&g.sheepFood, getPriceFutter(kaufState), g.sheep);
								break;
							case SCHWEIN:
								if (g.pigs) bought = productKaufen(&g.pigsFood, getPriceFutter(kaufState), g.pigs);
								break;
							case HUHN:
								if (g.chicken) bought = productKaufen(&g.chickenFood, getPriceFutter(kaufState), g.chicken);
								break;
							case VOGEL:
								if (g.birds) bought = productKaufen(&g.birdsFood, getPriceFutter(kaufState), g.birds);
								break;
						}
						updateSaveGame();
						if (bought) tierSound(kaufState, true);
					}
					break;
				case VERKAUFEN:
					if (onMarktFutter()) {
						switch (kaufState) {
							case KUH:
								productVerkaufen(&g.cowsProduct, 25);
								break;
							case SCHAF:
								productVerkaufen(&g.sheepProduct, 25);
								break;
							case SCHWEIN:
								productVerkaufen(&g.pigsProduct, 50);
								break;
							case HUHN:
								productVerkaufen(&g.chickenProduct, 7);
								break;
						}
					}
					updateSaveGame();
					break;
				case FUETTERN:
					if (canTrade) {
						switch (kaufState) {
							case KUH:
								if (!g.cowsFed) g.cowsFood -= g.cows;
								g.cowsFed = true;
								break;
							case SCHAF:
								if (!g.sheepFed) g.sheepFood -= g.sheep;
								g.sheepFed = true;
								break;
							case SCHWEIN:
								if (!g.pigsFed) g.pigsFood -= g.pigs;
								g.pigsFed = true;
								break;
							case HUHN:
								if (!g.chickenFed) g.chickenFood -= g.chicken;
								g.chickenFed = true;
								break;
							case VOGEL:
								if (!g.birdsFed) g.birdsFood -= g.birds;
								g.birdsFed = true;
								break;
						}
						updateSaveGame();
						tierSound(WEIDE, true);
					}
					break;
			}
			break;
		case 9:
			if (g.daytime == 0 || g.daytime == 2 || g.daytime == 4) {
				if (state == STALL && (g.cows > 0 && g.cowsFood >= g.cows && !g.cowsFed)
						|| (g.sheep > 0 && g.sheepFood >= g.sheep && !g.sheepFed)) {
					enter(FUETTERN, state, posX, posY);
					kaufState = KUH;
					statusStart = millis();
				}
				if (state == SCHWEINE && g.pigs > 0 && g.pigsFood >= g.pigs && !g.pigsFed) {
					enter(FUETTERN, state, posX, posY);
					kaufState = SCHWEIN;
					statusStart = millis();
				}
				if (state == CHICKEN && g.chicken > 0 && g.chickenFood >= g.chicken && !g.chickenFed) {
					enter(FUETTERN, state, posX, posY);
					kaufState = HUHN;
					statusStart = millis();
				}
				if (state == HAUS_OG && g.birds > 0 && g.birdsFood >= g.birds && !g.birdsFed) {
					enter(FUETTERN, state, posX, posY);
					kaufState = VOGEL;
					statusStart = millis();
				}
			}
			break;
		case 11:
			if (state == MARKT && onMarktFutter() && (g.daytime == 1 || g.daytime == 3)) {
				enter(VERKAUFEN, state, posX, posY);
				kaufState = KUH;
			}
			break;
		case 0:
			if ((g.daytime == 1 || g.daytime == 3) && g.cowsFed && state == STALL) g.cowsProduct += g.cows;
			if (g.daytime == 2 && g.sheepFed && state == STALL) g.sheepProduct += g.sheep;
			if (g.daytime == 1 && g.pigsFed && state == SCHWEINE) {
				g.pigsProduct++;
				g.pigs--;
			}
			if (g.daytime == 4 && g.chickenFed && state == CHICKEN) g.chickenProduct += g.chicken;
			updateSaveGame();
			break;
		case 10:
			increaseDayTime();
			clear(0);
			drawDaytime();
			stopTimeDisplay = millis() + 800;
			updateSaveGame();
			break;
		case 2:
			if (globalState != FUTTERMENGE && (state == STALL || state == HAUS_OG || state == SCHWEINE || state == CHICKEN)) {
				enter(FUTTERMENGE, state, posX, posY);
				statusStart = millis();
			} else
				enter(RUNNING, state, posX, posY);
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
			drawRectangle(rx(g.hasSchweine ? -5 : 0), ry(g.hasChicken ? -4 : 0), g.hasSchweine ? 19 : 14, g.hasChicken ? 17 : 13, 15);
			// durchgaenge
			set(rx(4), ry(12), 0);
			set(rx(13), ry(7), 0);
			// haus
			if (g.hasHaus) {
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
			drawRectangle(rx(15), ry(2), 9 + g.hasStall, 9 + g.hasStall, 15);
			set(rx(15), ry(7), 1);
			// schweine
			if (g.hasSchweine) {
				drawRectangle(rx(-3), ry(2), 3, 5, 15);
				set(rx(-2), ry(6), 2);		// eingang
			}
			// chicken
			if (g.hasChicken) {
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
			if (state == WEIDE) {
				if (!tierImStall()) drawTiere();
				tierSound(WEIDE, false);
			}
			break;
		case STALL:
			drawRectangle(rx(0), ry(0), 9 + g.hasStall, 9 + g.hasStall, 15);
			set(2, 2, (millis() / 100) % 3 + 1);		//player
			if (tierImStall()) drawTiere();
			set(rx(5), ry(8 + g.hasStall), 2);
			break;
		case SCHWEINE:
			drawRectangle(rx(0), ry(0), 9 + g.hasSchweine, 9 + g.hasSchweine, 15);
			set(2, 2, (millis() / 100) % 3 + 1);		//player
			drawTiere();
			set(rx(5), ry(8 + g.hasSchweine), 2);
			break;
		case CHICKEN:
			drawRectangle(rx(0), ry(0), 9 + g.hasChicken, 9 + g.hasChicken, 15);
			set(2, 2, (millis() / 100) % 3 + 1);		//player
			drawTiere();
			set(rx(5), ry(8 + g.hasChicken), 2);
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
			drawRectangle(rx(0 - g.hasCage), ry(0), 10 + g.hasCage, 10 + g.hasCage, 15);
			// treppe
			drawDottedLine(rx(7), ry(1), 0, 1, 4, 2, 1);
			drawDottedLine(rx(8), ry(1), 0, 1, 4, 2, 1);
			drawLine(rx(6), ry(2), 0, 1, 4, 7);
			drawLine(rx(6), ry(5), 1, 0, 3, 7);
			// vogelkaefig
			if (g.hasCage) {
				drawRectangle(rx(2 - g.hasCage), ry(2), 3 + g.hasCage, 3 + g.hasCage, 2);
				set(rx(2 - g.hasCage), ry(5 + g.hasCage), 2);
				set(rx(4), ry(5 + g.hasCage), 2);
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
	tierMinX = x;
	tierMaxX = x + dx - 1;
	tierMinY = y;
	tierMaxY = y + dy - 1;
}

void FarmGame::drawTiere(){
	if (tierPosX == NULL) return;
	for (uint16_t i = 0; i < totalTiere; i++) {
		switch (random(3000)) {
			case 0:
				if (tierPosX[i] > tierMinX) tierPosX[i]--;
				break;
			case 1:
				if (tierPosX[i], tierMaxX) tierPosX[i]++;
				break;
			case 2:
				if (tierPosY[i] > tierMinY) tierPosY[i]--;
				break;
			case 3:
				if (tierPosY[i] > tierMaxY) tierPosY[i]++;
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
	if ((random(4500) == 0) || force) switch (tierType) {
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
	g.daytime++;
	if (g.daytime == 6) g.daytime = 0;
	if (g.daytime == 0 || g.daytime == 2 || g.daytime == 4) {
		g.cowsFed = false;
		g.sheepFed = false;
		g.pigsFed = false;
		g.chickenFed = false;
		g.birdsFed = false;
	}
}

void FarmGame::productVerkaufen(uint16_t *product, uint16_t price){
	if (*product > 0) {
		g.money += *product * price;
		*product = 0;
		blinkKaufen = millis();
	}
}

bool FarmGame::productKaufen(uint16_t *product, uint16_t price, uint16_t amount){
	if (g.money >= price * amount) {
		*product += amount;
		g.money -= price * amount;
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
	switch (g.daytime) {
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

bool FarmGame::tierImStall(){
	if (g.daytime > 0 && g.daytime < 4)
		return false;
	else
		return true;
}

uint16_t FarmGame::getPriceFarm(char product){
	switch (product) {
		case STALL:
			return 500;
		case HAUS:
			return 500;
		case CAGE:
			return 100;
		case CHICKEN:
			return 50;
		case SCHWEINE:
			return 200;
	}
}

uint16_t FarmGame::getPriceTier(char product){
	switch (product) {
		case KUH:
			return 20;
		case SCHAF:
			return 20;
		case SCHWEIN:
			return 30;
		case HUHN:
			return 5;
		case VOGEL:
			return 10;
	}
}

uint16_t FarmGame::getPriceFutter(char product){
	return 1;
}

char FarmGame::getHappiness(){
	char happiness = 0;
	happiness += g.cowsFed ? 1 : -1;
	happiness += g.pigsFed ? 1 : -1;
	happiness += g.sheepFed ? 1 : -1;
	happiness += g.chickenFed ? 1 : -1;
	happiness += g.birdsFed ? 1 : -1;
	return happiness < -1 ? -1 : happiness > 1 ? 1 : 0;
}

void FarmGame::updateSaveGame(){
	saveGame((uint8_t*) &g, sizeof(SaveGame), 0, currentPlayer);
}

void FarmGame::selectPlayer(uint8_t player, bool resetSavedGame){
	if (!resetSavedGame)
		loadGame((uint8_t*) &g, sizeof(SaveGame), 0, player);
	else {
		g.money = 0;
		g.daytime = 0;
		g.day = 1;
		g.hasStall = 1;
		g.hasHaus = 0;
		g.hasSchweine = 0;
		g.hasChicken = 0;
		g.hasCage = 0;
		g.cows = 1;
		g.pigs = 0;
		g.sheep = 0;
		g.chicken = 0;
		g.birds = 0;
		g.cowsFood = 2;
		g.pigsFood = 0;
		g.sheepFood = 0;
		g.chickenFood = 0;
		g.birdsFood = 0;
		g.cowsFed = false;
		g.pigsFed = false;
		g.sheepFed = false;
		g.chickenFed = false;
		g.birdsFed = false;
		g.cowsProduct = 0;
		g.sheepProduct = 0;
		g.pigsProduct = 0;
		g.chickenProduct = 0;
	}
	currentPlayer = player;
}
