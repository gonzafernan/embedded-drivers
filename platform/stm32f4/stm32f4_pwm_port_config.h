/**
 * @file stm32f4_pwm_port_config.h
 * @brief Configuration header for STM32F4 PWM port.
 */

#ifndef STM32F4_PWM_PORT_CONFIG_H
#define STM32F4_PWM_PORT_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "stm32f4xx_hal.h"

typedef struct {
    TIM_HandleTypeDef *htim;
    uint32_t channel;
} stm32f4_pwm_port_t;

#ifdef __cplusplus
}
#endif

#endif  // STM32F4_PWM_PORT_CONFIG_H