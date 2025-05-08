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
#define ICM20948_USER_BANK_0_INT_ENABLE_1 0x11
#define ICM20948_USER_BANK_0_INT_STATUS 0x19
#define ICM20948_USER_BANK_0_INT_STATUS_1 0x1A
#define ICM20948_USER_BANK_0_ACCEL_XOUT_H 0x2D
#define ICM20948_USER_BANK_0_GYRO_XOUT_H 0x33

// ICM-20948 user bank 3 register map
#define ICM20948_USER_BANK_3_I2C_SLV0_ADDR 0x03
#define ICM20948_USER_BANK_3_I2C_SLV0_REG 0x04
#define ICM20948_USER_BANK_3_I2C_SLV0_CTRL 0x05

// ICM-20948 register bank select (common to all banks)
#define ICM20948_REG_BANK_SEL 0x7F

#define ICM20948_WHO_AM_I 0xEA

// ICM-20948 user bank register offsets
#define ICM20948_USER_CTRL_I2C_MST_RST 1
#define ICM20948_USER_CTRL_SRAM_RST 2
#define ICM20948_USER_CTRL_DMP_RST 3
#define ICM20948_USER_CTRL_I2C_IF_DIS 4
#define ICM20948_USER_CTRL_I2C_MST_EN 5
#define ICM20948_USER_CTRL_FIFO_EN 6
#define ICM20948_USER_CTRL_DMP_EN 7

#define ICM20948_INT_PIN_CFG_BYPASS_EN 1
#define ICM20948_INT_PIN_CFG_INT1_LATCH_EN 5
#define ICM20948_INT_PIN_CFG_INT1_OPEN 6
#define ICM20948_INT_PIN_CFG_INT1_ACTL 7

#define ICM20948_INT_STATUS_I2C_MST_INT 0
#define ICM20948_INT_STATUS_DMP_INT1 1
#define ICM20948_INT_STATUS_PLL_RDY_INT 2
#define ICM20948_INT_STATUS_WOM_INT 3

#define ICM20948_I2C_SLV0_ADDR_I2C_ID_0 0
#define ICM20948_I2C_SLV0_ADDR_I2C_SLV0_RNW 7

#define ICM20948_I2C_SLV0_CTRL_I2C_SLV0_LENG 0
#define ICM20948_I2C_SLV0_CTRL_I2C_SLV0_GRP 4
#define ICM20948_I2C_SLV0_CTRL_I2C_SLV0_REG_DIS 5
#define ICM20948_I2C_SLV0_CTRL_I2C_SLV0_BYTE_SW 6
#define ICM20948_I2C_SLV0_CTRL_I2C_SLV0_EN 7

#define ICM20948_REG_BANK_SEL_USER_BANK 4

// ICM-20948 user bank register masks
#define ICM20948_MSK_INT_STATUS_1_RAW_DATA_0_RDY_INT 0x01
#define ICM20948_MSK_INT_STATUS_2_FIFO_OVERFLOW_INT 0x1F
#define ICM20948_MSK_INT_STATUS_3_FIFO_WM_INT 0x1F

int icm20948_init(icm20948_t* self, void* context, uint16_t device_address,
                  uint32_t timeout) {
    self->context = context;
    self->device_address = device_address;
    self->timeout = timeout;
    if (i2c_init(context, device_address, timeout) < 0) {
        return -1;
    }
    uint8_t user_ctrl = 0x00;
    user_ctrl &= ~(1 << ICM20948_USER_CTRL_I2C_MST_EN);  // Disable I2C master
    if (i2c_blocking_write(context, device_address,
                           ICM20948_USER_BANK_0_USER_CTRL, &user_ctrl, 1,
                           timeout) < 0) {
        return -1;
    }
    uint8_t int_pin_cfg = 0x00;
    int_pin_cfg |= (1 << ICM20948_INT_PIN_CFG_BYPASS_EN);  // Set bypass mode
    int_pin_cfg |= (1 << ICM20948_INT_PIN_CFG_INT1_ACTL);  // Int active low
    int_pin_cfg |=
        (1 << ICM20948_INT_PIN_CFG_INT1_LATCH_EN);  // Int latch enable
    if (i2c_blocking_write(context, device_address,
                           ICM20948_USER_BANK_0_INT_PIN_CFG, &int_pin_cfg, 1,
                           timeout) < 0) {  // Set bypass mode
        return -1;
    }
    uint8_t pwr_mgmt_1 = 0x01;
    if (i2c_blocking_write(context, device_address,
                           ICM20948_USER_BANK_0_PWR_MGMT_1, &pwr_mgmt_1, 1,
                           timeout) < 0) {  // Set clock source
        return -1;
    }
    uint8_t int_enable_1 = 0x00;
    int_enable_1 |= (1 << 0);
    if (i2c_blocking_write(context, device_address,
                           ICM20948_USER_BANK_0_INT_ENABLE_1, &int_enable_1, 1,
                           timeout) < 0) {  // Set int 1 enable
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

int icm20948_clear_int_status(icm20948_t* self,
                              icm20948_int_status_t* int_status) {
    uint8_t data[4];
    if (i2c_blocking_read(self->context, self->device_address,
                          ICM20948_USER_BANK_0_INT_STATUS, data, 4,
                          self->timeout) < 0) {
        return -1;
    }

    int_status->i2c_mst_int =
        (data[0] >> ICM20948_INT_STATUS_I2C_MST_INT) & 0x01;
    int_status->dmp_int1 = (data[0] >> ICM20948_INT_STATUS_DMP_INT1) & 0x01;
    int_status->pll_rdy_int =
        (data[0] >> ICM20948_INT_STATUS_PLL_RDY_INT) & 0x01;
    int_status->wom_int = (data[0] >> ICM20948_INT_STATUS_WOM_INT) & 0x01;

    int_status->raw_data_0_rdy_int =
        (data[1] & ICM20948_MSK_INT_STATUS_1_RAW_DATA_0_RDY_INT);
    int_status->fifo_overflow_int =
        (data[2] & ICM20948_MSK_INT_STATUS_2_FIFO_OVERFLOW_INT);
    int_status->fifo_wm_int = (data[3] & ICM20948_MSK_INT_STATUS_3_FIFO_WM_INT);
    return 0;
}
