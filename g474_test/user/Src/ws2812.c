#include "ws2812.h"
#include "tim.h"

uint16_t send_Buf[NUM];
extern void uart_printf(char *fmt, ...);

void WS_Load(void) {
  HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, send_Buf, NUM);
}

void WS_WriteAll_RGB(uint8_t n_R, uint8_t n_G, uint8_t n_B) {
  uint16_t i, j;
  uint16_t dat[24];
  for (i = 0; i < 8; i++) {
    dat[i] = ((n_G & 0x80) ? WS1 : WS0);
    n_G <<= 1;
  }
  for (i = 0; i < 8; i++) {
    dat[i + 8] = ((n_R & 0x80) ? WS1 : WS0);
    n_R <<= 1;
  }
  for (i = 0; i < 8; i++) {
    dat[i + 16] = ((n_B & 0x80) ? WS1 : WS0);
    n_B <<= 1;
  }
  for (i = 0; i < PIXEL_NUM; i++) {
    for (j = 0; j < 24; j++) {
      send_Buf[i * 24 + j] = dat[j];
    }
  }
  for (i = PIXEL_NUM * 24; i < NUM; i++)
    send_Buf[i] = 0;
  WS_Load();
}

void breathing() {
	int i = 0;
  for (i; i < 80; i += 2) {
    WS_WriteAll_RGB(i, i, i);
    HAL_Delay(100);
  }
  for (i; i > 0; i -= 2) {
    WS_WriteAll_RGB(i, i, i);
    HAL_Delay(100);
  }
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
        HAL_TIM_PWM_Stop_DMA(&htim1,TIM_CHANNEL_1);
}