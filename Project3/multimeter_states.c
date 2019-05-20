/*
 * multimeter_states.c
 *    CPE 329-15
 *    Project 3: Digital Multimeter
 *    Author: Celestine Co & Matthew Michilot
 */

#include "multimeter_states.h"
#include "dmm.h"
#include "delay.h"
#include "msp.h"
#include "uart.h"
#include "adc.h"

dmm_state_type current_dmm_state = GET_FREQ;
dmm_state_type next_dmm_state;

int frequency = 0;
float dc_volt = 0;

void dmm_fsm(void) {
    while(1) {
        update_fsm();
        current_dmm_state = next_dmm_state;
    }
}

void update_fsm(void) {

    switch(current_dmm_state) {
    case GET_FREQ:
        frequency = get_freq(); // Get frequency
        if (frequency == 0)
            next_dmm_state = GET_DC_VAL;
        else
            next_dmm_state = GET_AC_VALS;
        break;

    case GET_DC_VAL:
        // Get DC voltage
        ADC14->CTL0 |= ADC14_CTL0_SC;   // start ADC sampling
        dc_volt = (0.0002 * get_voltage_adc()) - 0.09;    // convert voltage to a readable value
        next_dmm_state = UPD_TERM;
        break;

    case UPD_TERM:
        str_voltage(dc_volt);    // print voltage
        str_freq(frequency);
        next_dmm_state = GET_FREQ;
        frequency = 0;
        dc_volt = 0;
        break;

    case GET_AC_VALS:
        // Get AC Vrms and Vpp
        next_dmm_state = UPD_TERM;
        break;

    default:
        current_dmm_state = GET_FREQ;
        break;
    }

}
