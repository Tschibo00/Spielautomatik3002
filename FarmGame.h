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
#define WERBUNG 4

#define NONE -1
#define KUH 0
#define SCHAF 1
#define SCHWEIN 2
#define HUHN 3
#define VOGEL 4

#define MAX_TIER 10

class FarmGame: public Game {
	private:
		int posX = 1;
		int posY = 1;
		char state = FARM;
		char globalState = RUNNING;
		char kaufState = HAUS;
		unsigned long statusStart = 0;
		unsigned long blinkKaufen = 0;
		unsigned long werbungStart = 0;
		char werbungPointer = 0;
/*
		char *tierPosX = NULL;
		char *tierPosY = NULL;
		char *tierType = NULL;
*/
		char tierPosX [MAX_TIER];
		char tierPosY [MAX_TIER];
		char tierType [MAX_TIER];
		uint16_t totalTiere = 0;

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
		uint16_t cows = 4;
		uint16_t pigs = 0;
		uint16_t sheep = 2;
		uint16_t chicken = 0;
		uint16_t birds = 0;

		void showFarm();
		void enter(char newState, int startPosX, int startPosY);
		void initTiere(uint16_t count, char type, int x, int y, int dx, int dy);
		void drawTiere();
		void deinitTiere();
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
		uint8_t werbung[140] = { 0, 0, 0, 0, 15, 15, 15, 15, 15, 0, 0, 15, 15, 15, 15, 15, 15, 0, 0, 15, 15, 0, 15, 0, 0, 15, 0, 15, 15, 0,
				15, 0, 0, 15, 0, 15, 15, 0, 15, 0, 15, 0, 0, 0, 15, 15, 15, 15, 0, 0, 0, 15, 15, 15, 15, 15, 15, 0, 0, 0, 0, 15, 15, 0, 15, 0,
				0, 15, 15, 0, 0, 15, 0, 15, 15, 0, 2, 2, 2, 2, 0, 15, 15, 0, 0, 15, 15, 0, 15, 15, 15, 15, 15, 2, 2, 15, 15, 15, 15, 15, 0, 15,
				15, 0, 15, 2, 2, 15, 2, 15, 15, 2, 15, 2, 2, 15, 0, 15, 15, 0, 15, 15, 15, 15, 15, 2, 2, 15, 15, 2, 2, 15, 15, 2, 2, 15, 15, 2,
				15, 15 };

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
