/**
 * @file gpio_port.h
 * @brief Header file for the GPIO port operations.
 */

#ifndef GPIO_PORT_H
#define GPIO_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief Writes a value to the GPIO pin.
 * @param context Pointer to the GPIO port context.
 * @param value The value to write to the GPIO pin (0 or 1).
 */
void gpio_write(void *context, uint8_t value);

/**
 * @brief Reads the value from the GPIO pin.
 * @param context Pointer to the GPIO port context.
 * @return The value read from the GPIO pin (0 or 1).
 */
uint8_t gpio_read(void *context);

/**
 * @brief Toggles the state of the GPIO pin.
 * @param context Pointer to the GPIO port context.
 */
void gpio_toggle(void *context);

#ifdef __cplusplus
}
#endif

#endif  // GPIO_PORT_H