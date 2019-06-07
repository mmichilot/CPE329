/*
 * note.h
 *    CPE 329-15
 *    Final Project: IR Theremin
 *    Author: Celestine Co & Matthew Michilot
 */

#ifndef NOTE_H_
#define NOTE_H_

// need to convert the following from frequency to timer a counts
#define C_FREQ 2878 // 261.63 Hz
#define D_FREQ 2564 // 293.67 Hz
#define E_FREQ 2284 // 329.63 Hz
#define F_FREQ 2156 // 349.23 Hz
#define G_FREQ 1920 // 392.00 Hz
#define A_FREQ 1881 // 400.00 Hz
#define B_FREQ 1523 // 493.88 Hz

#define CHIP_SEL BIT0
#define LOWER_MASK 0xFF
#define UPPER_MASK 0x0F
#define DATA_OFS 8

typedef enum {A, B, C, D, E, F, G} Note;
static const uint16_t notes_LUT[7] = {
   A_FREQ, B_FREQ, C_FREQ, D_FREQ, E_FREQ, F_FREQ, G_FREQ
};

static const uint16_t vol_LUT[8] = {
   0, 1695, 2095, 2495, 2895, 3295, 3695, 4095
};

void print_note(Note curr_note);
void print_vol(uint32_t curr_vol);
Note get_note(Note curr_note);
uint32_t get_vol(uint32_t curr_vol);
void adjust_note(Note curr_note);
void adjust_vol(uint32_t curr_vol);

#endif /* NOTE_H_ */
