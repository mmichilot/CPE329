/**
 * main.c
 *    CPE 329-15
 *    Project 2: Function Generator
 *    Author: Celestine Co & Matthew Michilot
 */

#include "msp.h"
#include "set_dco.h"
#include "keypad.h"
#include "func_gen.h"
#include "func_gen_states.h"
#include "delay.h"
#include "LCD.h"

void init_keypad();

void main(void) {

    set_DCO(FREQ_12_MHz);   // set SMCLK to 12 Mhz

    init_keypad();      // Keypad Setup rows 6.4, 6.5, 6.6, 6.7 cols 5.5, 5.6, 5.7
    init_timer();
    init_SPI();         // initialize the SPI bus 1.0 CS, 1.5 SPI_CLK, 1.6 SIMO

    // LCD Data Bus Setup
    P4->SEL0 &= ~(BIT3|BIT2|BIT1|BIT0);         // P4.0-3 = Data Bus
    P4->SEL1 &= ~(BIT3|BIT2|BIT1|BIT0);
    P4->DIR |= BIT3|BIT2|BIT1|BIT0;             // set direction to output

    // LCD Control Signal Setup
    P3->SEL0 &= ~(BIT3|BIT2|BIT0);              // P3.0=RW, 3.2=RS, 3.3=EN
    P3->SEL1 &= ~(BIT3|BIT2|BIT0);
    P3->DIR |= BIT3|BIT2|BIT0;                  // set direction to output

    lcd_init();

    __enable_irq();     // enable global interrupts

    func_gen_fsm();

}


