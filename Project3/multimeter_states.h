/*
 * multimeter_states.h
 *    CPE 329-15
 *    Project 3: Digital Multimeter
 *    Author: Celestine Co & Matthew Michilot
 */

#ifndef MULTIMETER_STATES_H_
#define MULTIMETER_STATES_H_

typedef enum {
        GET_FREQ,
        GET_DC_VAL,
        UPD_TERM,
        GET_AC_VALS
} dmm_state_type;

void dmm_fsm(void);
void update_fsm(void);

#endif /* MULTIMETER_STATES_H_ */
