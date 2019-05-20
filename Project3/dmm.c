/*
 * dmm.c
 *    CPE 329-15
 *    Project 3: Digital Multimeter
 *    Author: Celestine Co & Matthew Michilot
 */

#include "dmm.h"
#include "msp.h"

volatile uint32_t count = 0;
volatile uint32_t num_trigs = 0;
volatile uint32_t curr_freq = 0;
volatile uint32_t flag = 0;

int get_freq(void) {
    curr_freq = 0;
    count = 0;
    num_trigs = 0;

    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_CM_1;    // enable capture
    TIMER_A0->CCR[0] += 48000;  // add 0.75s delay
    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;    // enable CCR0 interrupt
    TIMER_A0->CTL |= TIMER_A_CTL_CLR;   // clear current count

    while(flag == 0);

    return curr_freq;
}

void TA0_0_IRQHandler(void) {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;  // clear interrupt flag
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;   // disable CCR0 interrupt
    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CM_1;   // disable capture if it isn't disabled

    // calculate frequency if AC
    if (count != 0) {
        curr_freq = (int) (64000/count);    // calculate freq
        if (count > 64000)      // edge case where freq. is 1
            curr_freq++;
    }

    flag = 1;
}

void TA0_N_IRQHandler(void) {
    uint32_t tmp_count;

    if (TIMER_A0->CCTL[1] & TIMER_A_CCTLN_CCIFG) {

        TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;   // temporarily disable CCR0 interrupts
        TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;  // clear interrupt flag
        tmp_count = TIMER_A0->CCR[1];   // capture count

        if (count > tmp_count)
            count = count + (65535 - tmp_count);
        else
            count = tmp_count - count;

        if (num_trigs == 2)
            TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CM_1;   // disable capture
        else {
            num_trigs++;
            TIMER_A0->CCR[0] += 48000;   // push back timer;
        }

        TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;   // re-enable CCR0 interrupts
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

    // CCR1 -> Capture Frequency
    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_CM_1 |   // capture on rising edge
                         TIMER_A_CCTLN_CAP |    // capture on CCIxA
                         TIMER_A_CCTLN_CCIE |   // use capture mode
                         TIMER_A_CCTLN_SCS;     // synchronous capture

    NVIC->ISER[0] = 1 << (TA0_0_IRQn & 31); // enable CCR0 ISR
    NVIC->ISER[0] = 1 << (TA0_N_IRQn & 31); // enable CCR1 ISR
}
