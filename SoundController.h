/*
 * SoundController.h
 *
 *  Created on: Jan 5, 2021
 *      Author: hopper
 */

#ifndef SOUNDCONTROLLER_H_
#define SOUNDCONTROLLER_H_

#include "Arduino.h"

class SoundController {
private:
	int16_t freq;
	int16_t amp=0;
	int16_t freqDelta;
	int16_t ampDelta=0;
	int16_t counter = 0;
public:
	void tone(int16_t freq, int16_t amp,int16_t freqDelta,int16_t ampDelta);
	void play();
};

#endif /* SOUNDCONTROLLER_H_ */
