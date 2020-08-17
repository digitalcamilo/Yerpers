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
    //bool winner;
    //bool gameDone;
    //uint8_t LEDScores[2];
    //uint8_t overallScores[2];
} GameState_t;

#pragma pack ( pop )

void JoinGame();
void CreateGame();


#endif /* GAME_H_ */
