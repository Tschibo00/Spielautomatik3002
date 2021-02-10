#include "SoundController.h"

int8_t _slowdown = 0;
int16_t _freq = 0;
int16_t _amp = 0;
int16_t _freqDelta = 0;
int16_t _ampDelta = 0;
int16_t _counter = 0;
bool _playNoise = false;
// music stuff
char *_melody = NULL;
uint8_t _melodyLength = 0;
int16_t _melodyAmp = 0;
int16_t _melodyAmpDelta = 0;
int8_t _currentNote = -1;
int16_t _noteLength = 0;
int16_t _notePointer = -1;

int16_t notes[60] = { 262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932,
		988, 1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976, 2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322,
		3520, 3729, 3951, 4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, 7040, 7459, 7902 };

void initSoundController(){
	pinMode(SOUNDPLUSPIN, OUTPUT);
	pinMode(SOUNDMINUSPIN, OUTPUT);
	TCCR2A = _BV(WGM21) | _BV(COM2A0);
	TCCR2B = _BV(CS20);
}

void tone(int16_t freq, int16_t amp, int16_t freqDelta, int16_t ampDelta){
	_freq = freq;
	_amp = amp;
	_freqDelta = freqDelta;
	_ampDelta = ampDelta;
	_playNoise = false;
}

void noise(int16_t freq, int16_t amp, int16_t freqDelta, int16_t ampDelta){
	_freq = freq;
	_amp = amp;
	_freqDelta = freqDelta;
	_ampDelta = ampDelta;
	_playNoise = true;
}

void music(const char *melody, uint8_t melodyLength, uint16_t noteLength, int16_t amp, int16_t ampDelta){
	_melody = melody;
	_noteLength = noteLength;
	_notePointer = 0;
	_melodyAmp = amp;
	_melodyAmpDelta = ampDelta;
	_melodyLength = melodyLength;
	_currentNote = 0;
}

void tone(int16_t frequency, int16_t volume){
	uint8_t prescalarbits = 0b001;
	uint32_t ocr = 0;

	if ((frequency < 250) || (frequency > 10000)) {
		volume = 0;
		frequency = 3000;
	}
	if (volume < 0) volume = 0;
	if (volume > 255) volume = 255;

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

void soundPlay(){
	_slowdown--;
	if (_slowdown < 0) {
		_slowdown = 7;

		if ((_melody != NULL) && (_currentNote >= 0)) {
			if (_notePointer == 0) {
				if (_melody[_currentNote] == MUTE)
					tone(0, 0, 0, 0);
				else if (_melody[_currentNote] != PAUSE) tone(notes[_melody[_currentNote]], _melodyAmp, 0, _melodyAmpDelta);
			}
			_notePointer++;
			if (_notePointer >= _noteLength) {
				_notePointer = 0;
				_currentNote++;
				if (_currentNote >= _melodyLength) {
					_melody = NULL;
					_currentNote = -1;
				}
			}
		}

		_freq += _freqDelta;
		_amp += _ampDelta;
		if (_amp < 0) _amp = 0;
		if (_playNoise)
			tone(random(_freq), _amp);
		else
			tone(_freq, _amp);
	}
}
