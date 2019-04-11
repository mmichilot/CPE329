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
#define RS BIT2
#define RW BIT0
#define EN BIT3

#define FUNC_SET_1 0x03

// Function Set Definitions
#define FUNC_SET 0x20
#define LINE_DISP BIT3
#define FONT_DISP BIT2

// Display Clear Definitions
#define DISP_CLR 0x01

// Entry Mode Set Definitions
#define ENTRY_MODE 0x04
#define DIRECTION BIT1
#define SHIFT BIT0

// Display On/Off Definitions
#define DISP_CTRL 0x08
#define DISP_ON BIT2
#define CURS_ON BIT1
#define BLINK_ON BIT0

void clear_LCD();
void home_LCD();
void write_char_LCD(uint8_t chr);
void write_string_LCD();

void init();
void command(int delay, uint8_t cmd);
void check_busy_flag();
void toggle_EN();

// Character Codes
#define CHAR_A 0x41
#define CHAR_B 0x42
#define CHAR_C 0x43
#define CHAR_D 0x44
#define CHAR_E 0x45
#define CHAR_F 0x46
#define CHAR_G 0x47
#define CHAR_H 0x48
#define CHAR_I 0x49
#define CHAR_J 0x4A
#define CHAR_K 0x4B
#define CHAR_L 0x4C
#define CHAR_M 0x4D
#define CHAR_N 0x4E
#define CHAR_O 0x4F
#define CHAR_P 0x50
#define CHAR_Q 0x51
#define CHAR_R 0x52
#define CHAR_S 0x53
#define CHAR_T 0x54
#define CHAR_U 0x55
#define CHAR_V 0x56
#define CHAR_W 0x57
#define CHAR_X 0x58
#define CHAR_Y 0x59
#define CHAR_Z 0x5A
#define CHAR_SP 0x20


#endif /* LCD_H_ */
