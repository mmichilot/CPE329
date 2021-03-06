/**
 * main.c
 *    CPE 329-15
 *    Project 2: Function Generator
 *    Author: Celestine Co & Matthew Michilot
 */

#include <dac.h>
#include "msp.h"
#include "set_dco.h"
#include "keypad.h"
#include "func_gen_states.h"
#include "delay.h"
#include "LCD.h"
#include "gen_wave.h"

void main(void) {

    set_DCO(FREQ_12_MHz);   // set SMCLK to 12 Mhz

    init_keypad();      // Keypad Setup rows 6.4, 6.5, 6.6, 6.7 cols 5.5, 5.6, 5.7
    init_timer();
    init_SPI();         // initialize the SPI bus 1.0 CS, 1.5 SPI_CLK, 1.6 SIMO
    init_lcd();
    init_sine();

    update_fsm();
    func_gen_fsm();

}


