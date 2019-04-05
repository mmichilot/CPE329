/*
 * dco_set.c
 *
 *  Created on: Apr 5, 2019
 *  Author: Celestine Co & Matthew Michilot
 */

#include <msp432p401r.h>
void set_DCO(uint32_t freq)
{
    CS->KEY = CS_KEY_VAL; //unlock CS
    CS->CTL0 = freq;
    CS->KEY = 0; // lock CS
}
