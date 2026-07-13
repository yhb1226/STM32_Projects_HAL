/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "key.h"
#include "iic.h"
#include "mpu6050.h"
#include "motor.h"
#include "encoder.h"
#include "serial.h"
#include "blueserial.h"
#include <stdint.h>
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
  int16_t AX,AY,AZ,GX,GY,GZ;
  int number = 0,KeyNum = 0;
  int8_t PWM_L=30,PWM_R=30;
  int speedl = 0,speedr = 0;
  extern volatile uint8_t Serial_RxData;
  extern volatile uint8_t Serial_RxFlag;
  extern volatile uint8_t BlueSerial_RxFlag;
  extern volatile uint8_t BlueSerial_RxByte;   // 单字节接收缓冲区
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  //初始化放在开启时钟之后和GPIO初始化之后
  OLED_Init();
  MPU6050_Init();
  
  //启动定时器中断
  HAL_TIM_Base_Start_IT(&htim1);
  //HAL_TIM_Base_Start(&htim2);   // 仅启动计数，不启动中断
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
  HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
  // 正确写法：蓝牙用 BlueSerial_RxByte，普通串口用 Serial_RxData
  HAL_UART_Receive_IT(&huart1, (uint8_t *)&BlueSerial_RxByte, 1); // 蓝牙
  HAL_UART_Receive_IT(&huart2, (uint8_t *)&Serial_RxData, 1);     // 有线串口
  Serial_SendString("Hello");
  Serial_Printf("world");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    KeyNum = key_get();

    if (KeyNum!=0)
    {
			 if(KeyNum==1){PWM_L += 10;}
			 if(KeyNum==2){PWM_L += 10;}
			 if(KeyNum==3){PWM_R += 10;}
			 if(KeyNum==4){PWM_R += 10;}

    }
    BlueSerial_Printf("1");
    HAL_Delay(1000);
    // if(Serial_GetRxFlag() == 1)
    // {
    //   uint8_t RxData = Serial_GetRxData();
    //   OLED_Printf(0, 0, OLED_6X8, "RxData:%02X", RxData);
    //   OLED_Update();
    // }

    //  Serial_Printf("Hello from STM32\r\n");
    //   HAL_Delay(1000);   // 每隔1秒发送一次


    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 50);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 50);

    HAL_GPIO_WritePin(MOTOR_LEFT1_GPIO_Port, MOTOR_LEFT1_Pin, GPIO_PIN_SET);//PB12置高电平
    HAL_GPIO_WritePin(MOTOR_LEFT2_GPIO_Port, MOTOR_LEFT2_Pin, GPIO_PIN_RESET);//PB13置低电平
    // Motor_SetPWM(1, PWM_L);
    // Motor_SetPWM(2, PWM_R);

    //  OLED_ShowNum(0,0,speedl,6,OLED_6X8);
    //  OLED_ShowNum(0,0,speedr,6,OLED_6X8);
	  
    OLED_Update();










    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{   static uint8_t count;
  //判断是否为TIM1的中断
    if (htim->Instance == TIM1)   // 判断是否为 TIM1 触发的更新中断
    {
      MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
      count++;
      if(count>=60)
      {
        speedl = Encoder_Get(1); 
        speedr = Encoder_Get(2);
      }
      Key_loop();            
    }
}


















void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    // ==================== USART1：蓝牙模块 ====================
    if (huart->Instance == USART1)
    {
        static uint8_t RxState = 0;
        static uint8_t pRxPacket = 0;
        uint8_t RxData = BlueSerial_RxByte;

        if (RxState == 0)
        {
            if (RxData == '[' && BlueSerial_RxFlag == 0)
            {
                RxState = 1;
                pRxPacket = 0;
            }
        }
        else if (RxState == 1)
        {
            if (RxData == ']')
            {
                RxState = 0;
                BlueSerial_RxPacket[pRxPacket] = '\0';
                BlueSerial_RxFlag = 1;
            }
            else
            {
                if (pRxPacket < BLUESERIAL_RX_PACKET_SIZE - 1)
                {
                    BlueSerial_RxPacket[pRxPacket] = RxData;
                    pRxPacket++;
                }
            }
        }
        // 继续接收下一个字节
        HAL_UART_Receive_IT(&huart1, (uint8_t *)&BlueSerial_RxByte, 1);
    }
    // ==================== USART2：普通串口 ====================
    else if (huart->Instance == USART2)
    {
        // 原来普通串口的中断处理：收一个字节，存到 Serial_RxData，置标志
        Serial_RxData = (uint8_t)(huart->Instance->DR);  // 或者用 __HAL_UART_GET_DATA(huart)
        Serial_RxFlag = 1;
        // 继续接收下一个字节
        HAL_UART_Receive_IT(&huart2, (uint8_t *)&Serial_RxData, 1);
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
