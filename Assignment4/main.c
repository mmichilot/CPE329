/**
 * main.c
 *    CPE 329-15
 *    Assignment 4: Keypad
 *    Author: Celestine Co & Matthew Michilot
 */

#include <msp.h>

#include "delay.h"
#include "LCD.h"
#include "set_dco.h"
#include "keypad.h"

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer

    // LCD Data Bus Setup
    P4->SEL0 &= ~(BIT3|BIT2|BIT1|BIT0);         // P4.0-3 = Data Bus
    P4->SEL1 &= ~(BIT3|BIT2|BIT1|BIT0);
    P4->DIR |= BIT3|BIT2|BIT1|BIT0;             // set direction to output

    // LCD Control Signal Setup
    P3->SEL0 &= ~(BIT3|BIT2|BIT0);              // P3.0=RW, 3.2=RS, 3.3=EN
    P3->SEL1 &= ~(BIT3|BIT2|BIT0);
    P3->DIR |= BIT3|BIT2|BIT0;                  // set direction to output

    // Keypad Setup

    // LCD Program
    lcd_init();
    set_line(LINE1);
    write_string("Hello World");
    set_line(LINE2);
    write_string("Assignment 3");

}
