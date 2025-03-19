#include "app_timer.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrfx_gpiote.h"
#include "nrfx_timer.h"

#include "microbit_v2.h"

#include "distance_sensor.h"

static nrfx_timer_t DISTANCE_TIMER = NRFX_TIMER_INSTANCE(1);

static void timer_handler(nrf_timer_event_t event, void* context) {
  printf("hi lol\n");
}

void distance_sensor_init(){
  nrfx_timer_config_t timer_config = {
    .frequency = NRF_TIMER_FREQ_1MHz,
    .mode = NRF_TIMER_MODE_TIMER,
    .bit_width = NRF_TIMER_BIT_WIDTH_32,
    .interrupt_priority = 4,
    .p_context = NULL
  };
  nrfx_timer_init(&DISTANCE_TIMER, &timer_config, timer_handler);

  // enable, but pause the timer
  nrfx_timer_enable(&DISTANCE_TIMER);
  nrfx_timer_pause(&DISTANCE_TIMER);

  nrf_gpio_cfg_output(TRIG_OUTPUT);
  nrf_gpio_cfg_input(ECHO_INPUT, NRF_GPIO_PIN_NOPULL);
}

float distance_measure_blocking(){

  start_sensor();
  while(read_echo_output() == 0);

  nrfx_timer_clear(&DISTANCE_TIMER);
  nrfx_timer_resume(&DISTANCE_TIMER);
  printf("wow the echo turned high\n");

  while(read_echo_output() == 1);

  nrfx_timer_pause(&DISTANCE_TIMER);
  uint32_t count = nrfx_timer_capture(&DISTANCE_TIMER, NRF_TIMER_CC_CHANNEL1);
  printf("wow the echo turned low\n");
  printf("count: %ld\n", count);

  return (count * 0.000343) / 2;
}

void start_sensor(){
  nrf_gpio_pin_set(TRIG_OUTPUT);
  nrf_delay_us(10);
  nrf_gpio_pin_clear(TRIG_OUTPUT);
}

uint32_t read_echo_output(){
  uint32_t reading = nrf_gpio_pin_read(ECHO_INPUT);
  return reading;
}