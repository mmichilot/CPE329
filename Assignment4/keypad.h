/*
 * keypad.h
 *
 *    CPE 329-15
 *    Assignment 4: Keypad
 *    Author: Celestine Co & Matthew Michilot
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <msp.h>

// Row Signals
#define ROW1 BIT4
#define ROW2 BIT5
#define ROW3 BIT6
#define ROW4 BIT7

// Column Signals
#define COL1 BIT5
#define COL2 BIT6
#define COL3 BIT7

// Keypad Functions
void keypad_init();
uint8_t get_key();

#endif /* KEYPAD_H_ */
