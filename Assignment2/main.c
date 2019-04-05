#include "msp.h"
#include "dco_set.h"
#include "delay.h"

/**
 * main.c
 *    CPE 329-15
 *    Assignment 2: Blinking LED, Clock Control, and Software Delay
 *    Author: Celestine Co & Matthew Michilot
 */
void main(void)
{
    uint16_t i, color = 0;

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	// TURN ON RED LED
	P1->SEL0 &= ~BIT0; // P1.0 set as GPIO
	P1->SEL1 &= ~BIT1;

	P1->DIR |= 1;       // Set Direction to Output

}
