/**
 * @file hbridge_driver.c
 * @brief H-bridge driver for controlling motor direction and speed.
 */

#include <math.h>

#include "gpio_port.h"
#include "hbridge_driver.h"
#include "pwm_port.h"

int hbridge_init(hbridge_t *self, void *pwm_context, void *in1_context,
                 void *in2_context) {
    self->pwm_context = pwm_context;
    self->in1_context = in1_context;
    self->in2_context = in2_context;
    self->last_sign = 0;

    gpio_write(self->in1_context, 0);
    gpio_write(self->in2_context, 0);

    pwm_set_duty_cycle(self->pwm_context, 0.0);
    if (pwm_init(self->pwm_context) < 0) {
        return -1;
    }
    return 0;
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

void hbridge_coast(hbridge_t *self) {
    gpio_write(self->in1_context, 0);
    gpio_write(self->in2_context, 0);
    pwm_set_duty_cycle(self->pwm_context, 100.0f);
}

void hbridge_brake(hbridge_t *self) {
    gpio_write(self->in1_context, 1);
    gpio_write(self->in2_context, 1);
}

static inline void set_dir_forward(hbridge_t *self) {
    gpio_write(self->in1_context, 1);
    gpio_write(self->in2_context, 0);
}

static inline void set_dir_reverse(hbridge_t *self) {
    gpio_write(self->in1_context, 0);
    gpio_write(self->in2_context, 1);
}

// Signed command in [-100..100] %PWM, with safe reversal
void hbridge_set_output_signed(hbridge_t *self, float cmd) {
    if (!isfinite(cmd)) cmd = 0.f;
    if (fabsf(cmd) < 0.5f) {
        hbridge_coast(self);
        return;
    }

    int sign = (cmd > 0.f) - (cmd < 0.f);
    float duty = fabsf(cmd);

    if (sign != self->last_sign && self->last_sign != 0) {
        pwm_set_duty_cycle(self->pwm_context, 0.f);
        hbridge_coast(self);
    }

    if (sign >= 0)
        set_dir_forward(self);
    else
        set_dir_reverse(self);
    pwm_set_duty_cycle(self->pwm_context, duty);
    self->last_sign = sign;
}
