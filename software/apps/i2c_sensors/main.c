// I2C sensors app
//
// Read from I2C accelerometer/magnetometer on the Microbit

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "nrf_delay.h"
#include "nrf_twi_mngr.h"

#include "microbit_v2.h"
#include "lsm303agr.h"
#include "app_timer.h"

APP_TIMER_DEF(w);

// Global variables
NRF_TWI_MNGR_DEF(twi_mngr_instance, 1, 0);

void print_temperature() {
  lsm303agr_measurement_t acceleration = lsm303agr_read_accelerometer();
  lsm303agr_measurement_t magnetometer = lsm303agr_read_magnetometer();

  printf("Acceleration: x=%f, y=%f, z=%f\n", acceleration.x_axis, acceleration.y_axis, acceleration.z_axis); 
  printf("Magnetometer: x=%f, y=%f, z=%f\n", magnetometer.x_axis, magnetometer.y_axis, magnetometer.z_axis);

  lsm303agr_measurement_t tilt = calculate_tilt_angle_from_acceleration(acceleration);
  printf("Tilt: x=%f, y=%f, z=%f\n", tilt.x_axis, tilt.y_axis, tilt.z_axis);
}

int main(void) {
  printf("Board started!\n");

  // Initialize I2C peripheral and driver
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  // WARNING!!
  // These are NOT the correct pins for external I2C communication.
  // If you are using QWIIC or other external I2C devices, the are
  // connected to EDGE_P19 (a.k.a. I2C_QWIIC_SCL) and EDGE_P20 (a.k.a. I2C_QWIIC_SDA)
  i2c_config.scl = I2C_INTERNAL_SCL;
  i2c_config.sda = I2C_INTERNAL_SDA;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  i2c_config.interrupt_priority = 0;
  nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);

  // Initialize the LSM303AGR accelerometer/magnetometer sensor
  lsm303agr_init(&twi_mngr_instance);

  //TODO: implement me!
  app_timer_init();
  app_timer_create(&w, APP_TIMER_MODE_REPEATED, print_temperature);

  app_timer_start(w, 32768, NULL);

  // Loop forever
  while (1) {
    // Don't put any code in here. Instead put periodic code in a callback using a timer.
    nrf_delay_ms(1000);
  }
}

