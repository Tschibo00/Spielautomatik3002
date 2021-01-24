#include "AsteroidGame.h"

AsteroidGame::AsteroidGame(){
	initLevel();
}

void AsteroidGame::play(){

	if (asteroidRowWait == 0) asteroidRowWait = millis() + asteroidRowSpeed;

	clear(0);

	advanceAsteroids();
	copy(asteroids);

	if (getKeyClick(9)) if (playerPos > 0) playerPos--;
	if (getKeyClick(11)) if (playerPos < 3) playerPos++;
	if ((getKeyClick(10)) && (bulletY < 0)) {
		bulletX = playerPos;
		bulletY = 3;
		bulletWait = millis() + BULLETSPEED
		;
		tone(2500, 255, -8, -1);
	}
	getScreen()[16 + playerPos] = 15;
	if ((bulletY >= 0) && (bulletX >= 0)) {
		getScreen()[bulletY * 4 + bulletX] = 15;
	}

	if ((bulletY >= 0) && (asteroids[bulletY * 4 + bulletX] > 0)) {
		asteroids[bulletY * 4 + bulletX] -= 127;
		bulletY = -1;
		noise(5000, 255, -20, -2);
	}

	if ((bulletY >= 0) && (millis() >= bulletWait)) {
		bulletWait = millis() + BULLETSPEED
		;
		bulletY--;
	}

	flipBuffer();
}

void AsteroidGame::advanceAsteroids(){
	uint8_t i;

	if (millis() >= asteroidRowWait) {
		asteroidRowWait = millis() + asteroidRowSpeed;
		for (i = 19; i >= 4; i--)
			asteroids[i] = asteroids[i - 4];
		setFirstRow();
	}

	bool won = true;
	for (i = 0; i < 20; i++)
		if (asteroids[i] >= 0) won = false;
	if (won) {
		victory(level, 0, 8);
		asteroidRowSpeed -= 200;
		level++;
		initLevel();
	}
	bool lost = false;
	for (i = 16; i < 20; i++)
		if (asteroids[i] >= 0) lost = true;
	if (lost) {
		gameover(level);
		asteroidRowSpeed = ASTEROIDSTARTSPEED
		;
		level = 1;
		initLevel();
	}
}

void AsteroidGame::setFirstRow(){
	for (uint8_t i = 0; i < 4; i++)
		asteroids[i] = getPalette()[(rand() % 4) + 1];
}

void AsteroidGame::initLevel(){
	uint8_t i;
	for (i = 4; i < 20; i++)
		asteroids[i] = -1;
	setFirstRow();
	asteroidRowWait = 0;
}
