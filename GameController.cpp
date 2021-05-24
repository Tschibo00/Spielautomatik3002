#include "GameController.h"

#include <math.h>
#include "avr/pgmspace.h"

#include "DisplayController.h"
#include "SoundController.h"
#include "AsteroidGame.h"
#include "FarmGame.h"
#include "LabyrinthGame.h"
//#include "ToneTest.h"
#include "MarioGame.h"
#include "FlappyGame.h"
#include "WallGame.h"
//#include "TVGame.h"
#define GAME_COUNT 8

Game *runningGame = NULL;
char state = GAME_STARTUP;
int currentGame = -1;
char selectedPlayer = 0;
bool resetPlayer = false;

static const uint8_t icons[8][20] = { { 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 1, 1, 1, 1, 1, 1, 8, 1 },		// mario
		{ 7, 15, 3, 7, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 15, 0 },				// asteroid
		{ 15, 15, 15, 15, 0, 0, 0, 15, 15, 15, 3, 15, 15, 0, 0, 15, 15, 0, 15, 15 },	// labyrinth
		{ 15, 15, 15, 15, 15, 0, 0, 15, 15, 0, 0, 15, 15, 0, 0, 15, 15, 0, 15, 15 },	// farm
		{ 0, 2, 0, 0, 0, 0, 5, 0, 1, 3, 7, 15, 0, 0, 5, 0, 0, 2, 0, 0 }, 					// flappy
		{ 0, 0, 0, 0, 8, 8, 8, 8, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0 }//,					// wall
//		{ 15, 15, 15, 15, 15, 1, 3, 15, 15, 3, 1, 15, 15, 15, 15, 15, 8, 0, 0, 8 },	// TV
	//	{ 0, 0, 0, 15, 7, 1, 15, 15, 15, 15, 15, 15, 7, 1, 15, 15, 0, 0, 0, 15 }		// tonetest
};

void initGameController(){
}

void selectGame(int game){
	if ((game >= 0) && (game < GAME_COUNT)) {
		currentGame = game;
	}
}

void initGame(int game){
	switch (game) {
		case 0:
			runningGame = new MarioGame();
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
			runningGame = new FlappyGame();
			break;
		case 5:
			runningGame = new WallGame();
			break;
/*		case 6:
			runningGame = new TVGame();
			break;
		case 7:
			runningGame = new ToneTest();
			break;*/
	}
	if (runningGame->needsPlayerSelection())
		state = GAME_PLAYER_SELECTION;
	else
		state = GAME_RUNNING;
}

void play(){
	char select;
	switch (state) {
		case GAME_STARTUP:
			select = getNumberClick();
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
			break;
		case GAME_PLAYER_SELECTION:
			clear(0);
			select = getNumberClick();
			if (select == 3) {
				resetPlayer = false;
				selectedPlayer--;
				if (selectedPlayer < 0) selectedPlayer = 2;
				Serial.println(selectedPlayer);
			}
			if (select == 5) {
				resetPlayer = false;
				selectedPlayer++;
				if (selectedPlayer > 2) selectedPlayer = 0;
				Serial.println(selectedPlayer);
			}
			if (select == 4) {
				runningGame->selectPlayer(selectedPlayer, resetPlayer);
				state = GAME_RUNNING;
				Serial.println("start");
			}
			if (select == 11) resetPlayer = !resetPlayer;
			showCharacter(selectedPlayer + 49);
			if (resetPlayer) strike();
			flipBuffer();
			break;
		case GAME_RUNNING:
			runningGame->play();
			break;
	}
}
