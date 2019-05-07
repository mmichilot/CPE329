/*
 * func_gen_states.c
 *    CPE 329-15
 *    Project 2: Function Generator
 *    Author: Celestine Co & Matthew Michilot
 */

#include "msp.h"
#include "func_gen_states.h"
#include "keypad.h"
#include "LCD.h"
#include "delay.h"
#include "gen_wave.h"

waveform_state_type current_wf_state = SQUARE_WAVE;
waveform_state_type next_wf_state;
square_state_type current_sq_state = SQ_50;
event_type current_event;

int current_freq = 100;
int current_duty = 50;

void func_gen_fsm(void) {
    uint8_t input;

    while(1) {

        input = get_key();
        while (input == get_key());

        if (input == NULLCHAR) {
            current_event = idle;
        } else {
            current_event = change_freq;
            selection_ISR(input);
        }

        if (current_event == change_waveform)
            current_wf_state = next_wf_state;

        if (current_event != idle)
            update_fsm();
    }
}

void update_fsm(void) {
    __disable_irq();    // temporarily disable global interrupts

    switch(current_wf_state) {
    case SINE_WAVE:
        sine_wave(current_freq);
        break;

    case SAWTOOTH_WAVE:
        saw_wave(current_freq);
        break;

    case SQUARE_WAVE:
        switch (current_event) {
            case duty_cycle_incr:
                if (current_duty != 90)
                    current_duty += 10;
                break;

            case duty_cycle_decr:
                if (current_duty != 10)
                   current_duty -= 10;
                break;

            case duty_cycle_reset:
                current_duty = 50;
                break;

            default:
                break;
            }
        square_wave(current_freq, current_duty);

    default:
        current_wf_state = SQUARE_WAVE;
        break;

    }

    update_display();

    set_voltage(0);     // reset voltage to 0

    __enable_irq();     // re-enable global interrupts

}

void selection_ISR(uint8_t input) {  // triggered upon keypress!

    // 1-5  --> Change to corresponding frequency
    // 7    --> Square Wave
    // 8    --> Sine Wave
    // 9    --> Sawtooth Wave
    // *    --> Decr duty cycle 10%
    // 0    --> Reset duty cycle to 50%

    switch (input) {
    case KP_100:
        current_freq = 100;
        break;

    case KP_200:
        current_freq = 200;
        break;

    case KP_300:
        current_freq = 300;
        break;

    case KP_400:
       current_freq = 400;
       break;

    case KP_500:
        current_freq = 500;
        break;

    case KP_SQ:
        current_event = change_waveform;
        next_wf_state = SQUARE_WAVE;
        break;

    case KP_SIN:
        current_event = change_waveform;
        next_wf_state = SINE_WAVE;
        break;

    case KP_SAW:
        current_event = change_waveform;
        next_wf_state = SAWTOOTH_WAVE;
        break;

    case KP_DC_DECR:
        current_event = duty_cycle_decr;
        break;

    case KP_RESET:
        current_event = duty_cycle_reset;
        break;

    case KP_DC_INCR:
        current_event = duty_cycle_incr;
        break;

    default:
        current_event = idle;
        break;
    }
}

void update_display(void) {
    clear_LCD();

    switch(current_wf_state) {
    case SINE_WAVE:
        write_string(LCD_SIN_WV);
        write_frequency();
        break;

    case SAWTOOTH_WAVE:
        write_string(LCD_ST_WV);
        write_frequency();
        break;

    case SQUARE_WAVE:
        write_string(LCD_SQ_WV);
        write_frequency();
        set_line(LINE2);
        write_string(dutycycleLUT[current_duty/10]);
        write_string(LCD_DC);

    default:
        write_string("Error");
        break;
    }

}

void write_frequency(void) {
    switch(current_freq) {
    case 100:
        write_string(LCD_FR_100);
        break;
    case 200:
        write_string(LCD_FR_200);
        break;
    case 300:
        write_string(LCD_FR_300);
        break;
    case 400:
        write_string(LCD_FR_400);
        break;
    case 500:
        write_string(LCD_FR_500);
        break;
    }
}
