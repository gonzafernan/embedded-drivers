/**
 * @file hbridge_driver.h
 * @brief Header file for the H-bridge driver module.
 */

#ifndef HBRIDGE_DRIVER_H
#define HBRIDGE_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void *pwm_context;  // Context for PWM control
    void *in1_context;  // Context for IN1 gpio output
    void *in2_context;  // Context for IN2 gpio output
} hbridge_t;

/**
 * @brief Initialize the H-bridge driver.
 *
 * This function initializes the H-bridge driver with the provided contexts for
 * PWM and GPIO outputs.
 *
 * @param self Pointer to the H-bridge structure.
 * @param pwm_context Context for PWM control.
 * @param in1_context Context for IN1 gpio output.
 * @param in2_context Context for IN2 gpio output.
 * @return 0 on success, -1 on failure.
 */
int hbridge_init(hbridge_t *self, void *pwm_context, void *in1_context,
                 void *in2_context);

/**
 * @brief Apply a short brake to the H-bridge.
 * @param self Pointer to the H-bridge structure.
 */
void hbridge_short_brake(hbridge_t *self);

/**
 * @brief Set the output of the H-bridge.
 *
 * This function sets the output direction and duty cycle for the H-bridge.
 *
 * @param self Pointer to the H-bridge structure.
 * @param direction Direction of the motor (0 for forward, 1 for reverse, 2 for
 * stop).
 * @param duty_cycle Duty cycle percentage (0.0 to 100.0).
 */
void hbridge_set_output(hbridge_t *self, uint8_t direction, float duty_cycle);

#ifdef __cplusplus
}
#endif

#endif  // HBRIDGE_DRIVER_H