/*
 * SoundController.h
 *
 *  Created on: Jan 5, 2021
 *      Author: hopper
 */

#ifndef SOUNDCONTROLLER_H_
#define SOUNDCONTROLLER_H_

#include "Arduino.h"

#define SOUNDPLUSPIN 11
#define SOUNDMINUSPIN 5
#define PAUSE -1
#define MUTE -2

class SoundController {
private:
	int8_t slowdown = 0;
	int16_t freq = 0;
	int16_t amp = 0;
	int16_t freqDelta = 0;
	int16_t ampDelta = 0;
	int16_t counter = 0;
	bool playNoise = false;
	// music stuff
	char *melody=NULL;
	uint8_t melodyLength=0;
	int16_t melodyAmp=0;
	int16_t melodyAmpDelta=0;
	int8_t currentNote=-1;
	int16_t noteLength=0;
	int16_t notePointer=-1;

	void tone(int16_t frequency, int16_t volume);

public:
	SoundController();
	void tone(int16_t freq, int16_t amp, int16_t freqDelta, int16_t ampDelta);
	void noise(int16_t freq, int16_t amp, int16_t freqDelta, int16_t ampDelta);
	void music(char *melody, uint8_t melodyLength, uint16_t noteLength, int16_t amp, int16_t ampDelta);
	void play();

	int16_t notes[60] = { 262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932,
			988, 1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976, 2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322,
			3520, 3729, 3951, 4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, 7040, 7459, 7902 };
};

#endif /* SOUNDCONTROLLER_H_ */
