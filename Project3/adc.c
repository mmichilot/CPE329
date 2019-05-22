/*
 * ADC14.c
 *    CPE 329-15
 *    Project 3: Digital Multimeter
 *    Author: Celestine Co & Matthew Michilot
 */

#include "msp.h"
#include "adc.h"
#include "math.h"

#define PI 3.14

volatile static int flag = 0;

volatile int num_samples = 256;
volatile int curr_sample = 0;

volatile uint32_t offset = 0;

volatile int dc_mode = 1;
volatile int dc_voltage = 0;

static int sample_rate = 0;

volatile static int samples[256];

int average_samples(void);
int find_max(void);
float find_rms(void);

void init_ADC(void) {
    // initialize ADC
    ADC14->CTL0 &= ~ADC14_CTL0_ENC; // disable ADC for configuration

    ADC14->CTL0 = ADC14_CTL0_SHP  // use internal timer
                | ADC14_CTL0_SHT0_2 // 16 clocks per sample
                | ADC14_CTL0_SSEL_4 // select SMCLK
                | ADC14_CTL0_ON;    // turn on ADC14

    ADC14->CTL1 = (2 << ADC14_CTL1_CSTARTADD_OFS)  // start conversion using mem[2]
                | ADC14_CTL1_RES_3;                // use 14-bit conversion

    ADC14->MCTL[2] = ADC14_MCTLN_INCH_14;   // read from channel 14 (A14)

    // setup conversion interrupts
    ADC14->IER0 |= ADC14_IER0_IE2;          // enable interrupts on mem[2]
    NVIC->ISER[0] = 1 << (ADC14_IRQn & 31); // enable ADC ISR in NVIC

    ADC14->CTL0 |= ADC14_CTL0_ENC;

    // Configure Pin
    // P6.1 - A14
    P6->SEL0 |= BIT1;
    P6->SEL1 |= BIT1;
}

uint32_t get_flag_adc() {
    return flag;
}

uint32_t get_dc_voltage_adc(int frequency) {

    sample_rate = 0;
    dc_voltage = 0;
    offset = 0;
    flag = 0;

    if (frequency == 0) { // DC
        dc_mode = 1;
        ADC14->CTL0 |= ADC14_CTL0_SC;

    } else {   // AC
        dc_mode = 0;
        sample_rate = ((1/((float)frequency*256)) * 24000000) + 1;
        curr_sample = 0;
        TIMER_A1->CCR[0] = sample_rate;
        TIMER_A1->CCTL[0] |= TIMER_A_CCTLN_CCIE;   // enable CCR0 interrupt
    }

    while(flag == 0);   // wait for conversion to finish

    if (dc_mode)
        return dc_voltage;  // DC value
    else
        return (offset = average_samples()); // DC offset for AC voltage
}

uint32_t get_ac_pp_voltage_adc(void) {
    return (find_max() - offset) * 2;
}


void TA1_0_IRQHandler(void) {
    TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;  // clear interrupt flag

    // check if we have enough samples
    if (curr_sample == num_samples) {
        ADC14->CTL0 &= ~ADC14_CTL0_ON; // disable ADC
        TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;   // disable CCR1 interrupts
        flag = 1;
    }
    else
        ADC14->CTL0 |= ADC14_CTL0_SC;   // enable and start conversion

    TIMER_A1->CCR[0] += sample_rate;
}

void ADC14_IRQHandler(void) {
    if (dc_mode) {
        dc_voltage = ADC14->MEM[2];
        ADC14->CTL0 &= ~ADC14_CTL0_ON; // disable ADC
        flag = 1;
    }
    else {
        samples[curr_sample] = ADC14->MEM[2];
        curr_sample++;
    }
}

int average_samples(void) {
    int i;
    int total = 0;

    for (i = 0; i < 256; i++) {
        total += (samples[i] >> 8);
    }

    return total;
}

int find_max(void) {
    int i;
    int max = offset;

    for (i = 0; i < 256; i++)
        max = (samples[i] > max) ? samples[i] : max;

    return max;
}

float find_rms(void) {
    int i = 0;
    int num_vals = 0;
    uint32_t total = 0;
    uint32_t voltage = 0;

    // wait till we get the upper half of the waveform
    while (samples[i] < offset)
        i++;

    for (i = i; num_vals < 128; i++) {
        if (i > 255)
            i = 0;
        voltage = samples[i] - offset;
        total +=  (voltage * voltage) >> 7;
        num_vals++;
    }

    return 2*sqrt(total);
}
