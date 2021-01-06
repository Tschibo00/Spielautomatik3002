/*
 * GameController.h
 *
 *  Created on: Dec 27, 2020
 *      Author: hopper
 */

#ifndef GAMECONTROLLER_H_
#define GAMECONTROLLER_H_

#include "Game.h"
#include "DisplayController.h"
#include "KeyboardController.h"
#include "SoundController.h"

class GameController {
private:
	Game *gameList[10];
	int gameCount = 0;
	bool startup = true;
	int currentGame = -1;
	DisplayController *dc;
	KeyboardController *kc;
	SoundController *sc;
	uint8_t icon[52] = { 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 3, 15, 15, 15, 3, 3, 15, 15, 3, 3,
			3, 15, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };

public:
	GameController(DisplayController *dc, KeyboardController *kc, SoundController *sc);
	void registerGame(Game *game);
	void selectGame(int game);
	void play();
};

#endif /* GAMECONTROLLER_H_ */
