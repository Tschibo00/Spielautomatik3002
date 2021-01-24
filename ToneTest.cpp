#include "ToneTest.h"

ToneTest::ToneTest(){
}

void ToneTest::play(){
	clear(0);

	if (getKeyClick(0)) music(victoryMusic, 5, 130, 600, -4);
	if (getKeyClick(1)) music(gameOverMusic, 4, 500, 500, -2);
	if (getKeyClick(2)) tone(500, 255, 10, -2);
	if (getKeyClick(3)) tone(500, 255, 30, -10);
	if (getKeyClick(4)) tone(2500, 255, -8, -1);
	if (getKeyClick(5)) noise(5000, 255, -20, -2);
	if (getKeyClick(6)) noise(rand() % 1700 + 200, 170, 0, -4);

	if (getKeyClick(7)) tone(300, 500, 3, -1);
	if (getKeyClick(8)) tone(500, 500, 2000, -1);
	if (getKeyClick(9)) tone(500, 1000, 300, -2);
	if (getKeyClick(10)) noise(14500, 600, 30, -4);
	if (getKeyClick(11)) noise(200, 350, 30, -1);

	flipBuffer();
}
