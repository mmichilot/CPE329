/*
 * ir_sensor.c
 *
 *  Created on: May 30, 2019
 *      Author: mmichilot
 */

#include "msp.h"
#include "ir_sensor.h"

volatile uint32_t analog_val = 0;

void init_IR(void) {
    // Configure P6.1 (A14)
    P6->SEL0 |= BIT1;
    P6->SEL1 |= BIT1;

    // Initialize Timer A0 CCR1 as PWM

    TIMER_A0->CCR[0] = PERIOD_50MS;
    TIMER_A0->CCR[1] = PULSE_2MS;

    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7; // output mode: reset/set

    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2    // select SMCLK
                  | TIMER_A_CTL_ID_2        // CLK/4
                  | TIMER_A_CTL_MC_1;       // up mode


    // Initialize ADC
    ADC14->CTL0 &= ~ADC14_CTL0_ENC; // disable conversions

    ADC14->CTL0 = ADC14_CTL0_SHS_1      // use Timer A0 CCR1
                | ADC14_CTL0_SHP        // use sampling timer
                | ADC14_CTL0_CONSEQ_2   // repeat-single-channel
                | ADC14_CTL0_SSEL_4     // use SMCLK
                | ADC14_CTL0_SHT0_2     // 16 clock cycles
                | ADC14_CTL0_ON;        // turn on ADC

    ADC14->CTL1 = (1 << ADC14_CTL1_CSTARTADD_OFS)  // start conversion using mem[1]
                | ADC14_CTL1_RES_3;                // use 14-bit conversion

    ADC14->MCTL[1] = ADC14_MCTLN_WINC |     // enable window comparator
                    ADC14_MCTLN_INCH_14;   // read from channel 14 (A14)

    ADC14->HI0 = HI_THRESHOLD;  // set threshold to ~200 mV

    ADC14->CTL0 |= ADC14_CTL0_ENC;  // enable conversions

    // setup conversion interrupts
    ADC14->IER1 |= ADC14_IER1_HIIE;          // enable upper window interrupts
    NVIC->ISER[0] = 1 << (ADC14_IRQn & 31); // enable ADC ISR in NVIC
}



void ADC14_IRQHandler(void) {
    if (ADC14->IFGR1 & ADC14_IFGR1_HIIFG) {
        ADC14->CLRIFGR1 |= ADC14_CLRIFGR1_CLRHIIFG; // clear interrupt flag
        analog_val = ADC14->MEM[1];  // grab value
        P3->OUT ^= BIT0;
    }
}
