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

#define RS BIT2
#define RW BIT0
#define EN BIT3
#define FUNC_SET_1 0x03
#define FUNC_SET 0x20
#define LINE_DISP BIT3
#define FONT_DISP BIT2
#define DISP_CLR 0x01
#define ENTRY_MODE 0x04
#define DIRECTION BIT1
#define SHIFT BIT0

#define DISP_CTRL 0x08
#define DISP_ON BIT3
#define CURS_ON BIT2
#define BLINK_ON BIT1

void clear_LCD();
void home_LCD();
void write_char_LCD();
void write_string_LCD();

void init();
void command(int delay, uint8_t cmd);
void check_busy_flag();

#endif /* LCD_H_ */
