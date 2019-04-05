/*
 * dco_set.h
 *
 *  Created on: Apr 5, 2019
 *  Author: Celestine Co & Matthew Michilot
 */

#ifndef DCO_SET_H_
#define DCO_SET_H_

#define FREQ_1.5_MHz    CS_CTL0_DCORSEL_0;
#define FREQ_3_MHz      CS_CTL0_DCORSEL_1;
#define FREQ_6_MHz      CS_CTL0_DCORSEL_2;
#define FREQ_12_MHz     CS_CTL0_DCORSEL_3;
#define FREQ_24_MHz     CS_CTL0_DCORSEL_4;
#define FREQ_48_MHz     CS_CTL0_DCORSEL_5;

void set_DCO(uint32_t freq);

#endif /* DCO_SET_H_ */
