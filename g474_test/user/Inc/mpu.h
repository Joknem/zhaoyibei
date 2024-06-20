#ifndef __MPU_H_
#define __MPU_H_

#include "main.h"
#include <stdint.h>
#include "i2c.h"
#include "inv_mpu.h"

#define MPU_ADDR 0xd0
#define MPU_RA_GYRO_XOUT_H 0x43
#define MPU_RA_ACCEL_XOUT_H 0x3B
#define MPU_RA_MAG 0x00
#define WINDOW_SIZE 6

typedef struct {
  int16_t x;
  int16_t y;
  int16_t z;
} MPU_data;

void MPU_Init();
void MPU_ReadMulBytes(uint8_t reg, uint8_t length, uint8_t *buf);
float Angle_Get();
uint8_t MPU_Write_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);
uint8_t MPU_Read_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);
uint8_t MPU_Write_Byte(uint8_t addr, uint8_t reg, uint8_t data);
uint8_t MPU_Read_Byte(uint8_t addr, uint8_t reg);
#endif
