/**
 * @file test_ak09916_driver.cpp
 * @brief Unit tests for the AK09916 driver.
 *
 * This file contains unit tests for the AK09916 driver, which is used to
 * interface with the AK09916 sensor.
 *
 */

#include <gtest/gtest.h>

extern "C" {
#include "ak09916_driver.h"
#include "i2c_port.h"
#include "mock_i2c_port.h"
}

TEST(AK09916Test, InitSuccess) {
    ak09916_t ak09916;
    mock_i2c_context_t mock_context;
    uint32_t timeout = 1000;  // Example timeout

    int result = ak09916_init(&ak09916, (void *)&mock_context, timeout);

    EXPECT_EQ(result, 0);
}
