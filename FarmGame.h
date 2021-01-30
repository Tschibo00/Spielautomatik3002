#ifndef FARMGAME_H_
#define FARMGAME_H_
#include "Game.h"

#define FARM 0
#define HAUS_OG 1
#define MARKT 2
#define STALL 3
#define HAUS 4
#define CAGE 5
#define CHICKEN 6
#define SCHWEINE 7
#define WEIDE 8

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

#define MAX_TIER 100		// shouldn't be more than 100 to avoid memory problems

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

		char *tierPosX = NULL;
		char *tierPosY = NULL;
		char *tierType = NULL;
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
		uint16_t cows = 1;
		uint16_t pigs = 0;
		uint16_t sheep = 0;
		uint16_t chicken = 0;
		uint16_t birds = 0;

		void showFarm();
		void enter(char newState, int startPosX, int startPosY);
		void initTiere(uint16_t count, char type, int x, int y, int dx, int dy);
		void drawTiere();
		void deinitTiere();
		void logPosition();
		void tierSound(char tierType);
		inline int rx(int coord){
			return coord - posX + 2;
		}
		inline int ry(int coord){
			return coord - posY + 2;
		}

	public:
		FarmGame();
		void play() override;
};
#endif
