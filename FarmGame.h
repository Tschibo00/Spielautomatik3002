#ifndef FARMGAME_H_
#define FARMGAME_H_
#include "Game.h"

#define FARM 0
#define HAUS_OG 1
#define MARKT 2
#define STALL 5
#define HAUS 6
#define CAGE 7
#define CHICKEN 8
#define SCHWEINE 9
#define WEIDE 10

#define RUNNING 0
#define GELD 1
#define STATUS 2
#define KAUFEN 3
#define WERBUNG 4
#define VERKAUFEN 5

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
		unsigned long stopTimeDisplay = 0;

		char *tierPosX = NULL;
		char *tierPosY = NULL;
		char *tierType = NULL;
		uint16_t totalTiere = 0;

		// persistent values
		unsigned long money = 20000;
		char daytime = 0;	// 0=morgen,1=vormittag,2=mittag,3=nachmittag, 4=abend, 5=nacht
		uint16_t day = 1;
		char happiness = 0;
		uint16_t hasStall = 1;
		uint16_t hasHaus = 0;
		uint16_t hasSchweine = 0;
		uint16_t hasChicken = 0;
		uint16_t hasCage = 0;
		uint16_t cows = 1;
		uint16_t pigs = 0;
		uint16_t sheep = 0;
		uint16_t chicken = 0;
		uint16_t birds = 0;
		uint16_t cowsFood = 0;
		uint16_t pigsFood = 0;
		uint16_t sheepFood = 0;
		uint16_t chickenFood = 0;
		uint16_t birdsFood = 0;
		bool cowsFed = false;
		bool pigsFed = false;
		bool sheepFed = false;
		bool chickenFed = false;
		bool birdsFed = false;
		uint16_t cowsProduct = 0;
		uint16_t sheepProduct = 0;
		uint16_t pigsProduct = 0;
		uint16_t chickenProduct = 0;

		void showFarm();
		void enter(char newGlobalState, char newState, int startPosX, int startPosY);
		void initTiere(uint16_t count, char type, int x, int y, int dx, int dy);
		void drawTiere();
		void deinitTiere();
		void logPosition();
		void tierSound(char tierType, bool force);
		inline int rx(int coord){
			return coord - posX + 2;
		}
		inline int ry(int coord){
			return coord - posY + 2;
		}
		void increaseDayTime();
		bool productKaufen(uint16_t *product, uint16_t price, uint16_t amount);
		void productVerkaufen(uint16_t *product, uint16_t price);
		bool onMarktTiere();
		bool onMarktFutter();
		void drawDaytime();

	public:
		FarmGame();
		void play() override;
};
#endif
