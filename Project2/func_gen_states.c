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

waveform_state_type current_wf_state = SQUARE_WAVE;
waveform_state_type next_wf_state;
square_state_type current_sq_state = SQ_50;
event_type current_event;

uint32_t current_freq = 100;

void func_gen_fsm(void) {
    uint8_t input;

    while(1) {

        input = get_key();

        if (input == NULLCHAR) {
            current_event = idle;
        } else
            selection_ISR(input);

        if (current_event == change_waveform)
            current_wf_state = next_wf_state;

        switch(current_wf_state) {

        case SINE_WAVE:
            // sine wave stuff
            break;

        case SAWTOOTH_WAVE:
            // sawtooth stuff
            break;

        case SQUARE_WAVE:
            switch (current_sq_state) {
            case SQ_50:
                if (current_event == duty_cycle_incr)
                    current_sq_state = SQ_60;
                else if (current_event == duty_cycle_decr)
                    current_sq_state = SQ_40;
                break;

            case SQ_60:
                if (current_event == duty_cycle_reset || current_event == duty_cycle_decr)
                    current_sq_state = SQ_50;
                else if (current_event == duty_cycle_incr)
                     current_sq_state = SQ_70;
                break;

            case SQ_70:
                if (current_event == duty_cycle_reset)
                    current_sq_state = SQ_50;
                else if (current_event == duty_cycle_incr)
                     current_sq_state = SQ_80;
                else if (current_event == duty_cycle_decr)
                     current_sq_state = SQ_60;
                break;

            case SQ_80:
                if (current_event == duty_cycle_reset)
                    current_sq_state = SQ_50;
                else if (current_event == duty_cycle_incr)
                     current_sq_state = SQ_90;
                else if (current_event == duty_cycle_decr)
                     current_sq_state = SQ_70;
                break;

            case SQ_90:
                if (current_event == duty_cycle_reset)
                    current_sq_state = SQ_50;
                else if (current_event == duty_cycle_decr)
                     current_sq_state = SQ_80;
                break;

            case SQ_10:
                if (current_event == duty_cycle_reset)
                    current_sq_state = SQ_50;
                else if (current_event == duty_cycle_incr)
                     current_sq_state = SQ_20;
                else if (current_event == duty_cycle_decr)
                     current_sq_state = SQ_10;
                break;

            case SQ_20:
                if (current_event == duty_cycle_reset)
                    current_sq_state = SQ_50;
                else if (current_event == duty_cycle_incr)
                     current_sq_state = SQ_30;
                else if (current_event == duty_cycle_decr)
                     current_sq_state = SQ_10;
                break;

            case SQ_30:
                if (current_event == duty_cycle_reset)
                    current_sq_state = SQ_50;
                else if (current_event == duty_cycle_incr)
                     current_sq_state = SQ_40;
                else if (current_event == duty_cycle_decr)
                     current_sq_state = SQ_20;
                break;

            case SQ_40:
                if (current_event == duty_cycle_reset || current_event == duty_cycle_incr)
                     current_sq_state = SQ_50;
                else if (current_event == duty_cycle_decr)
                     current_sq_state = SQ_30;
                break;

            default:
                current_sq_state = SQ_50;
                break;
            }

            default:
                current_wf_state = SQUARE_WAVE;
                break;
        }

    }
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
        delay_us(20000);    // debounce button
        break;

    case KP_RESET:
        current_event = duty_cycle_reset;
        break;

    case KP_DC_INCR:
        current_event = duty_cycle_incr;
        delay_us(20000);    // debounce button
        break;

    default:
        current_event = idle;
        break;
    }

    update_display();

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
        switch(current_sq_state) {
        case SQ_50:
            write_string(LCD_DC_50);
            break;
        case SQ_60:
            write_string(LCD_DC_60);
            break;
        case SQ_70:
            write_string(LCD_DC_70);
            break;
        case SQ_80:
            write_string(LCD_DC_80);
            break;
        case SQ_90:
            write_string(LCD_DC_90);
            break;
        case SQ_10:
            write_string(LCD_DC_10);
            break;
        case SQ_20:
            write_string(LCD_DC_20);
            break;
        case SQ_30:
            write_string(LCD_DC_30);
            break;
        case SQ_40:
            write_string(LCD_DC_40);
            break;
        default:
            write_string("Error 1");
            break;
        }
        break;

        default:
            write_string("Error 2");
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
