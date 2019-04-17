/*
 * digi_lock.c
 *
 *    CPE 329-15
 *    Project 1: Digital Lock
 *    Author: Celestine Co & Matthew Michilot
 */

#include "delay.h"
#include "LCD.h"
#include "digi_lock.h"

void loading_screen() {
    delay_us(40000);
    clear_LCD();
    write_char('.');
    wait();
    write_char('.');
    wait();
    write_char('.');
    wait();
    clear_LCD();
}

void wait() {
    delay_us(40000);
    delay_us(40000);
    delay_us(40000);
    delay_us(40000);
    delay_us(40000);
    delay_us(40000);
    delay_us(40000);
    delay_us(40000);
    delay_us(40000);
    delay_us(40000);
    delay_us(40000);
}

void check_pin() {

}

