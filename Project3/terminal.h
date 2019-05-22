/*
 * terminal.h
 *
 *  Created on: May 19, 2019
 *      Author: mmichilot
 */

#ifndef TERMINAL_H_
#define TERMINAL_H_

// Terminal dimensions
#define NUM_ROWS 21
#define NUM_COLS 80
#define DIV 9

// VT100 Escape Sequences
#define ESC_SEQ 0x1B
#define BLINK_CURSOR "5m"
#define CLR_SCREEN "2J"
#define CLR_LINE "0K"

// Common cursor positions
#define DC_ROW 4
#define DC_COL 17
#define AC_FREQ_ROW DIV+3
#define AC_FREQ_COL 14
#define AC_VPP_ROW DIV+5
#define AC_VPP_COL 8
#define AC_VRMS_ROW DIV+7
#define AC_VRMS_COL 9

void init_terminal(void);
void update_terminal(int freq, float dc, float vpp, float vrms);


#endif /* TERMINAL_H_ */
