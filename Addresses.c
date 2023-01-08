#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h> 
#include "Addresses.h"
#include "Main.h"

void updateBoxes()      
{
    updateEntities();
    updatePlayer();
}

/* This will dellocate our entity structs on the heap so that we can add the updated values */
void deallocate()
{
    for(int i=0; i<16; i++) {
        if(entityArray[i] != NULL) {
            free(entityArray[i]);
        }
    }
}

void updateEntities()
{
    if(entityArray[0] != NULL) {
        deallocate();
    }

    uintptr_t p_entityArray;
    BOOL rpmEntityArray = ReadProcessMemory(hProcess, (LPCVOID) ENTITY_ARRAY_OFFSET, &p_entityArray, sizeof(int), NULL);

    if(rpmEntityArray == FALSE) {
        printf("Please try a differernt entity array offset \n");
        return;
    }

    for(int i=0; i<16; i++) {
        uintptr_t entityObject;
        float x = -1, y = -1, z = -1;
        int health = -1, armor = -1, team = -1; 
        char name[20] = { 0 };

        BOOL rpmEntityObject = ReadProcessMemory(hProcess, (LPCVOID) (p_entityArray + (i * 0x4)), &entityObject, sizeof(int), NULL);
        ReadProcessMemory(hProcess, (LPCVOID) (entityObject + 0x00EC), &health, sizeof(int), NULL);
        ReadProcessMemory(hProcess, (LPCVOID) (entityObject + 0x00F0), &armor, sizeof(int), NULL);
        ReadProcessMemory(hProcess, (LPCVOID) (entityObject + 0x030C), &team, sizeof(int), NULL);
        ReadProcessMemory(hProcess, (LPCVOID) (entityObject + 0x00205), &name, sizeof(name), NULL);
        ReadProcessMemory(hProcess, (LPCVOID) (entityObject + 0x0028), &x, sizeof(float), NULL);
        ReadProcessMemory(hProcess, (LPCVOID) (entityObject + 0x002C), &y, sizeof(float), NULL);
        ReadProcessMemory(hProcess, (LPCVOID) (entityObject + 0x0030), &z, sizeof(float), NULL);

        if(rpmEntityObject == FALSE || name[0] == 0) { // This means we are at the end of the entity array
            entityCount = i;
            return;
        }

        Vec3 vector;
        vector.x = x;
        vector.y = y;
        vector.z = z;

        struct Entity *aEnt = (struct Entity *) malloc(sizeof(struct Entity));
        strcpy(aEnt->name, name);
        aEnt->health = health;
        aEnt->armor = armor;
        aEnt->entityTeam = team;
        aEnt->vector = vector;
        entityArray[i] = aEnt;

        health = -1, armor = -1;
    }
    entityCount = MAX_ARRAY_LENGTH;
}

void updatePlayer()
{
    uintptr_t p_player;
    BOOL rpmPlayer = ReadProcessMemory(hProcess, (LPCVOID) PLAYER_OFFSET, &p_player, sizeof(int), NULL);

    if(rpmPlayer == FALSE) {
        printf("Please try a differernt player offset \n");
        return;
    }

    float x, y, z;
    int team;
    ReadProcessMemory(hProcess, (LPCVOID) (p_player + 0x00EC), &pData.playerHealth, sizeof(int), NULL);
    ReadProcessMemory(hProcess, (LPCVOID) (p_player + 0x00F0), &pData.playerArmor, sizeof(int), NULL);
    ReadProcessMemory(hProcess, (LPCVOID) (p_player + 0x00205), &pData.playerName, sizeof(pData.playerName), NULL);
    ReadProcessMemory(hProcess, (LPCVOID) (p_player + 0x0028), &x, sizeof(float), NULL);
    ReadProcessMemory(hProcess, (LPCVOID) (p_player + 0x002C), &y, sizeof(float), NULL);
    ReadProcessMemory(hProcess, (LPCVOID) (p_player + 0x000C), &z, sizeof(float), NULL);

    ReadProcessMemory(hProcess, (LPCVOID) (p_player + 0x030C), &team, sizeof(int), NULL);
    enum Team playersTeam = team;
    pData.team = playersTeam;

    ReadProcessMemory(hProcess, (PBYTE*) (0x0057DFD0), &pData.matrix, sizeof(pData.matrix), NULL);
    Vec3 vector;
    vector.x = x;
    vector.y = y;
    vector.z = z;
    pData.pos = vector;
}



