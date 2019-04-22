/**
 * main.c
 *    CPE 329-15
 *    Assignment 5: Interrupts and Timers
 *    Author: Celestine Co & Matthew Michilot
 */

#include "msp.h"
#include "set_dco.h"

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    P4->SEL0 |= BIT3;           // P4.3 set as MCLK
    P4->SEL1 &= ~BIT3;
    P4->DIR |= BIT3;

    P2->SEL0 &= ~BIT5;          // P2.5 set as GPIO
    P2->SEL1 &= ~BIT5;
    P2->DIR |= BIT5;            // Set Direction to Output

    set_DCO(FREQ_24_MHz);
}
