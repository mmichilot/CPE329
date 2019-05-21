/*
 * dmm.h
 *    CPE 329-15
 *    Project 3: Digital Multimeter
 *    Author: Celestine Co & Matthew Michilot
 */

#ifndef DMM_H_
#define DMM_H_

#define FREQ_DIV 64000
#define SEC_DELAY 65535
#define COUNT_UPP_BND 65535

int get_freq(void);
float get_dc_voltage(void);
void init_timer(void);

#endif /* DMM_H_ */
