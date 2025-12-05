/*
 * blink_led.h
 *
 *  Created on: Dec 2, 2016
 *      Author: Ebenezer
 *
 *
+Configuring Pin
*- Register
+    - Bits (Access IO Address / Register)
+        - DDxn   (DDRx)    1: Output Pin direction
+        - PORTxn (PORTx) 0/1: Pin state
+        - PINxn  (PINx)    1: Toggles pin irrespective of state of DDxn
 */

#ifndef BLINK_LED_H_
#define BLINK_LED_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

void inline blink_led(uint16_t on_period);
void inline blink_led_init(void);

/*
 * Name       : blink_led
 * Description: Configure pin PB5 to toggle on off
 */
void inline blink_led(uint16_t on_period)
{

    PORTB |= _BV(PORTB5);  // on pin
    _delay_ms(on_period/2);

    PORTB &= ~_BV(PORTB5); // off pin
    _delay_ms(on_period/2);

}

void inline blink_led_init()
{
    DDRB  |= _BV(DDB5);    // Output port direction
}


#endif /* BLINK_LED_H_ */
