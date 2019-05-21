/*
 * ADC14.c
 *    CPE 329-15
 *    Project 3: Digital Multimeter
 *    Author: Celestine Co & Matthew Michilot
 */

#include "msp.h"
#include "uart.h"
#include "adc.h"

static int flag = 0;
static int conv_flag = 0;
static int analogValue = 0;

volatile int sample_rate = DC_SAMPL_RATE;
volatile int num_samples = 0;
volatile int curr_sample = 0;

volatile uint32_t offset = 0;

volatile int dc_mode = 1;
volatile int max_voltage = 0;
volatile int min_voltage = 4096;

void init_ADC(void) {

    // initialize TA1
    TIMER_A1->CTL |= TIMER_A_CTL_TASSEL_2   // use SMCLK
                      | TIMER_A_CTL_MC_2;   // continuous mode

    TIMER_A1->CCR[1] = sample_rate;
    TIMER_A1->CCTL[1] |= TIMER_A_CCTLN_CCIE;    // enable CCR1 interrupt

    NVIC->ISER[0] = 1 << (TA1_N_IRQn & 31); // enable CCR1 TIMER A1 ISR

    // initialize ADC
    ADC14->CTL0 &= ~ADC14_CTL0_ENC; // disable ADC for configuration

    ADC14->CTL0 = ADC14_CTL0_SHP  // use internal timer
                | ADC14_CTL0_SHT0_2 // 16 clocks per sample
                | ADC14_CTL0_SSEL_3 // select MCLK
                | ADC14_CTL0_ON;    // turn on ADC14

    ADC14->CTL1 = (2 << ADC14_CTL1_CSTARTADD_OFS)  // start conversion using mem[2]
                | ADC14_CTL1_RES_3;                // use 14-bit conversion

    ADC14->MCTL[2] = ADC14_MCTLN_INCH_14;   // read from channel 14 (A14)

    // Configure Pin
    // P6.1 - A14
    P6->SEL0 |= BIT1;
    P6->SEL1 |= BIT1;

    // setup conversion interrupts
    ADC14->IER0 |= ADC14_IER0_IE2;          // enable interrupts on mem[2]
    NVIC->ISER[0] = 1 << (ADC14_IRQn & 31); // enable ADC ISR in NVIC
}

uint32_t get_flag_adc() {
    return flag;
}

uint32_t get_dc_voltage_adc(int frequency) {
    int voltageVal;

    if (frequency == 0) { // DC
        dc_mode = 1;
        sample_rate = DC_SAMPL_RATE;
    } else {   // AC
        dc_mode = 0;
        num_samples = (int)(64000 * 64 /frequency);
        sample_rate = DC_SAMPL_RATE;
        max_voltage = 0;
        min_voltage = 4096;
        curr_sample = 0;
    }

    TIMER_A1->CCR[1] = sample_rate;

    conv_flag = 1;
    TIMER_A1->CCTL[1] |= TIMER_A_CCTLN_CCIE;   // enable CCR0 interrupt

    while(flag == 0);   // wait for conversion to finish

    voltageVal = analogValue;

    //reset flag and voltage
    flag = 0;
    analogValue = 0;


    if (dc_mode)
        return voltageVal;  // DC value
    else
        offset = (min_voltage + max_voltage) / 2; // DC offset for AC voltage
    return offset;
}

uint32_t get_ac_pp_voltage_adc(void) {
    return max_voltage - min_voltage;
}

void TA1_N_IRQHandler(void) {
    if (TIMER_A1->CCTL[1] & TIMER_A_CCTLN_CCIFG) {
        TIMER_A1->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;  // clear interrupt flag

        TIMER_A1->CCR[1] += sample_rate;

        if (conv_flag == 1) {
            ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;   // enable and start conversion
            conv_flag = 0;
            TIMER_A1->CCTL[1] &= ~TIMER_A_CCTLN_CCIE;   // disable CCR0 interrupt
        }
    }
}

void ADC14_IRQHandler(void) {
    analogValue = ADC14->MEM[2];

    if (analogValue > max_voltage)
        max_voltage = analogValue;
    if (analogValue < min_voltage)
        min_voltage = analogValue;

    if (dc_mode) {
        ADC14->CTL0 &= ~ADC14_CTL0_ENC; // disable ADC
        flag = 1;
    }  else {
        curr_sample++;
    }

    if (curr_sample == num_samples) {
        ADC14->CTL0 &= ~ADC14_CTL0_ENC; // disable ADC
        flag = 1;

    } else {
        ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;   // enable and start conversion
    }
}

void str_dc_voltage(float voltage) {
    float ones, tenth, hundredth = 0;
    char volt_str[10] = "0.00 V DC";

    ones = (int)(voltage);
    tenth = (int)(10 * (voltage - ones));
    hundredth = (int)(10 * ((voltage - ones) * 10 - tenth));

    volt_str[0] += ones;  // ones digit
    volt_str[2] += tenth; // tenth digit
    volt_str[3] += hundredth; // hundredth digit

    print_string(volt_str);
    print_char('\n');
    print_char('\r');
}

void str_ac_pp_voltage(float voltage) {
    float ones, tenth, hundredth = 0;
    char volt_str[12] = "0.00 VPP AC";

    ones = (int)(voltage);
    tenth = (int)(10 * (voltage - ones));
    hundredth = (int)(10 * ((voltage - ones) * 10 - tenth));

    volt_str[0] += ones;  // ones digit
    volt_str[2] += tenth; // tenth digit
    volt_str[3] += hundredth; // hundredth digit

    print_string(volt_str);
    print_char('\n');
    print_char('\r');
}

void str_ac_rms_voltage(float voltage) {
    float ones, tenth, hundredth = 0;
    char volt_str[13] = "0.00 VRMS AC";

    ones = (int)(voltage);
    tenth = (int)(10 * (voltage - ones));
    hundredth = (int)(10 * ((voltage - ones) * 10 - tenth));

    volt_str[0] += ones;  // ones digit
    volt_str[2] += tenth; // tenth digit
    volt_str[3] += hundredth; // hundredth digit

    print_string(volt_str);
    print_char('\n');
    print_char('\r');
}

