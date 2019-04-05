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
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
}
