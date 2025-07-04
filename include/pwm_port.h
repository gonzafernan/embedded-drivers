/**
 * @file pwm_port.h
 * @brief PWM port header file
 */

#ifndef PWM_PORT_H
#define PWM_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the PWM port.
 *
 * This function initializes the PWM port with the provided context.
 *
 * @param context Pointer to the context for the PWM port.
 * @return 0 on success, -1 on failure.
 */
int pwm_init(void *context);

/**
 * @brief Set the duty cycle for the PWM port.
 *
 * This function sets the duty cycle for the PWM port.
 *
 * @param context Pointer to the context for the PWM port.
 * @param duty_cycle Duty cycle value (0.0 to 100.0).
 */
void pwm_set_duty_cycle(void *context, float duty_cycle);

#ifdef __cplusplus
}
#endif

#endif  // PWM_PORT_H