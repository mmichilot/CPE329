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
#include "dac.h"

void main(void)
{
    Note curr_note = C;
    uint32_t curr_vol = 1;

    Note tmp_note = curr_note;
    uint32_t tmp_vol = curr_vol;

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	init_IR();
	init_LCD();
	init_DAC();

	// Initialize TimerA2 CCR1 for Square Wave Gen
	TIMER_A2->CCR[0] = C_FREQ;
    TIMER_A2->CCR[1] = (C_FREQ >> 1);

    TIMER_A2->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7; // output mode: reset/set

    TIMER_A2->CTL = TIMER_A_CTL_TASSEL_2    // select SMCLK
                  | TIMER_A_CTL_ID_2        // CLK/4
                  | TIMER_A_CTL_MC_1;       // up mode

    // Pull TA2.1 on P5.6
    P5->SEL0 |= BIT6;
    P5->SEL1 &= ~BIT6;
    P5->DIR |= BIT6;

	write_string("Note: ");
	set_addr(0x40);
	write_string("Vol: ");

	print_note(curr_note);
	print_vol(curr_vol);

    __enable_irq();

    set_voltage(2048);


	while(1) {

	    // only update note if it changes
	    if (curr_note != (tmp_note = get_note(curr_note))) {
	        curr_note = tmp_note;
	        print_note(curr_note);
	    }

	    // only update vol if it changes
	    if (curr_vol != (tmp_vol = get_vol(curr_vol))) {
	        curr_vol = tmp_vol;
	        print_vol(curr_vol);
	    }
	}

}
