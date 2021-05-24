#include "MarioGame.h"

static const uint8_t manX[20] = { 0, 1, 2, 3, 3, 3, 3, 2, 1, 1, 0, 0, 1, 2, 2, 2, 1, 0, 1, 1 };
static const int8_t manY[20] = { 0, 0, 0, 0, 1, 2, 3, 3, 3, 4, 4, 3, 3, 3, 2, 1, 1, 1, 1, 0 };

MarioGame::MarioGame(){
	obstacles = (uint8_t*) malloc(60);
	initLevel();
}

void MarioGame::play(){
	clear(0);
	move();
	draw();
	flipBuffer();
}

void MarioGame::initLevel(){
	posX = 1;
	posY = 0;
	for (char i = 0; i < 60; i++)
		obstacles[i] = random(6);
}

void MarioGame::move(){
	switch (getNumberClick()) {
		case 0:
			posX--;
			posY++;
			break;
		case 1:
			posY++;
			break;
		case 2:
			posX++;
			posY++;
			break;
		case 3:
			posX--;
			break;
		case 5:
			posX++;
			break;
		case 6:
			posX--;
			posY--;
			break;
		case 7:
			posY--;
			break;
		case 8:
			posX++;
			posY--;
			break;
	}
	if (posY < 0) posY = 0;
	if (posX < 0) posX = 0;
	if (posX > 3) posX = 3;

	if (posY % 10 == 0) currentObstacle = obstacles[posY / 10];
	if (currentPhase != lastPhase) {
		if (currentObstacle == 2 || currentObstacle == 5) tone(500, 255, 30, -10);
		if (currentObstacle == 1 && currentPhase == 7) tone(2500, 255, -8, -1);
		if (currentObstacle == 4 && currentPhase == 0) tone(2500, 255, -8, -1);
		if ((currentObstacle == 0 || currentObstacle == 3) && currentPhase == 0) noise(5000, 255, -20, -2);
		if ((currentObstacle == 0 || currentObstacle == 3) && currentPhase == 4) tone(300, 500, 2, -1);
		lastPhase = currentPhase;
	}

	switch (currentObstacle) {
		case 0:
		case 3:
			currentPhase = (millis() / (200 - level * 4)) % 20;
			if ((posY % 10 >= 5) && (posY % 10 < 9) && currentPhase < 5) loose();
			break;
		case 1:
			currentPhase = (millis() / (100 - level * 4)) % 20;
			if (posY % 10 == 8 && (10 - currentPhase == posX || 9 - currentPhase == posX)) loose();
			break;
		case 4:
			currentPhase = (millis() / (100 - level * 4)) % 20;
			if (posY % 10 == 8 && (currentPhase == posX || currentPhase - 1 == posX)) loose();
			break;
		case 2:
			currentPhase = (millis() / (400 - level * 4)) % 20;
			if ((manX[currentPhase] == posX) && (manY[currentPhase] + (posY % 10) == 8)) loose();
			break;
		case 5:
			currentPhase = (millis() / (400 - level * 4)) % 20;
			if ((manX[19 - currentPhase] == posX) && (manY[19 - currentPhase] + (posY % 10) == 8)) loose();
			break;
	}

	if (posY >= level * 5 + 40) {
		victory(level, 9, 11);
		level++;
		initLevel();
	}
}

void MarioGame::loose(){
	gameover(level);
	level = 1;
	initLevel();
}

void MarioGame::draw(){
	for (char y = 0; y < 5; y++)
		for (char x = 0; x < 4; x++)
			set(x, y, ((posY + 10 - y) / 2) % 2 == (x / 2) % 2);

	set(posX, 4, (millis() / 50) % 3 + 1);		//player
	char obstaclePos = posY % 10 - 4;
	char pos;
	switch (currentObstacle) {
		case 0:
		case 3:
			pos = 4 - currentPhase / 5;
			drawBox(0, obstaclePos, 4, pos, pos + 2);
			break;
		case 1:
			set(10 - currentPhase, obstaclePos, 3);
			set(9 - currentPhase, obstaclePos, 3);
			break;
		case 4:
			set(currentPhase, obstaclePos, 3);
			set(currentPhase - 1, obstaclePos, 3);
			break;
		case 2:
			set(manX[currentPhase], manY[currentPhase] + obstaclePos, 15);
			break;
		case 5:
			set(manX[19 - currentPhase], manY[19 - currentPhase] + obstaclePos, 15);
			break;
	}
}
