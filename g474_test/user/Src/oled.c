#include "oled.h"
#include <stdarg.h>
#include <stdio.h>
#include "cmsis_os2.h"

extern void uart_printf(char *fmt, ...);

static char SSD1306_BUFFER[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
SSD1306_t SSD1306;

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c){

};

static uint8_t ssd1306_write(uint8_t command) {
  // return HAL_I2C_Mem_Write_DMA(&hi2c2, SSD1306_ADDRESS, 0x00, 1, &command,
  // 1);
  HAL_I2C_Mem_Write(&hi2c2, SSD1306_ADDRESS, 0x00, 1, &command, 1, 10);
}

void ssd1306_UpdateScreen() {
  uint8_t i;

  for (i = 0; i < 8; i++) {
    ssd1306_write(0xB0 + i);
    ssd1306_write(0x00);
    ssd1306_write(0x10);
    HAL_I2C_Mem_Write(&hi2c2, SSD1306_ADDRESS, 0x40, 1,
                      &SSD1306_BUFFER[SSD1306_WIDTH * i], SSD1306_WIDTH, 100);
  }
}

uint8_t ssd1306_init(void) {
  int ret = 0;
  // 关闭OLED
  ret += ssd1306_write(SSD1306_DISPLAYOFF);
  // 设置内存寻址模式
  ret += ssd1306_write(SSD1306_SET_MEMADDR_MODE);
  ret += ssd1306_write(SSD1306_PAGEADDR_MODE);

  ret += ssd1306_write(SSD1306_PAGESTART_B0);
  // configure direction, A1左右反, C8上下正常
  ret += ssd1306_write(SSD1306_SEGMENTREMAP0);
  ret += ssd1306_write(SSD1306_SCAN_DIRECTION1);
  //
  ret += ssd1306_write(SSD1306_LOWCOLUMN_START);
  ret += ssd1306_write(SSD1306_HIGHCOLUMN_START);
  // 设置屏幕(GDDRAM)起始行
  ret += ssd1306_write(SSD1306_SET_STARTLINE);
  // 设置对比度
  ret += ssd1306_write(SSD1306_SET_CONTRAST);
  ret += ssd1306_write(0xFF); // Set contrast control register to maximum
  // 正常显示颜色，不反显
  ret += ssd1306_write(SSD1306_NORMAL_DISPLAY);
  // 全局显示开启
  ret += ssd1306_write(SSD1306_DISPLAYALLON_RESUME); // output ram content
  // 设置显示偏移
  ret += ssd1306_write(SSD1306_SETDISPLAY_OFFSET);
  ret += ssd1306_write(0x00); // no offset
  // 设置时钟分频因子
  ret += ssd1306_write(SSD1306_SETCLOCK_DIVIDE_RATIO);
  ret += ssd1306_write(
      0xF0); // Set divide ratio to generate DCLK (Display Clock) from CLK
  // 设置预充电时间
  ret += ssd1306_write(SSD1306_SET_PRECHARGE_PERIOD);
  ret += ssd1306_write(0x22);
  // 设置列引脚配置
  ret += ssd1306_write(SSD1306_SET_COM_CONFIG);
  ret += ssd1306_write(0x12);
  // 设置VCOM调节器的输出
  ret += ssd1306_write(SSD1306_SET_DESELECT);
  ret += ssd1306_write(0x20); // 0x20,0.77xVcc
  // 设置charge pump enable
  ret += ssd1306_write(SSD1306_SET_CHARGEPUMP);
  ret += ssd1306_write(0x14);
  // 开启OLED
  ret += ssd1306_write(SSD1306_DISPLAYON);

  if (ret != 0) {
    return ret;
  }
  ssd1306_Fill(0x00);
  ssd1306_UpdateScreen();
  SSD1306.CurrentX = 0;
  SSD1306.CurrentY = 0;
  SSD1306.Initialized = 1;
  return 0;
}

void ssd1306_DrawPixel(uint8_t x, uint8_t y, uint8_t color) {
  if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
    // Don't write outside the buffer
    return;
  }

  // Check if pixel should be inverted
  if (SSD1306.Inverted) {
    color = ~color;
  }
  // Draw in the correct color
  if (color == WHITE) {
    SSD1306_BUFFER[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
  } else {
    SSD1306_BUFFER[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
  }
}

char ssd1306_WriteChar(char ch, FontDef Font, uint8_t color) {
  uint32_t i, b, j;
  // Check remaining space on current line
  if (SSD1306_WIDTH <= (SSD1306.CurrentX + Font.FontWidth) ||
      SSD1306_HEIGHT <= (SSD1306.CurrentY + Font.FontHeight)) {
    return 0;
  }
  // Translate font to screenbuffer
  for (i = 0; i < Font.FontHeight; i++) {
    b = Font.data[(ch - 32) * Font.FontHeight + i];
    for (j = 0; j < Font.FontWidth; j++) {
      if ((b << j) & 0x8000) {
        ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), color);
      } else {
        ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i),
                          (uint8_t)!color);
      }
    }
  }

  // The current space is now taken
  SSD1306.CurrentX += Font.FontWidth;

  // Return written char for validation
  return ch;
}

int ssd1306_WriteString(const char *str, int size, FontDef Font,
                        uint8_t color) {
  // Write until null-byte
  while (size--) {
    if (ssd1306_WriteChar(*str, Font, color) != *str) {
      return 1;
    }
    str++;
  }
  return 0;
}

void ssd1306_printf(FontDef Font, char *fmt, ...) {
  va_list args;
  int ret;
  static char str[64];
  va_start(args, fmt);
  ret = vsprintf(str, fmt, args);
  va_end(args);
  if (ret > 0) {
    ssd1306_WriteString(str, ret, Font, WHITE);
  }
  ssd1306_UpdateScreen();
}

void ssd1306_SetCursor(uint8_t x, uint8_t y) {
  SSD1306.CurrentX = x;
  SSD1306.CurrentY = y;
}

uint8_t STM32_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                             void *arg_ptr) {
  switch (msg) {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
      break;
   case U8X8_MSG_DELAY_MILLI: // delay arg_int * 1 milli second
      osDelay(1);
      break;
    default:
      return 0;
  }
  return 1;
}

uint8_t STM32_byte_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
  static uint8_t buffer[128]; 
  static uint8_t buf_idx;
  uint8_t *data;
  switch (msg) {
  case U8X8_MSG_BYTE_SEND:
    data = (uint8_t *)arg_ptr;
    while (arg_int > 0) {
      buffer[buf_idx++] = *data;
      data++;
      arg_int--;
    }
    break;
  case U8X8_MSG_BYTE_START_TRANSFER:
    buf_idx = 0;
    break;
  case U8X8_MSG_BYTE_END_TRANSFER:
    HAL_I2C_Master_Transmit(&hi2c2, SSD1306_ADDRESS, buffer, buf_idx, 1000);
    break;
  default:
    return 0;
  }
  return 1;
}

void u8g2_printf(u8g2_t *u8g2, uint8_t x, uint8_t y, const char *fmt, ...){
  va_list args;
  int ret;
  static char str[64];
  va_start(args, fmt);
  ret = vsprintf(str, fmt, args);
  va_end(args);
  if (ret > 0) {
    u8g2_DrawStr(u8g2, x, y, str);
  }
}
