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
#include "pwm.h"
#include "i2c.h"
#include "temp_sensor.h"
#include "capacitive_touch.h"
#include "speaker.h"
#include "ir_led.h"

#define VOLTAGE_MEASURE_CHANNEL NRF_SAADC_INPUT_AIN1
APP_TIMER_DEF(sample_timer);
APP_TIMER_DEF(event_timer);
APP_TIMER_DEF(reset_has_played_timer);

// Global variables
NRF_TWI_MNGR_DEF(twi_mngr_instance, 1, 0);

static void saadc_event_handler(nrfx_saadc_evt_t const *p_evt);

static void saadc_event_handler(nrfx_saadc_evt_t const *_unused)
{
  // don't care about saadc events
  // ignore this function
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

float past_distance = -1;
float curr_distance = -1;

bool has_played = false;

static void sample_timer_callback(void *__unused)
{
  printf("measuring distance: %f\n", distance_measure_blocking());
  if(curr_distance != -1) {
    past_distance = curr_distance;
  }
  curr_distance = distance_measure_blocking();
  printf("past distance: %f\n", past_distance);

  if(has_played){
    return;
  }
  else if(curr_distance != -1 && past_distance != -1) {
    if(curr_distance < 1.2){
      speaker_play(LERING_ACTIVATED);
    }
    else if(curr_distance - past_distance > 2) {
      printf("distance increased by 10\n");
      // play a sound
      speaker_play(GO_OTHER_WAY);
      nrf_delay_ms(1000);
    }
    else if(curr_distance - past_distance < -2) {
      printf("distance decreased by 10\n");
      // play a sound
      speaker_play(KEEP_WALKING);
      nrf_delay_ms(1000);
    }
  }
}

static void reset_has_played_callback(void *__unused)
{
  has_played = false;
}

static void event_timer_callback(void *__unused)
{
  printf("event timer\n");

  if(has_played){
    return;
  }

  if(read_temp_C() > 25){
    printf("temp is greater than 20\n");
    // play a sound
    speaker_play(RING_COMMAND);
    nrf_delay_ms(1000);
  }

  has_played = true;
}

int main(void)
{
  distance_sensor_init();
  // Initialize I2C peripheral and driver
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = EDGE_P19;
  i2c_config.sda = EDGE_P20;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  i2c_config.interrupt_priority = 0;
  nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);

  // initialize i2c client
  i2c_init(&twi_mngr_instance);

  pwm_init();

  // servo_init();

  // servo_write(50);

  speaker_play(LERING);

  // initialize app timers
  nrfx_gpiote_init();

  app_timer_init();
  app_timer_create(&sample_timer, APP_TIMER_MODE_REPEATED, sample_timer_callback);
  app_timer_create(&event_timer, APP_TIMER_MODE_REPEATED, event_timer_callback);
  app_timer_create(&reset_has_played_timer, APP_TIMER_MODE_REPEATED, reset_has_played_callback);

  // start timer
  // change the rate to whatever you want
  app_timer_start(sample_timer, 32768, NULL);
  app_timer_start(event_timer, 327680, NULL);
  app_timer_start(reset_has_played_timer, 655360, NULL);

  printf("sup lol\n");
  capacitive_touch_init();
  adc_init();
  while (1)
  {
    // servo_rotate();
    nrf_delay_ms(100);

    
    if(capacitive_touch_is_active() == 1 && has_played == false) {
      printf("capacitive touch is active\n");
      // play a sound
      speaker_play(ASHNAZG);
      nrf_delay_ms(1000);
      has_played = true;
    }
  }
}