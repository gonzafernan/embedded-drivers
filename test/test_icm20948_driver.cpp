/**
 * @file test_icm20948_driver.cpp
 * @brief Unit tests for the ICM20948 driver.
 *
 * This file contains unit tests for the ICM20948 driver, which is used to
 * interface with the ICM20948 sensor.
 *
 */

#include <gtest/gtest.h>

extern "C" {
#include "i2c_port.h"
#include "icm20948_driver.h"
#include "mock_i2c_port.h"
}

TEST(ICM20948Test, InitSuccess) {
    icm20948_t icm20948;
    mock_i2c_context_t mock_context;
    uint32_t timeout = 1000;  // Example timeout

    int result = icm20948_init(&icm20948, (void *)&mock_context,
                               ICM20948_I2C_ADDRESS0, timeout);

    EXPECT_EQ(result, 0);
}
