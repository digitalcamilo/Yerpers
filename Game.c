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

    // kill self
    //G8RTOS_KillSelf();

    // add semaphores
    G8RTOS_InitSemaphore(&CC3100Semaphore, 1);

    while(1);
}

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

    while(1);
}
