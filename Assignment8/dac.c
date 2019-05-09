/*
 * func_gen.c
 *    CPE 329-15
 *    Project 2: Function Generator
 *    Author: Celestine Co & Matthew Michilot
 */

#include "msp.h"
#include "dac.h"

void init_SPI(void) {

    // initialize serial device as SPI
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;     // put serial per. into reset state

    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_CKPL        // set CLK inactive high
                    | EUSCI_B_CTLW0_MSB         // MSB first
                    | EUSCI_B_CTLW0_MST         // master mode
                    | EUSCI_B_CTLW0_SYNC        // SPI is synchronous
                    | EUSCI_B_CTLW0_UCSSEL_2    // use SMCLK
                    | EUSCI_B_CTLW0_SWRST;      // keep SPI in reset

    EUSCI_B0->BRW = DIV_1;                       // run at full SMCLK speed

    // Configure Pins
    // P1.0 - CS | P1.5 - SCLK | P1.6 - SIMO
    P1->SEL0 |= (BIT5|BIT6);
    P1->SEL1 &= ~(BIT5|BIT6);
    P1->DIR |= BIT0;
    P1->OUT |= CHIP_SEL;

    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;    // activate SPI

}

void set_voltage(int voltage) {
    int loByte, hiByte;

    loByte = voltage & LOWER_MASK;                  // mask lower 8 bits
    hiByte = (voltage >> 8) & UPPER_MASK;           // mask upper 4 bits
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
