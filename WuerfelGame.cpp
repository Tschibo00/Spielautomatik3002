/*
 * WuerfelGame.cpp
 *
 *  Created on: Dec 29, 2020
 *      Author: hopper
 */
#include "WuerfelGame.h"

WuerfelGame::WuerfelGame(DisplayController *dc, KeyboardController *kc, SoundController *sc) {
	this->dc = dc;
	this->kc = kc;
	this->sc = sc;
}

void WuerfelGame::play() {
	char select = kc->getNumberStatus();
	if (select != lastNumber) {
		dc->clear(0);
		if (select >= 0) {
			for (char i = 0; i < numbers[select]; i++)
				setRandomLed();
			sc->tone(500,255,10,-2);
		} else {
			sc->tone(500,255,30,-10);
		}
		lastNumber = select;
		dc->flipBuffer();
	}
}

void WuerfelGame::setRandomLed() {
	char led;
	bool found = false;
	while (!found) {
		led = rand() % 20;
		if (dc->screen[led] == 0) {
			dc->screen[led] = 15;
			found = true;
		}
	}
}
