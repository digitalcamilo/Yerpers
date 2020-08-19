#include "Game.h"
#include <stdio.h>
#include "G8RTOS_IPC.h"
#include <driverlib.h>

GameState_t gamestate, packet;
uint8_t packet_buffer[sizeof(gamestate)];

/* Function to fill in a packet to be sent over through WiFi */
static inline void fillPacket(GameState_t * packet, uint8_t * buffer) {
    uint8_t * p = packet;

    for (int i = 0; i < sizeof(*packet); i++) {
        buffer[i] = *p;
        p++;
    }
}

/* Function to empty a packet after being sent over through WiFi */
static inline void emptyPacket(GameState_t * packet, uint8_t * buffer) {
    uint8_t * p = packet;

    for (int i = 0; i < sizeof(*packet); i++) {
        *p = buffer[i];
        p++;
    }
}

void JoinGame() {
    // connect client
    initCC3100(Client);

    // set initial SpecificPlayerInfo_t strict attributes
    gamestate.player.IP_address = getLocalIP();
    gamestate.player.displacementX = 0;
    gamestate.player.displacementY = 0;
    gamestate.player.playerNumber = 1;
    gamestate.player.ready = false;
    gamestate.player.joined = false;
    gamestate.player.acknowledge = true;

    // fill packet to send
    fillPacket(&gamestate, &packet_buffer);

    // send player info to host & wait for server response
    do SendData(packet_buffer, HOST_IP_ADDR, sizeof(packet_buffer));
    while(ReceiveData(packet_buffer, sizeof(packet_buffer)) < 0);
    emptyPacket(&packet, &packet_buffer);
    gamestate = packet;

    // turn on LED
    BITBAND_PERI(P1->OUT, 0) = !BITBAND_PERI(P1->OUT, 0);

    // add semaphores
    G8RTOS_InitSemaphore(&CC3100Semaphore, 1);
    G8RTOS_InitSemaphore(&LCDMutex, 1);

    InitBoardState();

    // add threads
    //G8RTOS_AddThread(DrawObjects, 50, "DrawObjects");
    G8RTOS_AddThread(ReceiveDataFromClient, 100, "ReceiveDataFromClient");
    G8RTOS_AddThread(SendDataToClient, 150, "SendDataToClient");
    //G8RTOS_AddThread(ReadJoystickHost, 200, "ReadJoystickHost");
    G8RTOS_AddThread(IdleThread, 255, "IdleThread");

    // kill self
    G8RTOS_KillSelf();
}

/*
 * Thread that receives game state packets from host
 */
void ReceiveDataFromHost()
{
    while (1)
    {
        G8RTOS_WaitSemaphore(&CC3100Semaphore);
        while (ReceiveData(packet_buffer, sizeof(packet_buffer)) < 0)
        {
            G8RTOS_SignalSemaphore(&CC3100Semaphore);

            // Sleeps to avoid deadlock
            G8RTOS_Sleep(1);

            G8RTOS_WaitSemaphore(&CC3100Semaphore);
        }
        G8RTOS_SignalSemaphore(&CC3100Semaphore);

        // Empties the packets content
        emptyPacket(&packet, &packet_buffer);

        //Check if game is done
//        if (gamestate.gameDone)
//            G8RTOS_AddThread(EndOfGameClient, 1, "EndGameClient");

        G8RTOS_Sleep(2);
    }
}

/*
 * Thread that sends UDP packets to host
 */
void SendDataToHost()
{
    while (1)
    {
        //fill packet to send
        fillPacket(&gamestate, &packet_buffer);

        //send packet
        G8RTOS_WaitSemaphore(&CC3100Semaphore);
        SendData(packet_buffer, HOST_IP_ADDR, sizeof(packet_buffer));
        G8RTOS_SignalSemaphore(&CC3100Semaphore);

        //adjust clients displacement after being sent once
        gamestate.player.displacementX = 0;
        gamestate.player.displacementY = 0;

        G8RTOS_Sleep(5);
    }
}


//================================== Host Threads

void CreateGame() {
    // set host mode
    initCC3100(Host);

    // receive packet from client
    while(!packet.player.acknowledge) {
        ReceiveData(packet_buffer, sizeof(packet_buffer));
        emptyPacket(&packet, &packet_buffer);
        gamestate = packet;
    }


    //while (ReceiveData(packet_buffer, sizeof(packet_buffer)) < 0);
    //emptyPacket(&packet, &packet_buffer);

    // copy player attributes including
    gamestate.player = packet.player;

    // turn on LED indicating connection with client
    BITBAND_PERI(P1->OUT, 0) = !BITBAND_PERI(P1->OUT, 0);

    // acknowledge to client that player has joined
    gamestate.player.joined = true;
    //gamestate.player.acknowledge = true;
    fillPacket(&gamestate, packet_buffer);
    SendData(packet_buffer, gamestate.player.IP_address, sizeof(packet_buffer));

    // add semaphores
    G8RTOS_InitSemaphore(&CC3100Semaphore, 1);
    G8RTOS_InitSemaphore(&LCDMutex, 1);

    // initialize the arena, paddles, scores
    InitBoardState();

    // add threads
    //G8RTOS_AddThread(DrawObjects, 50, "DrawObjects");
    G8RTOS_AddThread(ReceiveDataFromClient, 100, "ReceiveDataFromClient");
    G8RTOS_AddThread(SendDataToClient, 150, "SendDataToClient");
    //G8RTOS_AddThread(ReadJoystickHost, 200, "ReadJoystickHost");
    G8RTOS_AddThread(IdleThread, 255, "IdleThread");

    G8RTOS_KillSelf();
}

/*
 * Thread that sends game state to client
 */
void SendDataToClient()
{
    while (1)
    {
        // Fills the packet for the client
        fillPacket(&gamestate, &packet);

        // Sends the packet to the client
        G8RTOS_WaitSemaphore(&CC3100Semaphore);
        SendData(packet_buffer, gamestate.player.IP_address, sizeof(packet_buffer));
        G8RTOS_SignalSemaphore(&CC3100Semaphore);

        // Checks to see if the game is done
//        if (gamestate.gameDone)
//            G8RTOS_AddThread(EndOfGameHost, 1, "EndGameHost"); // Thread to end the game

        // Sleeps for 5ms (good amount of time for synchronization)
        G8RTOS_Sleep(5);
    }
}

/*
 * Thread that receives UDP packets from client
 */
void ReceiveDataFromClient()
{
    while (1)
    {
        G8RTOS_WaitSemaphore(&CC3100Semaphore);
        while (ReceiveData(packet_buffer, sizeof(packet_buffer)) < 0)
        {
            G8RTOS_SignalSemaphore(&CC3100Semaphore);

            // Sleeps to avoid deadlock
            G8RTOS_Sleep(1);

            G8RTOS_WaitSemaphore(&CC3100Semaphore);
        }
        G8RTOS_SignalSemaphore(&CC3100Semaphore);

        // Empties the packets content
        emptyPacket(&packet, &packet_buffer);

        //  Updates the players current center with the received displacement
        //gamestate.players[1].currentCenter += gamestate.player.displacement;

        G8RTOS_Sleep(2);
    }
}

void InitBoardState()
{
    G8RTOS_WaitSemaphore(&LCDMutex);

    LCD_Clear(LCD_GRAY);

    //char scores[3];

    //snprintf(scores, 3, "%.2u", gamestate.overallScores[0]);
    //LCD_Text(MIN_SCREEN_X + 10, MAX_SCREEN_Y - 20, scores, LCD_RED);

    //snprintf(scores, 3, "%.2u", gamestate.overallScores[1]);
    //LCD_Text(MIN_SCREEN_X + 10, MIN_SCREEN_Y + 5, scores, LCD_BLUE);

    // Draw ground
    LCD_DrawRectangle(ARENA_MIN_X, ARENA_MAX_X, ARENA_MAX_Y - 5, ARENA_MAX_Y, LCD_BLACK);

    // Draw clouds

    G8RTOS_SignalSemaphore(&LCDMutex);

    // Draw players
    //DrawPlayer(&gamestate.players[0]);
    //DrawPlayer(&gamestate.players[1]);
}

void IdleThread()
{
    while(1);
}
