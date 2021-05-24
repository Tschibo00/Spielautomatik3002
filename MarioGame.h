#ifndef MARIOGAME_H_
#define MARIOGAME_H_

#include "Game.h"

class MarioGame: public Game {
	private:
		uint8_t *obstacles = NULL;

		char posX = 0;
		int posY=0;
		uint16_t level = 1;
		char currentObstacle;
		char currentPhase;
		char lastPhase;

		void initLevel();
		void move();
		void draw();

	public:
		MarioGame();
		void play() override;
		void loose();
};

#endif
