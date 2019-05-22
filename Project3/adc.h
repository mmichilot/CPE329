/*
 * ADC14.h
 *    CPE 329-15
 *    Project 3: Digital Multimeter
 *    Author: Celestine Co & Matthew Michilot
 */

#ifndef ADC_H_
#define ADC_H_

#define DC_SAMPL_RATE 24

void init_ADC(void);
uint32_t get_flag_adc();
uint32_t get_dc_voltage_adc(int frequency);
uint32_t get_ac_pp_voltage_adc(void);

#endif /* ADC_H_ */
