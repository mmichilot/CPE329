/*
 * ir_sensor.h
 *    CPE 329-15
 *    Final Project: IR Theremin
 *    Author: Celestine Co & Matthew Michilot
 */

#ifndef IR_SENSOR_H_
#define IR_SENSOR_H_

#define PERIOD_75MS 56250
#define PULSE_2MS 1500
#define HI_THRESHOLD 1500

void init_IR(void);
uint32_t get_analog_val(void);
uint32_t get_analog_vol(void);


#endif /* IR_SENSOR_H_ */
