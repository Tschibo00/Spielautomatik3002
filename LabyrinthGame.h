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
	uint8_t *labyrinth = NULL;					// 0=unvisited,empty space,  1=wall, 2=visited,empty space
	int sizeX = 5;
	int sizeY = 5;
	void setPoint(int x, int y) {
		setPoint(x, y, 1);
	}
	void setPoint(int x, int y, uint8_t color) {
		uint16_t p = y * sizeX + x;
		uint8_t mask = 3;
		labyrinth[p / 4] = labyrinth[p / 4] & (255 - (mask << ((p % 4) * 2))) | (color << ((p % 4) * 2));
	}
	void clearPoint(int x, int y) {
		setPoint(x, y, 0);
	}
	uint8_t get(int x, int y) {
		uint16_t p = y * sizeX + x;
		uint8_t mask = 3;
		return (labyrinth[p / 4] & (mask << ((p % 4) * 2))) >> ((p % 4) * 2);
	}

	//player
	int posX = 0;
	int posY = 0;
	bool showHints = false;

protected:
	uint8_t icon[20] = { 15, 15, 15, 15, 0, 0, 0, 15, 15, 15, 3, 15, 15, 0, 0, 15, 15, 0, 15, 15 };

public:
	LabyrinthGame();
	uint8_t* getIcon() override {
		return icon;
	}
	void play() override;
	void generate(int sizeX, int sizeY);
};

#endif /* LABYRINTHGAME_H_ */
