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
#include "keypad.h"

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

void locked_state() {
    uint8_t input, pin1, pin2, pin3, pin4;

    set_line(LINE1);
    write_string("LOCKED");
    set_line(LINE2);
    write_string("ENTER KEY ");

    while(1) {
       input = get_key();
       if (input != NULLCHAR) {
           write_char(charLUT[input]);
           delay_us(40000);
           delay_us(40000);
           delay_us(40000);
       }

       if (get_addr() == EOP) {
           pin1 = read_data(PIN1);
           pin2 = read_data(PIN2);
           pin3 = read_data(PIN3);
           pin4 = read_data(PIN4);
           check_pin();
           loading_screen();
           write_string("UNLOCKED");
           set_line(LINE2);

           write_char(charLUT[pin1]);
           write_char(charLUT[pin2]);
           write_char(charLUT[pin3]);
           write_char(charLUT[pin4]);

       }
    }


}

void unlocked_state() {

}

void check_pin() {

}

