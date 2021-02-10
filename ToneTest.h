#ifndef TONETEST_H_
#define TONETEST_H_

#include "Game.h"

class ToneTest: public Game {
	private:
		char ledpos = 0;
	public:
		ToneTest();
		void play() override;
};

#endif /* TONETEST_H_ */
