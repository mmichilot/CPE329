/**
 * main.c
 *    CPE 329-15
 *    Assignment 5: Interrupts and Timers
 *    Author: Celestine Co & Matthew Michilot
 */

#include <msp.h>
#include <stdio.h>

#include "set_dco.h"

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer

    set_DCO(FREQ_24_MHz);

    // setup P6.0 as output
    P6->SEL0 &= ~BIT0;
    P6->SEL1 &= ~BIT0;
    P6->DIR |= BIT0;
    P6->OUT |= BIT0;

    // setup Timer A0 to use SMCLK in continuous mode
    TIMER_A0->CTL |= TIMER_A_CTL_TASSEL_2|TIMER_A_CTL_MC_2;

    // divide SMCLK by 24 in Timer A0
    TIMER_A0->CTL |= TIMER_A_CTL_ID_3;
    TIMER_A0->EX0 |= TIMER_A_EX0_TAIDEX_2;

    TIMER_A0->CCR[0] = 10; // high time: 10us
    TIMER_A0->CCR[1] = 40; // low time: 30us

    // enable interrupts on Timer A0
    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;
    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_CCIE;

    // enable CCR0 ISR
    NVIC->ISER[0] = 1 << (TA0_0_IRQn & 31);
    // enable CCR1 ISR
    NVIC->ISER[0] = 1 << (TA0_N_IRQn & 31);

    __enable_irq(); // enable global interrupts

    while(1);
}

void TA0_0_IRQHandler(void) {
    TIMER_A0->CCR[0] &= ~TIMER_A_CCTLN_CCIFG; //clear interrupt flag
    P6->OUT ^= BIT0; // toggle pin
    TIMER_A0->CCR[0] += 10; // increment 10us
}

void TA0_N_IRQHandler(void) {
    if (TIMER_A0->CCTL[1] & TIMER_A_CCTLN_CCIFG) {
        TIMER_A0->CCR[1] &= ~TIMER_A_CCTLN_CCIFG; // clear interrupt flag
        P6->OUT ^= BIT0; // toggle pin
        TIMER_A0->CCR[1] += 40; // increment 40us
    }
}

