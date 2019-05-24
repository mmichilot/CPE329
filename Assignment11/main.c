#include "msp.h"


/**
 * main.c
 */

#define PERIOD_20MS 60000
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	// Setup P2.5 as Timer A0 CCR2 Output
	P2->SEL0 |= BIT5;
	P2->SEL1 &= ~BIT5;
	P2->DIR |= BIT5;

    TIMER_A0->CCR[0] = PERIOD_20MS;
    TIMER_A0->CCR[2] = 30000;

    // Setup Timer A0 CCR2
    TIMER_A0->CCTL[2] = TIMER_A_CCTLN_OUTMOD_7; // set output mode as reset/set


	// Setup Timer A0
	TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 |  // use SMCLK
	                TIMER_A_CTL_MC_1;       // up mode

	// 7000 - 0 degrees
	// 4200 - 90 degrees
	// 1400 - 180 degrees

	while(1);

}
