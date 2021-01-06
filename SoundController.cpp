/*
 * SoundController.cpp
 *
 *  Created on: Jan 5, 2021
 *      Author: hopper
 */
#include "SoundController.h"

void SoundController::tone(int16_t freq, int16_t amp, int16_t freqDelta, int16_t ampDelta) {
	this->freq = freq;
	this->amp = amp;
	this->freqDelta = freqDelta;
	this->ampDelta = ampDelta;
}

void SoundController::play() {
	OCR2A = counter > 0 ? 0 : amp >> 7;
	counter += freq;
	freq += freqDelta;
	amp += ampDelta;
	if (amp < 0)
		amp = 0;
}
