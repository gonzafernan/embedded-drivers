/**
 * @file ak09916_driver.c
 * @brief AK09916 driver implementation
 *
 * This file contains the implementation of the AK09916 magnetometer driver.
 *
 */

#include "ak09916_driver.h"
#include "i2c_port.h"

#define AK09916_I2C_ADDRESS 0x0C        // I2C address for the AK09916
#define AK09916_I2C_ADDRESS_RW_BIT 0    // Read/Write bit for I2C
#define AK09916_I2C_ADDRESS_ADDR_BIT 1  // Address bit for I2C

#define AK09916_I2C_RW_READ 1   // Read bit for I2C
#define AK09916_I2C_RW_WRITE 0  // Write bit for I2C

// AK09916 register addresses
#define AK09916_WIA1 0x00   // Who Am I: Company ID
#define AK09916_WIA2 0x01   // Who Am I: Device ID
#define AK09916_ST1 0x10    // Status 1
#define AK09916_HXL 0x11    // Magnetometer X-axis data high lower byte
#define AK09916_ST2 0x18    // Status 2
#define AK09916_CNTL2 0x31  // Control 2

// AK09916 register addresses bits
#define AK09916_ST1_DRDY 0     // Data ready
#define AK09916_CNTL2_MODE0 0  // Mode 0
#define AK09916_CNTL2_MODE1 1  // Mode 1
#define AK09916_CNTL2_MODE2 2  // Mode 2
#define AK09916_CNTL2_MODE3 3  // Mode 3
#define AK09916_CNTL2_MODE4 4  // Mode 4

#define AK09916_MEASUREMENT_RANGE 4912
#define AK09916_REPRESENTATION_RANGE 32752.0

int ak09916_init(ak09916_t* self, void* context, uint32_t timeout) {
    self->context = context;
    self->timeout = timeout;
    uint8_t slave_address = 0x00;
    slave_address |= (AK09916_I2C_ADDRESS << AK09916_I2C_ADDRESS_ADDR_BIT);
    slave_address |= (AK09916_I2C_RW_READ << AK09916_I2C_ADDRESS_RW_BIT);
    if (i2c_init(context, slave_address, timeout) < 0) {
        return -1;
    }
    uint8_t operation_mode = (1 << AK09916_CNTL2_MODE1);
    if (i2c_blocking_write(context, slave_address, AK09916_CNTL2,
                           &operation_mode, 1, timeout) < 0) {
        return -1;
    }
    return 0;
}

uint8_t ak09916_who_am_i(ak09916_t* self) {
    uint8_t who_am_i;
    uint8_t slave_address = 0x00;
    slave_address |= (AK09916_I2C_ADDRESS << AK09916_I2C_ADDRESS_ADDR_BIT);
    slave_address |= (AK09916_I2C_RW_READ << AK09916_I2C_ADDRESS_RW_BIT);
    if (i2c_blocking_read(self->context, slave_address, AK09916_WIA1, &who_am_i,
                          1, self->timeout) < 0) {
        return -1;
    }
    return who_am_i;
}

int ak09916_read_magnetometer(ak09916_t* self, int16_t* mag_x, int16_t* mag_y,
                              int16_t* mag_z) {
    uint8_t data[6];
    uint8_t slave_address = 0x00;
    slave_address |= (AK09916_I2C_ADDRESS << AK09916_I2C_ADDRESS_ADDR_BIT);
    slave_address |= (AK09916_I2C_RW_READ << AK09916_I2C_ADDRESS_RW_BIT);
    if (i2c_blocking_read(self->context, slave_address, AK09916_HXL, data, 6,
                          self->timeout) < 0) {
        return -1;
    }
    uint8_t st2;
    if (i2c_blocking_read(self->context, slave_address, AK09916_ST2, &st2, 1,
                          self->timeout) < 0) {
        return -1;
    }
    *mag_x = (int16_t)((data[1] << 8) | data[0]);
    *mag_y = (int16_t)((data[3] << 8) | data[2]);
    *mag_z = (int16_t)((data[5] << 8) | data[4]);
    return 0;
}

float ak09916_raw_to_float(int16_t raw_value) {
    return (double)raw_value * AK09916_MEASUREMENT_RANGE /
           AK09916_REPRESENTATION_RANGE;
}
