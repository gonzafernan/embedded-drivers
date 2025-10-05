/**
 * @file stm32f4_pwm_port.c
 * @brief STM32F4 PWM port implementation
 */

#include "stm32f4_pwm_port_config.h"
#include "stm32f4xx_hal.h"

static inline float clamp01(float x) {
    return x < 0.f ? 0.f : (x > 100.f ? 100.f : x);
}

int pwm_init(void *context) {
    stm32f4_pwm_port_t *pwm_port = (stm32f4_pwm_port_t *)context;
    HAL_StatusTypeDef status =
        HAL_TIM_PWM_Start(pwm_port->htim, pwm_port->channel);
    return (status == HAL_OK) ? 0 : -1;
}

void pwm_set_duty_cycle(void *context, float duty_cycle) {
    stm32f4_pwm_port_t *pwm_port = (stm32f4_pwm_port_t *)context;
    float clamped_duty_cycle = clamp01(duty_cycle);
    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(pwm_port->htim);
    uint32_t ccr =
        (uint32_t)((clamped_duty_cycle * (arr + 1U)) / 100.0f + 0.5f);
    if (ccr > arr) ccr = arr;
    __HAL_TIM_SET_COMPARE(pwm_port->htim, pwm_port->channel, ccr);
}
