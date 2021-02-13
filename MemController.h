#ifndef MEMCONTROLLER_H_
#define MEMCONTROLLER_H_

#include <Arduino.h>

void saveGame(uint8_t *game, uint16_t size, uint16_t offset, uint8_t player);
void loadGame(uint8_t *game, uint16_t size, uint16_t offset, uint8_t player);

#endif
