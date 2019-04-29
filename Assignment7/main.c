/**
 * main.c
 *    CPE 329-15
 *    Assignment 7: Execution Timing
 *    Author: Celestine Co & Matthew Michilot
 */

#include "msp.h"
#include "set_dco.h"
#include <math.h>

int8_t TestFunction(int8_t num);
//int32_t TestFunction(int32_t num);
//int64_t TestFunction(int64_t num);
//float TestFunction(float num);
//double TestFunction(double num);

void main(void)
{
    int8_t mainVar;
//    int32_t mainVar;
//    int64_t mainVar;
//    float mainVar;
//    double mainVar;


    set_DCO(CS_CTL0_DCORSEL_1);

    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

    P1->SEL1 &= ~BIT0; //set P1.0 as simple I/O
    P1->SEL0 &= ~BIT0;
    P1->DIR |= BIT0; //set P1.0 as output

    P2->SEL1 &= ~BIT0; //set P2.0 as simple I/O
    P2->SEL0 &= ~BIT0;
    P2->DIR |= BIT0; //set P2.0 as output pins

    P2->OUT |= BIT0; // turn on Blue LED

    mainVar = TestFunction(15); // test function for timing

    P2->OUT &= ~BIT0; // turn off Blue LED

    while(1) // infinite loop to do nothing
        mainVar++; // increment mainVar to eliminate not used warning
}

int8_t TestFunction(int8_t num) {
//int32_t TestFunction(int32_t num) {
//int64_t TestFunction(int64_t num) {
//float TestFunction(float num) {
//double TestFunction(double num) {

    int8_t testVar;
//    int32_t testVar;
//    int64_t testVar;
//    float testVar;
//    double testVar;

    P1->OUT |= BIT0; // turn RED LED on
//    { insert_function_here (ie testVar = num;) }
    P1->OUT &= ~BIT0; // turn RED LED off
 return testVar;
}
