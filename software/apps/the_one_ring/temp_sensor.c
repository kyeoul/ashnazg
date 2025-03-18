#include "temp_sensor.h"
#include "i2c.h"

float read_temp_C() {
  uint16_t temp_raw;
  if(!get_register_16(TEMP_ACC_ADDRESS, TVAL, &temp_raw)){
    return -99.0;
  }
  printf("raw temp: %d\n", temp_raw);
  printf("raw temp hex: %X\n", temp_raw);
  printf("raw temp hex: %X\n", (temp_raw >> 8));
  printf("raw temp hex: %X\n", (temp_raw & 0xFF));
  uint16_t datac = (((temp_raw & 0xFF) << 8) | (temp_raw >> 8));

  float result;

  if(datac < 32768)
  {
    result = datac * 0.0078125;
  }

  if(datac >= 32768)
  {
    result = ((datac - 1) * 0.0078125) * -1;
  }

  return result;
}