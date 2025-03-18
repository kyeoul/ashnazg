#pragma once

#include "microbit_v2.h"
#include "nrfx_pwm.h"

static const nrfx_pwm_t SERVO_PWM_INSTANCE = NRFX_PWM_INSTANCE(0);
static const nrfx_pwm_t SPEAKER_PWM_INSTANCE = NRFX_PWM_INSTANCE(1);

void pwm_init();

void pwm_write(nrfx_pwm_t instance, uint16_t duty_cycle);