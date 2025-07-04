/**
 * @file mock_hardware.hpp
 * @brief Mock implementation of the hardware functions for testing.
 *
 * This file provides mock implementations of the hardware functions for unit
 * testing purposes.
 *
 */

#ifndef MOCK_HARDWARE_HPP
#define MOCK_HARDWARE_HPP

#include <gmock/gmock.h>

class MockHardware {
   public:
    // gpio
    MOCK_METHOD(void, gpio_write, (void *context, uint8_t value), ());
    MOCK_METHOD(uint8_t, gpio_read, (void *context), ());
    MOCK_METHOD(void, gpio_toggle, (void *context), ());

    // pwm
    MOCK_METHOD(int, pwm_init, (void *context), ());
    MOCK_METHOD(void, pwm_set_duty_cycle, (void *context, float duty_cycle),
                ());

    static MockHardware *instance;
};

typedef struct {
    uint8_t memory[256];
} mock_i2c_context_t;

#endif  // MOCK_HARDWARE_HPP
