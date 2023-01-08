#ifndef Addresses_h
#define Addresses_h

#include "MathUtil.h"

#define BASE_ADDRESS 0x00400000
#define BASE_PLAYER_OFFSET 0x18AC00
#define BASE_ENTITY_ARRAY_OFFSET 0x191FCC
#define PLAYER_OFFSET (BASE_ADDRESS + BASE_PLAYER_OFFSET)
#define ENTITY_ARRAY_OFFSET (BASE_ADDRESS + BASE_ENTITY_ARRAY_OFFSET)

#define MAX_ARRAY_LENGTH 16

enum Team {
    CLA = 0, RVSF = 1
};

struct Entity {
    struct Vec3 vector;
    enum Team entityTeam;
    char name[20];
    int health, armor;
};

struct PlayerData {
    struct Vec3 pos;
    enum Team team;
    float matrix[16];
    char playerName[20];
    int playerHealth, playerArmor;
    int arAmmo, arReserveAmmo, pistolAmmo, pistolReserveAmmo;
};

struct Entity *entityArray[MAX_ARRAY_LENGTH];
struct PlayerData pData;
int entityCount;

// struct Entity* updateBoxes();
void updateBoxes();
void updatePlayer();
void updateEntities();
void deallocate();

#endif // Addresses_h
