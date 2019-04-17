/*
 * digi_lock.h
 *
 *    CPE 329-15
 *    Project 1: Digital Lock
 *    Author: Celestine Co & Matthew Michilot
 */

#ifndef DIGI_LOCK_H_
#define DIGI_LOCK_H_

// Addresses
#define PIN1 0x4A
#define PIN2 0x4B
#define PIN3 0x4C
#define PIN4 0x4D
#define EOP 0x4E    // end of 4 digit pin

// Digital Lock Functions
void loading_screen();
void wait();
void check_pin();
void locked_state();
void unlocked_state();

#endif /* DIGI_LOCK_H_ */
