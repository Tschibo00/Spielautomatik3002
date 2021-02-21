#include "TVGame.h"

TVGame::TVGame(){
	start = millis();
}

void TVGame::play(){
	clear(0);

	if (millis() - start < 80) {
		for (char i = 0; i < 20; i++)
			getScreen()[i] = getPalette()[random(4)];
		noise(12000, 170, 0, -10);
	} else {
		switch (activeChannel) {
			case 0:
				drawChannel0();
				break;
			case 1:
				drawChannel1();
				break;
			case 2:
				drawChannel2();
				break;
			case 3:
				drawChannel3();
				break;
			case 4:
				drawChannel4();
				break;
			case 5:
				drawChannel5();
				break;
			case 6:
				drawChannel6();
				break;
			case 7:
				drawChannel7();
				break;
			case 8:
				drawChannel8();
				break;
			case 9:
				drawChannel9();
				break;
			case 10:
				drawChannel10();
				break;
			case 11:
				drawChannel11();
				break;
		}
	}

	char key = getNumberClick();
	if (key > -1 && key != activeChannel) {
		start = millis();
		activeChannel = key;
	}

	flipBuffer();
}

void TVGame::drawChannel0(){

}

void TVGame::drawChannel1(){

}

void TVGame::drawChannel2(){

}

void TVGame::drawChannel3(){

}

void TVGame::drawChannel4(){

}

void TVGame::drawChannel5(){

}

void TVGame::drawChannel6(){

}

void TVGame::drawChannel7(){

}

void TVGame::drawChannel8(){

}

void TVGame::drawChannel9(){

}

void TVGame::drawChannel10(){

}

void TVGame::drawChannel11(){

}
