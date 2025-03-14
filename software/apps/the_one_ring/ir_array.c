#include "ir_array.h"
#include "i2c.h"

float get_pixel_temperature(uint8_t pixel) {
  unsigned char reg = TEMPERATURE_REGISTER_START + (2 * pixel);
  uint16_t temp;
  if (get_register_16(IR_ARRAY_ACC_ADDRESS, reg, &temp)) {
    return (float) temp * 0.25;
  }
  return -99.0;
}

void set_interrupt_mode_absolute() {
  uint8_t value = 0;
  if(get_register_8(IR_ARRAY_ACC_ADDRESS, INT_CONTROL_REGISTER, &value)){
    value |= (1 << 1);
    set_register(IR_ARRAY_ACC_ADDRESS, INT_CONTROL_REGISTER, value);
  }
}

void set_upper_interrupt_value(uint16_t value) {
  uint8_t upper_lsb = value & 0xFF;
  uint8_t upper_msb = (value >> 8) & 0xFF;
  set_register(IR_ARRAY_ACC_ADDRESS, INT_LEVEL_REGISTER_UPPER_LSB, upper_lsb);
  set_register(IR_ARRAY_ACC_ADDRESS, INT_LEVEL_REGISTER_UPPER_MSB, upper_msb);
}

void set_lower_interrupt_value(uint16_t value) {
  uint8_t lower_lsb = value & 0xFF;
  uint8_t lower_msb = (value >> 8) & 0xFF;
  set_register(IR_ARRAY_ACC_ADDRESS, INT_LEVEL_REGISTER_LOWER_LSB, lower_lsb);
  set_register(IR_ARRAY_ACC_ADDRESS, INT_LEVEL_REGISTER_LOWER_MSB, lower_msb);
}

void set_hysteresis_interrupt_value(uint16_t value) {
  uint8_t hysteresis_lsb = value & 0xFF;
  uint8_t hysteresis_msb = (value >> 8) & 0xFF;
  set_register(IR_ARRAY_ACC_ADDRESS, INT_LEVEL_REGISTER_HYST_LSB, hysteresis_lsb);
  set_register(IR_ARRAY_ACC_ADDRESS, INT_LEVEL_REGISTER_HYST_MSB, hysteresis_msb);
}

bool interrupt_flag_set() {
  return true;
}

bool pixel_interrupt_set(uint8_t pixel) {
  return true;
}

void clear_interrupt_flag() {
  uint8_t value = 0x02;
  set_register(IR_ARRAY_ACC_ADDRESS, STATUS_CLEAR_REGISTER, value);
}

void print_temp_array() {
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      printf("%f ", get_pixel_temperature(i*8 + j));
    }
    printf("\n");
  }
}