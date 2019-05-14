/*
 * ADC14.c
 *      CPE 329-15
 *      Assignment 9: Analog to Digital Converter
 *      Author: Celestine Co & Matthew Michilot
 */

#include "msp.h"

void init_ADC(void) {
    // initialize ADC
    ADC14->CTL0 &= ~ADC14_CTL0_ENC; // disable ADC for configuration

    ADC14->CTL0 = ADC14_CTL0_SHP    // sample pulse, use internal timer
                | ADC14_CTL0_SHT0_2 // 16 clocks per sample
                | ADC14_CTL0_SSEL_3 // select MCLK
                | ADC14_CTL0_ON;    // turn on ADC14

    ADC14->CTL1 = (2 << ADC14_CTL1_CSTARTADD_OFS)  // start conversion using mem[2]
                | ADC14_CTL1_RES_3;                // use 14-bit conversion

    ADC14->MCTL[2] = ADC14_MCTLN_INCH_14;   // read from channel 14 (A14)

    // Configure Pin
    // P6.1 - A14
    P6->SEL0 |= BIT1;
    P6->SEL1 |= BIT1;

    ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;  // enable ADC and sampling

    // setup conversion interrupts
    ADC14->IER0 |= ADC14_IER0_IE2;          // enable interrupts on mem[2]
    NVIC->ISER[0] = 1 << (ADC14_IRQn & 31); // enable ADC ISR in NVIC
}
