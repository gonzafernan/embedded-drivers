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
using ::testing::Return;

TEST(HBridgeTest, InitSuccess) {
    MockHardware mock;
    MockHardware::instance = &mock;

    void* pwm = (void*)1;
    void* in1 = (void*)2;
    void* in2 = (void*)3;

    EXPECT_CALL(mock, gpio_write(in1, 0));
    EXPECT_CALL(mock, gpio_write(in2, 0));
    EXPECT_CALL(mock, pwm_set_duty_cycle(pwm, FloatEq(0.0f)));
    EXPECT_CALL(mock, pwm_init(pwm)).WillOnce(Return(0));

    hbridge_t hbridge;
    int ret = hbridge_init(&hbridge, pwm, in1, in2);

    EXPECT_EQ(ret, 0);
    MockHardware::instance = nullptr;
}

TEST(HBridgeTest, InitFailure) {
    MockHardware mock;
    MockHardware::instance = &mock;

    void* pwm = (void*)1;
    void* in1 = (void*)2;
    void* in2 = (void*)3;

    EXPECT_CALL(mock, gpio_write(in1, 0));
    EXPECT_CALL(mock, gpio_write(in2, 0));
    EXPECT_CALL(mock, pwm_set_duty_cycle(pwm, FloatEq(0.0f)));
    EXPECT_CALL(mock, pwm_init(pwm)).WillOnce(Return(-1));

    hbridge_t hbridge;
    int ret = hbridge_init(&hbridge, pwm, in1, in2);

    EXPECT_EQ(ret, -1);
    MockHardware::instance = nullptr;
}

TEST(HBridgeTest, ShortBreak) {
    MockHardware mock;
    MockHardware::instance = &mock;

    void* pwm = (void*)1;
    void* in1 = (void*)2;
    void* in2 = (void*)3;

    hbridge_t hbridge;
    int ret = hbridge_init(&hbridge, pwm, in1, in2);
    EXPECT_EQ(ret, 0);

    EXPECT_CALL(mock, gpio_write(in1, 0));
    EXPECT_CALL(mock, gpio_write(in2, 0));

    hbridge_short_brake(&hbridge);

    MockHardware::instance = nullptr;
}

TEST(HBridgeTest, SetOutput) {
    MockHardware mock;
    MockHardware::instance = &mock;

    void* pwm = (void*)1;
    void* in1 = (void*)2;
    void* in2 = (void*)3;

    hbridge_t hbridge;
    int ret = hbridge_init(&hbridge, pwm, in1, in2);
    EXPECT_EQ(ret, 0);

    EXPECT_CALL(mock, gpio_write(in1, 1));
    EXPECT_CALL(mock, gpio_write(in2, 0));
    EXPECT_CALL(mock, pwm_set_duty_cycle(pwm, FloatEq(50.0f)));

    hbridge_set_output(&hbridge, 0, 50.0f);

    EXPECT_CALL(mock, gpio_write(in1, 0));
    EXPECT_CALL(mock, gpio_write(in2, 1));
    EXPECT_CALL(mock, pwm_set_duty_cycle(pwm, FloatEq(25.0f)));

    hbridge_set_output(&hbridge, 1, 25.0f);

    EXPECT_CALL(mock, gpio_write(in1, 0));
    EXPECT_CALL(mock, gpio_write(in2, 0));
    EXPECT_CALL(mock, pwm_set_duty_cycle(pwm, FloatEq(75.0f)));

    hbridge_set_output(&hbridge, 2, 75.0f);

    MockHardware::instance = nullptr;
}