/**
 * @file stm32f4_pwm_port.c
 * @brief STM32F4 PWM port implementation
 */

#include "stm32f4_pwm_port_config.h"
#include "stm32f4xx_hal.h"

int pwm_init(void *context) {
    stm32f4_pwm_port_t *pwm_port = (stm32f4_pwm_port_t *)context;
    HAL_StatusTypeDef status =
        HAL_TIM_PWM_Start(pwm_port->htim, pwm_port->channel);
    return (status == HAL_OK) ? 0 : -1;
}

void pwm_set_duty_cycle(void *context, float duty_cycle) {
    stm32f4_pwm_port_t *pwm_port = (stm32f4_pwm_port_t *)context;
    uint32_t period = __HAL_TIM_GET_AUTORELOAD(pwm_port->htim);
    uint32_t pulse = (uint32_t)((duty_cycle / 100.0f) * period);
    __HAL_TIM_SET_COMPARE(pwm_port->htim, pwm_port->channel, pulse);
}
