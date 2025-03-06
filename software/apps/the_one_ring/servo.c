#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_pwm.h"
#include "servo.h"

static const nrfx_pwm_t PWM_INST = NRFX_PWM_INSTANCE(0);
static float current_angle = 0;

nrf_pwm_values_common_t sequence_data[1] = {0};

nrf_pwm_sequence_t pwm_sequence = {
  .values.p_common = sequence_data,
  .length = 1,
  .repeats = 0,
  .end_delay = 0,
};

static void gpio_init() {
  nrf_gpio_pin_dir_set(SERVO_OUT, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_cfg(SERVO_OUT, NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT,
      NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);

  nrf_gpio_pin_set(SERVO_OUT);
}

static void pwm_init() {
  nrfx_pwm_config_t config = {
    .output_pins = {SERVO_OUT, NRFX_PWM_PIN_NOT_USED, NRFX_PWM_PIN_NOT_USED, NRFX_PWM_PIN_NOT_USED},
    .irq_priority = 0,
    .base_clock = NRF_PWM_CLK_1MHz,
    .count_mode = NRF_PWM_MODE_UP,
    .top_value = 4000, // 20ms period (50Hz frequency)
    .load_mode = NRF_PWM_LOAD_COMMON,
    .step_mode = NRF_PWM_STEP_AUTO,
  };

  nrfx_pwm_init(&PWM_INST, &config, NULL);
}

void servo_write(float angle) {
  current_angle = angle;

  nrfx_pwm_stop(&PWM_INST, true);

  uint16_t pulse_width = (uint16_t)((angle / 180.0f) * 4000);

  sequence_data[0] = pulse_width;

  nrfx_pwm_simple_playback(&PWM_INST, &pwm_sequence, 1, NRFX_PWM_FLAG_LOOP);
}

void servo_rotate() {
  servo_write(0);
  nrf_delay_ms(2000);

  // Gradually move from 0 to 180
  while (true) {
    for (float angle = 0; angle <= 180; angle += 1.0f) {
      printf("angle: %f\n", servo_get_angle());
      servo_write(angle);
      nrf_delay_ms(50);  // Adjust this delay to control speed
    }

    nrf_delay_ms(1000);  // Pause at 180 degrees before reversing

    // Gradually move from 180 back to 0
    for (float angle = 180; angle >= 0; angle -= 1.0f) {
      printf("angle: %f\n", servo_get_angle());
      servo_write(angle);
      nrf_delay_ms(50);  // Adjust this delay to control speed
    }
    nrf_delay_ms(1000);  // Pause at 0 degrees before reversing
  }
}

void servo_stop() {
  nrfx_pwm_stop(&PWM_INST, true);
}

float servo_get_angle() {
  return current_angle;
}

void servo_init() {
  printf("Initializing servo\n");

  gpio_init();

  pwm_init();
}