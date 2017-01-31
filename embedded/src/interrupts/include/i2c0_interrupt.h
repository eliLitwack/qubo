/* Ross Baehr
   R@M 2017
   ross.baehr@gmail.com
*/

#ifndef _I2C0_INTERRUPT_H_
#define _I2C0_INTERRUPT_H_

// Change this to the i2c device to use
#define I2C_DEVICE I2C0_BASE

#include <stdbool.h>
#include <stdint.h>

#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <driverlib/rom.h>
#include <driverlib/gpio.h>
#include <driverlib/interrupt.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/i2c.h>

#include "include/i2c0_globals.h"

uint32_t i2c0_address;

uint8_t *i2c0_buffer;

uint32_t i2c0_count;

volatile uint16_t i2c0_int_state;

void I2C0IntHandler(void);

#endif