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
#include "delay.h"

volatile int flag = 0;
volatile int analogValue = 0;

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
	    if (flag) {
	        voltage = (0.0002 * analogValue) - 0.0112;
	        str_voltage(voltage);
//	        delay_us(40);
	        ADC14->CTL0 |= ADC14_CTL0_SC;

	        flag = 0;
	    }
	}
}

void ADC14_IRQHandler(void) {
    analogValue = ADC14->MEM[2];
    flag = 1;
}

void str_voltage(float voltage) {
    float ones, tenth, hundredth = 0;
    char volt_str[7] = "0.00 V";

    ones = (int)(voltage);
    tenth = (int)(10 * (voltage - ones));
    hundredth = (int)(10 * ((voltage - ones) * 10 - tenth));

    volt_str[0] += ones;  // ones digit
    volt_str[2] += tenth; // tenth digit
    volt_str[3] += hundredth; // hundredth digit

    print_string(volt_str);
}
