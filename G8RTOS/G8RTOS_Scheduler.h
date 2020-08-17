/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * | AUTHOR: Raz Aloni                                               |
 * | DATE: 01/12/2017                                                |
 * | MODIFIED BY: Camilo Chen                                        |
 * | SUMMARY: G8RTOS_Scheduler.h                                     |
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */

#ifndef G8RTOS_SCHEDULER_H_
#define G8RTOS_SCHEDULER_H_

#include "msp.h"

/*
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 *                        DEFINES
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 */

#define MAX_THREADS 24
#define MAXPTHREADS 6
#define STACKSIZE 512
#define OSINT_PRIORITY 7

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * Thread ID typedef
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
typedef uint32_t threadID_t;

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * Scheduler error code typedef
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
typedef enum {
    NO_ERROR = 0,
    THREAD_LIMIT_REACHED = -1,
    NO_THREADS_SCHEDULED = -2,
    THREADS_INCORRECTLY_ALIVE = -3,
    THREAD_DOES_NOT_EXIST = -4,
    CANNOT_KILL_LAST_THREAD = -5,
    IRQn_INVALID = -6,
    HWI_PRIORITY_INVALID = -7
} sched_ErrCode_t;

/*
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 *                    PUBLIC VARIABLES
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 */

/* current time for entire system */
extern uint32_t SystemTime;

/*
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 *                    PUBLIC FUNCTIONS
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 */

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * G8RTOS_Init
 * INPUTS: void
 * OUTPUTS: void
 * Initializes variables and hardware for G8RTOS usage
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
void G8RTOS_Init();

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * G8RTOS_Launch
 * INPUTS: void
 * OUTPUTS: (int) error
 * Starts G8RTOS scheduler
 *  - Initializes SysTick timer
 *  - Initializes interrupt priorities
 *  - Sets context to highest priority thread
 *  - Returns error code if scheduler fails
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
int G8RTOS_Launch();

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * G8RTOS_AddThread
 * INPUTS: (void)(* threadToAdd)(void), (uint8_t) priority,
 *         (char)(* name)
 * OUTPUTS: (int) error
 * Adds threads to G8RTOS Scheduler
 *  - Checks for available threads to add
 *  - Initializes the tcb for the provided thread
 *  - Initializes the stack for the provided thread
 *  - Links next and previous pointers
 *  - Returns error code if added thread fails
 *  - CRITICAL SECTION
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
int G8RTOS_AddThread(void (*threadToAdd)(void), uint8_t priority, char * name);

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * G8RTOS_AddPeriodicEvent
 * INPUTS: (void)(* PthreadToAdd)(void), (uint32_t) period
 * OUTPUTS: (int) error
 * Adds periodic threads to G8RTOS Scheduler
 *  - Initialize a periodic event struct to represent
 *    event
 *  - The struct will be added to a linked list of
 *    periodic events
 *  - CRITICAL SECTION
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
int G8RTOS_AddPeriodicEvent(void (*PthreadToAdd)(void), uint32_t period);

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * G8RTOS_Sleep
 * INPUTS: (uint32_t) durationMS
 * OUTPUTS: void
 * Puts the current thread into a sleep state.
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
void G8RTOS_Sleep(uint32_t durationMS);

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * G8RTOS_AddAperiodicEvent
 * INPUTS: (void)(* AthreadToAdd)(void),
 *         (uint8_t) priority, (IRQn_Type) IRQn
 * OUTPUTS: (sched_ErrCode_t) error
 * Adds aperiodic threads to G8RTOS Scheduler
 *  - Initialize a NVIC registers for aperiodic event
 *  - CRITICAL SECTION
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
sched_ErrCode_t G8RTOS_AddAperiodicEvent(void (*AthreadToAdd)(void), uint8_t priority, IRQn_Type IRQn);

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * G8RTOS_GetThreadId
 * INPUTS: void
 * OUTPUTS: (threadID_t) threadID
 * Get the thread ID of the currently running thread
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
threadID_t G8RTOS_GetThreadId();

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * G8RTOS_KillThread
 * INPUTS: (threadID_t) threadId
 * OUTPUTS: (sched_ErrCode_t) error
 * Kills a thread specified by threadID
 *  - Kills specified thread and context switches
 *    to next thread
 *  - Readjusts doubly linked list when thread is killed
 *  - CRITICAL SECTION
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
sched_ErrCode_t G8RTOS_KillThread(threadID_t threadId);

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * G8RTOS_KillSelf
 * INPUTS: void
 * OUTPUTS: (sched_ErrCode_t) error
 * Kills currently running thread
 *  - Kills currently running thread and context switches
 *    to next thread
 *  - Readjusts doubly linked list when thread is killed
 *  - CRITICAL SECTION
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
sched_ErrCode_t G8RTOS_KillSelf();

void G8RTOS_KillAllThreads();

#endif /* G8RTOS_SCHEDULER_H_ */
