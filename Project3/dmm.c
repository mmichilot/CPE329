/*
 * dmm.c
 *    CPE 329-15
 *    Project 3: Digital Multimeter
 *    Author: Celestine Co & Matthew Michilot
 */

#include "dmm.h"
#include "msp.h"
#include "dmm.h"

volatile uint32_t count = 0;
volatile uint32_t num_trigs = 0;
volatile uint32_t flag = 0;

/* Capture the current frequency */
int get_freq(void) {
    int curr_freq = 0;

    flag = 0;
    count = 0;
    num_trigs = 0;

    TIMER_A0->CCR[0] = SEC_DELAY;       // add 1s delay
    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;    // enable CCR0 interrupt
    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_CM_1;    // enable capture

    while(flag == 0);   // wait for capture to happen

    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CM_1;   // disable capture

    // calculate frequency if AC
    if (num_trigs && (count != 1)) {        // on rare occasions count=1 so the frequency was 64000
        curr_freq = (int) (FREQ_DIV/count);    // calculate freq
         if (count > FREQ_DIV)                // edge case where freq. is 1
             curr_freq++;
    }

    return curr_freq;
}

/* This ISR handles a DC signal. If CCR0 times out after 1s
 * then set the frequency to 0;
 */
void TA0_0_IRQHandler(void) {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;  // clear interrupt flag
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;   // disable CCR0 interrupt
    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CM_1;   // disable capture

    flag = 1;
}

/* This ISR handles an AC signal. Using the capture mode on Timer A,
 * it counts the period of the signal.
 */
void TA0_N_IRQHandler(void) {
    uint32_t tmp_count;

    if (TIMER_A0->CCTL[1] & TIMER_A_CCTLN_CCIFG) {
        TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;  // clear CCR1 interrupt flag

        tmp_count = TIMER_A0->CCR[1];   // capture count

        // calculate the count, and handle any count that wraps around
        if (count < tmp_count)
            count = tmp_count - count;
        else
            count = count + (COUNT_UPP_BND - tmp_count);

        // determine if it's the first rising edge
        if (num_trigs)
            flag = 1;
        else
            num_trigs++;
    }
}

/* Initialize Timer A0 with CCR0 and CCR1
 * CCR0 - DC signal
 * CCR1 - AC signal
 */
void init_timer(void) {

    // Configure P2.4 as CCR1 capture input
    P2->SEL0 |= BIT4;
    P2->SEL1 &= ~BIT4;
    P2->DIR &= ~BIT4;
    P2->REN |= BIT4;
    P2->OUT &= ~BIT4;

    // Configure TimerA0
    TIMER_A0->CTL |= TIMER_A_CTL_TASSEL_1 |     // use ACLK
                     TIMER_A_CTL_ID_1 |         // divide ACLK by 2
                     TIMER_A_CTL_MC_2;          // continuous mode

    TIMER_A0->CCR[0] = SEC_DELAY;

    // CCR1 -> Capture Frequency
    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_CM_1 |   // enable capturing on rising edge
                         TIMER_A_CCTLN_CAP |    // capture on CCIxA
                         TIMER_A_CCTLN_CCIE |   // use capture mode
                         TIMER_A_CCTLN_SCS;     // synchronous capture

    NVIC->ISER[0] = 1 << (TA0_0_IRQn & 31); // enable CCR0 ISR
    NVIC->ISER[0] = 1 << (TA0_N_IRQn & 31); // enable CCR1 ISR

    // initialize TimerA1
    TIMER_A1->CTL |= TIMER_A_CTL_TASSEL_2   // use SMCLK
                   | TIMER_A_CTL_ID_1       // divide SMCLK by 2
                   | TIMER_A_CTL_MC_2;      // continuous mode

    NVIC->ISER[0] = 1 << (TA1_0_IRQn & 31); // enable CCR0 ISR

}
