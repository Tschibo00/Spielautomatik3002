#include "KeyboardController.h"

KeyboardController::KeyboardController() {
	uint8_t i;
	for (i = 0; i < 3; i++)
		pinMode(keyLatchPin[i], OUTPUT);
	for (i = 0; i < 4; i++)
		pinMode(keyMatrixPin[i], INPUT_PULLUP);
	for (i = 0; i < 12; i++)
		keyLocked[i] = false;
}

void KeyboardController::scanKeyboard() {
	uint8_t i, j;

	for (j = 0; j < 3; j++) {
		for (i = 0; i < 3; i++)
			digitalWrite(keyLatchPin[i], j != i);
		for (i = 0; i < 4; i++)
			keyStatus[i * 3 + j] = !digitalRead(keyMatrixPin[i]);
	}
}

bool KeyboardController::getKeyStatus(uint8_t key) {
	return keyStatus[key];
}

bool KeyboardController::getKeyClick(uint8_t key) {
	if (getKeyStatus(key)) {
		if (keyLocked[key])
			return false;
		else {
			keyLocked[key] = true;
			return true;
		}
	} else {
		keyLocked[key] = false;
		return false;
	}
}

char KeyboardController::getNumberStatus(){
	for(uint8_t i=0;i<12;i++)
		if (getKeyStatus(i))
			return i;
	return -1;
}

char KeyboardController::getNumberClick(){
	for(uint8_t i=0;i<12;i++)
		if (getKeyClick(i))
			return i;
	return -1;
}

bool KeyboardController::anyKeyClick(uint8_t first, uint8_t last){
	for(uint8_t i=first;i<=last;i++)
		if (getKeyClick(i))
			return true;
	return false;
}
