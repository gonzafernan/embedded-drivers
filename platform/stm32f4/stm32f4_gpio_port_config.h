/**
 * @file stm32f4_gpio_port_config.h
 * @brief Configuration file for STM32F4 GPIO port settings.
 */

#ifndef STM32F4_GPIO_PORT_CONFIG_H
#define STM32F4_GPIO_PORT_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "stm32f4xx_hal.h"

typedef struct {
    GPIO_TypeDef *port;  // Pointer to the GPIO port
    uint16_t pin;        // GPIO pin number
} stm32f4_gpio_port_t;

#ifdef __cplusplus
}
#endif

#endif  // STM32F4_GPIO_PORT_CONFIG_H