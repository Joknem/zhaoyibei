#include "main.h"
#include "stm32g4xx_hal_gpio.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

extern void uart_printf(char *fmt, ...);
extern float angle;
char chcmd[64];
uint16_t cmd_i = 0;

void CmdProcessing(UART_HandleTypeDef *huart) {
  static uint16_t cmd_start = 0;

  if (chcmd[cmd_i] == '\n') {
    chcmd[cmd_i + 1] = 0;
    switch (chcmd[cmd_start]) {
    case 'r': {
      HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
      angle = 0.0;
    } break;
    default:
      break;
    }
    cmd_i = 0;
  } else {
    cmd_i++;
  }
  HAL_UART_Receive_IT(huart, (uint8_t *)&chcmd[cmd_i], 1);
}
