#include "msp.h"
#include "BSP.h"
#include "Game.h"
#include "LCDLib.h"
#include "G8RTOS.h"
#include "cc3100_usage.h"

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	G8RTOS_Init();

	// client
	if(PLAYER == 1) G8RTOS_AddThread(&JoinGame, 1, "JoinGame");
	else G8RTOS_AddThread(&CreateGame, 1, "CreateGame");

	LCD_Init(false);
	G8RTOS_Launch();
}
