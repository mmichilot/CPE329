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
Note get_note(void);

#endif /* NOTE_H_ */
