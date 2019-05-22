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
#include "terminal.h"

dmm_state_type current_dmm_state = GET_FREQ;
dmm_state_type next_dmm_state;

int frequency = 0;
float vdc = 0;
float vpp = 0;
float vrms = 0;

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
        vdc = (0.0002 * get_dc_voltage_adc(frequency)) + 0.01;    // convert voltage to a readable value
        next_dmm_state = UPD_TERM;
        break;

    case UPD_TERM:
        update_terminal(frequency, vdc, vpp, vrms);
        next_dmm_state = GET_FREQ;
        frequency = 0;
        vdc = 0;
        vpp = 0;
        vrms = 0;
        break;

    case GET_AC_VALS:
        // Get AC Vrms and Vpp
        vdc = (0.0002 * get_dc_voltage_adc(frequency)) + 0.03;    // convert voltage to a readable value
        vpp = (0.0002 * get_ac_pp_voltage_adc()) - 0.04;
        vrms = (find_rms() * 0.0002) - 0.02;
        next_dmm_state = UPD_TERM;
        break;

    default:
        current_dmm_state = GET_FREQ;
        break;
    }

}
