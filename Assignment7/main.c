/**
 * main.c
 *    CPE 329-15
 *    Assignment 7: Execution Timing
 *    Author: Celestine Co & Matthew Michilot
 */

#include "msp.h"
#include "set_dco.h"

void main(void) {
    int data = 2048;
    int loByte = 0;
    int hiByte = 0;

    set_DCO(FREQ_12_MHz); // set SMCLK to 12 Mhz

    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;    // put serial per. into reset state

    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_CKPL   // set CLK inactive high
                 | EUSCI_B_CTLW0_MSB    // MSB first
                 | EUSCI_B_CTLW0_MST    // master mode
                 | EUSCI_B_CTLW0_SYNC   // SPI is synchronous
                 | EUSCI_B_CTLW0_UCSSEL_2   // use SMCLK
                 | EUSCI_B_CTLW0_SWRST; // keep SPI in reset

    EUSCI_B0->BRW = 0x01;  // run at full SMCLK speed
    P1->SEL0 |= (BIT5|BIT6);   // configure for SMLCLK and SIMO
    P1->SEL1 &= ~(BIT5|BIT6);  // for EUSCI_B0
    P1->DIR |= BIT0;   // set P1.0 for CS
    P1->OUT |= BIT0;   // initialize CS high

    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;   // activate SPI

    loByte = data & 0xFF;   // mask lower 8 bits
    hiByte = (data >> 8) & 0x0F;    // mask upper 4 bits
    hiByte |= (BIT4|BIT5);  // set gain and shutdown bits to 1
    P1->OUT &= ~BIT0;   // set CS low

    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));    // wait for TX buffer to empty
    EUSCI_B0->TXBUF = loByte;   // send lower 8 bits
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));    // wait for TX buffer to empty
    EUSCI_B0->TXBUF = hiByte;   // send upper 8 bits;

    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG));    // wait for transmit to finish
    P1->OUT |= BIT0;    // set CS high

    while(1);

}
