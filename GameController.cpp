#include "GameController.h"
#include <math.h>

GameController::GameController() {
}

void GameController::registerGame(Game *game) {
	if (gameCount < 10) {
		gameList[gameCount] = game;
		gameCount++;
	}
}

void GameController::selectGame(int game) {
	if ((game >= 0) && (game < gameCount)) {
		currentGame = game;
	}
}

void GameController::play() {
	if (startup) {
		char select = getNumberClick();
		if (select >= 0) {
			if (select == currentGame)
				startup = false;
			else
				selectGame(select);
		}

		if (currentGame >= 0)
			copy(gameList[currentGame]->getIcon());
		else
			showScroller("spielautomatik 3001", (millis() / 200) % 80 - 4, false);
		flipBuffer();
	} else
		gameList[currentGame]->play();
}
