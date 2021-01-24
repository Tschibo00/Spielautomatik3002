#include <Arduino.h>
#include <avr/pgmspace.h>

#include "BatteryMonitor.h"
#include "DisplayController.h"
#include "KeyboardController.h"
#include "SoundController.h"
#include "GameController.h"
#include "WuerfelGame.h"
#include "AsteroidGame.h"
#include "LabyrinthGame.h"
#include "FarmGame.h"
#include "ToneTest.h"

#if defined(__AVR_ATmega328P__)
// Timer2 is the same on the mega328 and mega168
#define __AVR_ATmega168__
#endif

GameController *gc;

extern char *__brkval;


void setup() {
	pinMode(11, OUTPUT);

	Serial.begin(115200);
	Serial.println(freeMemory());
	initKeyboardController();
	Serial.println(freeMemory());
	initDisplayController();
	Serial.println(freeMemory());
	initSoundController();
	Serial.println(freeMemory());
	gc = new GameController();
	Serial.println(freeMemory());
	gc->registerGame(new WuerfelGame());
	gc->registerGame(new AsteroidGame());
	gc->registerGame(new LabyrinthGame());
	gc->registerGame(new FarmGame());
	gc->registerGame(new ToneTest());
	Serial.println(freeMemory());

	cli(); //disable interrupts
	//set timer1 interrupt at 7,8khz
	TCCR1A = 0;  // set entire TCCR1A register to 0
	TCCR1B = 0;  // same for TCCR1B
	TCNT1 = 0;  //initialize counter value to 0
	// set compare match register for 1hz increments
	OCR1A = 31;    // (16.000.000/64/7800)-1 = 31-1
	// turn on CTC mode
	TCCR1B |= (1 << WGM12);
	// Set CS10 and CS11 bits for 64 prescaler
	TCCR1B |= (1 << CS11) | (1 << CS10);
	// enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A) | (1 << OCIE2A);

	sei();  //enable interrupts

		BatteryMonitor::checkAndShow();

}

int freeMemory(){
	char top;
	return &top - __brkval;
}

ISR(TIMER1_COMPA_vect) {
	soundPlay();
	displayShow();
}

void loop() {
	scanKeyboard();
	gc->play();
}
