/*
 * ADC14.h
 *
 *  Created on: May 10, 2019
 *      Author: mmichilot
 */

#ifndef ADC_H_
#define ADC_H_


void init_ADC(void);
void str_voltage(float voltage);
uint32_t get_flag_adc();
uint32_t get_voltage_adc();


#endif /* ADC_H_ */
