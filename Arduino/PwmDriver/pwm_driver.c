/*
 * pwm_driver.c
 *
 *  Created on: Nov 2, 2017
 *      Author: ebenezer
 */

#include <avr/io.h>
#include <stdint.h>
// FIXME: Reuse the IO abstraction in shift_register project to map pins to PWM function.
void pwm_driver_init(void) {

    /* TC0 -8-bit Timer/ Counter0 with PWM register init */
    // enable TC0
    // Channel A (PB3)
    // Need to set DDR output dir pin accordingly for I/O register
    DDRB |= _BV(DDB3);    // Output port direction
    DDRD |= _BV(DDD6);    // Output port direction

    // Channel B (PD5)
    // Need to set DDR output dir pin accordingly for I/O register
    DDRD |= _BV(DDD5);    // Output port direction

    /* TCCR0A_R: TC0 Control Register A */
    // Set WGM0[1:0] = 1 : to Phase Correct PWM Mode
    // COM0A[1:0] = 2: use non-inverted so that output is high
    // when the OC match value is TOP 255
    // Also note that toggle not available for B channel
    // COM0B[1:0] = 2: use non-inverted so that output is high
    // when the OC match value is TOP 255
    // Also note that toggle not available of B channel
    //TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00); // FIXME: PD6 channel A not responding appropriately... see why current setting is responding and is it phase correct mode?
    TCCR0A = (1 << COM0A1) | (1 << WGM00) | (1 << COM0B1);
    TCCR2A = (1 << COM2A1) | (1 << WGM20);

    /* TCCR0B_R: TC0 Control Register B */
    // Set CS0[2:0] = 5; clk prescaler {1, 8, 64, 256, 1024}:{1, 2, 3, 4, 5} Clock select
    // TCCR0B_R.WGM02 = 1: to Phase Correct PWM Mode
    //TCCR0B = (1 << WGM02) | (1 << CS02);
    TCCR0B = (1 << CS02);
    TCCR2B = (1 << CS22);

    /* GTCCR_R: General Timer/Counter Control Register */
    // TSM: Timer/Counter Synchronization Mode - when 1, value written to PSRASY and PSRSYNC bits are
    // kept. This ensures corresponding Timer/Counters are halted and can be configured to same value
    // without risk of one Timer advancing during configuration. Setting TSM to 0 clears PSRASY and
    // PSRSYNC so that all Timers/Counters start simultaneously.
    // GTCCR |= (1 << TSM);
    // PSRSYNC: Prescaler Reset - A value of 1 resets the prescaler (stop timer/counter1 and 0) and
    // does not latch only when TSM is set. When TSM is cleared, PSRSYNC resets
    // GTCCR |= (1 << PSRSYNC);
    // PSRASY: Prescaler Reset Timer/Counter2 - A value of 1 resets the prescaler (stop timer/counter2) and
    // does not latch only when TSM is set. When TSM is cleared, PSRASY resets
    // GTCCR &= ~(1 << TSM); // Setting TSM to 0 clears PSRSYNC so that all Timers/Counters start simultaneously.
    /* OCR0A_R[7:0]: TC0 Output Compare Register A */
    OCR0A = 3; // G
    OCR2A = 4; // R

    /* OCR0B_R[7:0]: TC0 Output Compare Register B */
    OCR0B = 0x2f; // B

    /* TC0 -8-bit Timer/ Counter0 with PWM run time set for PWM output */
// set pwm dc
// OCR0A_R, OCR0B_R: output compare registers, compared against TCNT0
// result of compare match generates PWM on OC pins OC0A and OC0B
// Set freq:
    /* TCCR0A_R: TC0 Control Register B */
    // Set CS0[2:0]: Clock select
    /* OCR0A_R[7:0]: TC0 Output Compare Register A */

    /* OCR0B_R[7:0]: TC0 Output Compare Register B */
}
