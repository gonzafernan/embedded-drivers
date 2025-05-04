/**
 * @file mock_i2c_port.c
 * @brief Mock implementation of the I2C port functions for testing.
 *
 * This file provides mock implementations of the I2C port functions for unit
 * testing purposes. It simulates the behavior of the I2C driver without
 * requiring actual hardware.
 *
 */

#include "i2c_port.h"

int i2c_init(void *context, uint16_t device_address, uint32_t timeout) {
    // Mock implementation: always return success
    return 0;
}

int i2c_blocking_read(void *context, uint16_t device_address,
                      uint16_t reg_addres, uint8_t *data, uint16_t length,
                      uint32_t timeout) {
    // Mock implementation: simulate reading data
    for (uint16_t i = 0; i < length; i++) {
        data[i] = (uint8_t)(reg_addres + i);  // Simulate data read
    }
    return 0;  // Success
}

int i2c_blocking_write(void *context, uint16_t device_address,
                       uint16_t reg_addres, uint8_t *data, uint16_t length,
                       uint32_t timeout) {
    // Mock implementation: simulate writing data
    return 0;  // Success
}
