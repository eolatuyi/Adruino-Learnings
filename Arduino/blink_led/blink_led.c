/*
 * blink_led.c
 *
 *  Created on: Dec 2, 2016
 *      Author: Ebenezer
 *
 *
Configuring Pin
- Register
    - Bits (Access IO Address / Register)
        - DDxn   (DDRx)    1: Output Pin direction
        - PORTxn (PORTx) 0/1: Pin state
        - PINxn  (PINx)    1: Toggles pin irrespective of state of DDxn
 */

#include <avr/io.h>
#include <util/delay.h>



int main()
{
    while(1)
    {
        /* Configure pin PB5 to toggle on off */
        DDRB  |= _BV(DDB5);    // Output port direction

        PORTB |= _BV(PORTB5);  // on pin
        _delay_ms(500);

        PORTB &= ~_BV(PORTB5); // off pin
        _delay_ms(500);
    }

    return 0;
}
