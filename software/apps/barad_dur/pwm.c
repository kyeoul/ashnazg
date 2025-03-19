#include "nrfx_pwm.h"
#include "pwm.h"
#include "servo.h"


void pwm_init() {
  nrfx_pwm_config_t servo_config = {
    .output_pins = {SERVO_OUT, NRFX_PWM_PIN_NOT_USED, NRFX_PWM_PIN_NOT_USED, NRFX_PWM_PIN_NOT_USED},
    .irq_priority = 0,
    .base_clock = NRF_PWM_CLK_1MHz,
    .count_mode = NRF_PWM_MODE_UP,
    .top_value = 4000,
    .load_mode = NRF_PWM_LOAD_COMMON,
    .step_mode = NRF_PWM_STEP_AUTO
  };

  nrfx_pwm_init(&SERVO_PWM_INSTANCE, &servo_config, NULL);
}

void pwm_write(nrfx_pwm_t instance, uint16_t duty_cycle) {
  nrfx_pwm_stop(&instance, true);

  nrf_pwm_values_common_t sequence_data[1] = {duty_cycle};

  nrf_pwm_sequence_t pwm_sequence = {
    .values.p_common = sequence_data,
    .length = 1,
    .repeats = 0,
    .end_delay = 0,
  };

  nrfx_pwm_simple_playback(&instance, &pwm_sequence, 1, NRFX_PWM_FLAG_LOOP);
};