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
#include "SoundController.h"
#include "avr/pgmspace.h"

void registerGame(Game *game);
void selectGame(int game);
void play();

#endif /* GAMECONTROLLER_H_ */
