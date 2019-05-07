/*
 * gen_wave.h
 *
 *  Created on: May 5, 2019
 *      Author: mmichilot
 */

#ifndef GEN_WAVE_H_
#define GEN_WAVE_H_

#include "func_gen_states.h"

void saw_wave(int freq);
void set_voltage(int voltage);
void square_wave(int freq, int duty);
void sine_wave(int freq);
void init_sine();

#endif /* GEN_WAVE_H_ */
