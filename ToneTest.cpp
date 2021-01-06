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
	dc->clear(0);

	if (kc->getKeyClick(0))freq-=100;
	if (kc->getKeyClick(1))freq=3000;
	if (kc->getKeyClick(2))freq+=100;

	if (kc->getKeyClick(3))freqDelta-=1;
	if (kc->getKeyClick(4))freqDelta-=10;
	if (kc->getKeyClick(5))freqDelta-=100;
	if (kc->getKeyClick(6))freqDelta+=1;
	if (kc->getKeyClick(7))freqDelta+=10;
	if (kc->getKeyClick(8))freqDelta+=100;

	char select=kc->getNumberStatus();
	if ((select>=0)&&(select<3))dc->showNumber(freq);
	if ((select>=3)&&(select<9))dc->showNumber(freqDelta);

	if(kc->getKeyClick(9)){sc->tone(freq, 32000, freqDelta, -8);log();}
	if(kc->getKeyClick(10)){sc->tone(freq, 32000, freqDelta, -3);log();}
	if(kc->getKeyClick(11)){sc->tone(0,0,0,0);log();}

	dc->flipBuffer();
}

void ToneTest::log(){
	Serial.print("freq: ");
	Serial.print(freq);
	Serial.print(" delta ");
	Serial.println(freqDelta);
}
