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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define SCHEMES_COUNT 3

#define SPEED_MIN_MS 50
#define SPEED_MAX_MS 1000
#define SPEED_STEP_MS 50

volatile uint8_t current_scheme = 0;
volatile uint8_t blinking_enabled = 1;
volatile uint32_t blink_period_ms = 200;

uint32_t last_blink_time = 0;
uint8_t step = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void LEDs_All_Off(void)
{
    HAL_GPIO_WritePin(LD_GREEN_GPIO_Port,  LD_GREEN_Pin,  GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD_ORANGE_GPIO_Port, LD_ORANGE_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD_RED_GPIO_Port,    LD_RED_Pin,    GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD_BLUE_GPIO_Port,   LD_BLUE_Pin,   GPIO_PIN_RESET);
}

static void LEDs_Set(uint8_t green, uint8_t orange, uint8_t red, uint8_t blue)
{
    HAL_GPIO_WritePin(LD_GREEN_GPIO_Port,  LD_GREEN_Pin,  green  ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD_ORANGE_GPIO_Port, LD_ORANGE_Pin, orange ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD_RED_GPIO_Port,    LD_RED_Pin,    red    ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD_BLUE_GPIO_Port,   LD_BLUE_Pin,   blue   ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void Blink_Task(void)
{
    if (!blinking_enabled)
    {
        LEDs_All_Off();
        return;
    }

    uint32_t now = HAL_GetTick();

    if ((now - last_blink_time) < blink_period_ms)
    {
        return;
    }

    last_blink_time = now;

    switch (current_scheme)
    {
        case 0:
            HAL_GPIO_TogglePin(LD_GREEN_GPIO_Port,  LD_GREEN_Pin);
            HAL_GPIO_TogglePin(LD_ORANGE_GPIO_Port, LD_ORANGE_Pin);
            HAL_GPIO_TogglePin(LD_RED_GPIO_Port,    LD_RED_Pin);
            HAL_GPIO_TogglePin(LD_BLUE_GPIO_Port,   LD_BLUE_Pin);
            break;

        case 1:
            LEDs_All_Off();

            if (step == 0) HAL_GPIO_WritePin(LD_GREEN_GPIO_Port,  LD_GREEN_Pin,  GPIO_PIN_SET);
            if (step == 1) HAL_GPIO_WritePin(LD_ORANGE_GPIO_Port, LD_ORANGE_Pin, GPIO_PIN_SET);
            if (step == 2) HAL_GPIO_WritePin(LD_RED_GPIO_Port,    LD_RED_Pin,    GPIO_PIN_SET);
            if (step == 3) HAL_GPIO_WritePin(LD_BLUE_GPIO_Port,   LD_BLUE_Pin,   GPIO_PIN_SET);

            step = (step + 1) % 4;
            break;

        case 2:
            if (step == 0)
            {
                LEDs_Set(1, 0, 1, 0);
                step = 1;
            }
            else
            {
                LEDs_Set(0, 1, 0, 1);
                step = 0;
            }
            break;

        default:
            current_scheme = 0;
            break;
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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  Blink_Task();
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LD_GREEN_Pin|LD_ORANGE_Pin|LD_RED_Pin|LD_BLUE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BTN_Blue_Pin */
  GPIO_InitStruct.Pin = BTN_Blue_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BTN_Blue_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD_GREEN_Pin LD_ORANGE_Pin LD_RED_Pin LD_BLUE_Pin */
  GPIO_InitStruct.Pin = LD_GREEN_Pin|LD_ORANGE_Pin|LD_RED_Pin|LD_BLUE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : BTN_SLOW_DOWN_Pin BTN_ON_OFF_Pin */
  GPIO_InitStruct.Pin = BTN_SLOW_DOWN_Pin|BTN_ON_OFF_Pin|BTN_SPEED_UP_Pin|BTN_NEXT_SCHEME_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : BTN_PREV_SCHEME_Pin */
  GPIO_InitStruct.Pin = BTN_PREV_SCHEME_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BTN_PREV_SCHEME_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    static uint32_t last_irq_tick[16] = {0};
    uint32_t now = HAL_GetTick();

    uint8_t pin_index = 0;
    while (((GPIO_Pin >> pin_index) & 1U) == 0U)
    {
        pin_index++;
    }

    if ((now - last_irq_tick[pin_index]) < 150)
    {
        return;
    }

    last_irq_tick[pin_index] = now;

    // SWT1 LEFT = previous scheme = PC11
    if (GPIO_Pin == GPIO_PIN_11)
    {
        current_scheme = (current_scheme + SCHEMES_COUNT - 1) % SCHEMES_COUNT; // Previous scheme in a circular sequence
        step = 0;
        LEDs_All_Off();
    }

    // SWT3 RIGHT = next scheme = PC9
    else if (GPIO_Pin == GPIO_PIN_9)
    {
        current_scheme = (current_scheme + 1) % SCHEMES_COUNT; // Same here but in other direction
        step = 0;
        LEDs_All_Off();
    }

    // SWT5 UP = speed up = PC8
    else if (GPIO_Pin == GPIO_PIN_8)
    {
        if (blink_period_ms > SPEED_MIN_MS)
        {
            blink_period_ms -= SPEED_STEP_MS;
        }
    }

    // SWT4 DOWN = slow down = PC6
    else if (GPIO_Pin == GPIO_PIN_6)
    {
        if (blink_period_ms < SPEED_MAX_MS)
        {
            blink_period_ms += SPEED_STEP_MS;
        }
    }

    // SWT2 CENTER = ON/OFF = PA15
    else if (GPIO_Pin == GPIO_PIN_15)
    {
        blinking_enabled ^= 1; // Denys, I used here xor cause its fast(depends on compiler) way to negate*

        if (!blinking_enabled)
        {
            LEDs_All_Off();
        }
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
