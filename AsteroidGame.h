/*
 * AsteroidGame.h
 *
 *  Created on: Dec 29, 2020
 *      Author: hopper
 */

#ifndef ASTEROIDGAME_H_
#define ASTEROIDGAME_H_
#include "Game.h"

#define BULLETSPEED 80;
#define ASTEROIDSTARTSPEED 4000;

class AsteroidGame: public Game {
private:
	char playerPos = 2;
	char bulletX = -1;
	char bulletY = -1;
	unsigned long bulletWait = 0;
	char asteroids[20];
	unsigned long asteroidRowWait;
	unsigned long asteroidRowSpeed = ASTEROIDSTARTSPEED;
	char victoryScreen[20] = { 0, 0, 15, 15, 0, 0, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 };
	char gameOverScreen[20] = { 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 0, 0, 15, 15, 0, 0, 15, 15 };
	char gameOverMusic[4]={10,9,8,7};
	char victoryMusic[4]={2,2,2,2};


protected:
	uint8_t icon[20] = { 7, 15, 3, 7, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 15, 0 };

public:
	AsteroidGame(DisplayController *dc, KeyboardController *kc, SoundController *sc);
	uint8_t* getIcon() override {
		return icon;
	}
	void play() override;
	void advanceAsteroids();
	void initLevel();
	void showScreen(char *screen);
	void setFirstRow();
};
#endif /* ASTEROIDGAME_H_ */
