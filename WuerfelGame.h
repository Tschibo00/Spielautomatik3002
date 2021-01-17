/*
 * WuerfelGame.h
 *
 *  Created on: Dec 28, 2020
 *      Author: hopper
 */

#ifndef WUERFELGAME_H_
#define WUERFELGAME_H_

#include "Game.h"

class WuerfelGame: public Game {
private:
	uint8_t numbers[12] = { 16, 20, 10, 13, 2, 7, 19, 12, 3, 17, 5, 1 };
	void setRandomLed();
	char lastNumber = -1;

protected:
	uint8_t icon[20] = { 0, 15, 15, 0, 15, 0, 0, 15, 15, 0, 0, 15, 15, 0, 0, 15, 0, 15, 15, 0 };

public:
	WuerfelGame(DisplayController *dc, KeyboardController *kc, SoundController *sc);
	uint8_t* getIcon() override {
		return icon;
	}
	void play() override;
};

#endif /* WUERFELGAME_H_ */
