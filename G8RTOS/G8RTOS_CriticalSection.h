/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * | AUTHOR: Raz Aloni                                               |
 * | DATE: 12/11/2016                                                |
 * | MODIFIED BY: Camilo Chen                                        |
 * | SUMMARY: G8RTOS_CriticalSection.h                               |
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */

#ifndef G8RTOS_CRITICALSECTION_H_
#define G8RTOS_CRITICALSECTION_H_

/*
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 *                    PUBLIC FUNCTIONS
 * +=====+=====+=====+=====+=====+=====+=====+=====+=====+
 */

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * StartCriticalSection
 * INPUTS: void
 * OUTPUTS: (int32_t) IBit_State
 * Starts a critical section
 *  - Saves the state of the current PRIMASK (I-bit)
 *  - Disables interrupts
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
extern int32_t StartCriticalSection();

/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * EndCriticalSection
 * OUTPUTS: (int32_t) IBit_State
 * Ends a critical section
 *  - Restores the state of the PRIMASK given an input
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
extern void EndCriticalSection(int32_t IBit_State);

#endif /* G8RTOS_CRITICALSECTION_H_ */
