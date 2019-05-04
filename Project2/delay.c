/*
 * delay.c
 *
 *  Created on: Apr 5, 2019
 *  Author: Celestine Co & Matthew Michilot
 */

#include <msp432p401r.h>

void delay_us(int delay)
{
    int i;

    int slope = delay>>3;
    int adj_delay = slope*(1<<(CS->CTL0>>16)) - 4;

    for (i=65535;i>65535-adj_delay;i--);
}
