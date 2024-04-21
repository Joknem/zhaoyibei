#ifndef MPU_H_
#define MPU_H_

#include "main.h"
#include <stdint.h>

#define MPU_ADDR 0xd0
#define MPU_RA_GYRO_XOUT_H 0x43
#define MPU_RA_ACCEL_XOUT_H 0x3B

typedef struct {
  int16_t x;
  int16_t y;
  int16_t z;
} MPU_data;

void MPU_Init();
void MPU_ReadMulBytes(uint8_t reg, uint8_t length, uint8_t *buf);

#endif
