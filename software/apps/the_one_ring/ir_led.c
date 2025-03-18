#include <stdint.h>
#include <microbit_v2.h>
#include "nrf_delay.h"
#include "nrfx_timer.h"
#include "nrfx_gpiote.h"
#include "ir_led.h"

void ir_led_init(void)
{
  nrfx_gpiote_out_config_t out_config = NRFX_GPIOTE_CONFIG_OUT_SIMPLE(false);
  nrfx_gpiote_out_init(IR_LED_PIN, &out_config);
}

void ir_led_on(void)
{
  nrfx_gpiote_out_set(IR_LED_PIN);
}

void ir_led_off(void)
{
  nrfx_gpiote_out_clear(IR_LED_PIN);
}

void ir_led_toggle(void)
{
  nrfx_gpiote_out_toggle(IR_LED_PIN);
}