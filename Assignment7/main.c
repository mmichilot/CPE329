/**
 * main.c
 *    CPE 329-15
 *    Assignment 6: SPI DAC & Waveform Generation
 *    Author: Celestine Co & Matthew Michilot
 */

#include "msp.h"
#include "set_dco.h"
#include "delay.h"

#define SHDN BIT4
#define GAIN BIT5
#define CHIP_SEL BIT0
#define DIV_1 0x01
#define LOWER_MASK 0xFF
#define UPPER_MASK 0x0F
#define DATA_OFS 8
#define HALF_PERIOD 60000

volatile uint16_t voltage = 0;
volatile int8_t factor = 1;

void init_SPI(void);
void set_voltage(uint16_t val);

void main(void) {

    set_DCO(FREQ_12_MHz); // set SMCLK to 12 Mhz

    //setup Timer A0 to use SMCLK, continuous mode, and divide by 2
    TIMER_A0->CTL |= TIMER_A_CTL_TASSEL_2 |
                     TIMER_A_CTL_MC_2 |
                     TIMER_A_CTL_ID_1;

    TIMER_A0->CCR[0] = HALF_PERIOD; // 10 ms
    TIMER_A0->CCR[1] = 25; // 4 us
    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE; // enable interrupts for CCR0
    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_CCIE; // enable interrupts for CCR1

    NVIC->ISER[0] = 1 << (TA0_0_IRQn & 31); // enable CCR0 ISR
    NVIC->ISER[0] = 1 << (TA0_N_IRQn & 31); // enable CCR1 ISR

    init_SPI(); // initialize the SPI bus

    set_voltage(0); // set voltage out to 0 V

    __enable_irq(); // enable global interrupts

    while(1) {
        set_voltage(voltage);
    }
}

void init_SPI(void) {

    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;    // put serial per. into reset state

    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_CKPL   // set CLK inactive high
                    | EUSCI_B_CTLW0_MSB    // MSB first
                    | EUSCI_B_CTLW0_MST    // master mode
                    | EUSCI_B_CTLW0_SYNC   // SPI is synchronous
                    | EUSCI_B_CTLW0_UCSSEL_2   // use SMCLK
                    | EUSCI_B_CTLW0_SWRST; // keep SPI in reset

    EUSCI_B0->BRW = DIV_1;  // run at full SMCLK speed
    P1->SEL0 |= (BIT5|BIT6);   // configure for SMLCLK and SIMO
    P1->SEL1 &= ~(BIT5|BIT6);  // for EUSCI_B0
    P1->DIR |= BIT0;   // set P1.0 for CS
    P1->OUT |= CHIP_SEL;   // initialize CS high

    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;   // activate SPI
}

void set_voltage(uint16_t val) {
    int loByte;
    int hiByte;

    loByte = val & LOWER_MASK;   // mask lower 8 bits
    hiByte = (val >> DATA_OFS) & UPPER_MASK;    // mask upper 4 bits
    hiByte |= (SHDN|GAIN);  // set gain and shutdown bits to 1
    P1->OUT &= ~CHIP_SEL;   // set CS low

    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));    // wait for TX buffer to empty
    EUSCI_B0->TXBUF = hiByte;   // send lower 8 bits
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));    // wait for TX buffer to empty
    EUSCI_B0->TXBUF = loByte;   // send upper 8 bits;
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG));    // wait for transmit to finish
    P1->OUT |= CHIP_SEL;    // set CS high
}

void TA0_0_IRQHandler(void) {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;  // clear interrupt flag
    TIMER_A0->CCR[0] += HALF_PERIOD; // 10 ms (20ms/2)
    factor *= -1; // toggle b/w incrementing and decrementing voltage
}

void TA0_N_IRQHandler(void) {
    if (TIMER_A0->CCTL[1] & TIMER_A_CCTLN_CCIFG) {
        TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG; // clear interrupt flag
        TIMER_A0->CCR[1] += 50; // update voltage every 8 us
        voltage += factor*2; // calculate new voltage
    }
}
