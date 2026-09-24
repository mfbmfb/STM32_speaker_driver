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
#define BUZZER_TIM &htim1
#define BUZZER_CHANNEL TIM_CHANNEL_1

// Notaların Frekansları (Hz)
#define NOTE_C4  262  // Do
#define NOTE_D4  294  // Re
#define NOTE_E4  330  // Mi
#define NOTE_F4  349  // Fa
#define NOTE_G4  392  // Sol
#define NOTE_A4  440  // La
#define NOTE_B4  494  // Si
#define NOTE_C5  523  // Do (İnce)
#define REST     0    // Sessizlik

#define NOTE_D5  587  // İnce Re
#define NOTE_E5  659  // İnce Mi

const uint16_t long_melody[] = {
  // --- 1. KITAK (Giriş & Çiçekler) ---
  // Daha dün annemizin kollarında yaşarken
  NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4,
  // Çiçekli bahçemizin yollarında koşarken
  NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4,
  NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4,
  // Şimdi okullu olduk sınıfları doldurduk
  NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4,

  REST, // Bölüm arası kısa es

  // --- 2. KITA (Okul Sevgisi & Tekrar) ---
  // Sevinçliyiz hepimiz yaşasın okulumuz
  NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4,
  NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4,
  // Okul bizim yuvamız, insanlık kapımız
  NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4,

  REST, // Final öncesi es

  // --- NAKARAT / FİNAL ÇIKIŞI ---
  NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4,
  NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4
};

const uint16_t ari_viz_melody[] = {
	// --- 1. BÖLÜM: Yaz geldi çiçekler açtı ---
	NOTE_C4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_F4,
	REST,

	// --- 2. BÖLÜM: Arı vız vız vız, arı vız vız vız ---
	NOTE_B4, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_E5,
	REST,

	// --- 3. BÖLÜM: Arı vız vız vız diye dolaşır ---
	NOTE_C4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_F4,
	REST,

	// --- 4. BÖLÜM: Önce menekşeyi koklar, sonra gülü emeriz ---
	NOTE_B4, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_C5,
	REST,

	// --- 5. BÖLÜM (YENİ): Çiçek balını, çiçek balını, biz çok severiz ---
	NOTE_C4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_F4,
	NOTE_B4, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_C5
};

// Nota Süreleri (ms)
const uint16_t ari_viz_durations[] = {
	// 1. Bölüm
	300, 300, 300, 300, 300, 300, 400, 600,
	200,

	// 2. Bölüm
	300, 300, 300, 300, 300, 300, 800,
	200,

	// 3. Bölüm
	300, 300, 300, 300, 300, 300, 400, 600,
	200,

	// 4. Bölüm
	300, 300, 300, 300, 300, 300, 800,
	300,

	// 5. Bölüm (Çiçek Balı)
	300, 300, 300, 300, 300, 300, 400, 600,
	300, 300, 300, 300, 300, 300, 900
};

// Nota Süreleri (ms)
const uint16_t long_durations[] = {
  // 1. KITA
  350, 350, 350, 350, 350, 350, 700,
  350, 350, 350, 350, 350, 350, 700,
  350, 350, 350, 350, 350, 350, 700,
  350, 350, 350, 350, 350, 350, 700,
  350, 350, 350, 350, 350, 350, 700,
  350, 350, 350, 350, 350, 350, 700,

  500, // Ara es süresi

  // 2. KITA
  350, 350, 350, 350, 350, 350, 700,
  350, 350, 350, 350, 350, 350, 700,
  350, 350, 350, 350, 350, 350, 700,
  350, 350, 350, 350, 350, 350, 700,

  500, // Ara es süresi

  // NAKARAT / FİNAL
  350, 350, 350, 350, 350, 350, 700,
  350, 350, 350, 350, 350, 350, 700,
  350, 350, 350, 350, 350, 350, 1000
};

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
TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
void Play_Tone(uint16_t frequency) {
  if (frequency == 0) {
    __HAL_TIM_SET_COMPARE(BUZZER_TIM, BUZZER_CHANNEL, 0); // Sessizlik
    return;
  }

  // Timer giriş saatimiz 1 MHz olduğu için Period = (1.000.000 / Frekans) - 1
  uint32_t period = (1000000 / frequency) - 1;

  __HAL_TIM_SET_AUTORELOAD(BUZZER_TIM, period);
  __HAL_TIM_SET_COMPARE(BUZZER_TIM, BUZZER_CHANNEL, (period + 1) / 2); // %50 Duty Cycle
}


void Play_LongMelody(void) {
  HAL_TIM_PWM_Start(BUZZER_TIM, BUZZER_CHANNEL);

  uint16_t total_notes = sizeof(long_melody) / sizeof(long_melody[0]);

  for (int i = 0; i < total_notes; i++) {
    Play_Tone(long_melody[i]);
    HAL_Delay(long_durations[i]);

    // Notaların birbirine yapışmaması ve belirginleşmesi için staccato es
    Play_Tone(REST);
    HAL_Delay(40);
  }

  HAL_TIM_PWM_Stop(BUZZER_TIM, BUZZER_CHANNEL);
}

void Play_AriVizVizVizMelody(void) {
  HAL_TIM_PWM_Start(BUZZER_TIM, BUZZER_CHANNEL);

  uint16_t total_notes = sizeof(ari_viz_melody) / sizeof(ari_viz_melody[0]);

  for (int i = 0; i < total_notes; i++) {
    Play_Tone(ari_viz_melody[i]);
    HAL_Delay(ari_viz_durations[i]);

    // Notaların birbirine yapışmaması ve belirginleşmesi için staccato es
    Play_Tone(REST);
    HAL_Delay(40);
  }

  HAL_TIM_PWM_Stop(BUZZER_TIM, BUZZER_CHANNEL);
}
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
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  //Play_Melody();
  //Play_LongMelody();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  Play_AriVizVizVizMelody();
	  HAL_Delay(2000);
	  Play_LongMelody();
	  HAL_Delay(2000);
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

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 71;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1000;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

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
