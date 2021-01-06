/*
 * WuerfelGame.cpp
 *
 *  Created on: Dec 29, 2020
 *      Author: hopper
 */
#include "AsteroidGame.h"

AsteroidGame::AsteroidGame(DisplayController *dc, KeyboardController *kc, SoundController *sc) {
	this->dc = dc;
	this->kc = kc;
	this->sc = sc;
	initLevel();
}

void AsteroidGame::play() {

	if (asteroidRowWait == 0)
		asteroidRowWait = millis() + asteroidRowSpeed;

	dc->clear(0);

	advanceAsteroids();
	dc->copy(asteroids);

	if (kc->getKeyClick(9))
		if (playerPos > 0)
			playerPos--;
	if (kc->getKeyClick(11))
		if (playerPos < 3)
			playerPos++;
	if ((kc->getKeyClick(10)) && (bulletY < 0)) {
		bulletX = playerPos;
		bulletY = 3;
		bulletWait = millis() + BULLETSPEED;
		sc->tone(10250, 32000, -3, -15);
	}
	dc->screen[16 + playerPos] = 15;
	if ((bulletY >= 0) && (bulletX >= 0)) {
		dc->screen[bulletY * 4 + bulletX] = 15;
	}

	if (asteroids[bulletY * 4 + bulletX] > 0) {
		asteroids[bulletY * 4 + bulletX] -= 127;
		bulletY = -1;
		sc->tone(10000, 32000, -3400, -8);
	}

	if ((bulletY >= 0) && (millis() >= bulletWait)) {
		bulletWait = millis() + BULLETSPEED
		;
		bulletY--;
	}

	dc->flipBuffer();
}

void AsteroidGame::advanceAsteroids() {
	uint8_t i;

	if (millis() >= asteroidRowWait) {
		asteroidRowWait = millis() + asteroidRowSpeed;
		for (i = 19; i >= 4; i--)
			asteroids[i] = asteroids[i - 4];
		setFirstRow();
	}

	bool won = true;
	for (i = 0; i < 20; i++)
		if (asteroids[i] >= 0)
			won = false;
	if (won) {
		showScreen(victoryScreen);
		asteroidRowSpeed -= 200;
		initLevel();
	}
	bool lost = false;
	for (i = 16; i < 20; i++)
		if (asteroids[i] >= 0)
			lost = true;
	if (lost) {
		showScreen(gameOverScreen);
		asteroidRowSpeed = ASTEROIDSTARTSPEED
		;
		initLevel();
	}
}

void AsteroidGame::setFirstRow() {
	for (uint8_t i = 0; i < 4; i++)
		asteroids[i] = dc->palette[(rand() % 4) + 1];
}

void AsteroidGame::initLevel() {
	uint8_t i;
	for (i = 4; i < 20; i++)
		asteroids[i] = -1;
	setFirstRow();
	asteroidRowWait = 0;
}

void AsteroidGame::showScreen(char *screen) {
	dc->copy(screen);
	dc->flipBuffer();
	while (!kc->anyKeyClick(0, 8)) {
		kc->scanKeyboard();
	}
}
