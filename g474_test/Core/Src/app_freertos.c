/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mpu.h"
#include "oled.h"
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
static float angle;
extern u8g2_t u8g2;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for Get_Angle */
osThreadId_t Get_AngleHandle;
const osThreadAttr_t Get_Angle_attributes = {
  .name = "Get_Angle",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 256 * 4
};
/* Definitions for Oled_Display */
osThreadId_t Oled_DisplayHandle;
const osThreadAttr_t Oled_Display_attributes = {
  .name = "Oled_Display",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 256 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void Get_Angle_Task(void *argument);
void Oled_Display_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of Get_Angle */
  Get_AngleHandle = osThreadNew(Get_Angle_Task, NULL, &Get_Angle_attributes);

  /* creation of Oled_Display */
  Oled_DisplayHandle = osThreadNew(Oled_Display_Task, NULL, &Oled_Display_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    // HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Get_Angle_Task */
/**
* @brief Function implementing the Get_Angle thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Get_Angle_Task */
void Get_Angle_Task(void *argument)
{
  /* USER CODE BEGIN Get_Angle_Task */
      // if (mpu_dmp_get_data(&pitch, &roll, &yaw) == 0) {
    //   uart_printf("pitch:%4.1f\n", -pitch);
    //   ssd1306_printf(Font_11x18, "ang:%3.1f", -pitch);
    //   ssd1306_SetCursor(0, 0);
    //   HAL_Delay(50);
    // }
  /* Infinite loop */
  for(;;)
  {
    angle = Angle_Get();
    osDelay(30);
  }
  /* USER CODE END Get_Angle_Task */
}

/* USER CODE BEGIN Header_Oled_Display_Task */
/**
* @brief Function implementing the Oled_Display thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Oled_Display_Task */
void Oled_Display_Task(void *argument)
{
  /* USER CODE BEGIN Oled_Display_Task */ 
  u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R2, STM32_byte_i2c, STM32_gpio_and_delay);
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);
  u8g2_ClearDisplay(&u8g2);
  u8g2_SetFont(&u8g2, u8g2_font_wqy16_t_chinese1);
  /* Infinite loop */
  int x1, y1, x2, y2;
  for(;;)
  {
    // u8g2_ClearBuffer(&u8g2);
    u8g2_ClearBuffer(&u8g2);
    u8g2_printf(&u8g2, 0, 30, "ang:%3.1f", angle);
    x1 = 95 - 25 * cos(angle / 57.3);
    y1 = 30 + 25 * sin(angle / 57.3);
    x2 = 95 + 25 * cos(angle / 57.3);
    y2 = 30 - 25 * sin(angle / 57.3);
    u8g2_DrawCircle(&u8g2, 95, 30, 25, U8G2_DRAW_ALL);
    u8g2_DrawLine(&u8g2, x1, y1, x2, y2);
    u8g2_SendBuffer(&u8g2);
    osDelay(10);
  }
  /* USER CODE END Oled_Display_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

