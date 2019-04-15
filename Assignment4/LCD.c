/*
 * LCD.c
 *
 *  Created on: Apr 9, 2019
 *  CPE 329-15
 *  Assignment 3: LCD Display
 *  Author: Celestine Co & Matthew Michilot
 */

#include <msp.h>
#include <string.h>
#include <stdint.h>

#include "delay.h"
#include "LCD.h"


// LCD Helper Functions
void command(uint8_t cmd, int delay);
void write_char(uint8_t data);
void output_data(uint8_t data);
void toggle_EN();

// Basic LCD Functions
void clear_LCD() {
    command(DISP_CLR, 1600);
}

void home_LCD() {
    command(HOME, 1600);
}

void set_line(uint8_t line) {
    command(SET_LINE|line, 40);
}

void shift_LCD(uint8_t dir, uint8_t amt) {
    int i;
    for(i = 0; i < amt; i++)
        command(DISP_SHFT|dir, 40);
}

void write_string(char* str) {
    int i;
    for (i = 0; i < strlen(str); i++)
        write_char(str[i]);

}

void lcd_init() {
    // wait 40 ms
    // function set: RS=0, RW=0, DB=0011xxxx
    // 4-bit interface, 2-line display, 5x8 character font
    delay_us(40000);
    P3->OUT &= ~(RS|RW);        // set RS, RW low
    P3->OUT &= ~EN;             // set EN low

    P4->OUT = FUNC_SET_1;       // set 1st command 4 MSB
    toggle_EN();

    delay_us(40);

    // wait 37 us
    // function set: RS=0, RW=0, DB=0010NFxx
    command(FUNC_SET|LINE_DISP, 40);

    // wait 37 us
    // function set: RS=0, RW=0, DB=0010NFxx
    command(FUNC_SET|LINE_DISP, 40);

    // wait 37 us
    // function set: RS=0, RW=0, DB=00001DCB
    command(DISP_CTRL|DISP_ON|CURS_ON|BLINK_ON, 40);

    // wait 37 us
    // disp on/off: RS=0, RW=0, DB=00000001
    command(DISP_CLR, 1600);

    // wait 1.52 ms
    // entry mode: RS=0, RW=0, DB=00000111
    command(ENTRY_MODE|DIRECTION, 40);
}

void command(uint8_t cmd, int delay) {
    P3->OUT &= ~(RS|RW);        // set RS, RW low

    P3->OUT &= ~EN;             // set EN low

    output_data(cmd);

    delay_us(delay);
}

void write_char(uint8_t chr) {
    P3->OUT &= ~RW;         // set RW low
    P3->OUT |= RS;          // set RS high

    P3->OUT &= ~EN;         // set EN low

    output_data(chr);

    delay_us(40);
}

void toggle_EN() {
    P3->OUT |= EN;          // set EN high
    delay_us(0);
    P3->OUT &= ~EN;         // set EN low
}

void output_data(uint8_t data) {
    P4->OUT = data>>4;
    toggle_EN();

    P4->OUT = data;
    toggle_EN();
}
