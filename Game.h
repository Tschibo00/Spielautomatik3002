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

class Game {
protected:
	DisplayController *dc;
	KeyboardController *kc;
	SoundController *sc;

public:
	virtual ~Game(){};
	virtual uint8_t* getIcon();
	virtual void play();
};

#endif /* GAME_H_ */
