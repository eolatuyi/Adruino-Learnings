/*
 * pwm_driver_testbed.c
 *
 *  Created on: Nov 5, 2017
 *      Author: ebenezer
 */
#include <avr/io.h>
#include <stdint.h>
#include "pwm_driver.h"
#include "C:\\Users\\ebenezer\\workspace\\TestBed\\main_testbed.h"

#define LED_ON_PERIOD (500)

uint8_t pwm_driver_rgb_module_set(uint8_t arg_count, uint8_t* arg_list);
void    pwm_driver_rgb_module_init(void);

main_testbed_process_t main_testbed_process = (main_testbed_process_t)pwm_driver_rgb_module_set;
main_testbed_init_t    main_testbed_init = (main_testbed_init_t)pwm_driver_rgb_module_init;

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
