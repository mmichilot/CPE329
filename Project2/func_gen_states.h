/*
 * func_gen_states.h
 *    CPE 329-15
 *    Project 2: Function Generator
 *    Author: Celestine Co & Matthew Michilot
 */

#ifndef FUNC_GEN_STATES_H_
#define FUNC_GEN_STATES_H_

typedef enum {
        SINE_WAVE,
        SAWTOOTH_WAVE,
        SQUARE_WAVE
} waveform_state_type;

typedef enum {
    SQ_50,
    SQ_60,
    SQ_70,
    SQ_80,
    SQ_90,
    SQ_10,
    SQ_20,
    SQ_30,
    SQ_40,
} square_state_type;

typedef enum {
    idle,
    change_to_square,
    change_to_sine,
    change_to_sawtooth,
    duty_cycle_incr,
    duty_cycle_decr,
    duty_cycle_reset
} event_type;

// Keypad Select
#define KP_100 1
#define KP_200 2
#define KP_300 3
#define KP_400 4
#define KP_500 5
#define KP_SQ 7
#define KP_SIN 8
#define KP_SAW 9
#define KP_DC_DECR 10
#define KP_RESET 0
#define KP_DC_INCR 12

// LCD Display Macros
#define LCD_SQ_WV "SQUARE "
#define LCD_SIN_WV "SINUSOIDAL "
#define LCD_ST_WV "SAWTOOTH "
#define LCD_FR_100 "100 HZ"
#define LCD_FR_200 "200 HZ"
#define LCD_FR_300 "300 HZ"
#define LCD_FR_400 "400 HZ"
#define LCD_FR_500 "500 HZ"
#define LCD_DC_10 "10% DUTY CYCLE"
#define LCD_DC_20 "20% DUTY CYCLE"
#define LCD_DC_30 "30% DUTY CYCLE"
#define LCD_DC_40 "40% DUTY CYCLE"
#define LCD_DC_50 "50% DUTY CYCLE"
#define LCD_DC_60 "60% DUTY CYCLE"
#define LCD_DC_70 "70% DUTY CYCLE"
#define LCD_DC_80 "80% DUTY CYCLE"
#define LCD_DC_90 "90% DUTY CYCLE"

void display(void);
void func_gen_fsm(void);
void selection_ISR(uint8_t input);

#endif /* FUNC_GEN_STATES_H_ */
