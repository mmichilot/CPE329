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

waveform_state_type current_wf_state = SQUARE_WAVE;
square_state_type current_sq_state = SQ_50;
event_type current_event;


void func_gen_fsm(void) {
    uint8_t input;

    while(1) {

        input = get_key();

        if (input != NULLCHAR)
            selection_ISR(input);
        else
            current_event = idle;

        switch(current_wf_state) {

        case SINE_WAVE:
            if (current_event == idle)
                break;
            else if (current_event == change_to_square)
                current_wf_state = SQUARE_WAVE;
            else if (current_event == change_to_sawtooth)
                current_wf_state = SAWTOOTH_WAVE;
            break;

        case SAWTOOTH_WAVE:
            if (current_event == idle)
                break;
            else if (current_event == change_to_square)
                current_wf_state = SQUARE_WAVE;
            else if (current_event == change_to_sine)
                current_wf_state = SINE_WAVE;
            break;

        case SQUARE_WAVE:
            if (current_event == idle)
                break;
            else if (current_event == change_to_sine)
                current_wf_state = SINE_WAVE;
            else if (current_event == change_to_sawtooth)
                current_wf_state = SAWTOOTH_WAVE;

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
        break;

    case KP_200:
        break;

    case KP_300:
        break;

    case KP_400:
        break;

    case KP_500:
        break;

    case KP_SQ:
        clear_LCD();
        write_string(LCD_SQ_WV);
        current_event = change_to_square;
        break;

    case KP_SIN:
        clear_LCD();
        write_string(LCD_SIN_WV);
        current_event = change_to_sine;
        break;

    case KP_SAW:
        clear_LCD();
        write_string(LCD_ST_WV);
        current_event = change_to_sawtooth;
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

void display(void) {

}
