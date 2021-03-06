/**
 * main.c
 *    CPE 329-15
 *    Project 3: Digital Multimeter
 *    Author: Celestine Co & Matthew Michilot
 */

#include "msp.h"
#include "set_dco.h"
#include "uart.h"
#include "adc.h"
#include "dmm.h"
#include "terminal.h"
#include "multimeter_states.h"

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	set_DCO(FREQ_48_MHz);
	init_ADC();
	init_UART();
	init_timer();
	init_terminal();

	__enable_irq();

	dmm_fsm();
	while(1);
}
