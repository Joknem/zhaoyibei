#include "mpu.h"
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

uint8_t MPU_Write_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf) {
  HAL_I2C_Mem_Write(&hi2c1, (addr << 1), reg, I2C_MEMADD_SIZE_8BIT, buf, len,
                    0xfff);
  return 0;
}

uint8_t MPU_Read_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf) {
  HAL_I2C_Mem_Read(&hi2c1, (addr << 1), reg, I2C_MEMADD_SIZE_8BIT, buf, len,
                  0xfff);
  return 0;
}

uint8_t MPU_Write_Byte(uint8_t addr, uint8_t reg, uint8_t data) {
  HAL_I2C_Mem_Write(&hi2c1, (addr << 1), reg, I2C_MEMADD_SIZE_8BIT, &data, 1,
                    0xfff);
  return 0;
}

uint8_t MPU_Read_Byte(uint8_t addr, uint8_t reg) {
  uint8_t res;
  HAL_I2C_Mem_Read(&hi2c1, (addr << 1), reg, I2C_MEMADD_SIZE_8BIT, &res, 1,
                  0xfff);
  return res;
}

float Cal_Angle(){
}

float Angle_Get() {
  float pitch, roll, yaw;
  float angle_list1[WINDOW_SIZE];
  float angle_list2[WINDOW_SIZE];
  float sum1, sum2;
  switch_dev(0);
  for(int i = 0; i < WINDOW_SIZE; i++){
    mpu_dmp_get_data(&pitch, &roll, &yaw);
    angle_list1[i] = pitch;
    sum1 += angle_list1[i];
  }
  float aver1 = sum1 / WINDOW_SIZE;
  switch_dev(1);
  for(int i = 0; i < WINDOW_SIZE; i++){
    mpu_dmp_get_data(&pitch, &roll, &yaw);
    angle_list1[i] = pitch;
    sum2 += angle_list2[i];
  }
  float aver2 = sum2 / WINDOW_SIZE;
  return aver1;
}
