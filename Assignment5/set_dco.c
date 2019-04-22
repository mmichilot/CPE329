/*
 * set_dco.c
 *    CPE 329-15
 *    Assignment 5: Interrupts and Timers
 *    Author: Celestine Co & Matthew Michilot
 */

#include <msp432p401r.h>
#include "set_dco.h"

void set_DCO(int freq)
{
    if (freq == FREQ_48_MHz){
        /* Transition to VCORE Level 1: AM0_LDO --> AM1_LDO */
        while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
         PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_1;
        while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));

        /* Configure Flash wait-state to 1 for both banks 0 & 1 */
        FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL &
         ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) | FLCTL_BANK0_RDCTL_WAIT_1;
        FLCTL->BANK1_RDCTL = (FLCTL->BANK0_RDCTL &
         ~(FLCTL_BANK1_RDCTL_WAIT_MASK)) | FLCTL_BANK1_RDCTL_WAIT_1;
    }

    CS->KEY = CS_KEY_VAL; //unlock CS
    CS->CTL0 = freq;
    CS->CTL1 |= CS_CTL1_DIVM__1 | CS_CTL1_SELM__DCOCLK; // set MCLK Source to DCO and C
    CS->KEY = 0; // lock CS
}
