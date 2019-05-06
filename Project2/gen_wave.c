/*
 * gen_wave.c
 *
 *  Created on: May 5, 2019
 *      Author: mmichilot
 */

#include <msp.h>
#include "gen_wave.h"
#include "func_gen.h"
#include "func_gen_states.h"

volatile int volt_rate = 0;
volatile int voltage = 0;
volatile int ccr0_rate = 0;

void square_wave(int freq, int duty) {
    int period;
    float hi_time;

    period = 30000 / (freq/100);
    ccr0_rate = period;
    volt_rate = 4095;

    hi_time = period * duty/100;

    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;    // enable CCR0 interrupt
    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_CCIE;   // enable CCR1 interrupt

    TIMER_A0->CCR[0] = period/2;    // low time
    TIMER_A0->CCR[1] = hi_time + (period/2);    // high time
}

void saw_wave(int freq) {
    int base_freq = 100;

    ccr0_rate = 44;
    volt_rate = 6 * (int) (freq/base_freq);

    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;    // enable CCR0 interrupt
    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIE;   // disable CCR1 interrupt

    TIMER_A0->CCR[0] = 44;
}

void set_voltage(int voltage) {
    int loByte, hiByte;

    loByte = voltage & LOWER_MASK;                      // mask lower 8 bits
    hiByte = (voltage >> 8) & UPPER_MASK;               // mask upper 4 bits
    hiByte |= (BIT4|BIT5);                          // set gain and shutdown bits to 1
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));    // wait for TX buffer to empty

    // Beginning of transmission
    P1->OUT &= ~CHIP_SEL;                           // set CS low
    EUSCI_B0->TXBUF = hiByte;                       // send lower 8 bits
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));    // wait for TX buffer to empty
    EUSCI_B0->TXBUF = loByte;                       // send upper 8 bits;
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG));    // wait for transmit to finish
    P1->OUT |= CHIP_SEL;                            // set CS high
}

void TA0_0_IRQHandler(void) {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;      // clear interrupt flag
    TIMER_A0->CCR[0] += ccr0_rate;
    voltage += volt_rate;
    set_voltage(voltage);
}

void TA0_N_IRQHandler(void) {
    if (TIMER_A0->CCTL[1] & TIMER_A_CCTLN_CCIFG) {
        TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG; // clear interrupt flag
        TIMER_A0->CCR[1] += ccr0_rate;
        voltage = 0;
        set_voltage(voltage);
    }
}
