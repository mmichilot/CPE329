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
    change_waveform,
    change_freq,
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
#define LCD_SIN_WV "SINE "
#define LCD_ST_WV "SAWTOOTH "
#define LCD_FR_100 "100 HZ"
#define LCD_FR_200 "200 HZ"
#define LCD_FR_300 "300 HZ"
#define LCD_FR_400 "400 HZ"
#define LCD_FR_500 "500 HZ"
#define LCD_DC "% DUTY CYCLE"

void update_display(void);
void func_gen_fsm(void);
void selection_ISR(uint8_t input);
void write_frequency(void);
void update_fsm(void);

static char *dutycycleLUT[10] = {
  "", "10", "20", "30", "40", "50", "60", "70", "80", "90"
};

#endif /* FUNC_GEN_STATES_H_ */
