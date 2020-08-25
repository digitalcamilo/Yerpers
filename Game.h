#ifndef GAME_H_
#define GAME_H_

/*********************************************** Includes ********************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include "G8RTOS.h"
#include "cc3100_usage.h"
#include "LCDLib.h"
#include "time.h"
#include "stdlib.h"
#include "BSP.h"
/*********************************************** Includes ********************************************************************/

#define PLAYER 1

/* Size of game arena */
#define ARENA_MIN_X                  0
#define ARENA_MAX_X                  320
#define ARENA_MIN_Y                  0
#define ARENA_MAX_Y                  240

semaphore_t CC3100Semaphore;
semaphore_t LCDMutex;

#pragma pack ( push, 1)
/*
 * Struct to be sent from the client to the host
 */
typedef struct
{
    uint32_t IP_address;
    int16_t displacementX;
    int16_t displacementY;
    uint8_t playerNumber;
    bool ready;
    bool joined;
    bool acknowledge;
} SpecificPlayerInfo_t;

typedef struct
{
    SpecificPlayerInfo_t player;
    bool winner;
    bool gameDone;
    //uint8_t LEDScores[2];
    uint8_t overallScores[2];
} GameState_t;

#pragma pack ( pop )

//Client Threads
void JoinGame();
void SendDataToHost();
void ReceiveDataFromHost();
<<<<<<< HEAD
void ReadJoystickHost();

=======

// Host Threads
>>>>>>> f85f2b7557424451274a56bbb370d8d4497bb2e2
void CreateGame();
void SendDataToClient();
void ReceiveDataFromClient();
void ReadJoystickClient();

//Common Threads
void InitBoardState();
void IdleThread();
void DrawPlayer(uint16_t x, uint16_t y, uint16_t player[]);
void drawClouds(int16_t x, int16_t y);

#endif /* GAME_H_ */
