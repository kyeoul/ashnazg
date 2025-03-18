#include "nrf.h"
#include "nrf_delay.h"
#include "app_timer.h"

#include "servo.h"
#include "pwm.h"

APP_TIMER_DEF(servo_timer);

typedef enum {
  SERVO_STATE_IDLE,
  SERVO_STATE_ROTATE_TO_180,
  SERVO_STATE_PAUSE_AT_180,
  SERVO_STATE_ROTATE_TO_0,
  SERVO_STATE_PAUSE_AT_0
} servo_state_t;

static servo_state_t servo_state = SERVO_STATE_IDLE;
static float current_angle = 0;

static void gpio_init() {
  nrf_gpio_pin_dir_set(SERVO_OUT, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_cfg(SERVO_OUT, NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT,
      NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);

  nrf_gpio_pin_set(SERVO_OUT);
}

static void timer_handler() {
  servo_rotate();
}

static void timer_init() {
  app_timer_create(&servo_timer, APP_TIMER_MODE_REPEATED, timer_handler);
  app_timer_start(servo_timer, APP_TIMER_TICKS(50), NULL);
}

void servo_write(float angle) {
  current_angle = angle;

  uint16_t pulse_width = (uint16_t)((angle / 180.0f) * 4000);

  pwm_write(SERVO_PWM_INSTANCE, pulse_width);
}

void servo_rotate() {
  switch (servo_state) {
    case SERVO_STATE_IDLE:
      servo_write(0);
      servo_state = SERVO_STATE_ROTATE_TO_180;
      break;
    
    case SERVO_STATE_ROTATE_TO_180:
      if (current_angle < 180) {
        current_angle += 1.0f;
        servo_write(current_angle);
      } else {
        servo_state = SERVO_STATE_PAUSE_AT_180;
      }
      break;

    case SERVO_STATE_PAUSE_AT_180:
      servo_state = SERVO_STATE_ROTATE_TO_0;
      break;
    
    case SERVO_STATE_ROTATE_TO_0:
      if (current_angle > 0) {
        current_angle -= 1.0f;
        servo_write(current_angle);
      } else {
        servo_state = SERVO_STATE_PAUSE_AT_0;
      }
      break;

    case SERVO_STATE_PAUSE_AT_0:
      servo_state = SERVO_STATE_ROTATE_TO_180;
      break;
  }
}

void servo_stop() {
  pwm_write(SERVO_PWM_INSTANCE, 0);
}

float servo_get_angle() {
  return current_angle;
}

void servo_init() {
  printf("Initializing servo\n");

  gpio_init();

  timer_init();
}