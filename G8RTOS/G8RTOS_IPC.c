/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * | AUTHOR: Daniel Gonzalez                                         |
 * | DATE: 01/10/2017                                                |
 * | MODIFIED BY: Camilo Chen                                        |
 * | SUMMARY: G8RTOS_IPC.c                                           |
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */

/*
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 *                DEPENDENCIES AND EXTERNS
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 */

#include <stdint.h>
#include "msp.h"
#include "G8RTOS_Semaphores.h"
#include "G8RTOS_IPC.h"

/*
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 *                        DEFINES
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 */

#define FIFOSIZE 16
#define MAX_NUMBER_OF_FIFOS 4

/*
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 *                  DATA STRUCTURES USED
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 */

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * FIFO typedef
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
typedef struct FIFO FIFO_t;

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * FIFO
 * The FIFO structure holds the FIFO buffer and
 * information pertinent to the FIFO such as the head
 * and tail pointers, a lost data counter, and semaphores
 * for current size and mutual exclusion
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
struct FIFO {
    int32_t buffer[FIFOSIZE];
    int32_t *head;
    int32_t *tail;
    uint32_t lostData;
    semaphore_t currentSize;
    semaphore_t mutex;
};

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * FIFOs
 * An array of FIFOs
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
static FIFO_t FIFOs[MAX_NUMBER_OF_FIFOS];

/*
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 *                    PUBLIC FUNCTIONS
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 */

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * G8RTOS_InitFIFO
 * INPUTS: (uint32_t) FIFOIndex
 * OUTPUTS: (int) error
 * Initializes FIFO struct
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
int G8RTOS_InitFIFO(uint32_t FIFOIndex)
{
    /* return error code if FIFOIndex invalid */
    if (FIFOIndex > MAX_NUMBER_OF_FIFOS - 1)
        return -1;

    /* clear FIFO buffer */
    for (int i = 0; i < FIFOSIZE; i++)
        FIFOs[FIFOIndex].buffer[i] = 0;

    /* set head of FIFO */
    FIFOs[FIFOIndex].head = &FIFOs[FIFOIndex].buffer[0];

    /* set tail of FIFO */
    FIFOs[FIFOIndex].tail = &FIFOs[FIFOIndex].buffer[0];

    /* initialize currentSize semaphore */
    G8RTOS_InitSemaphore(&FIFOs[FIFOIndex].currentSize, 0);

    /* initialize mutex semaphore */
    G8RTOS_InitSemaphore(&FIFOs[FIFOIndex].mutex, 1);

    /* clear lost data */
    FIFOs[FIFOIndex].lostData = 0;

    /* return error code */
    return 1;
}

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * readFIFO
 * INPUTS: (uint32_t) FIFOChoice
 * OUTPUTS: (uint32_t) data
 * Reads FIFO
 * - Waits until CurrentSize semaphore is greater than
 *   zero
 * - Gets data and increments the head ptr
 *   (wraps if necessary)
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
uint32_t readFIFO(uint32_t FIFOChoice)
{
    /* decrement currentSize semaphore for current FIFO */
    G8RTOS_WaitSemaphore(&FIFOs[FIFOChoice].currentSize);

    /* wait until FIFO is available */
    G8RTOS_WaitSemaphore(&FIFOs[FIFOChoice].mutex);

    /* read data from head */
    uint32_t data = *FIFOs[FIFOChoice].head;

    /* increment head (wrap if necessary) */
    if (FIFOs[FIFOChoice].head == &FIFOs[FIFOChoice].buffer[FIFOSIZE - 1])
        FIFOs[FIFOChoice].head = &FIFOs[FIFOChoice].buffer[0];
    else
        FIFOs[FIFOChoice].head++;

    /* release FIFO semaphore */
    G8RTOS_SignalSemaphore(&FIFOs[FIFOChoice].mutex);

    /* return data */
    return data;
}

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * writeFIFO
 * INPUTS: (uint32_t) FIFOChoice, (uint32_t) Data
 * OUTPUTS: (int) error
 * Writes to FIFO
 * - Writes data to Tail of the buffer if the buffer is
 *   not full
 * - Increments tail (wraps if ncessary)
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
int writeFIFO(uint32_t FIFOChoice, uint32_t Data)
{
    /* wait until FIFO is available */
    //G8RTOS_WaitSemaphore(&FIFOs[FIFOChoice].mutex);

    /* handle lost data */
    if (FIFOs[FIFOChoice].currentSize == FIFOSIZE) {
        /* increment lost data if FIFO size exceeds max */
        FIFOs[FIFOChoice].lostData++;

        /* release FIFO semaphore */
        G8RTOS_SignalSemaphore(&FIFOs[FIFOChoice].mutex);

        /* return error code */
        return -1;
    }

    /* write data to tail */
    *FIFOs[FIFOChoice].tail = Data;

    /* increment tail (wrap if necessary) */
    if (FIFOs[FIFOChoice].tail == &FIFOs[FIFOChoice].buffer[FIFOSIZE - 1])
        FIFOs[FIFOChoice].tail = &FIFOs[FIFOChoice].buffer[0];
    else
        FIFOs[FIFOChoice].tail++;

    /* increment currentSize semaphore for current FIFO */
    G8RTOS_SignalSemaphore(&FIFOs[FIFOChoice].currentSize);

    /* release FIFO semaphore */
    //G8RTOS_SignalSemaphore(&FIFOs[FIFOChoice].mutex);

    /* return error code */
    return 1;
}
