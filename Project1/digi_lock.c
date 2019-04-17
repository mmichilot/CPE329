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
    uint8_t input;

    clear_LCD();
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

       if (input == 10)
           locked_state();

       if (get_addr() == EOP) {
           check_pin();
       }
    }


}

void unlocked_state() {
    loading_screen();

    clear_LCD();
    write_string("UNLOCKED");
}

void check_pin() {
    uint8_t pin1, pin2, pin3, pin4;
    char mypin[] = "1234";

    pin1 = read_data(PIN1);
    pin2 = read_data(PIN2);
    pin3 = read_data(PIN3);
    pin4 = read_data(PIN4);

    if (pin1 == mypin[0] && pin2 == mypin[1] && pin3 == mypin[2] && pin4 == mypin[3]) {
        unlocked_state();
    } else {
        locked_state();
    }

}

