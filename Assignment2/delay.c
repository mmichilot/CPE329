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

    int adj_delay = ((int)(delay>>3) - 2); // get # of loops

    for (i=65535;i>65535-(adj_delay);i--);
}


