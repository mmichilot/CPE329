/*
 * dco_set.h
 *    CPE 329-15
 *    Final Project: IR Theremin
 *    Author: Celestine Co & Matthew Michilot
 */

#ifndef SET_DCO_H_
#define SET_DCO_H_

#define FREQ_1_5_MHz    CS_CTL0_DCORSEL_0
#define FREQ_3_MHz      CS_CTL0_DCORSEL_1
#define FREQ_6_MHz      CS_CTL0_DCORSEL_2
#define FREQ_12_MHz     CS_CTL0_DCORSEL_3
#define FREQ_24_MHz     CS_CTL0_DCORSEL_4
#define FREQ_48_MHz     CS_CTL0_DCORSEL_5

void set_DCO(int freq);

#endif /* SET_DCO_H_ */
