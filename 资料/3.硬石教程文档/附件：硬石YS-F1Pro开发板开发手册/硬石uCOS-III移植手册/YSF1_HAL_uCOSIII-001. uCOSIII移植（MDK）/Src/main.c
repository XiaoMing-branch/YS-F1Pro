/******************************************************************************
  * �ļ�����: main.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: uCOSIII������
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F1Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/

#include <includes.h>
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/*
********************************************************************************
*                                ������ƿ�
********************************************************************************
*/
static  OS_TCB   AppTaskStartTCB;

/*
********************************************************************************
*                                  �����ջ
********************************************************************************
*/
static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];

/* ˽�к���ԭ�� --------------------------------------------------------------*/
static  void  AppTaskCreate(void);
static  void  AppObjCreate(void);
static  void  AppTaskStart(void *p_arg);

/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ϵͳʱ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;  // �ⲿ����8MHz
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;  // 9��Ƶ���õ�72MHz��ʱ��
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;       // ϵͳʱ�ӣ�72MHz
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;              // AHBʱ�ӣ�72MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;               // APB1ʱ�ӣ�36MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;               // APB2ʱ�ӣ�72MHz
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

 	// HAL_RCC_GetHCLKFreq()/1000    1ms�ж�һ��
	// HAL_RCC_GetHCLKFreq()/100000	 10us�ж�һ��
	// HAL_RCC_GetHCLKFreq()/1000000 1us�ж�һ��
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);  // ���ò�����ϵͳ�δ�ʱ��
  /* ϵͳ�δ�ʱ��ʱ��Դ */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  /* ϵͳ�δ�ʱ���ж����ȼ����� */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * ��������: ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
int  main (void)
{
	OS_ERR  err;
  /* ��λ�������裬��ʼ��Flash�ӿں�ϵͳ�δ�ʱ�� */
  HAL_Init();
  /* ����ϵͳʱ�� */
  SystemClock_Config();
	/* ��ʼ��"uC/OS-III"�ں� */ 
	OSInit(&err);                                               /* Init uC/OS-III.                                      */
	/* �������� */
	OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                /* Create the start task                                */
				 (CPU_CHAR   *)"App Task Start",
				 (OS_TASK_PTR ) AppTaskStart,
				 (void       *) 0,
				 (OS_PRIO     ) APP_TASK_START_PRIO,
				 (CPU_STK    *)&AppTaskStartStk[0],
				 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,
				 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,
				 (OS_MSG_QTY  ) 0,
				 (OS_TICK     ) 0,
				 (void       *) 0,
				 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
				 (OS_ERR     *)&err);
	/* ����������ϵͳ������Ȩ����uC/OS-III */
	OSStart(&err);            /* Start multitasking (i.e. give control to uC/OS-III). */
       
         
}


/**
  * ��������: �����������塣
  * �������: p_arg ���ڴ���������ʱ���ݵ��β�
  * �� �� ֵ: ��
  * ˵    ������
  */
static  void  AppTaskStart (void *p_arg)
{
  OS_ERR      err;

  (void)p_arg;

  BSP_Init();                                                 /* Initialize BSP functions                             */
  CPU_Init();

  Mem_Init();                                                 /* Initialize Memory Management Module                  */

#if OS_CFG_STAT_TASK_EN > 0u
  OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

  CPU_IntDisMeasMaxCurReset();

  AppTaskCreate();                                            /* Create Application Tasks                             */

  AppObjCreate();                                             /* Create Application Objects                           */

  while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
      LED1_TOGGLE;  
			OSTimeDlyHMSM(0, 0, 0, 500,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
    }
}

/**
  * ��������: ����Ӧ������
  * �������: p_arg ���ڴ���������ʱ���ݵ��β�
  * �� �� ֵ: ��
  * ˵    ������
  */
static  void  AppTaskCreate (void)
{
  
}


/**
  * ��������: uCOSIII�ں˶��󴴽�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static  void  AppObjCreate (void)
{
}

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
