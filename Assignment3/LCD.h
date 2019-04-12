/*
 * LCD.h
 *
 *  Created on: Apr 9, 2019
 *  CPE 329-15
 *  Assignment 3: LCD Display
 *  Author: Celestine Co & Matthew Michilot
 */

#ifndef LCD_H_
#define LCD_H_

#include <msp.h>

// Control Signals
#define RW BIT0
#define RS BIT2
#define EN BIT3

#define FUNC_SET_1  0x03

// Function Set Definitions
#define FUNC_SET    0x20
#define LINE_DISP   BIT3
#define FONT_DISP   BIT2

// Display Clear Definitions
#define DISP_CLR    0x01
#define HOME        0x02

// Entry Mode Set Definitions
#define ENTRY_MODE  0x04
#define DIRECTION   BIT1
#define SHIFT       BIT0

// Display On/Off Definitions
#define DISP_CTRL   0x08
#define DISP_ON     BIT2
#define CURS_ON     BIT1
#define BLINK_ON    BIT0

// LCD Lines
#define LINE1       0x00
#define LINE2       0x40
#define SET_LINE    0x80

// Display Shift Definitions
#define DISP_SHFT   0x18
#define SHFT_RIGHT  BIT2

// LCD Functions
void clear_LCD();
void home_LCD();
void set_line(uint8_t line);
void write_char(uint8_t chr);
void write_string(char* str);
void shift_LCD(uint8_t dir, uint8_t amt);
void init();

#endif /* LCD_H_ */
