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
			dc->showNumber(13);
			break;
		case 2:
			dc->showNumber(471);
			break;
		case 3:
			dc->showNumber(8935);
			break;
		case 4:
			dc->showNumber(32767);
			break;
		case 5:
			dc->showNumber(-9);
			break;
		case 6:
			dc->showNumber(-12);
			break;
		case 7:
			dc->showNumber(-128);
			break;
		case 8:
			dc->showNumber(-3284);
			break;
		case 9:
			dc->showNumber(-32000);
			break;
		case 10:
			break;
		case 11:
			sc->tone(7000,10000,-4,-1);
			break;
		}
//		dc->screen[(select%3)+(select/3)*4] = 15;
	}

	dc->flipBuffer();
}
