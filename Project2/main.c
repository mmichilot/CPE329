/**
 * main.c
 *    CPE 329-15
 *    Project 2: Function Generator
 *    Author: Celestine Co & Matthew Michilot
 */

#include "msp.h"
#include "set_dco.h"
#include "keypad.h"
#include "func_gen.h"

#define SHDN BIT4
#define GAIN BIT5
#define CHIP_SEL BIT0
#define DIV_1 0x01
#define LOWER_MASK 0xFF
#define UPPER_MASK 0x0F
#define DATA_OFS 8
#define HALF_PERIOD 60000
#define VPP_2 2482

volatile uint16_t voltage = VPP_2;

void init_keypad();
void init_SPI(void);
void set_voltage(uint16_t val);

void main(void) {

    set_DCO(FREQ_12_MHz); // set SMCLK to 12 Mhz

    // Keypad Setup
    init_keypad();

    //setup Timer A0 to use SMCLK, continuous mode, and divide by 2
    TIMER_A0->CTL |= TIMER_A_CTL_TASSEL_2 |
                     TIMER_A_CTL_MC_2 |
                     TIMER_A_CTL_ID_1;

    TIMER_A0->CCR[0] = 60000; // 10 ms
    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE; // enable interrupts for CCR0
    NVIC->ISER[0] = 1 << (TA0_0_IRQn & 31); //enable CCR0 ISR

    init_SPI(); // initialize the SPI bus

    set_voltage(voltage);

    __enable_irq(); // enable global interrupts

    while(1);
}

void init_keypad() {

    P5->DIR |= (COL1|COL2|COL3);             // set col to outputs 5.5, 5.6, 5.7
    P6->DIR &= ~(ROW1|ROW2|ROW3|ROW4);       // set rows to inputs   6.4, 6.5, 6.6, 6.7

    P6->REN |= (ROW1|ROW2|ROW3|ROW4);        // enable internal resistors

    P6->OUT &= ~(ROW1|ROW2|ROW3|ROW4);       // set resistors to pull down
    P5->OUT &= ~(COL1|COL2|COL3);            // set columns low
}

void init_SPI(void) {

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
}

void set_voltage(uint16_t val) {
    int loByte, hiByte;

    loByte = val & 0xFF;   // mask lower 8 bits
    hiByte = (val >> 8) & 0x0F;    // mask upper 4 bits
    hiByte |= (BIT4|BIT5);  // set gain and shutdown bits to 1
    P1->OUT &= ~BIT0;   // set CS low

    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));    // wait for TX buffer to empty
    EUSCI_B0->TXBUF = hiByte;   // send lower 8 bits
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));    // wait for TX buffer to empty
    EUSCI_B0->TXBUF = loByte;   // send upper 8 bits;
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG));    // wait for transmit to finish
    P1->OUT |= BIT0;    // set CS high
}

void TA0_0_IRQHandler(void) {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;  // clear interrupt flag
    TIMER_A0->CCR[0] += HALF_PERIOD; // 10 ms
    voltage ^= VPP_2; // toggle voltage value
    set_voltage(voltage); // set voltage
}
