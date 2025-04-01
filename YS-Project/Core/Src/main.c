/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
#include "dma.h"
#include "gpio.h"
#include "iwdg.h"
#include "usart.h"

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define ADDR_CodeWriteFlag 0X08070000 // ������ת��־λ�����ַ
#define ADDR_JumpToIAPFlag 0X08070001
#define FLASH_APP1_ADDR 0x08004000  // Bootloader ���һ�����õ�ַ
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick.
   */
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
  MX_IWDG_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_SET);
  // HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_RESET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_FLASH_Unlock(); ///*flash��������Ϊ��Ҫ����flash*/
  uint16_t IAPFlagBuf[2];
  uint16_t RxDataCount = 0;   // ���ڽ��յ������ݼ���
  uint16_t RxDataLength = 0;  // ���ڽ��յ������ݳ���
  uint16_t AppCodeLength = 0; // ���յ���app���볤��
  uint8_t RxCmdFlag = 0;
  uint8_t AppRunFlag = 0; // Ӧ�ó������б�־
  uint16_t JumpToAPPFlag; // ��ת��APP�����־λ
  uint16_t JumpToIAPFlag; // ��ת��IAP��־λ
  JumpToAPPFlag = STMFLASH_ReadHalfWord(ADDR_CodeWriteFlag); // ��ȡAPPд���־λ���ж��Ƿ����г���
  JumpToIAPFlag = STMFLASH_ReadHalfWord(ADDR_JumpToIAPFlag);
  IAPFlagBuf[0] = 0x11;
  IAPFlagBuf[1] = 0x00;

  printf("��ʾ������send����bin�ļ�!\r\n");
  while (1) 
  {
    if (JumpToAPPFlag !=0x11) // �ж��Ƿ�����APP����������У���ת��APP��������
    {
      if (JumpToIAPFlag ==0x11) // �ж��Ƿ��APP������ת����,����ǲ�������APP����
        JumpToIAPFlag = 0x00;
      if (USART_RX_CNT) // ��������ݽ���
      {
        if (RxDataCount == USART_RX_CNT) // ����û�����յ�������
        {
          RxDataLength = USART_RX_CNT;
          if (RxCmdFlag == 0 && RxDataLength == 4) // ���յ�IAPָ��
          {
            if (USART_RX_BUF[0] == 's' && USART_RX_BUF[1] == 'e' &&USART_RX_BUF[2] == 'n' &&USART_RX_BUF[3] == 'd') // �ж��Ƿ�ΪIAPָ��
            {
              RxCmdFlag = 1; // ���յ�����APP����ָ���־λ��1
              RxDataLength = 0; // ���ָ��ȣ���ֹӰ��������APP�����С
              printf("׼������app���������bin�ļ�!\r\n"); // ׼���ý���bin�ļ����ȴ��û����
            } 
			else 
			{
              CodeUpdateFlag = 0;
              AppCodeLength = 0;
              printf("ָ�����!\r\n"); // δ���յ�IAP����ָ������κδ��ڷ������ݶ���Ϊָ�����
            }
          }

          else if (RxCmdFlag == 1 && RxDataLength > 10) // ����APP����
          {
            CodeUpdateFlag =1; // ������±�־λ��λ������Ӧ�ó�����������ɺ�дFLASH
            RxCmdFlag = 0;
            AppCodeLength = USART_RX_CNT;
            printf("APP����������!\r\n");
            printf("�����С:%dBytes\r\n", AppCodeLength);
          }
          else 
		  {
            RxDataLength = 0;
            printf("�ļ���ָ�����!\r\n"); // ��������С����10Bytes����Ϊû����ȷ���bin�ļ�
          }
          RxDataCount = 0;
          USART_RX_CNT = 0;
        } 
		else 
		{
          RxDataCount = USART_RX_CNT;
        }
      }

      HAL_Delay(10); // ���Դ����жϵ�ʱ�䣬�ж��Ƿ�������

      if (CodeUpdateFlag) // ������±�־λ��λ
      {
        CodeUpdateFlag = 0;
        if (AppCodeLength) 
		{
          printf("���������...\r\n");
          if (((*(uint32_t *)(0X20001000 + 4)) & 0xFF000000) ==0x08000000) // �жϴ���Ϸ���
          {
            printf("�������س���!\r\n");
            //iap_write_appbin(FLASH_APP1_ADDR, USART_RX_BUF,AppCodeLength); // �´���д��FLASH
            AppRunFlag = 1;
          } 
		  else 
		  {
            printf("�������ʧ�ܣ�����bin�ļ��Ƿ���ȷ!\r\n");
            printf("��ת��ԭ��Ӧ�ó���!\r\n");
            iap_load_app(FLASH_APP1_ADDR); // ִ��FLASH APP����
          }
        } 
		else 
		{
          printf("û�г�����Ը���!\r\n");
        }
      }

      if (AppRunFlag) // App���б�־��λ
      {
        printf("��ʼ���г���!\r\n");
        HAL_Delay(10);
        if (((*(uint32_t *)(FLASH_APP1_ADDR + 4)) & 0xFF000000) ==0x08000000) // �жϴ���Ϸ���
        {
          AppRunFlag = 0;
          STMFLASH_Write(ADDR_CodeWriteFlag, IAPFlagBuf, 2); // д��APP�����־
          // RCC_DeInit(); //�ر�����
          //__disable_irq();
          iap_load_app(FLASH_APP1_ADDR); // ִ��FLASH APP����
        } else {
          printf("Ӧ�ó������!\r\n");
        }
      }
    } 
	else 
	{
      printf("����һ��Ӧ�ó���!\r\n");
      printf("��ʼ���г���!\r\n");
      HAL_Delay(10);
      iap_load_app(FLASH_APP1_ADDR); // ִ��FLASH APP����
    }
	HAL_IWDG_Refresh(&hiwdg);
  }
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */


/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType =
      RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    Error_Handler();

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    Error_Handler();
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 
{
  if (huart->Instance == USART3) // �ж����ĸ����ڽ����ж�
  {

  }
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1) {
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
void assert_failed(uint8_t *file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
     line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
