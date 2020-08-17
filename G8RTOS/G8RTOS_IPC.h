/*
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * | AUTHOR: Daniel Gonzalez                                         |
 * | DATE: 01/10/2017                                                |
 * | MODIFIED BY: Camilo Chen                                        |
 * | SUMMARY: G8RTOS_IPC.h                                           |
 * +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */

#ifndef G8RTOS_G8RTOS_IPC_H_
#define G8RTOS_G8RTOS_IPC_H_

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
int G8RTOS_InitFIFO(uint32_t FIFOIndex);

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
uint32_t readFIFO(uint32_t FIFO);

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
int writeFIFO(uint32_t FIFO, uint32_t data);

#endif /* G8RTOS_G8RTOS_IPC_H_ */
