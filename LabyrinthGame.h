/*
 * LabyrinthGame.h
 *
 *  Created on: Jan 10, 2021
 *      Author: hopper
 */

#ifndef LABYRINTHGAME_H_
#define LABYRINTHGAME_H_

#include "Game.h"

class LabyrinthGame: public Game {
private:
	char *labyrinth = NULL;
	int sizeX = 3;
	int sizeY = 3;
	void set(int x, int y) {
		labyrinth[y * sizeX + x] = 1;
	}
	void clear(int x, int y) {
		labyrinth[y * sizeX + x] = 0;
	}
	char get(int x, int y) {
		return labyrinth[y * sizeX + x];
	}

	//player
	int posX, posY;

	char victoryScreen[20] = { 0, 0, 15, 15, 0, 0, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 };
	char victoryMusic[5]={13,PAUSE,13,13,18};

	void showScreen(char *screen);

protected:
	uint8_t icon[20] = { 15, 15, 15, 15, 0, 0, 0, 15, 15, 15, 3, 15, 15, 0, 0, 15, 15, 0, 15, 15 };
public:
	LabyrinthGame(DisplayController *dc, KeyboardController *kc, SoundController *sc);
	uint8_t* getIcon() override {
		return icon;
	}
	void play() override;
	void generate(int sizeX, int sizeY);
};

#endif /* LABYRINTHGAME_H_ */
