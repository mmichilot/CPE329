/*
 * keypad.h
 *    CPE 329-15
 *    Project 2: Function Generator
 *    Author: Celestine Co & Matthew Michilot
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <msp.h>

// Row Signals     6.4, 6.5, 6.6, 6.7
#define ROW1 BIT4
#define ROW2 BIT5
#define ROW3 BIT6
#define ROW4 BIT7

// Column Signals  5.5, 5.6, 5.7
#define COL1 BIT5
#define COL2 BIT6
#define COL3 BIT7

#define NULLCHAR 0xFF
#define EOL1 0x10
#define EOL2 0x50

// Keypad Functions
uint8_t get_key();

#endif /* KEYPAD_H_ */
