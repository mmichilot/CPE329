/*
 * func_gen.h
 *    CPE 329-15
 *    Project 2: Function Generator
 *    Author: Celestine Co & Matthew Michilot
 */

#ifndef DAC_H_
#define DAC_H_

#define CHIP_SEL BIT0

#define DIV_1 0x01
#define LOWER_MASK 0xFF
#define UPPER_MASK 0x0F
#define DATA_OFS 8

void init_DAC(void);
void set_voltage(int voltage);

#endif /* DAC_H_ */
