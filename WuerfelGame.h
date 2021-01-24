#ifndef WUERFELGAME_H_
#define WUERFELGAME_H_

#include "Game.h"

class WuerfelGame: public Game {
	private:
		uint8_t numbers[12] = { 16, 20, 10, 13, 2, 7, 19, 12, 3, 17, 5, 1 };
		void setRandomLed();
		char lastNumber = -1;

	public:
		WuerfelGame();
		void play() override;
};

#endif /* WUERFELGAME_H_ */
