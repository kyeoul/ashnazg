#pragma once

#include "nrf_twi_mngr.h"

// Chip address for IR array
static const uint8_t IR_ARRAY_ACC_ADDRESS = 0x69;

// thank u https://github.com/sparkfun/SparkFun_GridEYE_Arduino_Library
typedef enum {
  POWER_CONTROL_REGISTER = 0x00,
  RESET_REGISTER = 0x01,
  FRAMERATE_REGISTER = 0x02,
  INT_CONTROL_REGISTER = 0x03,
  STATUS_REGISTER = 0x04,
  STATUS_CLEAR_REGISTER = 0x05,
  AVERAGE_REGISTER = 0x07,
  INT_LEVEL_REGISTER_UPPER_LSB = 0x08,
  INT_LEVEL_REGISTER_UPPER_MSB = 0x09,
  INT_LEVEL_REGISTER_LOWER_LSB = 0x0A,
  INT_LEVEL_REGISTER_LOWER_MSB = 0x0B,
  INT_LEVEL_REGISTER_HYST_LSB = 0x0C,
  INT_LEVEL_REGISTER_HYST_MSB = 0x0D,
  THERMISTOR_REGISTER_LSB = 0x0E,
  THERMISTOR_REGISTER_MSB = 0x0F,
  INT_TABLE_REGISTER_INT0 = 0x10,
  RESERVED_AVERAGE_REGISTER = 0x1F,
  TEMPERATURE_REGISTER_START = 0x80,
} ir_array_reg_t;

void ir_array_init();

float get_pixel_temperature(uint8_t pixel);

void set_interrupt_mode_absolute();

void set_upper_interrupt_value(uint16_t value);

void set_lower_interrupt_value(uint16_t value);

void set_hysteresis_interrupt_value(uint16_t value);

bool interrupt_flag_set();

bool pixel_interrupt_set(uint8_t pixel);

void clear_interrupt_flag();

void print_temp_array();

float get_average_temperature();