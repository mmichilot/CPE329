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

#define RS BIT5
#define RW BIT6
#define EN BIT7
#define DISP_ON 0x03
#define CURS_ON 0x0A
#define BLINK_ON 0x09

void clear_LCD();
void home_LCD();
void write_char_LCD();
void write_string_LCD();

#endif /* LCD_H_ */
