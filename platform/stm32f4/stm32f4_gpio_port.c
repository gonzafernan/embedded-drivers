/**
 * @file stm32f4_gpio_port.c
 * @brief GPIO port operations for STM32F4 platform.
 */

#include "stm32f4_gpio_port_config.h"
#include "stm32f4xx_hal.h"

void gpio_write(void *context, uint8_t value) {
    stm32f4_gpio_port_t *gpio_port = (stm32f4_gpio_port_t *)context;
    HAL_GPIO_WritePin(gpio_port->port, gpio_port->pin,
                      value ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

uint8_t gpio_read(void *context) {
    stm32f4_gpio_port_t *gpio_port = (stm32f4_gpio_port_t *)context;
    GPIO_PinState state = HAL_GPIO_ReadPin(gpio_port->port, gpio_port->pin);
    return (state == GPIO_PIN_SET) ? 1 : 0;
}

void gpio_toggle(void *context) {
    stm32f4_gpio_port_t *gpio_port = (stm32f4_gpio_port_t *)context;
    HAL_GPIO_TogglePin(gpio_port->port, gpio_port->pin);
}
