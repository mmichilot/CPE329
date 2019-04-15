/**
 * main.c
 *    CPE 329-15
 *    Assignment 4: Keypad
 *    Author: Celestine Co & Matthew Michilot
 */

#include <msp.h>

#include "delay.h"
#include "LCD.h"
#include "set_dco.h"

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
}
