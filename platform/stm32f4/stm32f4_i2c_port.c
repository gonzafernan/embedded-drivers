#include <stdint.h>

#include "i2c_port.h"
#include "stm32f4xx_hal.h"

int i2c_init(void *context, uint16_t device_address, uint32_t timeout) {
    I2C_HandleTypeDef *i2c_handle = (I2C_HandleTypeDef *)context;
    HAL_StatusTypeDef status;
    status = HAL_I2C_IsDeviceReady(i2c_handle, device_address, 3, timeout);
    return (status == HAL_OK) ? 0 : -1;
}

int i2c_blocking_read(void *context, uint16_t device_address,
                      uint16_t reg_addres, uint8_t *data, uint16_t length,
                      uint32_t timeout) {
    I2C_HandleTypeDef *i2c_handle = (I2C_HandleTypeDef *)context;
    HAL_StatusTypeDef status;
    status = HAL_I2C_Mem_Read(i2c_handle, device_address, reg_addres,
                              I2C_MEMADD_SIZE_8BIT, data, length, timeout);
    return (status == HAL_OK) ? 0 : -1;
}

int i2c_blocking_write(void *context, uint16_t device_address,
                       uint16_t reg_addres, uint8_t *data, uint16_t length,
                       uint32_t timeout) {
    I2C_HandleTypeDef *i2c_handle = (I2C_HandleTypeDef *)context;
    HAL_StatusTypeDef status;
    status = HAL_I2C_Mem_Write(i2c_handle, device_address, reg_addres,
                               I2C_MEMADD_SIZE_8BIT, data, length, timeout);
    return (status == HAL_OK) ? 0 : -1;
}