#include "TVGame.h"

TVGame::TVGame(){
	start = millis();
}

void TVGame::play(){

	if (millis() - start < 80) {
		clear(0);
		for (char i = 0; i < 20; i++)
			getScreen()[i] = getPalette()[random(4)];
		flipBuffer();
		noise(12000, 170, 0, -10);
	} else {
		frame = ((millis() - start - 80) / 100);
		if (frame != oldFrame) {
			oldFrame = frame;
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
			flipBuffer();
		}
	}

	char key = getNumberClick();
	if (key > -1 && key != activeChannel) {
		start = millis();
		activeChannel = key;
		frame = 0;
		oldFrame = -1;
	}
}

void TVGame::drawChannel0(){
	fade(1);
	getScreen()[random(20)] = 15;
	getScreen()[random(20)] = 15;
	getScreen()[random(20)] = 15;
}

void TVGame::drawChannel1(){
	clear(0);
	set(frame % 4, 0, 15);
	set(frame % 4, 4, 15);
	set((frame + 1) % 4, 1, 15);
	set((frame + 1) % 4, 3, 15);
	set((frame + 2) % 4, 2, 15);
}

void TVGame::drawChannel2(){
	switch (random(10)) {
		case 0:
			if (x1 > 0) x1--;
			break;
		case 1:
			if (y1 > 0) y1--;
			break;
		case 2:
			if (x1 < 3) x1++;
			break;
		case 3:
			if (y1 < 4) y1++;
			break;
		case 4:
			if (x2 > 0) x2--;
			break;
		case 5:
			if (y2 > 0) y2--;
			break;
		case 6:
			if (x2 < 3) x2++;
			break;
		case 7:
			if (y2 < 4) y2++;
			break;
	}
	clear(0);
	set(x1, y1, 15);
	set(x2, y2, 3);
	if (x1 == x2 && y1 == y2) {
		music(victoryMusic, 5, 130, 600, -4);
		delay(1000);
		x1 = random(4);
		y1 = random(5);
		x2 = random(4);
		y2 = random(5);
	}
}

void TVGame::drawChannel3(){
	clear(0);
	char i;
	for (i = 0; i < 5; i++) {
		set(fische[i], i, 2);
		switch (random(10)) {
			case 0:
				if (fische[i] > -2) fische[i]--;
				break;
			case 1:
				if (fische[i] < 5) fische[i]++;
				break;
		}
	}
	if (frame % 5 == 0) {
		riff[0] = riff[1];
		riff[1] = riff[2];
		riff[2] = riff[3];
		riff[3] = random(9);
	}
	for (i = 0; i < 4; i++)
		drawLine(i, riff[i], 0, 1, 5, 7);
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
