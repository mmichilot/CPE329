/*
 * gen_wave.h
 *
 *  Created on: May 5, 2019
 *      Author: mmichilot
 */

#ifndef GEN_WAVE_H_
#define GEN_WAVE_H_


typedef enum {
    SQUARE,
    SINE,
    SAWTOOTH
} wave_type;

void gen_wave(wave_type wave, int freq, int duty);

#endif /* GEN_WAVE_H_ */
