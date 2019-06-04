/*
 * note.h
 *
 *  Created on: May 31, 2019
 *      Author: mmichilot
 */

#ifndef NOTE_H_
#define NOTE_H_

typedef enum {A, B, C, D, E, F, G} Note;

void print_note(Note curr_note);
void print_vol(uint32_t curr_vol);
Note get_note(Note curr_note);
uint32_t get_vol(uint32_t curr_vol);

#endif /* NOTE_H_ */
