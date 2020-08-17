/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * | AUTHOR: Raz Aloni                                               |
 * | DATE: 01/12/2017                                                |
 * | MODIFIED BY: Camilo Chen                                        |
 * | SUMMARY: G8RTOS_Semaphores.h                                    |
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */

#ifndef G8RTOS_SEMAPHORES_H_
#define G8RTOS_SEMAPHORES_H_

/*
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 *                  DATA TYPE DEFINITIONS
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 */

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * Semaphore typedef
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
typedef int32_t semaphore_t;

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
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
void G8RTOS_InitSemaphore(semaphore_t *s, int32_t value);

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
void G8RTOS_WaitSemaphore(semaphore_t *s);

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
void G8RTOS_SignalSemaphore(semaphore_t *s);

#endif /* G8RTOS_SEMAPHORES_H_ */
