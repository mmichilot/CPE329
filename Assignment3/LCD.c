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

void clear_LCD()
{

}

void home_LCD()
{

}

void write_char_LCD()
{

}

void write_string_LCD()
{

}

void init()
{
    // wait 40 ms
    // function set: RS=0, RW=0, DB7-4=0011, DB3-0=xxxx
    // 4-bit interface, 2-line display, 5x8 character font
    delay_us(40000);
    P3->OUT &= ~(RS|RW);        // set RS, RW low
    P3->OUT &= ~EN;             // set EN low
    P4->OUT = 0x03              // set 1st command 4 MSB
    P3->OUT &= EN;              // set EN high

    // wait 37 us
    // function set: RS=0, RW=0, DB7-4=0010, DB3-0=xxxx
    // function set: RS=0, RW=0, DB7-4=NFxx, DB3-0=xxxx
    command(37, 0x30);

    // wait 37 us
    // function set: RS=0, RW=0, DB7-4=0010, DB3-0=xxxx
    // function set: RS=0, RW=0, DB7-4=NFxx, DB3-0=xxxx
    command(37, 0x30);

    // wait 37 us
    // function set: RS=0, RW=0, DB7-4=0000, DB3-0=xxxx
    // function set: RS=0, RW=0, DB7-4=1DCB, DB3-0=xxxx
    command(37, 0x0F);

    // wait 37 us
    // disp on/off: RS=0, RW=0, DB7-4=0000, DB3-0=xxxx
    // disp on/off: RS=0, RW=0, DB7-4=0001, DB3-0=xxxx
    command(37, 0x01);

    // wait 1.52 ms
    // entry mode: RS=0, RW=0, DB7-4=0000, DB3-0=xxxx
    // entry mode: RS=0, RW=0, DB7-4=0111, DB3-0=xxxx
    command(1520, 0x09);
    delay_us(40000);
}

void command(int delay, uint8_t cmd)
{
    delay_us(delay);

    P3->OUT &= ~(RS|RW);        // set RS, RW low
    P3->OUT &= ~EN;             // set EN low
    P4->OUT = cmd  // set 1st command 4 MSB
    P3->OUT &= EN;              // set EN high

    check_busy_flag();

    P3->OUT &= ~EN;             // set EN low
    P4->OUT = cmd<<4;           // set 2nd command 4 LSB
    P3->OUT &= EN;              // set EN high
    delay_us(0);

    P3->OUT &= ~EN;             // set EN low
}

void check_busy_flag()
{
    P4->DIR &= ~BIT3;           // set direction to input
    P3->OUT |= RW;              // set RW high
    delay_us(80);
    while((P4->IN)>>3 == 1)
    {
        P3->OUT &= ~EN;         // set EN high
        delay_us(0);
        P3->OUT &= EN;          // set EN low
        delay_us(80);
    }
    P4->DIR &= BIT3;            // set direction back to output
    P3->OUT |= ~RW;             // set RW low
}
