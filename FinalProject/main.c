#include "msp.h"
#include "ir_sensor.h"
#include <stdint.h>

/**
 * main.c
 */

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	P3->SEL0 &= ~BIT0;
	P3->SEL1 &= ~BIT0;
	P3->DIR |= BIT0;

	init_IR();
	__enable_irq();

	while(1);

}
