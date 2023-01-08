#ifndef Esp_h
#define Esp_h

#include <stdbool.h>

const int VIRTUAL_SCREEN_WIDTH = 800;
const int GAME_UNIT_MAGIC = 400;

const float PLAYER_HEIGHT = 5.25f;
const float PLAYER_WIDTH = 2.0f;
const float EYE_HEIGHT = 4.5f;

const int ESP_FONT_HEIGHT = 15;
const int ESP_FONT_WIDTH = 9;

int viewport[4];

bool isTeamgame();

#endif // Esp.h