/**
 * @file hbridge_driver.c
 * @brief H-bridge driver for controlling motor direction and speed.
 */

#include "hbridge_driver.h"
#include "gpio_port.h"
#include "pwm_port.h"

int hbridge_init(hbridge_t *self, void *pwm_context, void *in1_context,
                 void *in2_context) {
    self->pwm_context = pwm_context;
    self->in1_context = in1_context;
    self->in2_context = in2_context;

    gpio_write(self->in1_context, 0);
    gpio_write(self->in2_context, 0);

    pwm_set_duty_cycle(self->pwm_context, 0.0);
    if (pwm_init(self->pwm_context) < 0) {
        return -1;
    }
    return 0;
}

void hbridge_short_brake(hbridge_t *self) {
    gpio_write(self->in1_context, 0);
    gpio_write(self->in2_context, 0);
    pwm_set_duty_cycle(self->pwm_context, 0.0);
}

void hbridge_set_output(hbridge_t *self, uint8_t direction, float duty_cycle) {
    if (direction == 0) {
        gpio_write(self->in1_context, 1);
        gpio_write(self->in2_context, 0);
    } else if (direction == 1) {
        gpio_write(self->in1_context, 0);
        gpio_write(self->in2_context, 1);
    } else {
        gpio_write(self->in1_context, 0);
        gpio_write(self->in2_context, 0);
    }
    pwm_set_duty_cycle(self->pwm_context, duty_cycle);
}
