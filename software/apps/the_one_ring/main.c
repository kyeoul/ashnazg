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

APP_TIMER_DEF(distance_timer);
APP_TIMER_DEF(sample_timer);

static void distance_timer_callback(void* __unused){
  printf("measuring distance: %f\n", distance_measure_blocking());
}

static void sample_timer_callback(void* __unused){
  printf("sampling\n");
  printf("echo output: %d\n", read_echo_output());
}

int main(void){

  // distance_sensor_init();

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