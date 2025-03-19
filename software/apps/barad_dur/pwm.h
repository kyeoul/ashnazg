#pragma once

#include "microbit_v2.h"
#include "nrfx_pwm.h"

static const nrfx_pwm_t SERVO_PWM_INSTANCE = NRFX_PWM_INSTANCE(0);

#define PWM_TOP_VALUE 65

void pwm_init();

void pwm_write(nrfx_pwm_t instance, uint16_t duty_cycle);