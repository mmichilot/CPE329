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

#define RS BIT2
#define RW BIT0
#define EN BIT3
#define DISP_ON 0x03
#define CURS_ON 0x0A
#define BLINK_ON 0x09

void clear_LCD();
void home_LCD();
void write_char_LCD();
void write_string_LCD();

void init();

#endif /* LCD_H_ */
