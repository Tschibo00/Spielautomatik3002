/*
 * LabyrinthGame.cpp
 *
 *  Created on: Jan 10, 2021
 *      Author: hopper
 */
#include "LabyrinthGame.h"

LabyrinthGame::LabyrinthGame(DisplayController *dc, KeyboardController *kc, SoundController *sc) {
	this->dc = dc;
	this->kc = kc;
	this->sc = sc;
}

// Bo-taoshi algorithm for maze generation
// pillars at even intervals, and makes one wall for each pillar
void LabyrinthGame::generate(int sizeX, int sizeY) {
	int x, y;

	if (sizeX % 2 == 0)
		sizeX++;					// we need odd sizes
	if (sizeY % 2 == 0)
		sizeY++;
	this->sizeX = sizeX;
	this->sizeY = sizeY;

	if (labyrinth != NULL)
		free(labyrinth);
	labyrinth = (uint8_t*) malloc(sizeX * sizeY/4);

	for (y = 0; y < sizeY; y++)			// clear all
		for (x = 0; x < sizeX; x++)
			clear(x, y);
	for (y = 2; y < sizeY - 1; y += 2)
		for (x = 2; x < sizeX - 1; x += 2) {
			set(x, y);							// set pillars
			switch (rand() % 4) {			// and one of the adjacent walls
			case 0:
				set(x, y - 1);
				break;
			case 1:
				set(x, y + 1);
				break;
			case 2:
				set(x - 1, y);
				break;
			case 3:
				set(x + 1, y);
				break;
			}
		}
	for (y = 0; y < sizeY; y++) {			// set outer border
		set(0, y);
		set(sizeX - 1, y);
	}
	for (x = 0; x < sizeX; x++) {
		set(x, 0);
		set(x, sizeY - 1);
	}
	clear(1, 0);								// create entry+exit
	clear(sizeX - 2, sizeY - 1);

	posX = sizeX - 2;
	posY = sizeY - 1;
}

void LabyrinthGame::play() {
	dc->clear(0);

	if (labyrinth == NULL)
		generate(sizeX, sizeY);

	if (kc->getKeyClick(4))
		showHints=!showHints;

	if (kc->getKeyClick(1) && (get(posX, posY - 1) !=1)) {
		if (posY > 0) {
			posY--;
			sc->noise(rand() % 1700 + 200, 170, 0, -4);
		}
	}
	if (kc->getKeyClick(3) && (get(posX - 1, posY) !=1)) {
		if (posX > 0) {
			posX--;
			sc->noise(rand() % 1700 + 200, 170, 0, -4);
		}
	}
	if (kc->getKeyClick(5) && (get(posX + 1, posY) !=1)) {
		if (posX < sizeX - 1) {
			posX++;
			sc->noise(rand() % 1700 + 200, 170, 0, -4);
		}
	}
	if (kc->getKeyClick(7) && (get(posX, posY + 1) !=1)) {
		if (posY < sizeY - 1) {
			posY++;
			sc->noise(rand() % 1700 + 200, 170, 0, -4);
		}
	}

	for (int y = -2; y < 3; y++)
		for (int x = -2; x < 2; x++)
			if ((y + posY >= 0) && (y + posY < sizeY) && (x + posX >= 0) && (x + posX < sizeX)) {
				switch (get(x + posX, y + posY)) {
				case 0:
					dc->screen[x + 2 + (y + 2) * 4] = 0;
					break;
				case 1:
					dc->screen[x + 2 + (y + 2) * 4] = 15;
					break;
				case 2:
					if (showHints)
						dc->screen[x + 2 + (y + 2) * 4] = 1;
					else
						dc->screen[x + 2 + (y + 2) * 4] = 0;
					break;
				}
			} else
				dc->screen[x + 2 + (y + 2) * 4] = 0;			//outside of labyrinth
	dc->screen[10] = (millis() / 100) % 3 + 1;				//player
	set(posX,posY,2);

	if (posY == 0) {
		sc->music(victoryMusic, 5, 130, 600, -4);
		showScreen(victoryScreen);
		sizeX += 2;
		sizeY += 2;
		generate(sizeX, sizeY);
	}

	dc->flipBuffer();
}

void LabyrinthGame::showScreen(char *screen) {
	dc->copy(screen);
	dc->flipBuffer();
	while (!kc->anyKeyClick(9, 11)) {
		kc->scanKeyboard();
	}
}
