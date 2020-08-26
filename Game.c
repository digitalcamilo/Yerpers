#include "Game.h"
#include <stdio.h>
#include "G8RTOS_IPC.h"
#include <driverlib.h>

uint16_t redplayer[SIZE_OF_PLAYER] = {
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_BLACK, LCD_BLACK, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_BLACK, LCD_BLACK, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED,
   LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED,
   LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED,
   LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED,
   LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED,
   LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED,
   LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED,
   LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED,
   LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED,
   LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED,
   LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED,
   LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLACK, LCD_BLACK, LCD_YELLOW, LCD_YELLOW, LCD_BLACK, LCD_BLACK, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLACK, LCD_BLACK, LCD_YELLOW, LCD_YELLOW, LCD_BLACK, LCD_BLACK, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_RED, LCD_RED, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN
};

uint16_t blueplayer[SIZE_OF_PLAYER] = {
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_BLACK, LCD_BLACK, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_BLACK, LCD_BLACK, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE,
   LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE,
   LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE,
   LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE,
   LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE,
   LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE,
   LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE,
   LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE,
   LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE,
   LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE,
   LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE,
   LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLACK, LCD_BLACK, LCD_YELLOW, LCD_YELLOW, LCD_BLACK, LCD_BLACK, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLACK, LCD_BLACK, LCD_YELLOW, LCD_YELLOW, LCD_BLACK, LCD_BLACK, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN,
   LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_BLUE, LCD_BLUE, LCD_CYAN, LCD_CYAN, LCD_CYAN, LCD_CYAN
};

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
    G8RTOS_AddThread(updateObjects, 50, "updateObjects");
    G8RTOS_AddThread(ReceiveDataFromHost, 100, "ReceiveDataFromHost");
    G8RTOS_AddThread(SendDataToHost, 150, "SendDataToHost");
    G8RTOS_AddThread(ReadJoystickClient, 200, "ReadJoystickClient");
    G8RTOS_AddThread(IdleThread, 254, "IdleThread");

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

/*
 * Thread to read client's joystick
 */
void ReadJoystickClient() {
    int16_t xCord, yCord;

    while (1) {
        GetJoystickCoordinates(&xCord, &yCord);

        if (xCord < -1800) gamestate.player.displacementX = 4;
        else if (xCord > 1800) gamestate.player.displacementX = -4;
        else gamestate.player.displacementX = 0;

        // Sleep 10ms
        G8RTOS_Sleep(10);
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

    // copy player attributes including
    gamestate.player = packet.player;

    // init player values
    // host
    gamestate.players[0].currentCenterX = 294;
    gamestate.players[0].currentCenterY = 220;
    gamestate.players[0].color = player1;

    // client
    gamestate.players[1].currentCenterX = 26;
    gamestate.players[1].currentCenterY = 220;
    gamestate.players[1].color = player2;

    gamestate.gameDone = false;
    gamestate.winner = false;
    for(int i=0; i<MAX_NUM_OF_PLAYERS; i++) gamestate.overallScores[i] = 0;

    // turn on LED indicating connection with client
    BITBAND_PERI(P1->OUT, 0) = !BITBAND_PERI(P1->OUT, 0);

    // acknowledge to client that player has joined
    gamestate.player.joined = true;
    //gamestate.player.acknowledge = true;
    fillPacket(&gamestate, &packet_buffer);
    SendData(packet_buffer, gamestate.player.IP_address, sizeof(packet_buffer));

    // add semaphores
    G8RTOS_InitSemaphore(&CC3100Semaphore, 1);
    G8RTOS_InitSemaphore(&LCDMutex, 1);

    // initialize the arena, paddles, scores
    InitBoardState();


    // add threads
    G8RTOS_AddThread(updateObjects, 50, "updateObjects");
    G8RTOS_AddThread(ReceiveDataFromClient, 100, "ReceiveDataFromClient");
    G8RTOS_AddThread(SendDataToClient, 150, "SendDataToClient");
    G8RTOS_AddThread(ReadJoystickHost, 200, "ReadJoystickHost");
    G8RTOS_AddThread(IdleThread, 254, "IdleThread");

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
        gamestate.players[1].currentCenterX += gamestate.player.displacementX;

        G8RTOS_Sleep(2);
    }
}

/*
 * Thread to read host's joystick
 */
void ReadJoystickHost() {
    int16_t xCord, yCord, displacement;

    while (1) {
        GetJoystickCoordinates(&xCord, &yCord);

        if (xCord < -1800) displacement = 4;
        else if (xCord > 1800) displacement = -4;
        else displacement = 0;

        // Sleep to give fair advantage to client
        G8RTOS_Sleep(10);

        // Update position of host paddle
        gamestate.players[0].currentCenterX += displacement;
    }
}

void drawClouds(int16_t x, int16_t y)
{
    LCD_DrawRectangle(25 + x, 35 + x, 5 + y, 10 + y, LCD_WHITE);
    LCD_DrawRectangle(20 + x, 35 + x, 10 + y, 15 + y, LCD_WHITE);
    LCD_DrawRectangle(55 + x, 60 + x, 10 + y, 15 + y, LCD_WHITE);
    LCD_DrawRectangle(20 + x, 40 + x, 15 + y, 20 + y, LCD_WHITE);
    LCD_DrawRectangle(50 + x, 60 + x, 15 + y, 20 + y, LCD_WHITE);
    LCD_DrawRectangle(15 + x, 65 + x, 20 + y, 25 + y, LCD_WHITE);
    LCD_DrawRectangle(10 + x, 70 + x, 25 + y, 30 + y, LCD_WHITE);
    LCD_DrawRectangle(0 + x, 80 + x, 30 + y, 33 + y, LCD_WHITE);
}

void updateObjects()
{
    PrevPlayer_t prevPlayers[MAX_NUM_OF_PLAYERS];

    prevPlayers[0].centerX = gamestate.players[0].currentCenterX;
    prevPlayers[0].centerY = gamestate.players[0].currentCenterY;
    prevPlayers[1].centerX = gamestate.players[1].currentCenterX;
    prevPlayers[1].centerY = gamestate.players[1].currentCenterY;

    while(1)
    {
        for(int i=0; i<MAX_NUM_OF_PLAYERS; i++) {
            if( (gamestate.players[i].currentCenterX != prevPlayers[i].centerX) && (gamestate.players[i].currentCenterY != prevPlayers[i].centerY) ){
                G8RTOS_WaitSemaphore(LCDMutex);
                ErasePlayer(prevPlayers[i].centerX, prevPlayers[i].centerY);
                if(gamestate.players[i].color == player1) DrawPlayer(gamestate.players[i].currentCenterX, gamestate.players[i].currentCenterY, redplayer);
                else DrawPlayer(gamestate.players[i].currentCenterX, gamestate.players[i].currentCenterY, blueplayer);
                prevPlayers[i].centerX = gamestate.players[i].currentCenterX;
                prevPlayers[i].centerY = gamestate.players[i].currentCenterY;
                G8RTOS_SignalSemaphore(LCDMutex);
            }
        }

        G8RTOS_Sleep(20);
    }
}

void InitBoardState()
{
    G8RTOS_WaitSemaphore(&LCDMutex);

    LCD_Clear(LCD_CYAN);

    // Draw ground
    LCD_DrawRectangle(ARENA_MIN_X, ARENA_MAX_X, ARENA_MAX_Y - 5, ARENA_MAX_Y, LCD_BROWN);
    LCD_DrawRectangle(60, 100, ARENA_MAX_Y - 55, ARENA_MAX_Y - 50, LCD_BROWN);
    LCD_DrawRectangle(220, 260, ARENA_MAX_Y - 55, ARENA_MAX_Y - 50, LCD_BROWN);
    LCD_DrawRectangle(140, 180, ARENA_MAX_Y - 105, ARENA_MAX_Y - 100, LCD_BROWN);

    // draw players
    for(int i=0; i<MAX_NUM_OF_PLAYERS; i++) {
        if(gamestate.players[i].color == player1) DrawPlayer(gamestate.players[i].currentCenterX, gamestate.players[i].currentCenterY, redplayer);
        else DrawPlayer(gamestate.players[i].currentCenterX, gamestate.players[i].currentCenterY, blueplayer);
    }

    // Draw clouds
    drawClouds(0, 0);
    drawClouds(30, 15);
    drawClouds(100, 12);
    drawClouds(150, 35);
    drawClouds(200, 5);
    drawClouds(230, 17);

    G8RTOS_SignalSemaphore(&LCDMutex);
}

void DrawPlayer(uint16_t x, uint16_t y, uint16_t player[])
{
    LCD_DrawRectangleWithColor(x-6, x+7, y-21, y+14, player);
}

void ErasePlayer(uint16_t x, uint16_t y)
{
    LCD_DrawRectangle(x-6, x+7, y-21, y+14, LCD_CYAN);
}

void IdleThread()
{
    while(1);
}
