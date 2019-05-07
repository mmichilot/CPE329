/*
 * gen_wave.c
 *
 *  Created on: May 5, 2019
 *      Author: mmichilot
 */

#include <dac.h>
#include <msp.h>
#include <math.h>
#include "gen_wave.h"
#include "func_gen_states.h"

#define BASE_FREQ 100
#define VAL 0.0174
#define DAC_UPDATE_RATE 60
#define MAX_DAC_VOLTAGE 4095
#define MAX_COUNT 30000

volatile int dac_val = 0;
volatile int voltage = 0;
volatile int update_rate = 0;
volatile int period = 0;
volatile int sine_enable = 0;
volatile int index = 0;
volatile int index_factor = 0;

int sine_LUT[500];

void init_sine() {
    int i;

    for (i = 0; i < 500; i++)
        sine_LUT[i] = (2047*sin(i*0.0126) + 2047);

}

void square_wave(int freq, int duty) {
    float hi_time;

    voltage = 0;
    sine_enable = 0;

    period = MAX_COUNT / (freq/BASE_FREQ);
    update_rate = period;
    dac_val = MAX_DAC_VOLTAGE;

    hi_time = period * duty/100;

    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;    // enable CCR0 interrupt
    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_CCIE;    // enable CCR1 interrupt
    TIMER_A0->CCTL[2] &= ~TIMER_A_CCTLN_CCIE;   // disable CCR2 interrupt

    TIMER_A0->CCR[0] = period/2;                // low time
    TIMER_A0->CCR[1] = hi_time + (period/2);    // high time
}

void saw_wave(int freq) {
    sine_enable = 0;

    update_rate = DAC_UPDATE_RATE;
    dac_val = 8 * (int) (freq/BASE_FREQ);

    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;    // enable CCR0 interrupt
    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIE;   // disable CCR1 interrupt
    TIMER_A0->CCTL[2] &= ~TIMER_A_CCTLN_CCIE;   // disable CCR2 interrupt

    TIMER_A0->CCR[0] = DAC_UPDATE_RATE;
}

void sine_wave(int freq) {
    sine_enable = 1;

    period = MAX_COUNT / (freq/BASE_FREQ);
    update_rate = DAC_UPDATE_RATE;
    index_factor = freq/BASE_FREQ;

    voltage = 2047;
    set_voltage(voltage);

    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;    // enable CCR0 interrupt
    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIE;   // disable CCR1 interrupt
    TIMER_A0->CCTL[2] |= TIMER_A_CCTLN_CCIE;    // enable CCR2 interrupt

    TIMER_A0->CCR[0] = update_rate;
    TIMER_A0->CCR[2] = period;

    dac_val = 0;
}

void TA0_0_IRQHandler(void) {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;      // clear interrupt flag
    TIMER_A0->CCR[0] += update_rate;
    voltage = (!sine_enable) * voltage;
    index += sine_enable * index_factor;
    voltage += ((sine_enable) * sine_LUT[index]) + dac_val;
    set_voltage(voltage);
}

void TA0_N_IRQHandler(void) {
    if (TIMER_A0->CCTL[2] & TIMER_A_CCTLN_CCIFG) {
        TIMER_A0->CCTL[2] &= ~TIMER_A_CCTLN_CCIFG; // clear interrupt flag
        TIMER_A0->CCR[2] += period;
        index = 0;
    }
    else if (TIMER_A0->CCTL[1] & TIMER_A_CCTLN_CCIFG) {
        TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG; // clear interrupt flag
        TIMER_A0->CCR[1] += period;
        voltage = 0;
        set_voltage(voltage);
    }
}
