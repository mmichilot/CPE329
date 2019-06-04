/*
 * func_gen.c
 *    CPE 329-15
 *    Project 2: Function Generator
 *    Author: Celestine Co & Matthew Michilot
 */

#include <dac.h>
#include "msp.h"

void init_DAC(void) {

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

}

void set_voltage(int voltage) {
    int loByte, hiByte;

    loByte = voltage & LOWER_MASK;                      // mask lower 8 bits
    hiByte = (voltage >> DATA_OFS) & UPPER_MASK;               // mask upper 4 bits
    hiByte |= BIT4;                                     // set shutdown bit to 1
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));    // wait for TX buffer to empty

    // Beginning of transmission
    P1->OUT &= ~CHIP_SEL;                           // set CS low
    EUSCI_B0->TXBUF = hiByte;                       // send lower 8 bits
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));    // wait for TX buffer to empty
    EUSCI_B0->TXBUF = loByte;                       // send upper 8 bits;
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG));    // wait for transmit to finish
    P1->OUT |= CHIP_SEL;                            // set CS high
}
