/*
 * keypad.c
 *
 *    CPE 329-15
 *    Assignment 4: Keypad
 *    Author: Celestine Co & Matthew Michilot
 */

#include <msp.h>
#include <stdint.h>

#include "keypad.h"

uint8_t get_key()
{
    /* change all port 3 assignments to different port!!!! */

    int col;
    uint8_t key, rows;

    P3->OUT |= (COL1|COL2|COL3);        // check for button press
    _delay_cycles(25);
    rows = P6->IN && (ROW1|ROW2|ROW3|ROW4);
    if (rows == 0) {
        return 0xFF;                    // no button pressed
    }

    for (col = 0; col < 3; col++) {
        P3->OUT &= ~(COL1|COL2|COL3);   // clear columns
        P3->OUT |= (COL1 << col);       // set column high shifting each cycle
        _delay_cycles(25);
        rows = P6->IN && (ROW1|ROW2|ROW3|ROW4);
        if (rows != 0) {
            break;                      // check if button press
        }
    }

    if (col == 3) {
        return 0xFF;                    // no button press
    }

    if (rows == 4) {
        rows = 3;
    }

    if (rows == 8) {
        rows = 4;
    }

    key = rows * 3 - 2 + col;
    if (key == 11) {
        key = 0;                        // fix zero case
    }

    return key;

}
