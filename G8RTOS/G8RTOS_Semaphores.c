/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * | AUTHOR: Raz Aloni                                               |
 * | DATE: 01/12/2017                                                |
 * | MODIFIED BY: Camilo Chen                                        |
 * | SUMMARY: G8RTOS_Semaphores.c                                    |
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
#include "G8RTOS_CriticalSection.h"
#include "G8RTOS_Structures.h"

/*
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 *                    PUBLIC FUNCTIONS
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 */

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * G8RTOS_InitSemaphore
 * INPUTS: (semaphore_t *) s, (int32_t) value
 * OUTPUTS: void
 * Initializes a semaphore to a given value
 *  - CRITICAL SECTION
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
void G8RTOS_InitSemaphore(semaphore_t *s, int32_t value)
{
	/* start critical section & disable interrupts */
    int32_t state = StartCriticalSection();

    /* initialize semaphore to value */
    *s = value;

    /* end critical section & enable interrupts */
    EndCriticalSection(state);
}

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * G8RTOS_WaitSemaphore
 * INPUTS: (semaphore_t *) s
 * OUTPUTS: void
 * Waits for a semaphore to be available ((*s) > 0)
 *  - Decrements semaphore when available
 *  - Blocks semaphore if unavailable and moves to
 *    next thread
 *  - CRITICAL SECTION
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
void G8RTOS_WaitSemaphore(semaphore_t *s)
{
    /* start critical section & disable interrupts */
    int32_t state = StartCriticalSection();

    /* decrement semaphore */
    (*s)--;

    /* if semaphore is less than 0, it is unavailable and the thread is blocked */
    if ((*s) < 0) {
        /* block thread */
        CurrentlyRunningThread->blocked = s;

        /* end critical section & enable interrupts */
        EndCriticalSection(state);

        /* call PendSV */
        SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
    }

    /* end critical section & enable interrupts */
    EndCriticalSection(state);
}

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * G8RTOS_SignalSemaphore
 * INPUTS: (semaphore_t *) s
 * OUTPUTS: void
 * Signals the completion of the usage of a semaphore
 *  - Increments semaphore by 1
 *  - Unblocks first thread with the blocked semaphore
 *  - CRITICAL SECTION
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
void G8RTOS_SignalSemaphore(semaphore_t *s)
{
    /* temporary pointer */
    tcb_t *ptr;

    /* start critical section & disable interrupts */
    int32_t state = StartCriticalSection();

    /* increment semaphore */
    (*s)++;

    /* unblock first thread that is blocked by semaphore s */
    if ((*s) <= 0) {
        ptr = CurrentlyRunningThread->nextTCB;

        /* traverse TCBs until the blocked thread is found */
        while (ptr->blocked != s)
            ptr = ptr->nextTCB;

        /* free blocked thread */
        ptr->blocked = 0;
    }

    /* end critical section & enable interrupts */
    EndCriticalSection(state);
}
