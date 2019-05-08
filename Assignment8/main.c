/**
 * main.c
 *    CPE 329-15
 *    Assignment 8: UART Communication
 *    Author: Celestine Co & Matthew Michilot
 */

#include "msp.h"
#include "string.h"
#include "stdlib.h"
#include "dac.h"
#include "set_dco.h"

void print_char(char letter);
void print_string(char* string);

int flag = 0;
char inValue[5];
int index = 0;

void main(void)
{
    int voltage = 0;

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	set_DCO(FREQ_3_MHz);

	init_SPI();

	// initialize serial device
	EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;         // put serial device into reset state

	EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_PEN             // enable parity (odd)
	                | EUSCI_A_CTLW0_SPB             // 2 stop bits
	                | EUSCI_A_CTLW0_UCSSEL_2        // use SMCLK
	                | EUSCI_A_CTLW0_MODE_0          // UART mode
	                | EUSCI_A_CTLW0_SWRST;          // keep in reset

	EUSCI_A0->BRW = 0x01;   // from baud rate calculations
	EUSCI_A0->MCTLW = (10 << EUSCI_A_MCTLW_BRF_OFS)
	                | EUSCI_A_MCTLW_OS16;   // from calculations

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;        // enable serial device

	// setup UART interrupts
	EUSCI_A0->IE |= EUSCI_A_IE_RXIE;            // enable receive interrupt
	NVIC->ISER[0] = 1 << (EUSCIA0_IRQn & 31);   // enable interrupt in NVIC

	// configure P1.2 and P1.3
	P1->SEL0 |= (BIT2|BIT3);
	P1->SEL1 &= ~(BIT2|BIT3);   // TX and RX pins

	__enable_irq();

	while(1) {
	    if (flag) {
	        print_string(inValue);
	        voltage = atoi(inValue);
	        if (voltage < 4096 && voltage >= 0)
	            set_voltage(voltage);
	        flag = 0;
	        index = 0;
	        voltage = 0;
	    }
	}
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

void EUSCIA0_IRQHandler(void) {
    char letter;
    if (EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG) {
        // get user input
        letter = EUSCI_A0->RXBUF;   // transmit character

        // echo user input
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));    //wait for TX buffer to empty
        EUSCI_A0->TXBUF = letter;   // transmit character

        // parse user input
        if (letter >= '0' && letter <= '9' && index < 4) {
            inValue[index] = letter;
            index++;
        }

        if (letter == 0xD) {
            inValue[index] = '\0';
            flag = 1;
        }
    }
}
