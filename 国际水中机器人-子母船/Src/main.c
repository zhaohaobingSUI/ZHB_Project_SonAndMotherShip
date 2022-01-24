/* USER CODE BEGIN Header */
/**
PF1 I1 CLK
PF0 I2 EN
PE5 J1 CW

  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_uart.h"
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
/* Private variables ---------------------------------------------------------*/
extern rc_info_t rc;
char buf[200];
int16_t value;
int count;
void run_mod(int S,int speed);
void motor_init(void);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_DMA_Init();
  MX_USART6_UART_Init();
  MX_USART1_UART_Init();
  MX_TIM8_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	led_off();
	/* open dbus uart receive it */
	dbus_uart_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  { 
		

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
	//sprintf(buf, "CH1: %4d  CH2: %4d  CH3: %4d  CH4: %4d  SW1: %1d  SW2: %1d \n", rc.ch1, rc.ch2, rc.ch3, rc.ch4, rc.sw1, rc.sw2);
	HAL_UART_Transmit(&huart6, (uint8_t *)buf, (COUNTOF(buf) - 1), 55);
	HAL_Delay(50);	
		
	if (count==10)
	{
		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
		count=0;
	}
	//value = rc.ch1*2;
	//__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, value);
	if(rc.sw1==1)
	{
		 //__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 500);
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, 500);
       // __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, 500);
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, 500);
        __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, 500);//motor 1   1350 zheng 
        __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, 500);//servo 1
		    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, 500);//motor 2   1350 zheng 
				__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 2500);
				__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, 2500);
				__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 5000);//high-up servo1
				HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_SET);
		}
	if(rc.sw1==2)
	{
				 //__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 20000);
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, 2000);
		   // __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, 1500);
        //__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, 1350);
        //__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, 2000);//motor 1   1350 zheng   1500mid  2000 fan 
        __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, 2500);//servo 1
				__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 1500);
				__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, 1500);
				__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 3000);//high-up servo1

	}
	if(rc.sw1==3)
	{
		    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, 1500-rc.ch2/33.*50+0.5);//motor 1
				__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, 1500+rc.ch4/33.*50+0.5);//motor 2
				if(rc.sw2==1)
				{
					__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_4, 2500);//servo 2
				}
				if(rc.sw2==2)
				{
					__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_4, 1200);//servo 2
				}
	}
	
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void run_mod(int S,int speed)
{
	while(S--)
	{
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_Delay(speed);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_SET);
		HAL_Delay(speed);
	}
}

void motor_init(void)
{
		__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_4, 1200);
		__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, 2500);
		__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, 500);
		HAL_Delay(5000);
		__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, 1500);
		__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, 500);
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
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
