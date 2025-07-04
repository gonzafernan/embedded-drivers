/**
 * @file mock_hardware.cpp
 * @brief Mock implementation of the hardware functions for testing.
 */

#include "mock_hardware.hpp"
#include <stdint.h>

MockHardware *MockHardware::instance = nullptr;

extern "C" {

void gpio_write(void *context, uint8_t value) {
    if (MockHardware::instance) {
        MockHardware::instance->gpio_write(context, value);
    }
}

uint8_t gpio_read(void *context) {
    if (MockHardware::instance) {
        return MockHardware::instance->gpio_read(context);
    }
}

void gpio_toggle(void *context) {
    if (MockHardware::instance) {
        MockHardware::instance->gpio_toggle(context);
    }
}

int pwm_init(void *context) {
    if (MockHardware::instance) {
        return MockHardware::instance->pwm_init(context);
    }
}

void pwm_set_duty_cycle(void *context, float duty_cycle) {
    if (MockHardware::instance) {
        MockHardware::instance->pwm_set_duty_cycle(context, duty_cycle);
    }
}

}