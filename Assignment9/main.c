/**
 * main.c
 *    CPE 329-15
 *    Assignment 9: Analog to Digital Conversion
 *    Author: Celestine Co & Matthew Michilot
 */

#include "msp.h"
#include "set_dco.h"
#include "adc.h"

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	// set MCLK to 48 MHz
	set_DCO(FREQ_48_MHz);

	// initialize ADC
	init_ADC();

}
