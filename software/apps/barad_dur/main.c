// Breadboard example app
//
// Read from multiple analog sensors and control an RGB LED

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf_delay.h"

#include "app_timer.h"
#include "microbit_v2.h"
#include "pwm.h"
#include "servo.h"
#include "ir_array.h"
#include "i2c.h"

APP_TIMER_DEF(ir_array_timer);

static void ir_array_timer_callback(void *__unused)
{
  print_temp_array();
}

// Global variables
NRF_TWI_MNGR_DEF(twi_mngr_instance, 1, 0);

bool interrupt_table[64] = {0};

int main(void)
{
  // Initialize I2C peripheral and driver
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = EDGE_P19;
  i2c_config.sda = EDGE_P20;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  i2c_config.interrupt_priority = 0;
  nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);

  // // initialize i2c client
  i2c_init(&twi_mngr_instance);

  // set_interrupt_mode_absolute();
  // set_upper_interrupt_value(50);
  // set_lower_interrupt_value(0);
  // set_hysteresis_interrupt_value(5);

  app_timer_init();

  pwm_init();

  servo_init();

  // app_timer_create(&ir_array_timer, APP_TIMER_MODE_REPEATED, ir_array_timer_callback);
  // app_timer_start(ir_array_timer, APP_TIMER_TICKS(5000), NULL);

  while (1) {
    nrf_delay_ms(1000);
  }
}