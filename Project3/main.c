#include "msp.h"
#include "set_dco.h"

/**
 * main.c
 */

uint32_t count = 0;

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	set_DCO(FREQ_3_MHz);

	// Configure TimerA0
	TIMER_A0->CTL |= TIMER_A_CTL_TASSEL_2 |     // use SMCLK
	                 TIMER_A_CTL_MC_2;          // continuous mode

	TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_CM_1 |   // capture on rising edge
	                     TIMER_A_CCTLN_CAP |    // capture on CCIxA
	                     TIMER_A_CCTLN_CCIE |   // use capture mode
	                     TIMER_A_CCTLN_SCS;     // synchronous capture

	// Configure P2.4
	P2->SEL0 |= BIT4;
	P2->SEL1 &= ~BIT4;
	P2->DIR &= ~BIT4;
	P2->REN |= BIT4;
	P2->OUT &= ~BIT4;

    NVIC->ISER[0] =  1 << (TA0_N_IRQn & 31);   // enable CCR1 ISR

	__enable_irq();
	while(1);
}

void TA0_N_IRQHandler(void) {
    uint32_t tmp_count;
    if (TIMER_A0->CCTL[1] & TIMER_A_CCTLN_CCIFG) {
        TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;
        count = TIMER_A0->CCR[1];
//        if (tmp_count < count)
//            count = tmp_count + (65535 - count) ;
//        else
//            count = tmp_count - count;
    }
}
