#include "ToneTest.h"

ToneTest::ToneTest(){
}

void ToneTest::play(){
	clear(0);

	char key = getNumberClick();

	if (key >= 0) ledpos = (ledpos + 1) % 20;
/*
	switch (key) {
		case 0:
			music(victoryMusic, 5, 130, 600, -4);
			break;
		case 1:
			music(gameOverMusic, 4, 500, 500, -2);
			break;
		case 2:
			tone(500, 255, 10, -2);
			break;
		case 3:
			tone(500, 255, 30, -10);
			break;
		case 4:
			tone(2500, 255, -8, -1);
			break;
		case 5:
			noise(5000, 255, -20, -2);
			break;
		case 6:
			noise(rand() % 1700 + 200, 170, 0, -4);
			break;
		case 7:
			tone(300, 500, 3, -1);
			break;
		case 8:
			tone(500, 500, 2000, -1);
			break;
		case 9:
			tone(500, 1000, 300, -2);
			break;
		case 10:
			noise(14500, 600, 30, -4);
			break;
		case 11:
			noise(200, 350, 30, -1);
			break;
	}
*/

	getScreen()[ledpos] = 15;

	flipBuffer();
}
