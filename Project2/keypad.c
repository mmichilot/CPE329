/*
 * keypad.c
 *    CPE 329-15
 *    Project 2: Function Generator
 *    Author: Celestine Co & Matthew Michilot
 */

#include <msp.h>
#include <stdint.h>

#include "keypad.h"

void init_keypad() {

    P5->DIR |= (COL1|COL2|COL3);             // set col to outputs 5.5, 5.6, 5.7
    P6->DIR &= ~(ROW1|ROW2|ROW3|ROW4);       // set rows to inputs   6.4, 6.5, 6.6, 6.7

    P6->REN |= (ROW1|ROW2|ROW3|ROW4);        // enable internal resistors

    P6->OUT &= ~(ROW1|ROW2|ROW3|ROW4);       // set resistors to pull down
    P5->OUT &= ~(COL1|COL2|COL3);            // set columns low
}

uint8_t get_key()
{
    /* change all port 3 assignments to different port!!!! */

    uint8_t col;
    uint8_t key, rows;

    P5->OUT |= (COL1|COL2|COL3);        // check for button press
    _delay_cycles(50);
    rows = P6->IN & (ROW1|ROW2|ROW3|ROW4);
    if (rows == 0) {
        return 0xFF;                    // no button pressed
    }

    for (col = 0; col < 3; col++) {
        P5->OUT &= ~(COL1|COL2|COL3);   // clear columns
        P5->OUT |= (COL1 << col);       // set column high shifting each cycle
        _delay_cycles(25);
        rows = P6->IN & (ROW1|ROW2|ROW3|ROW4);
        if (rows != 0) {
            break;                      // check if button press
        }
    }

    if (col == 3) {
        return 0xFF;                    // no button press
    }

    rows = rows >> 4;

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
