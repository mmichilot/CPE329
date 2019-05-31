/**
 * main.c
 *    CPE 329-15
 *    Final Project: IR Theremin
 *    Author: Celestine Co & Matthew Michilot
 */

#include "msp.h"
#include "ir_sensor.h"
#include "lcd.h"
#include "note.h"



void main(void)
{
    Note curr_note = C;

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	init_IR();
	init_LCD();
	__enable_irq();

	write_string("Note: ");
	print_note(curr_note);

	while(1) {

	    // only update note if it changes
	    if (curr_note != get_note()) {
	        curr_note = get_note();
	        print_note(curr_note);
	    }
	}

}
