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

uint32_t count = 0;
uint32_t num_trig = 0;

void init_timer(void);

void main(void)
{
    int curr_freq = 0;

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	set_DCO(FREQ_48_MHz);
	init_timer();

	__enable_irq();

	while(1) {
	    if (num_trig == 2) {
	        TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CM_1;   // temporarily disable capture

	        curr_freq = (int) (64000/count);    // calculate freq
	        if (count > 64000)      // edge case where freq. is 1
	            curr_freq++;

	        count = 0;
	        num_trig = 0;
	        TIMER_A0->CTL |= TIMER_A_CTL_CLR;
	        TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_CM_1;
	    }

	}
}

void TA0_N_IRQHandler(void) {

    uint32_t tmp_count;

    if (TIMER_A0->CCTL[1] & TIMER_A_CCTLN_CCIFG) {
        TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;  // clear interrupt flag
        tmp_count = TIMER_A0->CCR[1];

        if (count > tmp_count)
            count = count + (65535 - tmp_count);
        else
            count = tmp_count - count;

        num_trig += 1;
    }
}

void init_timer(void) {
    // Configure P2.4
        P2->SEL0 |= BIT4;
        P2->SEL1 &= ~BIT4;
        P2->DIR &= ~BIT4;
        P2->REN |= BIT4;
        P2->OUT &= ~BIT4;

        // Configure TimerA0
        TIMER_A0->CTL |= TIMER_A_CTL_TASSEL_1 |     // use ACLK
                         TIMER_A_CTL_ID_1 |         // divide ACLK by 2
                         TIMER_A_CTL_MC_2;          // continuous mode

        TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_CM_1 |   // capture on rising edge
                             TIMER_A_CCTLN_CAP |    // capture on CCIxA
                             TIMER_A_CCTLN_CCIE |   // use capture mode
                             TIMER_A_CCTLN_SCS;     // synchronous capture

        NVIC->ISER[0] =  1 << (TA0_N_IRQn & 31);   // enable CCR1 ISR
}
