// Breadboard example app
//
// Read from multiple analog sensors and control an RGB LED

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "app_timer.h"
#include "nrf_delay.h"
#include "nrfx_saadc.h"
#include "nrfx_gpiote.h"

#include "microbit_v2.h"
#include "distance_sensor.h"
#include "servo.h"
#include "ir_array.h"
#include "i2c.h"
#include "temp_sensor.h"
// #include "capacitive_touch.h"

#define IR_LED_PIN EDGE_P0
#define VOLTAGE_MEASURE_CHANNEL NRF_SAADC_INPUT_AIN4
APP_TIMER_DEF(distance_timer);
APP_TIMER_DEF(sample_timer);

// Global variables
NRF_TWI_MNGR_DEF(twi_mngr_instance, 1, 0);

bool interrupt_table[64] = {0};
static void saadc_event_handler(nrfx_saadc_evt_t const *p_evt);
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

static void adc_init(void)
{
  // 1. Create a default SAADC config
  nrfx_saadc_config_t saadc_config = {
      .resolution = NRF_SAADC_RESOLUTION_12BIT,
      .oversample = NRF_SAADC_OVERSAMPLE_DISABLED,
      .interrupt_priority = 4,
      .low_power_mode = false,
  }; // 2. Initialize SAADC (no event handler, we'll do blocking calls)
  nrfx_err_t err = nrfx_saadc_init(&saadc_config, saadc_event_handler);
  if (err != NRFX_SUCCESS)
  {
    printf("SAADC init error: %lu\n", err);
  }

  // 3. Configure an SAADC channel for single-ended reads on AIN1
  nrf_saadc_channel_config_t channel_config =
      NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(VOLTAGE_MEASURE_CHANNEL);

  // 4. Initialize that channel at index 0
  err = nrfx_saadc_channel_init(0, &channel_config);
  if (err != NRFX_SUCCESS)
  {
    printf("SAADC channel init error: %lu\n", err);
  }
}

static float read_voltage(void)
{
  nrf_saadc_value_t adc_value = 0;
  // Perform a blocking sample on channel 0
  nrfx_err_t err = nrfx_saadc_sample_convert(0, &adc_value);
  if (err != NRFX_SUCCESS)
  {
    printf("SAADC sample error: %lu\n", err);
    return 0.0f;
  }

  // Convert 10-bit reading (0..1023) to voltage (assuming ~3.3 V reference)
  float voltage = (adc_value * 3.3f) / 1023.0f;
  return voltage;
}

static void distance_timer_callback(void *__unused)
{
  printf("measuring distance: %f\n", distance_measure_blocking());
}

static void sample_timer_callback(void *__unused)
{
  printf("sampling\n");
  printf("temp: %f\n", read_temp_C());
  // print_temp_array();
  // if(interrupt_flag_set()){
  //   printf("interrupt flag set\n");
  //   for(int i = 0; i < 64; i++){
  //     interrupt_table[i] = pixel_interrupt_set(i);
  //   }
  //   for(int i = 0; i < 8; i++){
  //     for(int j = 0; j < 8; j++){
  //       printf("%d ", interrupt_table[i*8 + j]);
  //     }
  //     printf("\n");
  //   }
  //   clear_interrupt_flag();
  // }
}

static void saadc_event_handler(nrfx_saadc_evt_t const *_unused)
{
  // don't care about saadc events
  // ignore this function
}

int main(void)
{

  // distance_sensor_init();
  // Initialize I2C peripheral and driver
  // nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  // i2c_config.scl = EDGE_P19;
  // i2c_config.sda = EDGE_P20;
  // i2c_config.frequency = NRF_TWIM_FREQ_100K;
  // i2c_config.interrupt_priority = 0;
  // nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);

  // distance_sensor_init();

  // // initialize i2c client
  // i2c_init(&twi_mngr_instance);

  // set_interrupt_mode_absolute();
  // set_upper_interrupt_value(50);
  // set_lower_interrupt_value(0);
  // set_hysteresis_interrupt_value(5);

  // servo_init();

  // servo_rotate();

  // // initialize app timers
  nrfx_gpiote_init();
  app_timer_init();
  // app_timer_create(&sample_timer, APP_TIMER_MODE_REPEATED, sample_timer_callback);
  // app_timer_create(&distance_timer, APP_TIMER_MODE_REPEATED, distance_timer_callback);

  // // // start timer
  // // // change the rate to whatever you want
  // app_timer_start(sample_timer, 32768, NULL);
  // app_timer_start(distance_timer, 163840, NULL);

  printf("sup lol\n");
  // capacitive_touch_init();
  ir_led_init();
  adc_init();
  while (1)
  {
    // Turn IR LED ON
    ir_led_on();
    nrf_delay_ms(500);

    // Read voltage at P1
    float voltage_on = read_voltage();
    printf("IR LED ON - Measured Voltage: %.2f V\n", voltage_on);

    // Turn IR LED OFF
    ir_led_off();
    nrf_delay_ms(500);

    // Read voltage at P1 again
    float voltage_off = read_voltage();
    printf("IR LED OFF - Measured Voltage: %.2f V\n", voltage_off);

    // printf("Capacitive Touch Active: %d\n", capacitive_touch_is_active());

    nrf_delay_ms(1000);
    // printf("%d\n", capacitive_touch_is_active());
  }
}