/*
 * ToneTest.cpp
 *
 *  Created on: Jan 5, 2021
 *      Author: hopper
 */

#include "ToneTest.h"

ToneTest::ToneTest(DisplayController *dc, KeyboardController *kc, SoundController *sc) {
	this->dc = dc;
	this->kc = kc;
	this->sc = sc;
}

void ToneTest::play() {
	char select = kc->getNumberStatus();
	dc->clear(0);

	if (select >= 0) {
		switch (select) {
		case 0:
			sc->tone(0,0,0,0);
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			sc->tone(7000,10000,-4,-1);
			break;
		case 10:
			break;
		case 11:
			break;
		}
		dc->screen[(select%3)+(select/3)*4] = 15;
	}

	dc->flipBuffer();
}
