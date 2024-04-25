#include "i2c.h"
#include "mpu.h"
#include <stdint.h>

uint8_t MPU_readbuf[128];
MPU_data gryo_data;
MPU_data acc_data;
uint8_t ak_id;

void MPU_ReadMulBytes(uint8_t reg, uint8_t length, uint8_t *buf) {
  HAL_I2C_Mem_Read(&hi2c1, 0x18, reg, I2C_MEMADD_SIZE_8BIT, buf, length,
                   200);
  switch (reg) {
  case MPU_RA_GYRO_XOUT_H: {
    gryo_data.x = (int16_t)((buf[0] << 8) | buf[1]);
    gryo_data.y = (int16_t)((buf[2] << 8) | buf[3]);
    gryo_data.z = (int16_t)((buf[4] << 8) | buf[5]);
  } break;
  case MPU_RA_ACCEL_XOUT_H: {
    acc_data.x = (int16_t)((buf[0] << 8) | buf[1]);
    acc_data.y = (int16_t)((buf[2] << 8) | buf[3]);
    acc_data.z = (int16_t)((buf[4] << 8) | buf[5]);
  } break;
  case MPU_RA_MAG: {
    ak_id = buf[0];
  } break;
  }
}
