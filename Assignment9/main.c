/**
 * main.c
 *    CPE 329-15
 *    Assignment 9: Analog to Digital Conversion
 *    Author: Celestine Co & Matthew Michilot
 */

#include "msp.h"
#include "set_dco.h"
#include "adc.h"
#include "uart.h"

void str_voltage(float voltage);

void main(void)
{
    float voltage = 0;

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	// set MCLK to 48 MHz
	set_DCO(FREQ_48_MHz);

	// initialize ADC
	init_ADC();

	//initialize UART
	init_UART();

	__enable_irq();

	while(1) {
	    if (get_flag_adc()) {
	        voltage = (0.0002 * get_voltage_adc()) - 0.0112;
	        str_voltage(voltage);
	        ADC14->CTL0 |= ADC14_CTL0_SC;
	    }
	}
}
