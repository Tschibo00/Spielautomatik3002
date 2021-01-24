/*
 * BatteryMonitor.h
 *
 *  Created on: Jan 5, 2021
 *      Author: hopper
 */

#ifndef BATTERYMONITOR_H_
#define BATTERYMONITOR_H_

#include "DisplayController.h"

class BatteryMonitor {
private:
	static void show(float level) {
		const char symbol[20] = { 0, 3, 3, 0, 3, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 3, 3, 3, 3, 3 };

		char l = (char) (level * 15.);

		if (l == 0) {
			while (true) {
				if (millis() % 500 < 200)
					clear(0);
				else
					copy(symbol);
				flipBuffer();
			}
		} else {
			copy(symbol);
			setNumber(3, l);
			setNumber(2, l - 5);
			setNumber(1, l - 10);
			flipBuffer();
			delay(1500);
		}
	}

	static void setNumber(char row, char number) {
		char l = (number < 0 ? 0 : (number > 4 ? 4 : number));
		getScreen()[row * 4 + 1] = getPalette()[l];
		getScreen()[row * 4 + 2] = getPalette()[l];
	}

public:
	static void checkAndShow() {
		// 860 for 100% (about 4.2v per cell),  750 for 0% (about 3.7v per cell)
		float level = (analogRead(A7) - 750);
		if (level > -250) {
			level = level / 110.;
			if (level < 0.)
				level = 0.;
			if (level > 1.)
				level = 1.;
			show(level);
		}
	}
};

#endif /* BATTERYMONITOR_H_ */
