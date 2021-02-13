#include "WallGame.h"

WallGame::WallGame(){
	initLevel();
}

void WallGame::play(){
	clear(0);

	move();
	if (millis() >= nextStep) {
		nextStep = millis() + brake;
		advanceWall();
	}
	draw();

	flipBuffer();
}

void WallGame::initLevel(){
	pos = 1;
	progress = 0;
	brake = 400 - level * 40;
	if (brake < 10) brake = 10;
}

void WallGame::advanceWall(){
	if (jumping > 0) jumping--;
	noise(random(1700) + 200, 170, 0, -4);
	wallPos++;
	if (wallPos == 5) {
		wallPos = 0;
		loch = random(5) - 1;
	}
	progress++;
}

void WallGame::move(){
	switch (getNumberClick()) {
		case 3:
			if (pos > 0) pos--;
			break;
		case 5:
			if (pos < 3) pos++;
			break;
		case 4:
			tone(300, 500, 3, -1);
			jumping = 2;
			break;
	}

	if (progress == 125) {
		victory(level, 9, 11);
		level++;
		initLevel();
	}

	bool lost = false;
	if (loch < 0) {
		if (jumping == 0 && wallPos == 4) lost = true;
	} else {
		if (wallPos == 4 && pos != loch) lost = true;
	}

	if (lost) {
		gameover(level);
		level = 1;
		initLevel();
	}
}

void WallGame::draw(){
	if (loch < 0)
		drawLine(0, wallPos, 1, 0, 4, 8);
	else {
		drawLine(0, wallPos, 1, 0, 4, 3);
		set(loch, wallPos, 0);
	}
	set(pos, 4, (millis() / 100) % 3 + 1);		//player
}
