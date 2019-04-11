/*
 * LCD.c
 *
 *  Created on: Apr 9, 2019
 *  CPE 329-15
 *  Assignment 3: LCD Display
 *  Author: Celestine Co & Matthew Michilot
 */

#include "delay.h"
#include "stdint.h"
#include "LCD.h"
#include "msp.h"
#include "string.h"

// LCD Helper Functions
void command(int delay, uint8_t cmd);
void check_busy_flag();
void toggle_EN();
void write_data(uint8_t data, int delay);

void write_data(uint8_t data, int delay)
{
    P3->OUT &= ~EN;         // set EN low

    P4->OUT = (data)>>4;
    toggle_EN();
    P4->OUT = (data);
    toggle_EN();
    delay_us(delay);
}

void clear_LCD()
{
    P3->OUT &= ~(RS|RW);    // set RS, RW low
    write_data(DISP_CLR, 1520);
}

void home_LCD()
{
    P3->OUT &= ~(RS|RW);    // set RS, RW low
    write_data(HOME|BIT1, 1520);
}

void write_char_LCD(uint8_t chr)
{
    P3->OUT |= RS;          // set RS high
    write_data(chr, 40);
}

void set_line(uint8_t line)
{
    P3->OUT &= ~(RS|RW);    // set RS, RW low
    write_data(SET_LINE|line, 40);
}

void write_string_LCD(char *str)
{
    int i;
    for (i = 0; i < strlen(str); i++){
        write_char_LCD(str[i]);
    }
}

void init()
{
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
    command(40, FUNC_SET|LINE_DISP);

    // wait 37 us
    // function set: RS=0, RW=0, DB=0010NFxx
    command(40, FUNC_SET|LINE_DISP);

    // wait 37 us
    // function set: RS=0, RW=0, DB=00001DCB
    command(40, DISP_CTRL|DISP_ON|CURS_ON|BLINK_ON);

    // wait 37 us
    // disp on/off: RS=0, RW=0, DB=00000001
    command(1600, DISP_CLR);

    // wait 1.52 ms
    // entry mode: RS=0, RW=0, DB=00000111
    command(40, ENTRY_MODE|DIRECTION);
}

void command(int delay, uint8_t cmd)
{
    P3->OUT &= ~(RS|RW);        // set RS, RW low
    P3->OUT &= ~EN;             // set EN low

    P4->OUT = cmd>>4;           // set 1st command 4 MSB
    toggle_EN();

    P4->OUT = cmd;              // set 2nd command 4 LSB
    toggle_EN();

    delay_us(delay);
}

void toggle_EN()
{
    P3->OUT |= EN;              // set EN high
    delay_us(0);
    P3->OUT &= ~EN;             // set EN low
}

void check_busy_flag()
{
    P3->OUT &= ~EN;         // set EN low
    delay_us(0);
    P4->DIR &= ~BIT3;           // set direction to input
    P3->OUT |= RW;              // set RW high
    delay_us(80);
    while((P4->IN && 0x04)>>3 == 1)
    {
        P3->OUT |= EN;          // set EN high
       delay_us(0);
       P3->OUT &= ~EN;         // set EN low
       delay_us(80);
    }
    P4->DIR &= BIT3;            // set direction back to output
    P3->OUT |= ~RW;             // set RW low
}
