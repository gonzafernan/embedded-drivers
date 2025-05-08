/**
 * @file i2c_port.h
 * @brief I2C port header file
 *
 * This file defines the public interface for the I2C driver porting layer.
 *
 */

#ifndef I2C_PORT_H
#define I2C_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief Initialize the I2C port.
 * @param context Pointer to the I2C port context.
 * @param device_address I2C device address.
 * @param timeout Timeout for I2C operations.
 * @return 0 on success, -1 on failure.
 */
int i2c_init(void *context, uint16_t device_address, uint32_t timeout);

/**
 * @brief Read data from the I2C device.
 * @param context Pointer to the I2C port context.
 * @param device_address I2C device address.
 * @param reg_addres Register address to read from.
 * @param data Pointer to the buffer to store the read data.
 * @param length Number of bytes to read.
 * @param timeout Timeout for I2C operations.
 * @return 0 on success, -1 on failure.
 */
int i2c_blocking_read(void *context, uint16_t device_address,
                      uint16_t reg_addres, uint8_t *data, uint16_t length,
                      uint32_t timeout);

/**
 * @brief Write data to the I2C device.
 * @param context Pointer to the I2C port context.
 * @param device_address I2C device address.
 * @param reg_addres Register address to write to.
 * @param data Pointer to the buffer containing the data to write.
 * @param length Number of bytes to write.
 * @param timeout Timeout for I2C operations.
 * @return 0 on success, -1 on failure.
 */
int i2c_blocking_write(void *context, uint16_t device_address,
                       uint16_t reg_addres, uint8_t *data, uint16_t length,
                       uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif  // I2C_PORT_H
