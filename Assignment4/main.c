/**
 * main.c
 *    CPE 329-15
 *    Assignment 4: Keypad
 *    Author: Celestine Co & Matthew Michilot
 */

#include <msp.h>
#include <stdio.h>

#include "delay.h"
#include "LCD.h"
#include "set_dco.h"
#include "keypad.h"

static const uint8_t charLUT[13] = {
  '0', '1', '2', '3', '4', '5' ,'6', '7', '8', '9', '*', ' ', '#'
};

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer
    uint8_t input;

    // LCD Data Bus Setup
    P4->SEL0 &= ~(BIT3|BIT2|BIT1|BIT0);         // P4.0-3 = Data Bus
    P4->SEL1 &= ~(BIT3|BIT2|BIT1|BIT0);
    P4->DIR |= BIT3|BIT2|BIT1|BIT0;             // set direction to output

    // LCD Control Signal Setup
    P3->SEL0 &= ~(BIT3|BIT2|BIT0);              // P3.0=RW, 3.2=RS, 3.3=EN
    P3->SEL1 &= ~(BIT3|BIT2|BIT0);
    P3->DIR |= BIT3|BIT2|BIT0;                  // set direction to output

    // Keypad Setup
    P5->DIR |= (COL1|COL2|COL3);             // set col to outputs 5.5, 5.6, 5.7
    P6->DIR &= ~(ROW1|ROW2|ROW3|ROW4);          // set rows to inputs   6.4, 6.5, 6.6, 6.7
    P6->REN |= (ROW1|ROW2|ROW3|ROW4);           // enable internal resistors
    P6->OUT &= ~(ROW1|ROW2|ROW3|ROW4);          // set resistors to pull down
    P5->OUT &= ~(COL1|COL2|COL3);             // set columns low

    // LCD Program
    lcd_init();
    set_line(LINE1);
    while(1) {
       input = get_key();
       if (input != 0xFF) {
           write_char(charLUT[input]);
           delay_us(40000);
           delay_us(40000);
           delay_us(40000);
       }

       if (get_addr() == 0x10)
           set_line(LINE2);

       if (get_addr() == 0x50)
           clear_LCD();
    }
}
