#include "mpu.h"
#include "i2c.h"
#include <stdint.h>

uint8_t MPU_readbuf[128];
MPU_data gryo_data;
MPU_data acc_data;
uint8_t ak_id;

void MPU_ReadMulBytes(uint8_t reg, uint8_t length, uint8_t *buf) {
  switch (reg) {
  case MPU_RA_GYRO_XOUT_H: {
    HAL_I2C_Mem_Read(&hi2c1, 0x68, reg, I2C_MEMADD_SIZE_8BIT, buf, length, 200);
    gryo_data.x = (int16_t)((buf[0] << 8) | buf[1]);
    gryo_data.y = (int16_t)((buf[2] << 8) | buf[3]);
    gryo_data.z = (int16_t)((buf[4] << 8) | buf[5]);
  } break;
  case MPU_RA_ACCEL_XOUT_H: {
    HAL_I2C_Mem_Read(&hi2c1, 0x68, reg, I2C_MEMADD_SIZE_8BIT, buf, length, 200);
    acc_data.x = (int16_t)((buf[0] << 8) | buf[1]);
    acc_data.y = (int16_t)((buf[2] << 8) | buf[3]);
    acc_data.z = (int16_t)((buf[4] << 8) | buf[5]);
  } break;
  case MPU_RA_MAG: {
    HAL_I2C_Mem_Read(&hi2c1, 0x0C, reg, I2C_MEMADD_SIZE_8BIT, buf, length, 200);
    ak_id = buf[0];
  } break;
  }
}

float Cal_Angle(){
}

float Angle_Get() {
  float roll;
  float yaw;
  float pitch;
  float angle_list[WINDOW_SIZE];
  float sum = 0;
  for(int i = 0; i < WINDOW_SIZE; i++){
    mpu_dmp_get_data(&pitch, &roll, &yaw);
    angle_list[i] = pitch;
    sum += angle_list[i];
  }
  float aver_angle = sum / WINDOW_SIZE;
  return aver_angle;
}
