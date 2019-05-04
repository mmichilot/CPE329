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
#include "delay.h"

void init_keypad();

void main(void) {

    set_DCO(FREQ_12_MHz);   // set SMCLK to 12 Mhz

    //init_keypad();      // Keypad Setup rows 6.4, 6.5, 6.6, 6.7 cols 5.5, 5.6, 5.7
    init_timer();
    init_SPI();         // initialize the SPI bus

    __enable_irq();     // enable global interrupts

    while(1);

}


