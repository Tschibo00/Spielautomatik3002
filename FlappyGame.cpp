#include "FlappyGame.h"

FlappyGame::FlappyGame(){
	initLevel();
}

void FlappyGame::play(){
	clear(0);

	move();
	if (millis() >= nextStep) {
		nextStep = millis() + brake;
		advanceTunnel();
	}
	drawTunnel();

	flipBuffer();
}

void FlappyGame::initLevel(){
	width = 10;
	pos = 0;
	progress = 0;
	brake = 300 - level * 20;
	if (brake < 10) brake = 10;
	for (char i = 0; i < 4; i++) {
		tunnelUp[i] = -5;
		tunnelDown[i] = 5;
	}
}

void FlappyGame::advanceTunnel(){
	for (char x = 1; x < 4; x++) {
		tunnelUp[x - 1] = tunnelUp[x];
		tunnelDown[x - 1] = tunnelDown[x];
	}
	switch (random(3)) {
		case 0:
			tunnelUp[3] = tunnelUp[2] - 1;
			break;
		case 1:
			tunnelUp[3] = tunnelUp[2];
			break;
		case 2:
			tunnelUp[3] = tunnelUp[2] + 1;
			break;
	}
	tunnelDown[3] = tunnelUp[3] + width;

	progress++;
	width = 10 - progress / 30;
}

void FlappyGame::move(){
	switch (getNumberClick()) {
		case 0:
		case 1:
		case 2:
			pos--;
			break;
		case 6:
		case 7:
		case 8:
			pos++;
			break;
	}

	if (progress == 125) {
		victory(level, 9, 11);
		level++;
		initLevel();
	}

	if (pos <= tunnelUp[0] || pos >= tunnelDown[0]) {
		gameover(level);
		level = 1;
		initLevel();
	}

	noise(pos > 10 ? 600 : pos < -15 ? 8100 : 3600 - pos * 300, 50, 0, -8);
}

void FlappyGame::drawTunnel(){
	set(3 - progress % 4, (progress / 4 - pos) % 5, 1);
	for (char x = 0; x < 4; x++) {
		if (tunnelDown[x] - tunnelUp[x] == 6) drawRand(x, 0, true, 1);	// finish line
		drawRand(x, tunnelUp[x] + 2 - pos, false, 5);
		drawRand(x, tunnelDown[x] + 2 - pos, true, 5);
	}
	set(0, 2, 15);
}

void FlappyGame::drawRand(char x, int y, bool down, char color){
	if (x < 0 || x > 3) return;
	if (y > 4 && down) return;
	if (y < 0 && !down) return;
	if (down)
		for (char yy = y; yy < 5; yy++)
			set(x, yy, color);
	else
		for (char yy = y; yy >= 0; yy--)
			set(x, yy, color);
}
