/*
 * WuerfelGame.cpp
 *
 *  Created on: Dec 29, 2020
 *      Author: hopper
 */
#include "WuerfelGame.h"

WuerfelGame::WuerfelGame() {
}

void WuerfelGame::play() {
	char select = getNumberStatus();
	if (select != lastNumber) {
		clear(0);
		if (select >= 0) {
			for (char i = 0; i < numbers[select]; i++)
				setRandomLed();
			tone(500, 255, 10, -2);
		} else {
			tone(500, 255, 30, -10);
		}
		lastNumber = select;
		flipBuffer();
	}
}

void WuerfelGame::setRandomLed() {
	char led;
	bool found = false;
	while (!found) {
		led = rand() % 20;
		if (getScreen()[led] == 0) {
			getScreen()[led] = 15;
			found = true;
		}
	}
}
