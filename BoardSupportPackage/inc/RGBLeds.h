/*
 * RGBLeds.h
 *
 *  Created on: Nov 14, 2019
 *      Author: voyager
 */

#ifndef RGBLEDSH
#define RGBLEDSH
#include <msp432p401r.h>
#include "driverlib.h"
#include "msp.h"

/* Enums for RGB LEDs */
typedef enum device
{
    BLUE = 0,
    GREEN = 1,
    RED = 2
} unit_desig;

typedef struct rgb_stuct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} RGB_Data;

/*
 * i2c_write
 * Performs a basic write operation to relevant chip
 */
void i2c_write(uint8_t chip_addr, uint8_t data1, uint8_t data2);

/*
 * LP3943_SetRGB
 * This function will set the relevant PWM settings to display
 * the requested color on the indexed LED
 */
void LP3943_SetRGB(RGB_Data color, uint16_t LED_DATA, bool primary);

/*
 * LP3943_ColorSet
 * This function will set the frequencies and PWM duty cycle
 * for each register of the specified unit.
 */
void LP3943_ColorSet(uint32_t unit, uint8_t freq, uint8_t duty_cycle, bool primary );

/*
 * LP3943_LedModeSet
 * This function will set each of the LEDs to the desired operating
 * mode. The operating modes are on, off, PWM1, PWM2.
 */
void LP3943_LedModeSet(uint32_t unit, uint16_t LED_DATA, uint8_t mode);

/*
 * Performs necessary initializations for RGB LEDS
 */
void init_RGBLEDS();

#endif / RGBLEDSH */
