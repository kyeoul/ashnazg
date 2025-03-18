#include "i2c.h"

static const nrf_twi_mngr_t* i2c_manager = NULL;

void i2c_init(const nrf_twi_mngr_t* i2c) {
  i2c_manager = i2c;
}

bool get_register_16(uint8_t addr, uint8_t reg, uint16_t* value) {
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    NRF_TWI_MNGR_WRITE(addr, &reg, 1, NRF_TWI_MNGR_NO_STOP),
    NRF_TWI_MNGR_READ(addr, value, 2, 0)
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

bool get_register_8(uint8_t addr, uint8_t reg, uint8_t* value) {
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    NRF_TWI_MNGR_WRITE(addr, &reg, 1, NRF_TWI_MNGR_NO_STOP),
    NRF_TWI_MNGR_READ(addr, value, 1, 0)
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

bool set_register(uint8_t addr, uint8_t reg, uint8_t value) {
  //TODO: implement me
  //Note: there should only be a single two-byte transfer to be performed
  uint8_t buf[2] = {reg, value};

  nrf_twi_mngr_transfer_t const write_transfer[] = {
    NRF_TWI_MNGR_WRITE(addr, buf, 2, 0)
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