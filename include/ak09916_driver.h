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

// AK09916 Operation modes.
typedef enum {
    AK09916_MODE_POWER_DOWN = 0b00000,
    AK09916_MODE_SINGLE_MEASUREMENT = 0b00001,
    AK09916_MODE_CONTINUOUS_MEASUREMENT_1 = 0b00010,
    AK09916_MODE_CONTINUOUS_MEASUREMENT_2 = 0b00100,
    AK09916_MODE_CONTINUOUS_MEASUREMENT_3 = 0b00110,
    AK09916_MODE_CONTINUOUS_MEASUREMENT_4 = 0b01000,
    AK09916_MODE_SELF_TEST = 0b10000,
} ak09916_mode_t;

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
 * @brief Set the operation mode of the AK09916 magnetometer.
 *
 * This function sets the operation mode of the AK09916 magnetometer.
 *
 * @param self Pointer to the AK09916 structure.
 * @param mode The desired operation mode.
 * @return 0 on success, -1 on failure.
 */
int ak09916_set_mode(ak09916_t* self, ak09916_mode_t mode);

/**
 * @brief Read the WHO_AM_I register of the AK09916 magnetometer.
 * @param self Pointer to the AK09916 structure.
 * @return The value of the WHO_AM_I register.
 */
uint8_t ak09916_who_am_i(ak09916_t* self);

/**
 * @brief Read magnetometer data from the AK09916 magnetometer.
 *
 * This function reads the magnetometer data from the AK09916 magnetometer.
 *
 * @param self Pointer to the AK09916 structure.
 * @param mag_x Pointer to store the X-axis magnetometer data.
 * @param mag_y Pointer to store the Y-axis magnetometer data.
 * @param mag_z Pointer to store the Z-axis magnetometer data.
 * @return 0 on success, -1 on failure.
 */
int ak09916_read_magnetometer(ak09916_t* self, int16_t* mag_x, int16_t* mag_y,
                              int16_t* mag_z);

/**
 * @brief Convert raw magnetometer data to float.
 * @param raw_value The raw magnetometer data.
 * @return The converted float value.
 */
float ak09916_raw_to_float(int16_t raw_value);

#ifdef __cplusplus
}
#endif

#endif  // AK09916_DRIVER_H