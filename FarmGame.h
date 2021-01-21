#ifndef FARMGAME_H_
#define FARMGAME_H_
#include "Game.h"

#define FARM 0
#define WEIDE 1
#define MARKT 2
#define STALL 3
#define HAUS 4
#define CAGE 5
#define CHICKEN 6
#define SCHWEINE 7
#define HAUS_OG 8

#define RUNNING 0
#define GELD 1
#define STATUS 2
#define KAUFEN 3

class FarmGame: public Game {
	private:
		int posX = 1;
		int posY = 1;
		char state = FARM;
		char globalState = RUNNING;
		char kaufState = HAUS;
		unsigned long statusStart = 0;
		unsigned long blinkKaufen = 0;

		// persistent values
		unsigned long money = 20000;
		char daytime = 0;	// 0=morgen,1=vormittag,2=mittag,3=nachmittag, 4=abend, 5=nacht
		char happiness = 0;
		char hasStall = 1;
		char hasHaus = 0;
		char hasSchweine = 0;
		char hasChicken = 0;
		char hasCage = 0;
		unsigned char day = 0;

		void showFarm();
		void enter(char newState, int startPosX, int startPosY);
		inline int rx(int coord){
			return coord - posX + 2;
		}
		inline int ry(int coord){
			return coord - posY + 2;
		}

		uint8_t faces[60] = { 3, 15, 15, 3, 15, 15, 15, 15, 15, 1, 1, 15, 1, 15, 15, 1, 15, 15, 15, 15, 3, 15, 15, 3, 15, 15, 15, 15, 15, 15,
				15, 15, 1, 1, 1, 1, 15, 15, 15, 15, 3, 15, 15, 3, 15, 15, 15, 15, 15, 15, 15, 15, 1, 15, 15, 1, 15, 1, 1, 15 };
		uint8_t goods[100] = { 15, 15, 15, 15, 15, 0, 0, 15, 15, 0, 0, 15, 15, 0, 0, 15, 15, 15, 15, 15, 0, 0, 15, 0, 0, 15, 15, 15, 15, 15,
				15, 15, 15, 5, 5, 15, 15, 15, 15, 15, 3, 3, 3, 3, 3, 0, 0, 3, 3, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 3, 3, 0,
				0, 3, 3, 0, 0, 3, 3, 3, 3, 3, 15, 15, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 15, 15, 0 };

	protected:
		uint8_t icon[20] = { 15, 15, 15, 15, 15, 0, 0, 15, 15, 0, 0, 15, 15, 0, 0, 15, 15, 0, 15, 15 };

	public:
		FarmGame(DisplayController *dc, KeyboardController *kc, SoundController *sc);
		uint8_t* getIcon() override{
			return icon;
		}
		void play() override;
};
#endif
