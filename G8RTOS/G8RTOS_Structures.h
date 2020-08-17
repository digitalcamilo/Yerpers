/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * | AUTHOR: Raz Aloni                                               |
 * | DATE: 01/12/2017                                                |
 * | MODIFIED BY: Camilo Chen                                        |
 * | SUMMARY: G8RTOS_Structure.h                                     |
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */

#ifndef G8RTOS_STRUCTURES_H_
#define G8RTOS_STRUCTURES_H_

/*
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 *                DEPENDENCIES AND EXTERNS
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 */

#include "G8RTOS.h"

/*
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 *                        DEFINES
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 */

#define MAX_NAME_LENGTH 16

/*
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 *               DATA STRUCTURE DEFINITIONS
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 */

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * Thread Control Block typedef
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
typedef struct tcb tcb_t;

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * Thread Control Block
 * The Thread Control Block holds information about the
 * thread such as the stack pointer, priority level,
 * blocked status, next and previous TCB pointers
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
struct tcb {
    int32_t *stackPointer;
    struct tcb *nextTCB;
    struct tcb *previousTCB;
    semaphore_t *blocked;
    uint32_t sleepCount;
    bool asleep;
    uint8_t priority;
    bool alive;
    threadID_t threadID;
    char threadName[MAX_NAME_LENGTH];
};

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * Periodic Thread Control Block typedef
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
typedef struct ptcb ptcb_t;

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * Periodic Thread Control Block
 * The Periodic Thread Control Block holds information
 * about the periodic thread such as function pointer,
 * period, execute & current time and the next and
 * previous pointers for the doubly linked list
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
struct ptcb {
    void (*handler)();
    uint32_t period;
    uint32_t executeTime;
    uint32_t currentTime;
    struct ptcb *previousPTCB;
    struct ptcb *nextPTCB;
};

/*
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 *                    PUBLIC VARIABLEs
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 */

/* pointer to active thread */
tcb_t * CurrentlyRunningThread;

#endif /* G8RTOS_STRUCTURES_H_ */
