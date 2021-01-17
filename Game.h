/*
 * Game.h
 *
 *  Created on: Dec 27, 2020
 *      Author: hopper
 */

#ifndef GAME_H_
#define GAME_H_

#include <Arduino.h>
#include "DisplayController.h"
#include "KeyboardController.h"
#include "SoundController.h"

static const char victoryScreen[20] = { 0, 0, 15, 15, 0, 0, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 };
static const char gameOverScreen[20] = { 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 0, 0, 15, 15, 0, 0, 15, 15 };
static const char gameOverMusic[4] = { 10, 9, 8, 7 };
static const char victoryMusic[5] = { 13, PAUSE, 13, 13, 18 };

class Game {
protected:
	DisplayController *dc;
	KeyboardController *kc;
	SoundController *sc;

public:
	virtual ~Game() {
	}
	;
	virtual uint8_t* getIcon();
	virtual void play();
};

#endif /* GAME_H_ */
