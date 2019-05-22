/*
 * terminal.c
 *
 *  Created on: May 19, 2019
 *      Author: mmichilot
 */

#include "uart.h"
#include "terminal.h"

void update_bar(float voltage, int row);
void str_voltage(float voltage);
void int_to_str(int num, char* str);
void str_freq(int freq);
void esc_seq(char* escSeq);
void move_cursor(int row, int col);

/* Generates the display
 */
void init_terminal(void) {
    int i;

    esc_seq(CLR_SCREEN);

    // print top and bottom
    for (i = 0; i <= NUM_COLS; i++) {
        move_cursor(0, i);
        print_char('-');
        move_cursor(NUM_ROWS, i);
        print_char('-');

    }

    // print divider
    for (i = 0; i <= NUM_COLS; i++) {
        move_cursor(DIV, i);
        print_char('-');
    }

    // print sides
    for (i = 0; i <= NUM_ROWS; i++) {
       move_cursor(i, 0);
       print_char('|');
       move_cursor(i, NUM_COLS);
       print_char('|');
    }

    // DC Window
    move_cursor(2, 3);
    print_string("DC Measurements");

    move_cursor(4, 3);
    print_string("DC/DC Offset:");

    move_cursor(7, 3);
    print_char('|');
    move_cursor(8, 3);
    print_char('0');

    move_cursor(7, 39);
    print_char('|');
    move_cursor(8, 38);
    print_string("1.65");

    move_cursor(7, 75);
    print_char('|');
    move_cursor(8, 74);
    print_string("3.3");

    move_cursor(8, 78);
    print_char('V');

    // AC Window
    move_cursor(DIV+1, 3);
    print_string("AC Measurements");

    move_cursor(DIV+3, 3);
    print_string("Frequency:");

    move_cursor(DIV+5, 3);
    print_string("Vpp:");

    move_cursor(DIV+7, 3);
    print_string("Vrms:");

    move_cursor(DIV+10, 3);
    print_char('|');
    move_cursor(DIV+11, 3);
    print_char('0');

    move_cursor(DIV+10, 40);
    print_char('|');
    move_cursor(DIV+11, 39);
    print_string("1.65");

    move_cursor(DIV+10, 75);
    print_char('|');
    move_cursor(DIV+11, 74);
    print_string("3.3");

    move_cursor(DIV+11, 78);
    print_char('V');

}

void update_terminal(int freq, float dc, float vpp, float vrms) {

    // update DC Measurements
    move_cursor(DC_ROW, DC_COL);
    str_voltage(dc);
    print_string(" V");

    update_bar(dc, 6);

    // update AC Measurements
    move_cursor(AC_FREQ_ROW, AC_FREQ_COL);
    esc_seq(CLR_LINE);
    str_freq(freq);
    move_cursor(DIV+3, 80);
    print_char('|');

    move_cursor(AC_VPP_ROW, AC_VPP_COL);
    str_voltage(vpp);
    print_string(" V");

    move_cursor(AC_VRMS_ROW, AC_VRMS_COL);
    str_voltage(vrms);
    print_string(" V");

    update_bar(vrms, DIV+9);
}

void update_bar(float voltage, int row) {
    int i = 0;
    int cnt = 0;

    cnt = voltage / 0.041;

    // edge cases
    if (cnt > 73)
        cnt = 75;
    else if (cnt < 4) {
        cnt = 4;
    }

    for (i = 3; i < cnt; i++) {
        move_cursor(row, i);
        print_char('+');
    }

    esc_seq(CLR_LINE);
    move_cursor(row, 80);
    print_char('|');

}

void str_voltage(float voltage) {
    float ones, tenth, hundredth = 0;
    char volt_str[10] = "0.00";

    ones = (int)(voltage);
    tenth = (int)(10 * (voltage - ones));
    hundredth = (int)(10 * ((voltage - ones) * 10 - tenth));

    volt_str[0] += ones;  // ones digit
    volt_str[2] += tenth; // tenth digit
    volt_str[3] += hundredth; // hundredth digit

    print_string(volt_str);
}

void str_freq(int freq) {
    char str[5];

    int_to_str(freq, str);
    print_string(str);
    print_string(" Hz");

}

void int_to_str(int num, char* str) {
    int i, len = 0;
    int val = num;

    // get length of number
    while (num > 0) {
        len++;
        num /= 10;
    }

    // account for when the number is 0
    if (len == 0)
        len = 1;

    // place individual digits
    for (i = len-1; i >= 0; i--) {
        str[i] = val%10 + '0';
        val /= 10;
    }

    str[len] = 0;
}

void esc_seq(char* escSeq) {
    print_char(ESC_SEQ);
    print_char('[');
    print_string(escSeq);
}

void move_cursor(int row, int col) {
    char buf[5];

    // convert row to string
    int_to_str(row, buf);
    print_char(ESC_SEQ);
    print_char('[');
    print_string(buf);
    print_char(';');

    // convert col to string
    int_to_str(col, buf);
    print_string(buf);
    print_char('H');
}



