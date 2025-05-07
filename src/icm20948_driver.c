#include <stdbool.h>
#include <stdint.h>

#include "i2c_port.h"
#include "icm20948_driver.h"

// ICM-20948 user bank 0 register map
#define ICM20948_USER_BANK_0_WHO_AM_I 0x00
#define ICM20948_USER_BANK_0_USER_CTRL 0x03
#define ICM20948_USER_BANK_0_LP_CONFIG 0x05
#define ICM20948_USER_BANK_0_PWR_MGMT_1 0x06
#define ICM20948_USER_BANK_0_PWR_MGMT_2 0x07
#define ICM20948_USER_BANK_0_INT_PIN_CFG 0x0F
#define ICM20948_USER_BANK_0_ACCEL_XOUT_H 0x2D
#define ICM20948_USER_BANK_0_GYRO_XOUT_H 0x33

// ICM-20948 user bank 3 register map
#define ICM20948_USER_BANK_3_I2C_SLV0_CTRL 0x05

// ICM-20948 register bank select (common to all banks)
#define ICM20948_REG_BANK_SEL 0x7F

#define ICM20948_WHO_AM_I 0xEA

int icm20948_init(icm20948_t* self, void* context, uint16_t device_address,
                  uint32_t timeout) {
    self->context = context;
    self->device_address = device_address;
    self->timeout = timeout;
    if (i2c_init(context, device_address, timeout) < 0) {
        return -1;
    }
    uint8_t pwr_mgmt_1 = 0x01;
    if (i2c_blocking_write(context, device_address,
                           ICM20948_USER_BANK_0_PWR_MGMT_1, &pwr_mgmt_1, 1,
                           timeout) < 0) {  // Set clock source
        return -1;
    }
    return 0;
}

bool icm20948_is_device_available(icm20948_t* self) {
    uint8_t who_am_i;
    if (i2c_blocking_read(self->context, self->device_address,
                          ICM20948_USER_BANK_0_WHO_AM_I, &who_am_i, 1,
                          self->timeout) < 0) {
        return false;
    }
    return who_am_i == ICM20948_WHO_AM_I;
}

int icm20948_read_accelerometer(icm20948_t* self, int16_t* accel_x,
                                int16_t* accel_y, int16_t* accel_z) {
    uint8_t data[6];
    if (i2c_blocking_read(self->context, self->device_address,
                          ICM20948_USER_BANK_0_ACCEL_XOUT_H, data, 6,
                          self->timeout)) {
        return -1;
    }
    *accel_x = (int16_t)((data[0] << 8) | data[1]);
    *accel_y = (int16_t)((data[2] << 8) | data[3]);
    *accel_z = (int16_t)((data[4] << 8) | data[5]);
    return 0;
}

int icm20948_read_gyroscope(icm20948_t* self, int16_t* gyro_x, int16_t* gyro_y,
                            int16_t* gyro_z) {
    uint8_t data[6];
    if (i2c_blocking_read(self->context, self->device_address,
                          ICM20948_USER_BANK_0_GYRO_XOUT_H, data, 6,
                          self->timeout)) {
        return -1;
    }
    *gyro_x = (int16_t)((data[0] << 8) | data[1]);
    *gyro_y = (int16_t)((data[2] << 8) | data[3]);
    *gyro_z = (int16_t)((data[4] << 8) | data[5]);
    return 0;
}
