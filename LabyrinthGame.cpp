/*
 * LabyrinthGame.cpp
 *
 *  Created on: Jan 10, 2021
 *      Author: hopper
 */
#include "LabyrinthGame.h"

LabyrinthGame::LabyrinthGame() {
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
	labyrinth = (uint8_t*) malloc(sizeX * sizeY / 4 + 1);
	if (labyrinth == NULL)
		Serial.println("malloc problem");

	for (y = 0; y < sizeY; y++)			// clear all
		for (x = 0; x < sizeX; x++)
			clearPoint(x, y);
	for (y = 2; y < sizeY - 1; y += 2)
		for (x = 2; x < sizeX - 1; x += 2) {
			setPoint(x, y);							// set pillars
			switch (rand() % 4) {			// and one of the adjacent walls
			case 0:
				setPoint(x, y - 1);
				break;
			case 1:
				setPoint(x, y + 1);
				break;
			case 2:
				setPoint(x - 1, y);
				break;
			case 3:
				setPoint(x + 1, y);
				break;
			}
		}
	for (y = 0; y < sizeY; y++) {			// set outer border
		setPoint(0, y);
		setPoint(sizeX - 1, y);
	}
	for (x = 0; x < sizeX; x++) {
		setPoint(x, 0);
		setPoint(x, sizeY - 1);
	}
	clearPoint(1, 0);								// create entry+exit
	clearPoint(sizeX - 2, sizeY - 1);

	posX = sizeX - 2;
	posY = sizeY - 1;
}

void LabyrinthGame::play() {
	clear(0);

	if (labyrinth == NULL)
		generate(sizeX, sizeY);

	if (getKeyClick(4))
		showHints = !showHints;

	if (getKeyClick(1) && (get(posX, posY - 1) != 1)) {
		if (posY > 0) {
			posY--;
			noise(rand() % 1700 + 200, 170, 0, -4);
		}
	}
	if (getKeyClick(3) && (get(posX - 1, posY) != 1)) {
		if (posX > 0) {
			posX--;
			noise(rand() % 1700 + 200, 170, 0, -4);
		}
	}
	if (getKeyClick(5) && (get(posX + 1, posY) != 1)) {
		if (posX < sizeX - 1) {
			posX++;
			noise(rand() % 1700 + 200, 170, 0, -4);
		}
	}
	if (getKeyClick(7) && (get(posX, posY + 1) != 1)) {
		if (posY < sizeY - 1) {
			posY++;
			noise(rand() % 1700 + 200, 170, 0, -4);
		}
	}

	for (int y = -2; y < 3; y++)
		for (int x = -2; x < 2; x++)
			if ((y + posY >= 0) && (y + posY < sizeY) && (x + posX >= 0) && (x + posX < sizeX)) {
				switch (get(x + posX, y + posY)) {
				case 0:
					set(x + 2, y + 2, 0);
					break;
				case 1:
					set(x + 2, y + 2, 15);
					break;
				case 2:
					if (showHints)
						set(x + 2, y + 2, 1);
					else
						set(x + 2, y + 2, 0);
					break;
				}
			} else
				set(x + 2, y + 2, 0);				//outside of labyrinth
	set(2, 2, (millis() / 100) % 3 + 1);		//player
	setPoint(posX, posY, 2);

	if (posY == 0) {
		victory((sizeX - 3) / 2, 9, 11);
		sizeX += 2;
		sizeY += 2;
		generate(sizeX, sizeY);
	} else
		flipBuffer();
}
