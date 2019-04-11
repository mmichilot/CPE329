/**
 * main.c
 *    CPE 329-15
 *    Assignment 3: LCD Display
 *    Author: Celestine Co & Matthew Michilot
 */

#include "msp.h"
#include "LCD.h"
#include "set_dco.h"

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    P4->SEL0 &= ~(BIT3|BIT2|BIT1|BIT0);        // P4.0-3 = Data Bus
    P4->SEL1 &= ~(BIT3|BIT2|BIT1|BIT0);
    P4->DIR |= BIT3|BIT2|BIT1|BIT0;            // set direction to output

    P3->SEL0 &= ~(BIT3|BIT2|BIT0);         // P3.0=RW, 3.2=RS, 3.3=EN
    P3->SEL1 &= ~(BIT3|BIT2|BIT0);
    P3->DIR |= BIT3|BIT2|BIT0;            // set direction to output

    init();
    write_char_LCD(CHAR_H);
    write_char_LCD(CHAR_E);
    write_char_LCD(CHAR_L);
    write_char_LCD(CHAR_L);
    write_char_LCD(CHAR_O);
    write_char_LCD(CHAR_SP);
    write_char_LCD(CHAR_W);
    write_char_LCD(CHAR_O);
    write_char_LCD(CHAR_R);
    write_char_LCD(CHAR_L);
    write_char_LCD(CHAR_D);

}
