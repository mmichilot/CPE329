/*
 * uart.h
 *      CPE 329-15
 *      Assignment 8: UART Communication
 *      Author: Celestine Co & Matthew Michilot
 */

#ifndef UART_H_
#define UART_H_

#define DIV_1 0x01
#define ASCII_OFS 0x30

// VT100 Escape Sequences
#define ESC_SEQ 0x1B
#define BLINK_CURSOR "5m"

void init_UART(void);
void print_char(char letter);
void print_string(char* string);
void esc_seq(char* escSeq);

uint32_t check_flag(void);
uint32_t get_voltage(void);
void EUSCIA0_IRQHandler(void);

#endif /* UART_H_ */
