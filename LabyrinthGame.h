#ifndef LABYRINTHGAME_H_
#define LABYRINTHGAME_H_

#include "Game.h"

class LabyrinthGame: public Game {
	private:
		uint8_t *labyrinth = NULL;					// 0=unvisited,empty space,  1=wall, 2=visited,empty space
		int sizeX = 5;									// max 55*55
		int sizeY = 5;
		void setPoint(int x, int y){
			setPoint(x, y, 1);
		}
		void setPoint(int x, int y, uint8_t color){
			uint16_t p = y * sizeX + x;
			uint8_t mask = 3;
			labyrinth[p / 4] = labyrinth[p / 4] & (255 - (mask << ((p % 4) * 2))) | (color << ((p % 4) * 2));
		}
		void clearPoint(int x, int y){
			setPoint(x, y, 0);
		}
		uint8_t get(int x, int y){
			uint16_t p = y * sizeX + x;
			uint8_t mask = 3;
			return (labyrinth[p / 4] & (mask << ((p % 4) * 2))) >> ((p % 4) * 2);
		}

		//player
		int posX = 0;
		int posY = 0;
		bool showHints = false;

	public:
		LabyrinthGame();
		void play() override;
		void generate(int sizeX, int sizeY);
};

#endif /* LABYRINTHGAME_H_ */
