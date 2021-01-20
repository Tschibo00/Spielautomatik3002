/*
 * ToneTest.cpp
 *
 *  Created on: Jan 5, 2021
 *      Author: hopper
 */

#include "ToneTest.h"

ToneTest::ToneTest(DisplayController *dc, KeyboardController *kc, SoundController *sc){
	this->dc = dc;
	this->kc = kc;
	this->sc = sc;
}

void ToneTest::play(){
	dc->clear(0);

	if (kc->getKeyClick(0)) sc->music(victoryMusic, 5, 130, 600, -4);
	if (kc->getKeyClick(1)) sc->music(gameOverMusic, 4, 500, 500, -2);
	if (kc->getKeyClick(2)) sc->tone(500, 255, 10, -2);
	if (kc->getKeyClick(3)) sc->tone(500, 255, 30, -10);
	if (kc->getKeyClick(4)) sc->tone(2500, 255, -8, -1);
	if (kc->getKeyClick(5)) sc->noise(5000, 255, -20, -2);
	if (kc->getKeyClick(6)) sc->noise(rand() % 1700 + 200, 170, 0, -4);

	if (kc->getKeyClick(7)) sc->tone(300, 500, 3, -1);
	if (kc->getKeyClick(8)) sc->tone(500, 500, 2000, -1);
	if (kc->getKeyClick(9)) sc->tone(500, 1000, 300, -2);
	if (kc->getKeyClick(10)) sc->noise(14500, 600, 30, -4);
	if (kc->getKeyClick(11)) sc->noise(200, 350, 30, -1);

	dc->flipBuffer();
}
