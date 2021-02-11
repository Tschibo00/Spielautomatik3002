#include "FlappyGame.h"

FlappyGame::FlappyGame(){
}

void FlappyGame::play(){
	clear(0);
/*
	move();

	drawTunnel();
*/
	flipBuffer();
}

void FlappyGame::initLevel(){
	width = 10;
	pos = 0;
	brake = 500 - level * 10;
	for (char i = 0; i < 4; i++)
		tunnel[i] = -5;
}

void FlappyGame::advanceTunnel(){

}
