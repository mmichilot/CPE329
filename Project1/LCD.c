/*
 * LCD.c
 *
 *    CPE 329-15
 *    Project 1: Digital Lock
 *    Author: Celestine Co & Matthew Michilot
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
void set_addr(uint8_t addr);

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
    // function set: RS=0, RW=0, DB=0011xxxx
    // 4-bit interface, 2-line display, 5x8 character font
    delay_us(40000);            // wait 40 ms
    P3->OUT &= ~(RS|RW);        // set RS, RW low
    P3->OUT &= ~EN;             // set EN low

    P4->OUT = FUNC_SET_1;       // set 1st command 4 MSB
    toggle_EN();

    delay_us(40);

    // function set: RS=0, RW=0, DB=0010NFxx; wait >37 us
    command(FUNC_SET|LINE_DISP, 40);

    // function set: RS=0, RW=0, DB=0010NFxx; wait >37 us
    command(FUNC_SET|LINE_DISP, 40);

    // function set: RS=0, RW=0, DB=00001DCB; wait >37 us
    command(DISP_CTRL|DISP_ON|CURS_ON|BLINK_ON, 40);

    // disp on/off: RS=0, RW=0, DB=00000001; wait >1.52 ms
    command(DISP_CLR, 1600);

    // entry mode: RS=0, RW=0, DB=00000111; wait >37 us
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

uint8_t get_addr() {
    uint8_t upper, lower;

    P3->OUT &= ~RS;                         // set RS low
    P3->OUT |= RW;                          // set RW high
    P3->OUT &= ~EN;

    P4->DIR &= ~(BIT3|BIT2|BIT1|BIT0);      // set direction to input

    P3->OUT |= EN;                          // get upper 4 bits of addr
    upper = P4->IN & (BIT2|BIT1|BIT0);
    P3->OUT &= ~EN;

    P3->OUT |= EN;                          // get lower 4 bits of addr
    lower = P4->IN & (BIT3|BIT2|BIT1|BIT0);
    P3->OUT &= ~EN;

    P4->DIR |= BIT3|BIT2|BIT1|BIT0;         // set direction to output

    return (upper << 4) | lower;

}

void set_addr(uint8_t addr) {
    command(0x80|addr, 40);
}

uint8_t read_data(uint8_t addr) {
//    uint8_t data;
    uint8_t upper, lower;

    // Set address to read from
    set_addr(addr);

    P3->OUT |= RS; // set RS high
    P3->OUT |= RW;  // set RW high
    P3->OUT &= ~EN;

    P4->DIR &= ~(BIT3|BIT2|BIT1|BIT0);     // set direction to input

    P3->OUT |= EN;
//    data = P4->IN & (BIT3|BIT2|BIT1|BIT0); // get upper 4 bits of addr
    upper = P4->IN & (BIT3|BIT2|BIT1|BIT0); // get upper 4 bits of addr
    P3->OUT &= ~EN;

    P3->OUT |= EN;
//    data = P4->IN & (BIT3|BIT2|BIT1|BIT0); // get lower 4 bits of addr
    lower = P4->IN & (BIT3|BIT2|BIT1|BIT0); // get upper 4 bits of addr
    P3->OUT &= ~EN;

    P4->DIR |= BIT3|BIT2|BIT1|BIT0;         // set direction to output

//    return 0x0F & data;
    return (upper << 4) | lower;
}
