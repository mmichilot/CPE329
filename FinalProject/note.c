/*
 * note.c
 *
 *  Created on: May 31, 2019
 *      Author: mmichilot
 */

#include "stdint.h"

#include "ir_sensor.h"
#include "lcd.h"
#include "note.h"

Note get_note(void);

void print_note(Note curr_note) {
    set_addr(0x06);
    write_char(0x41 + curr_note);
}

Note get_note(void) {
    uint32_t analog_val = 0;

    analog_val = get_analog_val();

    if (analog_val > 14000)
        return C;
    else if (analog_val > 10000)
        return D;
    else if (analog_val > 7000)
        return E;
    else if (analog_val > 5000)
        return F;
    else if (analog_val > 3500)
        return G;
    else if (analog_val > 2500)
        return A;
    else if (analog_val > 1500)
        return B;
    else
        return C;
}
