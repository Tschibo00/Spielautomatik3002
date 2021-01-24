#ifndef GAME_H_
#define GAME_H_

#include <Arduino.h>
#include "DisplayController.h"
#include "KeyboardController.h"
#include "SoundController.h"

static const char victoryScreen[20] = { 0, 0, 15, 15, 0, 0, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 };
static const char gameOverScreen[20] = { 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 0, 0, 15, 15, 0, 0, 15, 15 };
static const char gameOverMusic[4] = { 10, 9, 8, 7 };
static const char victoryMusic[5] = { 13, PAUSE, 13, 13, 18 };

class Game {
	private:
		char scroller[9];
		void levelEnd(const char *screen, int level, char keyFirst, char keyLast){
			unsigned long start = millis();
			sprintf(scroller, "Level%d", level);
			while (!anyKeyClick(keyFirst, keyLast)) {
				if (millis() - start < 1000L)
					copy(screen);
				else
					showScroller(scroller, ((millis() - start) / 200 + 19) % 32 - 4, true);
				flipBuffer();
				scanKeyboard();
			}
		}

	protected:
		void victory(int level, char keyFirst, char keyLast){
			music(victoryMusic, 5, 130, 600, -4);
			levelEnd(victoryScreen, level, keyFirst, keyLast);
		}

		void gameover(int level){
			music(gameOverMusic, 4, 500, 500, -2);
			levelEnd(gameOverScreen, level, 0, 11);
		}

	public:
		virtual ~Game(){
		}
		;
		virtual void play();
};

#endif /* GAME_H_ */
