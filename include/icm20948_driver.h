/**
 * @file icm20948_driver.h
 * @brief ICM20948 driver header file
 *
 * This file defines the public interface for interacting with the ICM-20948
 * IMU. It must be used in conjunction with a platform-specific implementation
 * of the low-level I2C access functions defined in icm20948_port.h.
 *
 */

#ifndef ICM20948_DRIVER_H
#define ICM20948_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define ICM20948_I2C_ADDRESS0 0b11010000  // I2C address 0 for the ICM-20948
#define ICM20948_I2C_ADDRESS1 0b11010010  // I2C address 1 for the ICM-20948

typedef struct {
    void* context;            // Context for the I2C port
    uint16_t device_address;  // I2C device address
    uint32_t timeout;         // Timeout for I2C operations
} icm20948_t;

/**
 * @brief Initialize the ICM-20948 IMU.
 *
 * This function initializes the ICM-20948 IMU. It must be called before any
 * other functions in this driver.
 *
 * @param context Pointer to the I2C port context.
 * @param device_address I2C device address of the IMU.
 * @param timeout Timeout for I2C operations.
 */
int icm20948_init(icm20948_t* self, void* context, uint16_t device_address,
                  uint32_t timeout);

/**
 * @brief Check if the ICM-20948 IMU is available.
 *
 * This function checks if the ICM-20948 IMU is available by reading the
 * WHO_AM_I register. If the value matches the expected value, the IMU is
 * considered available.
 *
 * @param self Pointer to the ICM-20948 IMU structure.
 * @return true if the IMU is available, false otherwise.
 */
bool icm20948_is_device_available(icm20948_t* self);

/**
 * @brief Read accelerometer data from the ICM-20948 IMU.
 *
 * This function reads the accelerometer data from the ICM-20948 IMU.
 *
 * @param accel_x Pointer to store the X-axis accelerometer data.
 * @param accel_y Pointer to store the Y-axis accelerometer data.
 * @param accel_z Pointer to store the Z-axis accelerometer data.
 * @return 0 on success, -1 on failure.
 */
int icm20948_read_accelerometer(icm20948_t* self, int16_t* accel_x,
                                int16_t* accel_y, int16_t* accel_z);

/**
 * @brief Read gyroscope data from the ICM-20948 IMU.
 *
 * This function reads the gyroscope data from the ICM-20948 IMU.
 *
 * @param gyro_x Pointer to store the X-axis gyroscope data.
 * @param gyro_y Pointer to store the Y-axis gyroscope data.
 * @param gyro_z Pointer to store the Z-axis gyroscope data.
 * @return 0 on success, -1 on failure.
 */
int icm20948_read_gyroscope(icm20948_t* self, int16_t* gyro_x, int16_t* gyro_y,
                            int16_t* gyro_z);

#ifdef __cplusplus
}
#endif

#endif  // ICM20948_DRIVER_H
