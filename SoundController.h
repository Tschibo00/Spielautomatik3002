#ifndef SOUNDCONTROLLER_H_
#define SOUNDCONTROLLER_H_

#include "Arduino.h"

#define SOUNDPLUSPIN 11
#define SOUNDMINUSPIN 5
#define PAUSE -1
#define MUTE -2

void initSoundController();
void tone(int16_t frequency, int16_t volume);
void tone(int16_t freq, int16_t amp, int16_t freqDelta, int16_t ampDelta);
void noise(int16_t freq, int16_t amp, int16_t freqDelta, int16_t ampDelta);
void music(const char *melody, uint8_t melodyLength, uint16_t noteLength, int16_t amp, int16_t ampDelta);
void soundPlay();

#endif /* SOUNDCONTROLLER_H_ */
