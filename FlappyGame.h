#ifndef FLAPPYGAME_H_
#define FLAPPYGAME_H_

#include "Game.h"

class FlappyGame: public Game {
	private:
		int width = 10;
		int pos = 0;
		int tunnel[4];
		uint16_t level=0;
		uint16_t brake=500;
		unsigned long nextStep=0;

		void initLevel();
		void advanceTunnel();
//		void drawRand

	public:
		FlappyGame();
		void play() override;
};

#endif
