/* USER CODE BEGIN Header */
/**
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "NOKIA5110.h"
#include "FONT_IMAGE.h"
#include"snake_lib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct Point {
	int x, y;
	int x0, y0;
}Point;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi2_tx;

/* USER CODE BEGIN PV */
int snakeLength; // do dai con ran
int score; // diem
const int DIRECTION = 3;
Point Snake[100]; // con ran
Point direction; // huong di
Point Egg; // do an
int check=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void drawSnake();
void initGame();
void moveSnake();
void gameOver();
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == LEFT_Pin)
	{
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		if(direction.x != 3 && direction.y != 0)
		{
			direction.x = -3;
			direction.y = 0;
		}
		for(int i = 0; i < 100000; i++); // Delay
		int timeout;
		while(!HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1) & (timeout < 50000))
		{
			timeout++;
		}
		EXTI->PR |= GPIO_PIN_1;
	}
	if(GPIO_Pin == RIGHT_Pin)
	{
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		if(direction.x != -3 && direction.y != 0)
		{
			direction.x = 3;
			direction.y = 0;
		}
		for(int i = 0; i < 100000; i++); // Delay
		int timeout;
		while(!HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2) & (timeout < 50000))
		{
			timeout++;
		}
		EXTI->PR |= GPIO_PIN_2;
	}
	if(GPIO_Pin == UP_Pin)
	{
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		if(direction.x != 0 && direction.y != 3)
		{
			direction.x = 0;
			direction.y = -3;
		}
		for(int i = 0; i < 100000; i++); // Delay
		int timeout;
		while(!HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3) & (timeout < 50000))
		{
			timeout++;
		}
		EXTI->PR |= GPIO_PIN_3;
	}
	if(GPIO_Pin == DOWN_Pin)
	{
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		if(direction.x != 0 && direction.y != -3)
		{
			direction.x = 0;
			direction.y = 3;
		}
		for(int i = 0; i < 100000; i++); // Delay
		int timeout;
		while(!HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4) & (timeout < 50000))
		{
			timeout++;
		}
		EXTI->PR |= GPIO_PIN_4;
	}
}
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
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
  N5110_Init();
 start_game();
  initGame();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if(check == 0)
		{
			N5110_Clear();
			egg(Egg.x, Egg.y);
			moveSnake();
			N5110_Update();
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, RST_Pin|CE_Pin|DC_Pin|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : RST_Pin CE_Pin DC_Pin PD15 */
  GPIO_InitStruct.Pin = RST_Pin|CE_Pin|DC_Pin|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : LEFT_Pin RIGHT_Pin UP_Pin DOWN_Pin */
  GPIO_InitStruct.Pin = LEFT_Pin|RIGHT_Pin|UP_Pin|DOWN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}

/* USER CODE BEGIN 4 */
bool checkPoint()
{
	for(int i = 0; i < snakeLength; i++)
	{
		if(Egg.x == Snake[i].x && Egg.y == Snake[i].y)
			return false;
	}
	return true;
}

void initGame()
{
	snakeLength = 4;
	score = 0;
	// tao snake
	for (int i = 1; i <= snakeLength; i++)
	{
		Snake[snakeLength-i].x = i*3;
		Snake[snakeLength-i].y = 6;
	}
	Snake[0].x0 = Snake[0].x;
	Snake[0].y0 = Snake[0].y;

	// tao egg
	do {
			Egg.x = (rand()%23 + 1) *3;
			Egg.y = (rand()%15 + 1) *3;
	}while(checkPoint() == false);

	// Huong di
	direction.x = 3;
	direction.y = 0;
}

void drawSnake()
{
	for (int i = 0; i < snakeLength; i++)
	{
		snake(Snake[i].x, Snake[i].y);
	}
}

void moveSnake()
{
	for (int i = 0; i < snakeLength; i++)
	{
		if(i == 0)
		{
			Snake[0].x0 = Snake[0].x;
			Snake[0].y0 = Snake[0].y;
			Snake[0].x += direction.x;
			Snake[0].y += direction.y;
		}
		else
		{
			Snake[i].x0 = Snake[i].x;
			Snake[i].y0 = Snake[i].y;
			Snake[i].x = Snake[i-1].x0;
			Snake[i].y = Snake[i-1].y0;
		}

		if(Snake[i].x >= 84) Snake[i].x -= 84;
		if(Snake[i].x <= 0) Snake[i].x += 84;
		if(Snake[i].y >= 48) Snake[i].y -= 48;
		if(Snake[i].y <= 0) Snake[i].y += 48;

		if(i != 0 && Snake[0].x == Snake[i].x && Snake[0].y == Snake[i].y)
			gameOver();
	}
	HAL_Delay(200);

	if(Snake[0].x == Egg.x && Snake[0].y == Egg.y)
	{
		Snake[snakeLength].x = Snake[snakeLength-1].x0;
		Snake[snakeLength].y = Snake[snakeLength-1].y0;
		snakeLength++;
		score++;
		do {
			Egg.x = (rand()%23 + 1) *3;
			Egg.y = (rand()%15 + 1) *3;
		}while(checkPoint() == false);
	}
	drawSnake();
}

void gameOver()
{
	check = 1;
	char *str_core;
	int div = score%10;
	str_core[0] = div + 0x30;
	int i=1;
	while(score/10)
	{
		score = score/10;
		div = score%10;
		str_core[i] = div + 0x30;
		i++;
	}
	N5110_Clear();
	N5110_SetFont(SmallFont);
	N5110_PrintStr(N5110_MAX_WIDTH/2-(4.5*6), 12, "Game Over", true);
	N5110_PrintStr(N5110_MAX_WIDTH/2-(1*6), 25, str_core,true);
	N5110_Update();
	HAL_Delay(20000);
	initGame();
	check = 0;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
