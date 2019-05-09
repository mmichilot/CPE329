/**
 * main.c
 *    CPE 329-15
 *    Assignment 8: UART Communication
 *    Author: Celestine Co & Matthew Michilot
 */

#include "msp.h"
#include "string.h"
#include "dac.h"
#include "set_dco.h"
#include "uart.h"

void main(void)
{
    int voltage = 0;

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	set_DCO(FREQ_3_MHz);

	// initialize serial devices
	init_SPI();
	init_UART();

	__enable_irq();

	esc_seq(BLINK_CURSOR);

	while(1) {
	    if (check_flag()) {
	        voltage = get_voltage();
	        if (voltage < 4096 && voltage >= 0)
	            set_voltage(voltage);
	        else {
	            print_char('\n');
	            print_string("Voltage must be between 0 and 4096");
	        }
	        print_char('\n');
	        print_char('\r');
	    }
	}
}
