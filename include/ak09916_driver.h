/**
 * @file ak09916_driver.h
 * @brief AK09916 driver header file
 *
 * This file defines the public interface for interacting with the AK09916
 * magnetometer.
 *
 */

#ifndef AK09916_DRIVER_H
#define AK09916_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct {
    void* context;     // Context for the I2C port
    uint32_t timeout;  // Timeout for I2C operations
} ak09916_t;

/**
 * @brief Initialize the AK09916 magnetometer.
 *
 * This function initializes the AK09916 magnetometer. It must be called before
 * any other functions in this driver.
 *
 * @param self Pointer to the AK09916 structure.
 * @param context Pointer to the I2C port context.
 * @param timeout Timeout for I2C operations.
 * @return 0 on success, -1 on failure.
 */
int ak09916_init(ak09916_t* self, void* context, uint32_t timeout);

/**
 * @brief Read the WHO_AM_I register of the AK09916 magnetometer.
 * @param self Pointer to the AK09916 structure.
 * @return The value of the WHO_AM_I register.
 */
uint8_t ak09916_who_am_i(ak09916_t* self);

#ifdef __cplusplus
}
#endif

#endif  // AK09916_DRIVER_H