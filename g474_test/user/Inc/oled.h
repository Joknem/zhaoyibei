#ifndef _OLED_H
#define _OLED_H

#include "fonts.h"
#include "i2c.h"
#include "main.h"
#include "stm32g4xx.h"
#include "u8g2.h"

#define SSD1306_ADDRESS 0x78
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64
#define WHITE 0x00
#define BLACK 0x01

#define SSD1306_LOWCOLUMN_START 0x00
#define SSD1306_HIGHCOLUMN_START 0x10
#define SSD1306_HOR_MODE 0x00
#define SSD1306_VER_MODE 0x01
#define SSD1306_SET_MEMADDR_MODE 0x20
#define SSD1306_PAGEADDR_MODE 0x10
#define SSD1306_VERADDR_MODE 0x10
#define SSD1306_HORADDR_MODE 0x00
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR 0x22
#define SSD1306_SET_STARTLINE 0x40
#define SSD1306_SET_CONTRAST 0x81
#define SSD1306_SET_CHARGEPUMP 0x8D
#define SSD1306_SEGMENTREMAP0 0xA0
#define SSD1306_SEGMENTREMAP1 0xA1
#define SSD1306_DISPLAYALLON_RESUME 0xA4 // output ram content
#define SSD1306_DISPLAYALLON 0xA5        // output ignore ram
#define SSD1306_NORMAL_DISPLAY 0xA6
#define SSD1306_INVER_TDISPLAY 0xA7
#define SSD1306_MULT_RATIO 0xA8
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_PAGESTART_B0 0xB0
#define SSD1306_PAGESTART_B1 0xB1
#define SSD1306_PAGESTART_B2 0xB2
#define SSD1306_PAGESTART_B3 0xB3
#define SSD1306_PAGESTART_B4 0xB4
#define SSD1306_PAGESTART_B5 0xB5
#define SSD1306_PAGESTART_B6 0xB6
#define SSD1306_PAGESTART_B7 0xB7
#define SSD1306_SCAN_DIRECTION1 0xC0
#define SSD1306_SCAN_DIRECTION2 0xC8
#define SSD1306_SETDISPLAY_OFFSET                                              \
  0xD3 // two byte command, must write offset after send this command
#define SSD1306_SETCLOCK_DIVIDE_RATIO                                          \
  0xD5 // Set the divide ratio to generate DCLK (Display Clock) from CLK
#define SSD1306_SET_PRECHARGE_PERIOD                                           \
  0xD9 // This command is used to set the duration of the pre-charge period.
#define SSD1306_SET_COM_CONFIG                                                 \
  0xDA // This command sets the COM signals pin configuration to match the OLED
       // panel hardware layout.
#define SSD1306_SET_DESELECT                                                   \
  0xDB // This command adjusts the V COMH regulator output.

typedef struct {
  uint16_t CurrentX;
  uint16_t CurrentY;
  uint8_t Inverted;
  uint8_t Initialized;
} SSD1306_t;

void ssd1306_UpdateScreen(void);
void ssd1306_DrawPixel(uint8_t x, uint8_t y, uint8_t color);
char ssd1306_WriteChar(char ch, FontDef Font, uint8_t color);
int ssd1306_WriteString(const char *str, int size, FontDef Font, uint8_t color);
void ssd1306_SetCursor(uint8_t x, uint8_t y);
void ssd1306_printf(FontDef Font, char *fmt, ...);
uint8_t STM32_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                             void *arg_ptr);
uint8_t STM32_byte_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                       void *arg_ptr);
void u8g2_printf(u8g2_t *u8g2, uint8_t x, uint8_t y, const char *fmt, ...);
#endif
