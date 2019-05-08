/**
 * main.c
 *    CPE 329-15
 *    Assignment 8: UART Communication
 *    Author: Celestine Co & Matthew Michilot
 */

#include "msp.h"
#include "dac.h"

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

}
