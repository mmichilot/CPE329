/*
 * uart.h
 *    CPE 329-15
 *    Project 3: Digital Multimeter
 *    Author: Celestine Co & Matthew Michilot
 */

#ifndef UART_H_
#define UART_H_

#include "msp.h"

#define DIV_1 0x01
#define ASCII_OFS 0x30

void init_UART(void);
void print_char(char letter);
void print_string(char* string);

uint32_t check_flag(void);
uint32_t get_voltage(void);
void EUSCIA0_IRQHandler(void);

#endif /* UART_H_ */
