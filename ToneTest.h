/*
 * ToneTest.h
 *
 *  Created on: Jan 5, 2021
 *      Author: hopper
 */

#ifndef TONETEST_H_
#define TONETEST_H_

#include "Game.h"

class ToneTest: public Game {
	protected:
		uint8_t icon[20] = { 0, 0, 0, 15, 7, 1, 15, 15, 15, 15, 15, 15, 7, 1, 15, 15, 0, 0, 0, 15 };

	public:
		ToneTest(DisplayController *dc, KeyboardController *kc, SoundController *sc);
		uint8_t* getIcon() override{
			return icon;
		}
		void play() override;
};

#endif /* TONETEST_H_ */
