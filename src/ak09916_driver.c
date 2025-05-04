#include "ak09916_driver.h"
#include "i2c_port.h"

#define AK09916_I2C_ADDRESS 0x0C  // I2C address for the AK09916

// AK09916 register addresses
#define AK09916_WIA1 0x00  // Who Am I: Company ID
#define AK09916_WIA2 0x01  // Who Am I: Device ID
#define AK09916_ST1 0x10   // Status 1

// AK09916 register addresses offseets
#define AK09916_ST1_DRDY 0  // Data ready

int ak09916_init(ak09916_t* self, void* context, uint32_t timeout) {
    self->context = context;
    self->timeout = timeout;
    uint8_t slave_address = 0x00;
    slave_address |= (AK09916_I2C_ADDRESS << 1);  // Set the I2C address
    if (i2c_init(context, slave_address, timeout) < 0) {
        return -1;
    }
    return 0;
}

uint8_t ak09916_who_am_i(ak09916_t* self) {
    uint8_t who_am_i;
    uint8_t slave_address = 0x00;
    slave_address |= (AK09916_I2C_ADDRESS << 1);  // Set the I2C address
    slave_address |= (1 << 0);                    // Set the read bit
    if (i2c_blocking_read(self->context, slave_address, AK09916_WIA1, &who_am_i,
                          1, self->timeout) < 0) {
        return -1;
    }
    return who_am_i;
}
