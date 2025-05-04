/**
 * @file mock_i2c_port.c
 * @brief Mock implementation of the I2C port functions for testing.
 *
 * This file provides mock implementations of the I2C port functions for unit
 * testing purposes.
 *
 */

#ifndef MOCK_I2C_PORT_H
#define MOCK_I2C_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct {
    uint8_t memory[256];
} mock_i2c_context_t;

#ifdef __cplusplus
}
#endif

#endif  // MOCK_I2C_PORT_H
