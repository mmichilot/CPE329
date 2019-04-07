#include "msp.h"
#include "dco_set.h"
#include "delay.h"

/**
 * main.c
 *    CPE 329-15
 *    Assignment 2: Blinking LED, Clock Control, and Software Delay
 *    Author: Celestine Co & Matthew Michilot
 */

void blink_red();

void main(void)
{
    uint32_t freq = FREQ_1_5_MHz;
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	P4->SEL0 |= BIT3; // P4.3 set as MCLK
	P4->SEL1 &= ~BIT3;
	P4->DIR |= BIT3;

	set_DCO(freq);

	while (1);

}

void blink_red() {

    uint16_t i = 0;
    // TURN ON RED LED
    P1->SEL0 &= ~BIT0; // P1.0 set as GPIO
    P1->SEL1 &= ~BIT1;

    P1->DIR |= 1;       // Set Direction to Output

    while (1) {
        P1->OUT ^= BIT0;
        for (i=65535;i > 0;i++);
    }
}


