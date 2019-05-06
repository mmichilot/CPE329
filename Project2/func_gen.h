/*
 * func_gen.h
 *    CPE 329-15
 *    Project 2: Function Generator
 *    Author: Celestine Co & Matthew Michilot
 */

#ifndef FUNC_GEN_H_
#define FUNC_GEN_H_

#define CHIP_SEL BIT0
#define DIV_1 0x01
#define LOWER_MASK 0xFF
#define UPPER_MASK 0x0F
#define DATA_OFS 8
#define HALF_PERIOD HF_PD_50_HZ

#define VPP_2 2482      // 2 V
#define VPP_3_3 4095   // 3.3 V

#define HF_PD_50_HZ 60000
#define HF_PD_100_HZ 30000
#define HF_PD_200_HZ 15000
#define HF_PD_300_HZ 10000
#define HF_PD_400_HZ 7500
#define HF_PD_500_HZ 6000

// Use for incr/decr duty cycle by 10%
#define DC_100HZ 6000
#define DC_200HZ 3000
#define DC_300HZ 2000
#define DC_400HZ 1500
#define DC_500HZ 1200

void init_SPI(void);
void init_timer(void);

#endif /* FUNC_GEN_H_ */
