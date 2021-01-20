#ifndef FARMGAME_H_
#define FARMGAME_H_
#include "Game.h"

#define FARM 0
#define STALL 1
#define WEIDE 2
#define MARKT 3
#define HAUS 4
#define CAGE 5
#define CHICKEN 6

class FarmGame: public Game {
	private:
		int posX = 0;
		int posY = 0;
		char state = FARM;

		void show();
		void drawLine(int x, int y, int dx, int dy, int steps, char color);
		void drawBox(int x, int y, int width, int height, char color);

	protected:
		uint8_t icon[20] = { 7, 15, 3, 7, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 15, 0 };

	public:
		FarmGame(DisplayController *dc, KeyboardController *kc, SoundController *sc);
		uint8_t* getIcon() override{
			return icon;
		}
		void play() override;
};
#endif
