/*
 * SoundController.cpp
 *
 *  Created on: Jan 5, 2021
 *      Author: hopper
 */
#include "SoundController.h"

SoundController::SoundController() {
	pinMode(SOUNDPLUSPIN, OUTPUT);
	pinMode(SOUNDMINUSPIN, OUTPUT);
	TCCR2A = _BV(WGM21) | _BV(COM2A0);
	TCCR2B = _BV(CS20);
}

void SoundController::tone(int16_t freq, int16_t amp, int16_t freqDelta, int16_t ampDelta) {
	this->freq = freq;
	this->amp = amp;
	this->freqDelta = freqDelta;
	this->ampDelta = ampDelta;
	playNoise = false;
}

void SoundController::noise(int16_t freq, int16_t amp, int16_t freqDelta, int16_t ampDelta) {
	this->freq = freq;
	this->amp = amp;
	this->freqDelta = freqDelta;
	this->ampDelta = ampDelta;
	playNoise = true;
}

void SoundController::music(const char *melody, uint8_t melodyLength, uint16_t noteLength, int16_t amp, int16_t ampDelta) {
	this->melody = melody;
	this->noteLength = noteLength;
	this->notePointer = 0;
	melodyAmp = amp;
	melodyAmpDelta = ampDelta;
	this->melodyLength = melodyLength;
	currentNote = 0;
}

void SoundController::tone(int16_t frequency, int16_t volume) {
	uint8_t prescalarbits = 0b001;
	uint32_t ocr = 0;

	if ((frequency < 250) || (frequency > 10000)) {
		volume = 0;
		frequency = 3000;
	}
	if (volume < 0)
		volume = 0;
	if (volume > 255)
		volume = 255;

	ocr = F_CPU / frequency / 2 - 1;
	prescalarbits = 0b001;  // ck/1: same for both timers
	if (ocr > 255) {
		ocr = F_CPU / frequency / 2 / 8 - 1;
		prescalarbits = 0b010;  // ck/8: same for both timers

		if (ocr > 255) {
			ocr = F_CPU / frequency / 2 / 32 - 1;
			prescalarbits = 0b011;
			if (ocr > 255) {
				ocr = F_CPU / frequency / 2 / 64 - 1;
				prescalarbits = 0b100;
				if (ocr > 255) {
					ocr = F_CPU / frequency / 2 / 128 - 1;
					prescalarbits = 0b101;
					if (ocr > 255) {
						ocr = F_CPU / frequency / 2 / 256 - 1;
						prescalarbits = 0b110;
						if (ocr > 255) {
							// can't do any better than /1024
							ocr = F_CPU / frequency / 2 / 1024 - 1;
							prescalarbits = 0b111;
						}
					}
				}
			}
		}

		TCCR2B = (TCCR2B & 0b11111000) | prescalarbits;

		OCR2A = ocr;
	}
	analogWrite(SOUNDMINUSPIN, 255 - volume);
}

void SoundController::play() {
	slowdown--;
	if (slowdown < 0) {
		slowdown = 7;

		if ((melody != NULL) && (currentNote >= 0)) {
			if (notePointer == 0) {
				if (melody[currentNote] == MUTE)
					tone(0, 0, 0, 0);
				else if (melody[currentNote] != PAUSE)
					tone(notes[melody[currentNote]], melodyAmp, 0, melodyAmpDelta);
			}
			notePointer++;
			if (notePointer >= noteLength) {
				notePointer = 0;
				currentNote++;
				if (currentNote >= melodyLength) {
					melody = NULL;
					currentNote = -1;
				}
			}
		}

		freq += freqDelta;
		amp += ampDelta;
		if (amp < 0)
			amp = 0;
		tone(freq, amp);
	}
	if (playNoise)
		freq = rand() % freq;
}
