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

#include "microbit_v2.h"
#include "distance_sensor.h"
#include "servo.h"
#include "ir_array.h"

APP_TIMER_DEF(distance_timer);
APP_TIMER_DEF(sample_timer);

// Global variables
NRF_TWI_MNGR_DEF(twi_mngr_instance, 1, 0);

bool interrupt_table[64] = {0};

static void distance_timer_callback(void* __unused){
  printf("measuring distance: %f\n", distance_measure_blocking());
}

static void sample_timer_callback(void* __unused){
  printf("sampling\n");
  // print_temp_array();
  if(interrupt_flag_set()){
    printf("interrupt flag set\n");
    for(int i = 0; i < 64; i++){
      interrupt_table[i] = pixel_interrupt_set(i);
    }
    for(int i = 0; i < 8; i++){
      for(int j = 0; j < 8; j++){
        printf("%d ", interrupt_table[i*8 + j]);
      }
      printf("\n");
    }
    clear_interrupt_flag();
  }
}

int main(void){

  // distance_sensor_init();
  // Initialize I2C peripheral and driver
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  // WARNING!!
  // These are NOT the correct pins for external I2C communication.
  // If you are using QWIIC or other external I2C devices, the are
  // connected to EDGE_P19 (a.k.a. I2C_QWIIC_SCL) and EDGE_P20 (a.k.a. I2C_QWIIC_SDA)
  i2c_config.scl = EDGE_P19;
  i2c_config.sda = EDGE_P20;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  i2c_config.interrupt_priority = 0;
  nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);

  distance_sensor_init();
  ir_array_init(&twi_mngr_instance);

  // set_interrupt_mode_absolute();
  // set_upper_interrupt_value(50);
  // set_lower_interrupt_value(0);
  // set_hysteresis_interrupt_value(5);

  servo_init();

  servo_rotate();

  // // initialize app timers
  // app_timer_init();
  // app_timer_create(&sample_timer, APP_TIMER_MODE_REPEATED, sample_timer_callback);
  // app_timer_create(&distance_timer, APP_TIMER_MODE_REPEATED, distance_timer_callback);

  // // start timer
  // // change the rate to whatever you want
  // app_timer_start(sample_timer, 32768, NULL);
  // app_timer_start(distance_timer, 163840, NULL);

  printf("sup lol\n");

  while (1) {
    // Don't put any code in here. Instead put periodic code in `sample_timer_callback()`
    
    nrf_delay_ms(1000);
  }
}