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

// global states
#define RUNNING 0
#define GELD 1
#define STATUS 2
#define KAUFEN 3
#define WERBUNG 4
#define VERKAUFEN 5
#define FUETTERN 6
#define FUTTERMENGE 7

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
		uint8_t currentPlayer = 0;

		char *tierPosX = NULL, *tierPosY = NULL, *tierType = NULL;
		uint16_t totalTiere = 0;
		int16_t tierMinX = 0, tierMaxX = 0, tierMinY = 0, tierMaxY = 0;
		bool cowsHarvested = false;
		bool sheepHarvested = false;
		bool pigsHarvested = false;
		bool chickenHarvested = false;

		struct SaveGame {
				unsigned long money;
				char daytime;	// 0=morgen,1=vormittag,2=mittag,3=nachmittag, 4=abend, 5=nacht
				uint16_t day;
				uint16_t hasStall;
				uint16_t hasHaus;
				uint16_t hasSchweine;
				uint16_t hasChicken;
				uint16_t hasCage;
				uint16_t cows;
				uint16_t pigs;
				uint16_t sheep;
				uint16_t chicken;
				uint16_t birds;
				uint16_t cowsFood;
				uint16_t pigsFood;
				uint16_t sheepFood;
				uint16_t chickenFood;
				uint16_t birdsFood;
				bool cowsFed;
				bool pigsFed;
				bool sheepFed;
				bool chickenFed;
				bool birdsFed;
				uint16_t cowsProduct;
				uint16_t sheepProduct;
				uint16_t pigsProduct;
				uint16_t chickenProduct;
		};
		// persistent values
		SaveGame g;

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
		bool tierImStall();
		uint16_t getPriceFarm(char product);
		uint16_t getPriceTier(char product);
		uint16_t getPriceFutter(char product);
		char getHappiness();		// total happiness (-1 unhappy, 0 neutal, 1 happy)
		void updateSaveGame();

	public:
		FarmGame();
		void play() override;
		bool needsPlayerSelection() override{
			return true;
		}
		void selectPlayer(uint8_t player, bool resetSavedGame) override;
};
#endif
