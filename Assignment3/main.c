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
    set_DCO(FREQ_24_MHz);

}
