#include "MemController.h"

#include "EEPROM.h"

void saveGame(uint8_t *game, uint16_t size, uint16_t offset, uint8_t player){
	for (uint16_t i = 0; i < size; i++)
		EEPROM.update(offset + player * size + i, game[i]);
}

void loadGame(uint8_t *game, uint16_t size, uint16_t offset, uint8_t player){
	for (uint16_t i = 0; i < size; i++)
		game[i] = EEPROM.read(offset + player * size + i);
}
