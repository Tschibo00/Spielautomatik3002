#ifndef FLAPPYGAME_H_
#define FLAPPYGAME_H_

#include "Game.h"

class FlappyGame: public Game {
	private:
		int width = 10;
		int pos = 0;
		int16_t tunnelUp[4];
		int16_t tunnelDown[4];
		uint16_t level = 1;
		uint16_t brake = 500;
		uint16_t progress = 0;
		unsigned long nextStep = 0;

		void initLevel();
		void advanceTunnel();
		void drawRand(char x, int y, bool down, char color);
		void move();
		void drawTunnel();

	public:
		FlappyGame();
		void play() override;
};

#endif
