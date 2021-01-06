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
	static void show(DisplayController *dc, float level) {
		const char symbol[20] = { 0, 3, 3, 0, 3, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 3, 3, 3, 3, 3 };

		char l = (char) (level * 15.);

		if (l == 0) {
			while (true) {
				if (millis() % 500 < 200)
					dc->clear(0);
				else
					dc->copy(symbol);
				dc->flipBuffer();
			}
		} else {
			dc->copy(symbol);
			setNumber(dc, 3, l);
			setNumber(dc, 2, l - 5);
			setNumber(dc, 1, l - 10);
			dc->flipBuffer();
			delay(1500);
		}
	}

	static void setNumber(DisplayController *dc, char row, char number) {
		char l = (number < 0 ? 0 : (number > 4 ? 4 : number));
		dc->screen[row * 4 + 1] = dc->palette[l];
		dc->screen[row * 4 + 2] = dc->palette[l];
	}

public:
	static void checkAndShow(DisplayController *dc) {
		// 860 for 100% (about 4.2v per cell),  750 for 0% (about 3.7v per cell)
		float level = (analogRead(A7) - 750);
		if (level > -250) {
			level = level / 110.;
			if (level < 0.)
				level = 0.;
			if (level > 1.)
				level = 1.;
			show(dc, level);
		}
	}
};

#endif /* BATTERYMONITOR_H_ */
