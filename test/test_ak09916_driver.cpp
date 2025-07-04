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
}

#include "mock_hardware.hpp"

constexpr float MEASUREMENT_RANGE = 4912.0f;
constexpr float REPRESENTATION_RANGE = 32752.0f;
constexpr float SCALE = MEASUREMENT_RANGE / REPRESENTATION_RANGE;

constexpr float EPSILON = 1e-3f;

TEST(AK09916Test, InitSuccess) {
    ak09916_t ak09916;
    mock_i2c_context_t mock_context;
    uint32_t timeout = 1000;  // Example timeout

    int result = ak09916_init(&ak09916, (void *)&mock_context, timeout);

    EXPECT_EQ(result, 0);
}

TEST(AK09916SetModeTest, SetModeSuccess) {
    ak09916_t ak09916;
    mock_i2c_context_t mock_context;
    ak09916_init(&ak09916, (void *)&mock_context, 1000);

    int result =
        ak09916_set_mode(&ak09916, AK09916_MODE_CONTINUOUS_MEASUREMENT_1);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(mock_context.memory[0x31], AK09916_MODE_CONTINUOUS_MEASUREMENT_1);
}

TEST(AK09916RawToFloatTest, ConvertsZeroCorrectly) {
    float result = ak09916_raw_to_float(0);
    EXPECT_NEAR(result, 0.0f, EPSILON);
}

TEST(AK09916RawToFloatTest, ConvertsPositiveRawValue) {
    int16_t raw = 16376;  // Midpoint
    float expected = raw * SCALE;
    float result = ak09916_raw_to_float(raw);
    EXPECT_NEAR(result, expected, EPSILON);
}

TEST(AK09916RawToFloatTest, ConvertsNegativeRawValue) {
    int16_t raw = -16376;
    float expected = raw * SCALE;
    float result = ak09916_raw_to_float(raw);
    EXPECT_NEAR(result, expected, EPSILON);
}

TEST(AK09916RawToFloatTest, ConvertsMaxPositiveValue) {
    int16_t raw = 32752;
    float result = ak09916_raw_to_float(raw);
    EXPECT_NEAR(result, MEASUREMENT_RANGE, EPSILON);
}

TEST(AK09916RawToFloatTest, ConvertsMaxNegativeValue) {
    int16_t raw = -32752;
    float result = ak09916_raw_to_float(raw);
    EXPECT_NEAR(result, -MEASUREMENT_RANGE, EPSILON);
}