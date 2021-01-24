#include "GameController.h"
#include <math.h>
#include "WuerfelGame.h"
#include "AsteroidGame.h"
#include "LabyrinthGame.h"
#include "FarmGame.h"
#include "ToneTest.h"
#include "DisplayController.h"
#include "SoundController.h"
#include "avr/pgmspace.h"

#define GAME_COUNT 5
Game *runningGame = NULL;
bool startup = true;
int currentGame = -1;

static const uint8_t icons[5][20] = { { 0, 15, 15, 0, 15, 0, 0, 15, 15, 0, 0, 15, 15, 0, 0, 15, 0, 15, 15, 0 },		// wuerfel
		{ 7, 15, 3, 7, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 15, 0 },				// asteroid
		{ 15, 15, 15, 15, 0, 0, 0, 15, 15, 15, 3, 15, 15, 0, 0, 15, 15, 0, 15, 15 },	// labyrinth
		{ 15, 15, 15, 15, 15, 0, 0, 15, 15, 0, 0, 15, 15, 0, 0, 15, 15, 0, 15, 15 },	// farm
		{ 0, 0, 0, 15, 7, 1, 15, 15, 15, 15, 15, 15, 7, 1, 15, 15, 0, 0, 0, 15 }		// tonetest
};

void initGameController(){
}

void selectGame(int game){
	if ((game >= 0) && (game < GAME_COUNT)) {
		currentGame = game;
	}
}

void initGame(int game){
	startup = false;
	switch (game) {
		case 0:
			runningGame = new WuerfelGame();
			break;
		case 1:
			runningGame = new AsteroidGame();
			break;
		case 2:
			runningGame = new LabyrinthGame();
			break;
		case 3:
			runningGame = new FarmGame();
			break;
		case 4:
			runningGame = new ToneTest();
			break;
	}
}

void play(){
	if (startup) {
		char select = getNumberClick();
		if (select >= 0) {
			if (select == currentGame) {
				initGame(currentGame);
			} else
				selectGame(select);
		}

		if (currentGame >= 0)
			copy(icons[currentGame]);
		else
			showScroller("spielautomatik 3001", (millis() / 200) % 80 - 4, false);
		flipBuffer();
	} else
		runningGame->play();
}
