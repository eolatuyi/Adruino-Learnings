/*
 * pwm_driver_testbed.c
 *
 *  Created on: Nov 5, 2017
 *      Author: ebenezer
 */
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "pwm_driver.h"

uint8_t pwm_driver_rgb_module_set(uint8_t arg_count, uint8_t* arg_list);
void    pwm_driver_rgb_module_init(void);

int main()
{
    pwm_driver_rgb_module_init();
    while(1)
    {
        uint8_t levels[3] = {2, 0, 255}; // R, G, B
        pwm_driver_rgb_module_set(3, levels);
    }

    return 0;
}

// FIXME: Application specific testbed needs header: expected number of arg = 3 where
// arg[0] = 8 bit level for red led component
// arg[1] = 8 bit level for green led component
// arg[2] = 8 bit level for blue led component
uint8_t pwm_driver_rgb_module_set(uint8_t arg_count, uint8_t* arg_list)
{
    if (arg_count != 3)
    {
        return 1;
    }
    else
    {
        OCR2A = arg_list[0]; // R
        OCR0A = arg_list[1]; // G
        OCR0B = arg_list[2]; // B
    }
    return 0;
}

void pwm_driver_rgb_module_init(void)
{
    pwm_driver_init();
}
