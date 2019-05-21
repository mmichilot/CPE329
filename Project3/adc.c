/*
 * ADC14.c
 *    CPE 329-15
 *    Project 3: Digital Multimeter
 *    Author: Celestine Co & Matthew Michilot
 */

#include "msp.h"
#include "uart.h"
#include "adc.h"

static int flag = 0;
static int conv_flag = 0;
static int analogValue = 0;

void init_ADC(void) {

    // initialize TA1
    TIMER_A1->CTL |= TIMER_A_CTL_TASSEL_2   // use SMCLK
                      | TIMER_A_CTL_MC_2;   // continuous mode

    TIMER_A1->CCR[1] = DC_SAMPL_RATE;
    TIMER_A1->CCTL[1] |= TIMER_A_CCTLN_CCIE;    // enable CCR1 interrupt

    NVIC->ISER[0] = 1 << (TA1_N_IRQn & 31); // enable CCR1 TIMER A1 ISR

    // initialize ADC
    ADC14->CTL0 &= ~ADC14_CTL0_ENC; // disable ADC for configuration

    ADC14->CTL0 = ADC14_CTL0_SHP  // use internal timer
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

    // setup conversion interrupts
    ADC14->IER0 |= ADC14_IER0_IE2;          // enable interrupts on mem[2]
    NVIC->ISER[0] = 1 << (ADC14_IRQn & 31); // enable ADC ISR in NVIC
}

void str_voltage(float voltage) {
    float ones, tenth, hundredth = 0;
    char volt_str[7] = "0.00 V";

    ones = (int)(voltage);
    tenth = (int)(10 * (voltage - ones));
    hundredth = (int)(10 * ((voltage - ones) * 10 - tenth));

    volt_str[0] += ones;  // ones digit
    volt_str[2] += tenth; // tenth digit
    volt_str[3] += hundredth; // hundredth digit

    print_string(volt_str);
    print_char('\n');
    print_char('\r');
}

uint32_t get_flag_adc() {
    return flag;
}

uint32_t get_voltage_adc() {
    int voltageVal;

    TIMER_A1->CCTL[1] |= TIMER_A_CCTLN_CCIE;   // enable CCR0 interrupt
    conv_flag = 1;

    while(flag == 0);   // wait for conversion to finish

    voltageVal = analogValue;

    //reset flag and voltage
    flag = 0;
    analogValue = 0;

    return voltageVal;
}

void TA1_N_IRQHandler(void) {
    if (TIMER_A1->CCTL[1] & TIMER_A_CCTLN_CCIFG) {
        TIMER_A1->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;  // clear interrupt flag

        TIMER_A1->CCR[1] += DC_SAMPL_RATE;

        if (conv_flag == 1) {
            ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;   // enable and start conversion
            conv_flag = 0;
            TIMER_A1->CCTL[1] &= ~TIMER_A_CCTLN_CCIE;   // disable CCR0 interrupt
        }
    }
}

void ADC14_IRQHandler(void) {
    analogValue = ADC14->MEM[2];
    ADC14->CTL0 &= ~ADC14_CTL0_ENC; // disable ADC
    flag = 1;
}
