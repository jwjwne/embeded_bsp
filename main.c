/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "bsp_i2c.h"
#include "bsp_usart1.h"
#include "bsp_led.h"

#define  DATA_Size			256
#define  EEP_Firstpage      0x00
uint8_t I2c_Buf_Write[DATA_Size];
uint8_t I2c_Buf_Read[DATA_Size];
uint8_t I2C_Test(void);
void SystemClock_Config(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_I2CX_Init();
	MX_LEDX_Init();
	MX_USARTX_UART_Init();
	printf("\r\n 这是一个I2C外设(AT24C02)读写测试例程 \r\n");
	if(I2C_Test() ==1)
  {
      LED(ON);
  }
  else
  {
		while(1)
		{
      LED(ON);
			HAL_Delay(10);
			LED(OFF);
			HAL_Delay(10);			
		}
  }
  while (1)
  {
		
  }
}

uint8_t I2C_Test(void)
{
	uint16_t i;
	for(i=0;i<DATA_Size;i++)
	{
		I2c_Buf_Write[i]=i;
		printf("0x%02X ", I2c_Buf_Write[i]);
		if(i%16 == 15)    
		printf("\n\r");    
	}
	I2C_EE_BufferWrite(I2c_Buf_Write,EEP_Firstpage,DATA_Size);
	printf("数据输入完成");
	I2C_EE_BufferRead(I2c_Buf_Read,EEP_Firstpage,DATA_Size);
	for(i=0;i<DATA_Size;i++)
	{
		if(I2c_Buf_Read[i]!=I2c_Buf_Write[i])
		{
			printf("数据对比错误");
			printf("0x%02X ", I2c_Buf_Read[i]);
			printf("%d",i);
			return 0;
		}
		printf("0x%02X ", I2c_Buf_Read[i]);
		//printf("%d",i);
		if(i%16 == 15)    
		printf("\n\r");
	}
	printf("I2C(AT24C02)读写测试成功");
	return 1;
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

