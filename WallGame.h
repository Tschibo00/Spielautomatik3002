#ifndef WALLGAME_H_
#define WALLGAME_H_

#include "Game.h"

class WallGame: public Game {
	private:
		uint8_t wallPos;
		uint8_t pos = 0;
		char jumping = 0;
		char loch = 0;
		uint16_t level = 1;
		uint16_t brake = 500;
		uint16_t progress = 0;
		unsigned long nextStep = 0;

		void initLevel();
		void advanceWall();
		void move();
		void draw();
		void initWall();

	public:
		WallGame();
		void play() override;
};

#endif
