/**
 * @file test_hbridge_driver.cpp
 * @brief Unit tests for the H-bridge driver.
 *
 * This file contains unit tests for the H-bridge driver, which is used to
 * control the direction and speed of a motor using PWM and GPIO outputs.
 *
 */

#include <gtest/gtest.h>

extern "C" {
#include "hbridge_driver.h"
}

#include "mock_hardware.hpp"

using ::testing::FloatEq;
using ::testing::InSequence;
using ::testing::Return;

// ----------------------------------------------------------------------------
// Helpers
// ----------------------------------------------------------------------------

// Helper to initialize H-bridge driver
static void init_hbridge(MockHardware &mock, hbridge_t &hbridge, void *pwm,
                         void *in1, void *in2) {
    EXPECT_CALL(mock, gpio_write(in1, 0));
    EXPECT_CALL(mock, gpio_write(in2, 0));
    EXPECT_CALL(mock, pwm_set_duty_cycle(pwm, FloatEq(0.0f)));
    EXPECT_CALL(mock, pwm_init(pwm)).WillOnce(Return(0));
    hbridge_init(&hbridge, pwm, in1, in2);
}

// ----------------------------------------------------------------------------
// Initialization
// ----------------------------------------------------------------------------

TEST(HBridgeTest, InitSuccess) {
    MockHardware mock;
    MockHardware::instance = &mock;

    void *pwm = (void *)1;
    void *in1 = (void *)2;
    void *in2 = (void *)3;

    EXPECT_CALL(mock, gpio_write(in1, 0));
    EXPECT_CALL(mock, gpio_write(in2, 0));
    EXPECT_CALL(mock, pwm_set_duty_cycle(pwm, FloatEq(0.0f)));
    EXPECT_CALL(mock, pwm_init(pwm)).WillOnce(Return(0));

    hbridge_t hbridge;
    EXPECT_EQ(hbridge_init(&hbridge, pwm, in1, in2), 0);

    MockHardware::instance = nullptr;
}

TEST(HBridgeTest, InitFailure) {
    MockHardware mock;
    MockHardware::instance = &mock;

    void *pwm = (void *)1;
    void *in1 = (void *)2;
    void *in2 = (void *)3;

    EXPECT_CALL(mock, gpio_write(in1, 0));
    EXPECT_CALL(mock, gpio_write(in2, 0));
    EXPECT_CALL(mock, pwm_set_duty_cycle(pwm, FloatEq(0.0f)));
    EXPECT_CALL(mock, pwm_init(pwm)).WillOnce(Return(-1));

    hbridge_t hbridge;
    EXPECT_EQ(hbridge_init(&hbridge, pwm, in1, in2), -1);

    MockHardware::instance = nullptr;
}

// ----------------------------------------------------------------------------
// Brake
// ----------------------------------------------------------------------------

// TB6612FNG short brake: IN1=1, IN2=1
TEST(HBridgeTest, BrakeSetsBothPinsHigh) {
    MockHardware mock;
    MockHardware::instance = &mock;

    void *pwm = (void *)1;
    void *in1 = (void *)2;
    void *in2 = (void *)3;

    hbridge_t hbridge;
    init_hbridge(mock, hbridge, pwm, in1, in2);

    EXPECT_CALL(mock, gpio_write(in1, 1));
    EXPECT_CALL(mock, gpio_write(in2, 1));

    hbridge_brake(&hbridge);

    MockHardware::instance = nullptr;
}

// ----------------------------------------------------------------------------
// hbridge_set_output_signed
// ----------------------------------------------------------------------------

TEST(HBridgeTest, SetOutputSignedForward) {
    MockHardware mock;
    MockHardware::instance = &mock;

    void *pwm = (void *)1;
    void *in1 = (void *)2;
    void *in2 = (void *)3;

    hbridge_t hbridge;
    init_hbridge(mock, hbridge, pwm, in1, in2);

    // Positive cmd: IN1=1, IN2=0, duty = cmd
    EXPECT_CALL(mock, gpio_write(in1, 1));
    EXPECT_CALL(mock, gpio_write(in2, 0));
    EXPECT_CALL(mock, pwm_set_duty_cycle(pwm, FloatEq(50.0f)));

    hbridge_set_output_signed(&hbridge, 50.0f);

    MockHardware::instance = nullptr;
}

TEST(HBridgeTest, SetOutputSignedReverse) {
    MockHardware mock;
    MockHardware::instance = &mock;

    void *pwm = (void *)1;
    void *in1 = (void *)2;
    void *in2 = (void *)3;

    hbridge_t hbridge;
    init_hbridge(mock, hbridge, pwm, in1, in2);

    // Negative cmd: IN1=0, IN2=1, duty = |cmd|
    EXPECT_CALL(mock, gpio_write(in1, 0));
    EXPECT_CALL(mock, gpio_write(in2, 1));
    EXPECT_CALL(mock, pwm_set_duty_cycle(pwm, FloatEq(25.0f)));

    hbridge_set_output_signed(&hbridge, -25.0f);

    MockHardware::instance = nullptr;
}

TEST(HBridgeTest, SetOutputSignedCoastOnNearZero) {
    MockHardware mock;
    MockHardware::instance = &mock;

    void *pwm = (void *)1;
    void *in1 = (void *)2;
    void *in2 = (void *)3;

    hbridge_t hbridge;
    init_hbridge(mock, hbridge, pwm, in1, in2);

    // |cmd| < 0.5 -> coast: IN1=0, IN2=0, pwm=100
    EXPECT_CALL(mock, gpio_write(in1, 0));
    EXPECT_CALL(mock, gpio_write(in2, 0));
    EXPECT_CALL(mock, pwm_set_duty_cycle(pwm, FloatEq(100.0f)));

    hbridge_set_output_signed(&hbridge, 0.3f);

    MockHardware::instance = nullptr;
}

TEST(HBridgeTest, SetOutputSignedDirectionChange) {
    MockHardware mock;
    MockHardware::instance = &mock;

    void *pwm = (void *)1;
    void *in1 = (void *)2;
    void *in2 = (void *)3;

    hbridge_t hbridge;
    init_hbridge(mock, hbridge, pwm, in1, in2);

    // First command: forward
    EXPECT_CALL(mock, gpio_write(in1, 1));
    EXPECT_CALL(mock, gpio_write(in2, 0));
    EXPECT_CALL(mock, pwm_set_duty_cycle(pwm, FloatEq(50.0f)));
    hbridge_set_output_signed(&hbridge, 50.0f);

    // Direction change forward -> reverse:
    // 1. pwm_set_duty_cycle(0)
    // 2. coast: IN1=0, IN2=0, pwm=100
    // 3. set_dir_reverse: IN1=0, IN2=1
    // 4. pwm_set_duty_cycle(duty)
    {
        InSequence sequence;
        EXPECT_CALL(mock, pwm_set_duty_cycle(pwm, FloatEq(0.0f)));
        EXPECT_CALL(mock, gpio_write(in1, 0));
        EXPECT_CALL(mock, gpio_write(in2, 0));
        EXPECT_CALL(mock, pwm_set_duty_cycle(pwm, FloatEq(100.0f)));
        EXPECT_CALL(mock, gpio_write(in1, 0));
        EXPECT_CALL(mock, gpio_write(in2, 1));
        EXPECT_CALL(mock, pwm_set_duty_cycle(pwm, FloatEq(25.0f)));
    }
    hbridge_set_output_signed(&hbridge, -25.0f);

    MockHardware::instance = nullptr;
}
