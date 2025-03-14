#include <stdint.h>

#pragma once

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

float read_temp_C();