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


	if (kc->getKeyClick(0)){



		sc->music(music, 4,500,500,-2);




	}



//	if (kc->getKeyClick(0))freq-=250;
	if (kc->getKeyClick(1))freq=3000;
	if (kc->getKeyClick(2))freq+=250;

	if (kc->getKeyClick(3))freqDelta-=1;
	if (kc->getKeyClick(4))freqDelta-=10;
	if (kc->getKeyClick(5))freqDelta-=100;
	if (kc->getKeyClick(6))freqDelta+=1;
	if (kc->getKeyClick(7))freqDelta+=10;
	if (kc->getKeyClick(8))freqDelta+=100;

	char select=kc->getNumberStatus();
	if ((select>=0)&&(select<3))dc->showNumber(freq);
	if ((select>=3)&&(select<9))dc->showNumber(freqDelta);

	if(kc->getKeyClick(9)){sc->tone(freq, 32000, freqDelta, -100);log();}
	if(kc->getKeyClick(10)){sc->tone(freq, 32000, freqDelta, -10);log();}
	if(kc->getKeyClick(11)){sc->tone(freq, 32000, freqDelta, -3);log();}

	dc->flipBuffer();
}

void ToneTest::log(){
	Serial.print("freq: ");
	Serial.print(freq);
	Serial.print(" delta ");
	Serial.println(freqDelta);
}

/*
 * freq	delta	amp	delta	sound
 * === asteroid ===
 * 10000	-6		32000	-10	shot 1
 * 10250	-3		32000	-15	shot 2
 * 10000	-3400	32000	-7		explosion
 * === wuerfel ===
 * 3250	3		32000	-3		boing (druck)
 * 3250	600	32000	-12	zisch (loslassen)
 * === music ===
 * 500	-1		32000	-80	bassdrum
 * 6250	10000	20000	-150	closed hi
 * 6250	10000	20000	-20	open hi
 * 6250	10000	32000	-40	snare
 * 4000	-2		32000	-10	quaak
 */
