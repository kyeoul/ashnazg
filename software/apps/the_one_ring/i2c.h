#include "nrf_twi_mngr.h"

void i2c_init(const nrf_twi_mngr_t* i2c);

bool get_register_16(uint8_t addr, uint8_t reg, uint16_t* value);

bool get_register_8(uint8_t addr, uint8_t reg, uint8_t* value);

bool set_register(uint8_t addr, uint8_t reg, uint8_t value);