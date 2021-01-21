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
#define HAUS_OG 7

#define RUNNING 0
#define GELD 1
#define STATUS 2

class FarmGame: public Game {
	private:
		int posX = 1;
		int posY = 1;
		char state = FARM;
		char globalState = RUNNING;
		unsigned long money = 200;

		void showFarm();
		void enter(char newState, int startPosX, int startPosY);
		inline int rx(int coord){return coord - posX + 2;}
		inline int ry(int coord){return coord - posY + 2;}

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
