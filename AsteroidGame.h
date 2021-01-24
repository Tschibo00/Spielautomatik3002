#ifndef ASTEROIDGAME_H_
#define ASTEROIDGAME_H_
#include "Game.h"

#define BULLETSPEED 80;
#define ASTEROIDSTARTSPEED 4000;

class AsteroidGame: public Game {
	private:
		char playerPos = 2;
		char bulletX = -1;
		char bulletY = -1;
		unsigned long bulletWait = 0;
		char asteroids[20];
		unsigned long asteroidRowWait;
		unsigned long asteroidRowSpeed = ASTEROIDSTARTSPEED
		;
		int level = 1;

		void advanceAsteroids();
		void initLevel();
		void setFirstRow();

	public:
		AsteroidGame();
		void play() override;
};
#endif /* ASTEROIDGAME_H_ */
