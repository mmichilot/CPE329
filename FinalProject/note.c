/*
 * note.c
 *    CPE 329-15
 *    Final Project: IR Theremin
 *    Author: Celestine Co & Matthew Michilot
 */

#include "stdint.h"
#include "msp.h"
#include "ir_sensor.h"
#include "lcd.h"
#include "note.h"

volatile int vol_flag = 0;
volatile uint32_t volume = 0;

void print_note(Note curr_note) {
    set_addr(0x06);
    write_char('A' + curr_note);
}

void print_vol(uint32_t curr_vol) {
    set_addr(0x45);
    write_char('0' + curr_vol);
}

Note get_note(Note curr_note) {
    uint32_t analog_val = 0;

    analog_val = get_analog_val();

    if (analog_val > 13500)
        return C;
    else if (analog_val > 9000)
        return D;
    else if (analog_val > 7000)
        return E;
    else if (analog_val > 5000)
        return F;
    else if (analog_val > 3500)
        return G;
    else if (analog_val > 2500)
        return A;
    else if (analog_val > 1500)
        return B;
    else
        return curr_note;
}

uint32_t get_vol(uint32_t curr_vol) {
    uint32_t analog_vol = 0;

    analog_vol = get_analog_vol();

    if (analog_vol > 13500)
        return 1;
    if (analog_vol > 9000)
        return 2;
    if (analog_vol > 7000)
        return 3;
    if (analog_vol > 5000)
        return 4;
    if (analog_vol > 3500)
        return 5;
    if (analog_vol > 2500)
        return 6;
    if (analog_vol > 1500)
        return 7;
    else
        return curr_vol;
}

void adjust_note(Note curr_note) {
    TIMER_A2->CCR[0] = notes_LUT[curr_note];   // reset
    TIMER_A2->CCR[1] = (notes_LUT[curr_note] >> 1);   // toggle
}

void adjust_vol(uint32_t curr_vol) {
    int loByte, hiByte;

    volume = vol_LUT[curr_vol];

    loByte = volume & LOWER_MASK;                      // mask lower 8 bits
    hiByte = (volume >> DATA_OFS) & UPPER_MASK;               // mask upper 4 bits
    hiByte |= BIT4 | BIT5;                                     // set shutdown bit to 1
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));    // wait for TX buffer to empty

    // Beginning of transmission
    P1->OUT &= ~CHIP_SEL;                           // set CS low
    EUSCI_B0->TXBUF = hiByte;                       // send lower 8 bits
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));    // wait for TX buffer to empty
    EUSCI_B0->TXBUF = loByte;                       // send upper 8 bits;
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG));    // wait for transmit to finish
    P1->OUT |= CHIP_SEL;                            // set CS high
}
