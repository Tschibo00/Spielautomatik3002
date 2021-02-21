#ifndef TVGAME_H_
#define TVGAME_H_

#include "Game.h"

class TVGame: public Game {
	private:
		unsigned long start = 0;
		char activeChannel = 0;

		void drawChannel0();
		void drawChannel1();
		void drawChannel2();
		void drawChannel3();
		void drawChannel4();
		void drawChannel5();
		void drawChannel6();
		void drawChannel7();
		void drawChannel8();
		void drawChannel9();
		void drawChannel10();
		void drawChannel11();

	public:
		TVGame();
		void play() override;
};

#endif
