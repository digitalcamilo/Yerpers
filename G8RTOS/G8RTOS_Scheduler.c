/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * | AUTHOR: Raz Aloni                                               |
 * | DATE: 01/12/2017                                                |
 * | MODIFIED BY: Camilo Chen                                        |
 * | SUMMARY: G8RTOS_Scheduler.c                                     |
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */

/*
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 *                DEPENDENCIES AND EXTERNS
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 */

#include <BSP.h>
#include <stdint.h>
#include "msp.h"
#include "G8RTOS_Scheduler.h"
#include "G8RTOS_Structures.h"
#include "G8RTOS_CriticalSection.h"

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * G8RTOS_Start
 * INPUTS: void
 * OUTPUTS: void
 * ASM function to start the G8RTOS
 *  - CRITICAL SECTION
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
extern void G8RTOS_Start();

/* System Core Clock From system_msp432p401r.c */
extern uint32_t SystemCoreClock;

/* pointer to the currently running Thread Control Block */
extern tcb_t * CurrentlyRunningThread;

/*
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 *                        DEFINES
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 */

/* Status Register with the Thumb-bit Set */
#define THUMBBIT 0x01000000

/* desired overflow time for SysTick */
#define SysTickHigh 0.001f

/*
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 *                  DATA STRUCTURES USED
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 */

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * Thread Control Blocks
 * An array of thread control blocks to hold pertinent
 * information for each thread
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
static tcb_t threadControlBlocks[MAX_THREADS];

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * Thread Stacks
 * An array of arrays that will act as individual
 * stacks for each thread
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
static int32_t threadStacks[MAX_THREADS][STACKSIZE];

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * Periodic Event Threads
 * An array of periodic events to hold pertinent
 * information for each thread
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
static ptcb_t Pthread[MAXPTHREADS];

/*
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 *                    PRIVATE VARIABLES
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 */

/* current number of threads in the scheduler */
static uint32_t NumberOfThreads;

/* current number of periodic threads in the scheduler */
static uint32_t NumberOfPthreads;

/* current number of IDs */
static uint16_t IDCounter;

/*
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 *                    PRIVATE FUNCTIONS
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 */

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * InitSysTick
 * INPUTS: (uint32_t) numCycles
 * OUTPUTS: void
 * Initializes the Systick and Systick Interrupt
 *  - The Systick interrupt will be responsible for
 *    starting a context switch between threads
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
static void InitSysTick(uint32_t numCycles)
{
    /* configure SysTick */
    SysTick_Config(numCycles - 1);

    /* enable SysTick interrupts */
    SysTick_enableInterrupt();
}

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * G8RTOS_Scheduler
 * INPUTS: void
 * OUTPUTS: void
 * Chooses the next thread to run in the priority
 * cheduler that is neither blocked, sleeping,
 * or dead
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
void G8RTOS_Scheduler()
{
    /* set a default max priority to lowest priority */
    uint8_t currentMaxPriority = UINT8_MAX;

    /* set a temporary next thread */
    tcb_t * tempNextThread = CurrentlyRunningThread;

    /* traverse through tcb linked list until the highest priority thread is neither sleeping or blocked */
    for (int i = 0; i < NumberOfThreads; i++) {
        if (!tempNextThread->blocked && !tempNextThread->asleep && tempNextThread->alive) {
            if (tempNextThread->priority < currentMaxPriority) {
                /* update currently running thread */
                CurrentlyRunningThread = tempNextThread;

                /* set new max priority */
                currentMaxPriority = CurrentlyRunningThread->priority;
            }
        }

        /* update temporary next thread to check other threads' priorities */
        tempNextThread = tempNextThread->nextTCB;
    }
}

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * SysTick_Handler
 * INPUTS: void
 * OUTPUTS: void
 * Increments system time and starts the scheduler by
 * setting the PendSV flag. Additionally, periodic
 * threads that are ready to execute will be run and
 * sleeping threads that are ready to wake up will be
 * activated
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
void SysTick_Handler()
{
    /* increment system time */
    SystemTime++;

    /* temporary periodic thread pointer */
    ptcb_t * Pptr;

    /* traverse through ptcb linked list to execute periodic threads that are ready */
    for (int i = 0; i < NumberOfPthreads; i++) {
        /* point to new periodic thread */
        Pptr = &Pthread[i];

        /* if periodic thread is ready to execute, run the function pointer */
        if (Pptr->executeTime == SystemTime) {
            /* update execute time */
            Pptr->executeTime = Pptr->period + SystemTime;

            /* run function pointer */
            (*Pptr->handler)();
        }
    }

    /* temporary thread pointer */
    tcb_t * ptr = CurrentlyRunningThread->nextTCB;

    /* traverse through tcb linked list to wake up sleeping threads that finished sleeping */
    for (int i = 0; i < NumberOfThreads; i++) {
        /* if thread is asleep and it is done sleeping, wake it up */
        if (ptr->asleep && ptr->sleepCount == SystemTime)
            ptr->asleep = false;

        /* point to next thread */
        ptr = ptr->nextTCB;
    }

    /* set PendSV flag to start context switch */
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

/*
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 *                    PUBLIC VARIABLES
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 */

/* Holds the current time for the whole System */
uint32_t SystemTime;

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
void G8RTOS_Init()
{
    /* init system time to 0 */
    SystemTime = 0;

    /* init number of threads to 0 */
    NumberOfThreads = 0;

    /* init IDCounter */
    IDCounter = 0;

    /* init all hardware on board */
    BSP_InitBoard();

    /* relocate ISRs interrupt vectors to SRAM */
    uint32_t newVTORTable = 0x20000000;
    memcpy((uint32_t *)newVTORTable, (uint32_t *)SCB->VTOR, 57*4);
    SCB->VTOR = newVTORTable;
}

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
int G8RTOS_Launch()
{
    /* set SysTick to lowest priority */
    NVIC_SetPriority(SysTick_IRQn, OSINT_PRIORITY);

    /* set PendSV to lowest priority */
    NVIC_SetPriority(PendSV_IRQn, OSINT_PRIORITY);

    /* set SysTick interrupt every 1 ms */
    InitSysTick(ClockSys_GetSysFreq() * SysTickHigh);

    /* set a default max priority */
    uint8_t currentMaxPriority = UINT8_MAX;

    /* traverse through tcb array to find highest priority thread and launch with this active thread */
    for (int i = 0; i < NumberOfThreads; i++) {
        if (threadControlBlocks[i].priority < currentMaxPriority) {
            /* if higher priority is found, update currently running thread */
            CurrentlyRunningThread = &threadControlBlocks[i];

            /* update max priority */
            currentMaxPriority = CurrentlyRunningThread->priority;
        }
    }

    /* set context of first thread */
    G8RTOS_Start();

    /* return error code */
    return NO_THREADS_SCHEDULED;
}

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
int G8RTOS_AddThread(void (*threadToAdd)(void), uint8_t priority, char * name)
{
    /* start critical section & disable interrupts */
    int32_t status = StartCriticalSection();

    /* return error code and end critical section if thread limit reached */
    if (NumberOfThreads == MAX_THREADS) {
        EndCriticalSection(status);
        return THREAD_LIMIT_REACHED;
    }

    /* thread offset */
    int i = 0;

    /* search for dead thread */
    while (i < NumberOfThreads) {
        /* once dead thread is found, break out of loop */
        if (!threadControlBlocks[i].alive)
            break;

        /* increment i to search through threads */
        i++;
    }

    /* point next & prev TCB pointers properly */
    if (NumberOfThreads == 0) {
        threadControlBlocks[0].nextTCB = &threadControlBlocks[0];
        threadControlBlocks[0].previousTCB = &threadControlBlocks[0];
    } else {
        if (i == 0) {
            for (int j = 0; j < MAX_THREADS; j++) {
                if (threadControlBlocks[j].alive == true) {
                    threadControlBlocks[0].nextTCB = &threadControlBlocks[j];
                    threadControlBlocks[0].previousTCB = threadControlBlocks[j].previousTCB;
                    break;
                }
            }
        }
            //threadControlBlocks[i].previousTCB = &threadControlBlocks[NumberOfThreads - 1];
        else {
            threadControlBlocks[i].previousTCB = &threadControlBlocks[i - 1];

            /* thread's next ptr points to next of previous thread */
            threadControlBlocks[i].nextTCB = threadControlBlocks[i].previousTCB->nextTCB;
        }

        /* thread's next's previous pointer pointing to added thread */
        threadControlBlocks[i].nextTCB->previousTCB = &threadControlBlocks[i];

        /* thread's previous' next pointer pointing to added thread */
        threadControlBlocks[i].previousTCB->nextTCB = &threadControlBlocks[i];
    }

    /* assign default values to thread properties */
    threadControlBlocks[i].blocked = 0;
    threadControlBlocks[i].sleepCount = 0;
    threadControlBlocks[i].asleep = false;
    threadControlBlocks[i].priority = priority;
    threadControlBlocks[i].alive = true;

    /* assign thread id */
    threadControlBlocks[i].threadID = ((IDCounter++) << 16) | i;

    /* assign name to thread */
    uint8_t name_offset = 0;
    while (*name)
        threadControlBlocks[i].threadName[name_offset++] = *name++;

    /* store sp of thread */
    threadControlBlocks[i].stackPointer = &threadStacks[i][STACKSIZE - 16];

    /* store context */
    threadStacks[i][STACKSIZE - 1] = THUMBBIT;                // PSR w/ thumbit enabled
    threadStacks[i][STACKSIZE - 2] = (int32_t)(threadToAdd);  // PC w/ functiooon pointer to thread
    threadStacks[i][STACKSIZE - 3] = 0x14141414;              // LR w/ dummy data
    threadStacks[i][STACKSIZE - 4] = 0x12121212;              // r12 w/ dummy data
    threadStacks[i][STACKSIZE - 5] = 0x03030303;              // r3 w/ dummy data
    threadStacks[i][STACKSIZE - 6] = 0x02020202;              // r2 w/ dummy data
    threadStacks[i][STACKSIZE - 7] = 0x01010101;              // r1 w/ dummy data
    threadStacks[i][STACKSIZE - 8] = 0x00000000;              // r0 w/ dummy data
    threadStacks[i][STACKSIZE - 9] = 0x11111111;              // r11 w/ dummy data
    threadStacks[i][STACKSIZE - 10] = 0x10101010;             // r10 w/ dummy data
    threadStacks[i][STACKSIZE - 11] = 0x09090909;             // r9 w/ dummy data
    threadStacks[i][STACKSIZE - 12] = 0x08080808;             // r8 w/ dummy data
    threadStacks[i][STACKSIZE - 13] = 0x07070707;             // r7 w/ dummy data
    threadStacks[i][STACKSIZE - 14] = 0x06060606;             // r6 w/ dummy data
    threadStacks[i][STACKSIZE - 15] = 0x05050505;             // r5 w/ dummy data
    threadStacks[i][STACKSIZE - 16] = 0x04040404;             // r4 w/ dummy data

    /* increment number of threads */
    NumberOfThreads++;

    /* end critical section and enable interrupts */
    EndCriticalSection(status);

    /* return error code */
    return NO_ERROR;
}

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
int G8RTOS_AddPeriodicEvent(void (*PthreadToAdd)(void), uint32_t period)
{
    /* start critical section & disable interrupts */
    int32_t status = StartCriticalSection();

    /* end critical section, enable interrupts, and return error code if max periodic threads */
    if (NumberOfPthreads == MAXPTHREADS) {
        EndCriticalSection(status);
        return THREAD_LIMIT_REACHED;
    }

    /* initialize periodic thread's data structure */
    Pthread[NumberOfPthreads].handler = PthreadToAdd;
    Pthread[NumberOfPthreads].period = period;
    Pthread[NumberOfPthreads].currentTime = SystemTime;
    Pthread[NumberOfPthreads].executeTime = NumberOfPthreads + 1;

    /* point next & prev PTCB properly */
    if (NumberOfPthreads == 0) {
        Pthread[NumberOfPthreads].nextPTCB = &Pthread[NumberOfPthreads];
        Pthread[NumberOfPthreads].previousPTCB = &Pthread[NumberOfPthreads];
    }
    else {
        Pthread[NumberOfPthreads].nextPTCB = &Pthread[0];
        Pthread[NumberOfPthreads].previousPTCB = &Pthread[NumberOfPthreads - 1];
        Pthread[NumberOfPthreads - 1].nextPTCB = &Pthread[NumberOfPthreads];
        Pthread[0].previousPTCB = &Pthread[NumberOfPthreads];
    }

    /* increment number of periodic threads */
    NumberOfPthreads++;

    /* end critical section and enable interrupts */
    EndCriticalSection(status);

    /* return error code */
    return NO_ERROR;
}

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
sched_ErrCode_t G8RTOS_AddAperiodicEvent(void (*AthreadToAdd)(void), uint8_t priority, IRQn_Type IRQn)
{
    /* start critical section & disable interrupts */
    int32_t status = StartCriticalSection();

    /* verify that IRQn is less than PSS_IRQn and greater than PORT6_IRQn */
    if (IRQn < PSS_IRQn || IRQn > PORT6_IRQn) {
        /* end critical section and enable interrupts */
        EndCriticalSection(status);

        /* return error code */
        return IRQn_INVALID;
    }

    /* verify that IRQn priority is not greater than the greatest user priority */
    if (priority > 6) {
        /* end critical section and enable interrupts */
        EndCriticalSection(status);

        /* return error code */
        return HWI_PRIORITY_INVALID;
    }


    /* initialize NVIC registers */
    __NVIC_SetVector(IRQn, (uint32_t)AthreadToAdd);
    __NVIC_SetPriority(IRQn, priority);
    NVIC_EnableIRQ(IRQn);

    /* end critical section and enable interrupts */
    EndCriticalSection(status);

    /* return error code */
    return NO_ERROR;
}

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * G8RTOS_Sleep
 * INPUTS: (uint32_t) durationMS
 * OUTPUTS: void
 * Puts the current thread into a sleep state.
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
void G8RTOS_Sleep(uint32_t durationMS)
{
    /* set sleep duration */
    CurrentlyRunningThread->sleepCount = durationMS + SystemTime;

    /* sleep thread */
    CurrentlyRunningThread->asleep = true;

    /* set PendSV flag to start scheduler */
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * G8RTOS_GetThreadId
 * INPUTS: void
 * OUTPUTS: (threadID_t) threadID
 * Get the thread ID of the currently running thread
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
threadID_t G8RTOS_GetThreadId()
{
    return CurrentlyRunningThread->threadID;
}

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
sched_ErrCode_t G8RTOS_KillThread(threadID_t threadId)
{
    /* start critical section & disable interrupts */
    int32_t status = StartCriticalSection();

    /* check if there is only one thread */
    if (NumberOfThreads == 1) {
        /* end critical section and enable interrupts */
        EndCriticalSection(status);

        /* return error code */
        return CANNOT_KILL_LAST_THREAD;
    }

    /* thread offset */
    int i = 0;

    /* find thread to kill */
    while (i < MAX_THREADS) {
        if (threadControlBlocks[i].threadID == threadId && threadControlBlocks[i].alive == true)
            break;
        i++;
    }

    /* check if thread exists */
    if (i == MAX_THREADS) {
        /* end critical section and enable interrupts */
        EndCriticalSection(status);

        /* return error code */
        return THREAD_DOES_NOT_EXIST;
    }

    /* kill thread and adjust doubly linked list */
    threadControlBlocks[i].alive = false;
    threadControlBlocks[i].previousTCB->nextTCB = threadControlBlocks[i].nextTCB;
    threadControlBlocks[i].nextTCB->previousTCB = threadControlBlocks[i].previousTCB;
    threadControlBlocks[i].asleep = false;
    threadControlBlocks[i].blocked = false;
    threadControlBlocks[i].priority = 255;
    threadControlBlocks[i].sleepCount = 0;
    threadControlBlocks[i].threadID = 0;

    /* decrement number of threads */
    NumberOfThreads--;

    /* check if thread being killed is currently running */
    if (CurrentlyRunningThread == &threadControlBlocks[i]) {
        /* flush pipelines to context switch immediately */
        __DSB();
        __ISB();

        /* end critical section and enable interrupts */
        EndCriticalSection(status);

        /* set PendSV flag to start scheduler */
        SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

        /* return error code */
        return NO_ERROR;
    }

    /* end critical section and enable interrupts */
    EndCriticalSection(status);

    /* return error code */
    return NO_ERROR;
}

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
sched_ErrCode_t G8RTOS_KillSelf()
{
    /* start critical section & disable interrupts */
    int32_t status = StartCriticalSection();

    /* check if there is only one thread */
    if (NumberOfThreads == 1) {
        /* end critical section and enable interrupts */
        EndCriticalSection(status);

        /* return error code */
        return CANNOT_KILL_LAST_THREAD;
    }

    /* kill thread and adjust doubly linked list */
    CurrentlyRunningThread->alive = false;
    CurrentlyRunningThread->previousTCB->nextTCB = CurrentlyRunningThread->nextTCB;
    CurrentlyRunningThread->nextTCB->previousTCB = CurrentlyRunningThread->previousTCB;
    CurrentlyRunningThread->asleep = false;
    CurrentlyRunningThread->blocked = false;

    /* decrement number of threads */
    NumberOfThreads--;

    /* flush pipelines to context switch immediately */
    __DSB();
    __ISB();

    /* end critical section and enable interrupts */
    EndCriticalSection(status);

    /* set PendSV flag to start scheduler */
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

    /* return error code */
    return NO_ERROR;
}

/*
 * kills all threads but currently running thread
 */
void G8RTOS_KillAllThreads()
{
    uint32_t status = StartCriticalSection();
    for (int i = 0; i < MAX_THREADS; i++) {
        if (CurrentlyRunningThread->threadID != threadControlBlocks[i].threadID && threadControlBlocks[i].alive == true) G8RTOS_KillThread(threadControlBlocks[i].threadID);
    }
    EndCriticalSection(status);
}
