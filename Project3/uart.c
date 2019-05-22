/*
 * uart.c
 *    CPE 329-15
 *    Project 3: Digital Multimeter
 *    Author: Celestine Co & Matthew Michilot
 */

#include "msp.h"
#include "uart.h"
#include "string.h"

static uint32_t flag = 0;
static uint32_t voltage = 0;

void int_to_str(int num, char* str);

void init_UART(void) {

    // initialize serial device as UART
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;         // put serial device into reset state

    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_PEN             // enable parity (odd)
                    | EUSCI_A_CTLW0_SPB             // 2 stop bits
                    | EUSCI_A_CTLW0_UCSSEL_2        // use SMCLK
                    | EUSCI_A_CTLW0_MODE_0          // UART mode
                    | EUSCI_A_CTLW0_SWRST;          // keep in reset

    // Setup of registers based on calculations
    EUSCI_A0->BRW = 26;
    EUSCI_A0->MCTLW |= EUSCI_A_MCTLW_OS16 | (0xB5 << EUSCI_A_MCTLW_BRS_OFS);

    // Configure Pins
    // P1.2 - RX | P1.3 - TX
    P1->SEL0 |= (BIT2|BIT3);
    P1->SEL1 &= ~(BIT2|BIT3);

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;    // activate UART

    // setup UART receive interrupts
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;            // enable receive interrupt
    NVIC->ISER[0] = 1 << (EUSCIA0_IRQn & 31);   // enable interrupt in NVIC
}

void print_char(char letter) {
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));    //wait for TX buffer to empty
    EUSCI_A0->TXBUF = letter;   // transmit character
}

void print_string(char* string) {
    int i;

    for (i = 0; i < strlen(string); i++)
        print_char(string[i]);
}

uint32_t check_flag(void) {
    return flag;
}

uint32_t get_voltage(void) {
    int voltageVal = voltage;

    //reset flag and voltage
    flag = 0;
    voltage = 0;

    return voltageVal;
}

void EUSCIA0_IRQHandler(void) {
    int input;

    if (EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG) {

        // get user input
        input = EUSCI_A0->RXBUF;

        // echo user input
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));    //wait for TX buffer to empty
        EUSCI_A0->TXBUF = input;   // transmit character

        // check for carriage return
        if (input == '\r') {
            flag = 1;
        }
        else {
            // parse user input
            input -= ASCII_OFS;
            if (input <= 9)
                voltage = (voltage*10) + input;
        }
    }
}

