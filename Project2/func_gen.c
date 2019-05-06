/*
 * func_gen.c
 *    CPE 329-15
 *    Project 2: Function Generator
 *    Author: Celestine Co & Matthew Michilot
 */

#include "msp.h"
#include "func_gen.h"

void init_SPI(void) {

    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;     // put serial per. into reset state

    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_CKPL        // set CLK inactive high
                    | EUSCI_B_CTLW0_MSB         // MSB first
                    | EUSCI_B_CTLW0_MST         // master mode
                    | EUSCI_B_CTLW0_SYNC        // SPI is synchronous
                    | EUSCI_B_CTLW0_UCSSEL_2    // use SMCLK
                    | EUSCI_B_CTLW0_SWRST;      // keep SPI in reset

    EUSCI_B0->BRW = DIV_1;                       // run at full SMCLK speed

    P1->SEL0 |= (BIT5|BIT6);                    // configure for SMLCLK and SIMO
    P1->SEL1 &= ~(BIT5|BIT6);                   // for EUSCI_B0
    P1->DIR |= BIT0;                            // set P1.0 for CS
    P1->OUT |= CHIP_SEL;                            // initialize CS high

    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;    // activate SPI

    set_voltage();

}

void init_timer(void) {

    //setup Timer A0 to use SMCLK, continuous mode, and divide by 2
    TIMER_A0->CTL |= TIMER_A_CTL_TASSEL_2 |
                     TIMER_A_CTL_MC_2 |
                     TIMER_A_CTL_ID_1;

    // enable interrupts on Timer A0
    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;
    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_CCIE;

    NVIC->ISER[0] = 1 << (TA0_0_IRQn & 31);     //enable CCR0 ISR
    NVIC->ISER[0] = 1 << (TA0_N_IRQn & 31);     // enable CCR1 ISR
}


void incr_duty_cycle(void) {
    TIMER_A0->CCR[0] += DC_100HZ; // high time increased by 10%
    TIMER_A0->CCR[1] -= DC_100HZ; // low time decreased by 10%
}

void decr_duty_cycle(void) {
    TIMER_A0->CCR[0] -= DC_100HZ; // high time decreased by 10%
    TIMER_A0->CCR[1] += DC_100HZ; // low time increased by 10%
}

