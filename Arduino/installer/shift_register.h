/*
 * shift_register.h
 *
 *  Created on: Jun 10, 2017
 *      Author: ebenezer
 */

#include <stdint.h>

#ifndef SHIFT_REGISTER_H_
#define SHIFT_REGISTER_H_

#define IS_BIT_SET(var, pos) ( !!( ( var ) & ( 1 << (pos) ) ) )

enum shift_register_output_state
{
    OFF,
    ON,
};
typedef enum shift_register_output_state shift_register_output_state_t;

/* Shift Register Structure */
typedef void (*shift_register_io_set_t)(shift_register_output_state_t state);

struct shift_register
{
    uint8_t  data;
    uint8_t  latched_output;
    uint8_t  latch_enable;
    uint8_t  clear_data;

    struct
    {
        shift_register_io_set_t    set_ser;   // first stage of shift register
        shift_register_io_set_t    set_srclr; // clear shift register data
        shift_register_io_set_t    set_oe;    // output enable
        shift_register_io_set_t    set_srclk;
        shift_register_io_set_t    set_rclk;
    }digital_input;
};
typedef struct shift_register shift_register_cblk_t;

void shift_register_shift_in(shift_register_cblk_t *cblk,uint8_t shift_count, shift_register_output_state_t binary_value);
void shift_register_set_output(shift_register_cblk_t *cblk, uint8_t output_num, shift_register_output_state_t output_pin_state);
void shift_register_clear_register(shift_register_cblk_t *cblk);
void shift_register_latch_data_to_output(shift_register_cblk_t *cblk);
void shift_register_init(shift_register_cblk_t *cblk);

/* Shift Register Definition Template */
#endif /* SHIFT_REGISTER_H_ */
