/*
 * RGBLeds.c
 *
 *  Created on: Nov 14, 2019
 *      Author: voyager
 */

#include "RGBLeds.h"

/*
 * i2c_write
 * Performs a basic write operation to relevant chip
 */
void i2c_write(uint8_t chip_addr, uint8_t data1, uint8_t data2)
{
    UCB2I2CSA = chip_addr;

    UCB2CTLW0 |= UCTR;

    // Generate START condition
    UCB2CTLW0 |= UCTXSTT;

    // Wait for buffer availability
    while ((UCB2CTLW0 & UCTXSTT) != 0);

    // Send initial register address
    UCB2TXBUF = data1;

    // Wait for buffer availability
    while (!(UCB2IFG & UCTXIFG0));

    UCB2TXBUF = data2;

    // Wait for buffer availability
    while (!(UCB2IFG & UCTXIFG0));

    // Generate STOP condition
    UCB2CTLW0 |= UCTXSTP;

    while ((UCB2CTLW0 & UCTXSTP) != 0);
}


/*
 * LP3943_ColorSet
 * This function will set the frequencies and PWM duty cycle
 * for each register of the specified unit.
 */
void LP3943_ColorSet(uint32_t unit, uint8_t freq, uint8_t duty_cycle, bool primary )
{
   if (primary) {
       i2c_write(0x60 + unit, 0x02, freq);
       i2c_write(0x60 + unit, 0x03, duty_cycle);
   } else {
       i2c_write(0x60 + unit, 0x04, freq);
       i2c_write(0x60 + unit, 0x05, duty_cycle);
   }

   return;
}

/*
 * LP3943_SetRGB
 * This function will set the relevant PWM settings to display
 * the requested color on the indexed LED
 */
void LP3943_SetRGB(RGB_Data color, uint16_t LED_DATA, bool primary) {
    LP3943_ColorSet(RED, 0x01, color.red, primary);
    LP3943_ColorSet(GREEN, 0x01, color.green, primary);
    LP3943_ColorSet(BLUE, 0x01, color.blue, primary);

    if (primary)
    {
        LP3943_LedModeSet(BLUE, LED_DATA, 2);
        LP3943_LedModeSet(RED, LED_DATA, 2);
        LP3943_LedModeSet(GREEN, LED_DATA, 2);
    } else {
        LP3943_LedModeSet(BLUE, LED_DATA, 3);
        LP3943_LedModeSet(RED, LED_DATA, 3);
        LP3943_LedModeSet(GREEN, LED_DATA, 3);
    }
}

/*
 * LP3943_LedModeSet
 * This function will set each of the LEDs to the desired operating
 * mode. The operating modes are on, off, PWM1, PWM2.
 */
void LP3943_LedModeSet(uint32_t unit, uint16_t LED_DATA, uint8_t mode)
{
    int i = 0;
    uint32_t LS_REG_DATA = 0;


    // Convert the passed in value to a 32 bit
    // representation for placing in the LP registers
    for (i = 0; i < 32; i+=2)
    {
        if (LED_DATA & 1) {
            if (!mode)
                LS_REG_DATA |= (1 << i);
            else
                LS_REG_DATA |= (mode << i);
        }
        LED_DATA >>= 1;
    }

    // Only write to register that actually need value updates
    // Prevents overwriting data unintentionally
    for (i = 0; i < 4; i++) {
        uint8_t testData = LS_REG_DATA >> (8 * i);
        if (testData && mode)
            i2c_write(0x60 + unit, 0x06 + i,testData);
        else if (testData && !mode)
            i2c_write(0x60 + unit, 0x06 + i, 0x00);
    }

}

/*
 * Performs necessary initializations for RGB LEDS
 */
void init_RGBLEDS()
{
    uint16_t UNIT_OFF = 0x00;

    // Software reset enable
    UCB2CTLW0 |= UCSWRST;

    // Init I2C master
    // Set as master, I2C mode, Clock sync, SMCLK source, Transmitter
    UCB2CTLW0 |= UCMODE_3 | UCMST | UCSSEL_2 | UCTR;

    // Set the Fclk as 400khz
    // Presumes that the SMCLK is select as source and Fsmclk is 12Mhz..
    UCB2BRW = 30;

    // In conjuction with the next line, this sets the pins as I2C mode.
    // (Table found on p160 of SLAS826E)
    // Set P3.6 as UCB2_SDA and 3.7 as UCB2_SLC
    P3SEL0 |= GPIO_PIN6 | GPIO_PIN7;
    P3SEL1 &= ~(GPIO_PIN6 | GPIO_PIN7);

    // Bitwide ansding of all bits except UCSWRST
    UCB2CTLW0 &= ~UCSWRST;

    // Start with all lights set to off
    LP3943_LedModeSet(RED, UNIT_OFF, 1);
    LP3943_LedModeSet(GREEN, UNIT_OFF, 1);
    LP3943_LedModeSet(BLUE, UNIT_OFF, 1);


}

