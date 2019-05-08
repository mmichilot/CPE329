/**
 * main.c
 *    CPE 329-15
 *    Assignment 8: UART Communication
 *    Author: Celestine Co & Matthew Michilot
 */

#include "msp.h"
#include "dac.h"

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

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

	// configure P1.2 and P1.3
	P1->SEL0 |= (BIT2|BIT3);
	P1->SEL1 &= ~(BIT2|BIT3);   // TX and RX pins

	EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;        // enable serial device


}
