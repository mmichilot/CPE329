/**
 * main.c
 *    CPE 329-15
 *    Assignment 2: Blinking LED, Clock Control, and Software Delay
 *    Author: Celestine Co & Matthew Michilot
 */

#include "set_dco.h"
#include "msp.h"
#include "delay.h"

void blink_red();

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	P4->SEL0 |= BIT3; // P4.3 set as MCLK
	P4->SEL1 &= ~BIT3;
	P4->DIR |= BIT3;

	P2->SEL0 &= ~BIT5; // P2.5 set as GPIO
	P2->SEL1 &= ~BIT5;
	P2->DIR |= BIT5; // Set Direction to Output

	set_DCO(FREQ_1_5_MHz);

	while (1){
	    P2->OUT ^= BIT5; // Toggle P2.5
	    delay_us(20);
	}

}

void blink_red() {

    uint16_t i = 0;
    // TURN ON RED LED
    P1->SEL0 &= ~BIT0; // P1.0 set as GPIO
    P1->SEL1 &= ~BIT0;

    P1->DIR |= 1;       // Set Direction to Output

    while (1) {
        P1->OUT ^= BIT0;
        for (i=65535;i > 0;i++);
    }
}


