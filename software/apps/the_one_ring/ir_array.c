#include "ir_array.h"

// Pointer to an initialized I2C instance to use for transactions
static const nrf_twi_mngr_t* i2c_manager = NULL;

void ir_array_init(const nrf_twi_mngr_t* i2c) {
  i2c_manager = i2c;
}

float get_pixel_temperature(uint8_t pixel) {
  unsigned char reg = TEMPERATURE_REGISTER_START + (2 * pixel);
  uint16_t temp;
  if (get_register_16(reg, &temp)) {
    return (float) temp * 0.25;
  }
  return -99.0;
}

void set_interrupt_mode_absolute() {
  uint8_t value = 0;
  if(get_register_8(INT_CONTROL_REGISTER, &value)){
    value |= (1 << 1);
    set_register(INT_CONTROL_REGISTER, value);
  }
}

void set_upper_interrupt_value(uint16_t value) {
  uint8_t upper_lsb = value & 0xFF;
  uint8_t upper_msb = (value >> 8) & 0xFF;
  set_register(INT_LEVEL_REGISTER_UPPER_LSB, upper_lsb);
  set_register(INT_LEVEL_REGISTER_UPPER_MSB, upper_msb);
}

void set_lower_interrupt_value(uint16_t value) {
  uint8_t lower_lsb = value & 0xFF;
  uint8_t lower_msb = (value >> 8) & 0xFF;
  set_register(INT_LEVEL_REGISTER_LOWER_LSB, lower_lsb);
  set_register(INT_LEVEL_REGISTER_LOWER_MSB, lower_msb);
}

void set_hysteresis_interrupt_value(uint16_t value) {
  uint8_t hysteresis_lsb = value & 0xFF;
  uint8_t hysteresis_msb = (value >> 8) & 0xFF;
  set_register(INT_LEVEL_REGISTER_HYST_LSB, hysteresis_lsb);
  set_register(INT_LEVEL_REGISTER_HYST_MSB, hysteresis_msb);
}

bool interrupt_flag_set() {
  return true;
}

bool pixel_interrupt_set(uint8_t pixel) {
  return true;
}

void clear_interrupt_flag() {
  uint8_t value = 0x02;
  set_register(STATUS_CLEAR_REGISTER, value);
}

bool get_register_16(uint8_t reg, uint16_t* value) {
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    NRF_TWI_MNGR_WRITE(IR_ARRAY_ACC_ADDRESS, &reg, 1, NRF_TWI_MNGR_NO_STOP),
    NRF_TWI_MNGR_READ(IR_ARRAY_ACC_ADDRESS, value, 2, 0)
  };
  ret_code_t result = nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 2, NULL);
  if (result != NRF_SUCCESS) {
    // Likely error codes:
    //  NRF_ERROR_INTERNAL            (0x0003) - something is wrong with the driver itself
    //  NRF_ERROR_INVALID_ADDR        (0x0010) - buffer passed was in Flash instead of RAM
    //  NRF_ERROR_BUSY                (0x0011) - driver was busy with another transfer still
    //  NRF_ERROR_DRV_TWI_ERR_OVERRUN (0x8200) - data was overwritten during the transaction
    //  NRF_ERROR_DRV_TWI_ERR_ANACK   (0x8201) - i2c device did not acknowledge its address
    //  NRF_ERROR_DRV_TWI_ERR_DNACK   (0x8202) - i2c device did not acknowledge a data byte
    printf("I2C transaction failed! Error: %lX\n", result);
    return false;
  }

  return true;
}

bool get_register_8(uint8_t reg, uint8_t* value) {
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    NRF_TWI_MNGR_WRITE(IR_ARRAY_ACC_ADDRESS, &reg, 1, NRF_TWI_MNGR_NO_STOP),
    NRF_TWI_MNGR_READ(IR_ARRAY_ACC_ADDRESS, value, 1, 0)
  };
  ret_code_t result = nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 2, NULL);
  if (result != NRF_SUCCESS) {
    // Likely error codes:
    //  NRF_ERROR_INTERNAL            (0x0003) - something is wrong with the driver itself
    //  NRF_ERROR_INVALID_ADDR        (0x0010) - buffer passed was in Flash instead of RAM
    //  NRF_ERROR_BUSY                (0x0011) - driver was busy with another transfer still
    //  NRF_ERROR_DRV_TWI_ERR_OVERRUN (0x8200) - data was overwritten during the transaction
    //  NRF_ERROR_DRV_TWI_ERR_ANACK   (0x8201) - i2c device did not acknowledge its address
    //  NRF_ERROR_DRV_TWI_ERR_DNACK   (0x8202) - i2c device did not acknowledge a data byte
    printf("I2C transaction failed! Error: %lX\n", result);
    return false;
  }

  return true;
}

bool set_register(uint8_t reg, uint8_t value) {
  //TODO: implement me
  //Note: there should only be a single two-byte transfer to be performed
  uint8_t buf[2] = {reg, value};

  nrf_twi_mngr_transfer_t const write_transfer[] = {
    NRF_TWI_MNGR_WRITE(IR_ARRAY_ACC_ADDRESS, buf, 2, 0)
  };
  ret_code_t result = nrf_twi_mngr_perform(i2c_manager, NULL, write_transfer, 1, NULL);
  if (result != NRF_SUCCESS) {
    // Likely error codes:
    //  NRF_ERROR_INTERNAL            (0x0003) - something is wrong with the driver itself
    //  NRF_ERROR_INVALID_ADDR        (0x0010) - buffer passed was in Flash instead of RAM
    //  NRF_ERROR_BUSY                (0x0011) - driver was busy with another transfer still
    //  NRF_ERROR_DRV_TWI_ERR_OVERRUN (0x8200) - data was overwritten during the transaction
    //  NRF_ERROR_DRV_TWI_ERR_ANACK   (0x8201) - i2c device did not acknowledge its address
    //  NRF_ERROR_DRV_TWI_ERR_DNACK   (0x8202) - i2c device did not acknowledge a data byte
    printf("I2C transaction failed! Error: %lX\n", result);
    return false;
  }

  return true;
}

void print_temp_array() {
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      printf("%f ", get_pixel_temperature(i*8 + j));
    }
    printf("\n");
  }
}