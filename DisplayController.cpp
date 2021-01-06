#include "DisplayController.h"

DisplayController::DisplayController() {
	uint8_t i;
	for (i = 0; i < 4; i++) {
		pinMode(ledLatchPin[i], OUTPUT);
	}
	for (i = 0; i < 5; i++) {
		pinMode(ledMatrixPin[i], OUTPUT);
	}
	for (i = 0; i < 20; i++) {
		_displayInternal[i] = 0;
		screen[i] = 0;
	}
}

void DisplayController::show() {
	uint8_t i;

	if (displayWait == 0)
		for (i = 0; i < 4; i++)
			digitalWrite(ledLatchPin[i], HIGH);

	for (i = 0; i < 5; i++) {
		digitalWrite(ledMatrixPin[i], _displayInternal[i * 4 + currentRow] > displayWait);
	}
	if (displayWait == 0)
		digitalWrite(ledLatchPin[currentRow], LOW);

	displayWait++;
	if (displayWait == 15) {
		displayWait = 0;
		currentRow++;
		if (currentRow > 3)
			currentRow = 0;
	}
}

void DisplayController::flipBuffer() {
	for (uint8_t i = 0; i < 20; i++)
		_displayInternal[i] = screen[i];
}

void DisplayController::clear(uint8_t color) {
	for (uint8_t i = 0; i < 20; i++)
		screen[i] = color;
}

void DisplayController::copy(const uint8_t *source) {
	copy((const char*)source);
}

void DisplayController::copy(const char *source) {
	for (uint8_t i = 0; i < 20; i++)
		if ((source[i] >= 0) && (source[i] < 16))
			screen[i] = source[i];
}
