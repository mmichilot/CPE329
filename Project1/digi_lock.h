/*
 * digi_lock.h
 *
 *    CPE 329-15
 *    Project 1: Digital Lock
 *    Author: Celestine Co & Matthew Michilot
 */

#ifndef DIGI_LOCK_H_
#define DIGI_LOCK_H_

// Pin digit addresses when entering pin
#define RD_PIN1 0x4A
#define RD_PIN2 0x4B
#define RD_PIN3 0x4C
#define RD_PIN4 0x4D

// Pin digit addresses when writing new pin
#define WR_PIN1 0x40
#define WR_PIN2 0x41
#define WR_PIN3 0x42
#define WR_PIN4 0x43

// Checks end of 4-digit pin
#define EOP_UL 0x44     // end of 4 digit pin in unlocked state
#define EOP_L 0x4E      // end of 4 digit pin in locked state

// Digital Lock Functions
void check_pin();
void locked_state();
void unlocked_state();
void save_pin();

#endif /* DIGI_LOCK_H_ */
