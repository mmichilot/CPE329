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

dmm_state_type current_dmm_state = GET_FREQ;
dmm_state_type next_dmm_state;

void dmm_fsm(void) {
    while(1) {
        update_fsm();
        current_dmm_state = next_dmm_state;
    }
}

void update_fsm(void) {
    int frequency = 0;

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
        next_dmm_state = UPD_TERM;
        break;

    case UPD_TERM:
        // Update Terminal
        next_dmm_state = WAIT;
        break;

    case GET_AC_VALS:
        // Get AC Vrms and Vpp
        next_dmm_state = UPD_TERM;
        break;

    case WAIT:
        delay_us(40000);    // delay
        next_dmm_state = GET_FREQ;
        break;

    default:
        current_dmm_state = WAIT;
        break;
    }

}
