/*
 * shift_register.c
 *
 *  Created on    : Jun 10, 2017
 *      Author    : ebenezer
 *  Description   : Shift register abstraction
 */
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <string.h>
#include "shift_register.h"

/*
 * Name       : shift_register_set_output
 * Description:
 */
void shift_register_set_output(shift_register_cblk_t *cblk, uint8_t output_num, shift_register_output_state_t output_pin_state)
{

    // Shift in
    shift_register_shift_in(cblk, output_num, output_pin_state);

    // latch_data to output
    shift_register_latch_data_to_output(cblk);


}


/*
 * Name       : shift_register_shift_in
 * Description:
 */
void shift_register_shift_in(shift_register_cblk_t *cblk,uint8_t shift_count, shift_register_output_state_t binary_value)
{
    uint8_t idx;
    uint8_t data_bit_set = IS_BIT_SET(cblk->data, shift_count);

    cblk->digital_input.set_srclr(OFF);
    cblk->clear_data = 0;

    // if data at shift_count is not set, then shift in IS_BIT_SET(cblk->data, pos)

    if ((!data_bit_set && binary_value == ON) ||
            (data_bit_set && binary_value == OFF))
    {

        if (binary_value == ON)
        {
            cblk->data |=  (1 << shift_count);
        }
        else
        {
            cblk->data &= ~(1 << shift_count);
        }

        for (idx = 7; idx >= 0 && idx <= 7; idx--)
        {

                if (IS_BIT_SET(cblk->data, idx))
                {
                    cblk->digital_input.set_ser(ON);
                }
                else
                {
                    cblk->digital_input.set_ser(OFF);
                }
            // Assert register data
            cblk->digital_input.set_srclk(ON);
            cblk->digital_input.set_srclk(OFF);
        }



        if (binary_value == ON)
        {
            cblk->data |=  (1 << shift_count);
        }
        else
        {
            cblk->data &= ~(1 << shift_count);
        }
    }
}

/*
 * Name       : shift_register_clear_register
 * Description:
 */
void shift_register_clear_register(shift_register_cblk_t *cblk)
{
    cblk->digital_input.set_srclr(ON);
    cblk->clear_data = 1;

    cblk->data = 0;
}


/*
 * Name       : shift_register_latch_data_to_output
 * Description:
 */
void shift_register_latch_data_to_output(shift_register_cblk_t *cblk)
{
    // latch data command
    cblk->digital_input.set_rclk(ON);
    cblk->digital_input.set_rclk(OFF);

    cblk->latch_enable   = 1;
    cblk->latched_output = cblk->data;
}


/*
 * Name       : shift_register_init
 * Description:
 */
void shift_register_init(shift_register_cblk_t *cblk)
{
    // Init Device


    cblk->digital_input.set_oe(OFF);

    shift_register_clear_register(cblk);

    cblk->digital_input.set_srclk(OFF);

    cblk->digital_input.set_rclk(ON);
    cblk->digital_input.set_rclk(OFF);

    cblk->digital_input.set_oe(ON);

    // Init Parameters and Variables
    //memset(cblk,0,sizeof(shift_register_cblk_t));

}
