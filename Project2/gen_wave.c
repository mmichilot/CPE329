/*
 * gen_wave.c
 *
 *  Created on: May 5, 2019
 *      Author: mmichilot
 */

#include <msp.h>
#include <math.h>
#include "gen_wave.h"
#include "func_gen.h"
#include "func_gen_states.h"

#define BASE_FREQ 100
#define VAL 0.0174

volatile int voltage = 0;
volatile int ccr0_rate = 0;
volatile int ccr1_rate = 1;
volatile int index = 0;
volatile int volt[682];
volatile int factor = 0;

void init_sine() {
    int x;

    for (x = 0; x < 341; x += 1) {
        volt[x] = 2047 * sin(x*VAL*0.528) + 2047;
    }

    for (x = 341; x < 682; x += 1) {
        volt[x] = 2047 * sin(x*VAL*0.528) - 2047;
    }
}

void square_wave(int freq, int duty) {
    int period;
    float hi_time;

    index = 0;
    period = 30000 / (freq/BASE_FREQ);
    ccr0_rate = period;
    volt[0] = 4095;

    hi_time = period * duty/100;

    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;    // enable CCR0 interrupt
    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_CCIE;   // enable CCR1 interrupt

    TIMER_A0->CCR[0] = period/2;    // low time
    TIMER_A0->CCR[1] = hi_time + (period/2);    // high time
}

void saw_wave(int freq) {
    index = 0;
    ccr0_rate = 44;
    volt[0] = 6 * (int) (freq/BASE_FREQ);

    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;    // enable CCR0 interrupt
    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIE;   // disable CCR1 interrupt

    TIMER_A0->CCR[0] = ccr0_rate;
}

void sine_wave(int freq) {
    index = 1;
    ccr0_rate = 44;
    ccr1_rate = 30000 / (freq/BASE_FREQ);
    factor = freq/BASE_FREQ;

    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;    // enable CCR0 interrupt
    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_CCIE;   // disable CCR1 interrupt

    TIMER_A0->CCR[0] = ccr0_rate;
    TIMER_A0->CCR[1] = 30000 / (freq/BASE_FREQ);

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
    index += factor;
    voltage = volt[index];
    set_voltage(voltage);
}

void TA0_N_IRQHandler(void) {
    if (TIMER_A0->CCTL[1] & TIMER_A_CCTLN_CCIFG) {
        TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG; // clear interrupt flag

        TIMER_A0->CCR[1] += ccr1_rate;
        index = 0;
    }
}

//void TA0_0_IRQHandler(void) {
//    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;      // clear interrupt flag
//    TIMER_A0->CCR[0] += ccr0_rate;
//    index += factor;
//    voltage += volt[0];
//    set_voltage(voltage);
//}

//void TA0_N_IRQHandler(void) {
//    if (TIMER_A0->CCTL[1] & TIMER_A_CCTLN_CCIFG) {
//        TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG; // clear interrupt flag
//        TIMER_A0->CCR[1] += ccr0_rate;
//        voltage = factor*voltage;
//        set_voltage(voltage);
//    }
//}
