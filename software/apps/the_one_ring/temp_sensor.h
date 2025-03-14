#pragma once

#include "nrf_twi_mngr.h"

// Chip address for temp sensor
static const uint8_t TEMP_ACC_ADDRESS = 0x48;

// thank u https://github.com/sparkfun/SparkFun_AS6212_Qwiic_Arduino_Library/tree/main
typedef enum {
  //Internal Register Addresses
  TVAL      =       0x0,    //Temperature Register
  CONFIG    =       0x1,    //Configuration Register
  TLOW      =       0x2,    //Low Temperature Threshold
  THIGH     =       0x3,    //High Temperature Threshold
  
  //Helpful preset definitions for configuration register
  DEFAULTM  =       0x40A0,   //Default state
  SLEEPMODE =       0x41A0,   //Sleep Mode
  SLEEPSS   =       0xC1A0,   //Sleep Mode Single Shot

  /* Register values (MSB -> LSB) */
  SINGLESHOT	=	0x8000,	//15
  CFAULT_1		=	0x0800,	//12
  CFAULT_0		=	0x0400,	//11
  POLARITY		=	0x0200,	//10
  INTERRUPT		=	0x0100,	//9
  SLEEP			=	0x0080,	//8
  CONVER_RATE_1 =	0x0040,	//7
  CONVER_RATE_0 =	0x0020,	//6
  ALERT			=	0x0010,	//5
} temp_reg_t;

void ir_array_init();

float get_pixel_temperature(uint8_t pixel);

void set_interrupt_mode_absolute();

void set_upper_interrupt_value(uint16_t value);

void set_lower_interrupt_value(uint16_t value);

void set_hysteresis_interrupt_value(uint16_t value);

bool interrupt_flag_set();

bool pixel_interrupt_set(uint8_t pixel);

void clear_interrupt_flag();

bool get_register_16(uint8_t reg, uint16_t* value);

bool get_register_8(uint8_t reg, uint8_t* value);

bool set_register(uint8_t reg, uint8_t value);

void print_temp_array();