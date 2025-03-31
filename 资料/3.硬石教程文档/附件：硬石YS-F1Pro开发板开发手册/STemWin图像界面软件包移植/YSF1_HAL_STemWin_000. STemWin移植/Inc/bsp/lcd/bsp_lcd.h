#ifndef __BSP_LCD_H__
#define	__BSP_LCD_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
/******************************************************************************
2^26 =0X0400 0000 = 64MB,ÿ�� BANK ��4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

ѡ��BANK1-BORSRAM4 ���� TFT����ַ��ΧΪ0X6C00 0000 ~ 0X6FFF FFFF
YS-F1P������ѡ�� FSMC_A0 ��LCD��DC(�Ĵ���/����ѡ��)��
�Ĵ�������ַ = 0X6C00 0000
RAM����ַ = 0X6C00 0002 = 0X6C00 0000+(1<<(0+1))
�����·���ʱѡ��ͬ�ĵ�ַ��ʱ����ַҪ���¼���  
*******************************************************************************/
/******************************* ILI9488 ��ʾ���� FSMC �������� ***************/
#define FSMC_LCD_CMD                   ((uint32_t)0x6C000000)	    //FSMC_Bank1_NORSRAM1����LCD��������ĵ�ַ
#define FSMC_LCD_DATA                  ((uint32_t)0x6C000002)      //FSMC_Bank1_NORSRAM1����LCD���ݲ����ĵ�ַ      
#define LCD_WRITE_CMD(x)               *(__IO uint16_t *)FSMC_LCD_CMD  = x 
#define LCD_WRITE_DATA(x)              *(__IO uint16_t *)FSMC_LCD_DATA = x
#define LCD_READ_DATA()                *(__IO uint16_t *)FSMC_LCD_DATA

#define FSMC_LCD_BANKx                 FSMC_NORSRAM_BANK4

/************************* ILI9488 ��ʾ��8080ͨѶ���Ŷ��� *********************/
#define FSMC_LCD_CS_GPIO_ClK_ENABLE()  __HAL_RCC_GPIOG_CLK_ENABLE() 
#define FSMC_LCD_CS_PORT               GPIOG
#define FSMC_LCD_CS_PIN                GPIO_PIN_12

#define FSMC_LCD_DC_GPIO_ClK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()
#define FSMC_LCD_DC_PORT               GPIOF
#define FSMC_LCD_DC_PIN                GPIO_PIN_0

#define FSMC_LCD_BK_GPIO_ClK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()   
#define FSMC_LCD_BK_PORT               GPIOF
#define FSMC_LCD_BK_PIN                GPIO_PIN_10
 
#define LCD_BK_ON()                    HAL_GPIO_WritePin(FSMC_LCD_BK_PORT, FSMC_LCD_BK_PIN, GPIO_PIN_SET);
#define LCD_BK_OFF()                   HAL_GPIO_WritePin(FSMC_LCD_BK_PORT, FSMC_LCD_BK_PIN, GPIO_PIN_RESET);

/**************** ��ʾ����ѡ�񣬿�ѡ(1,2,3,4)�ĸ����� *************************/
#define LCD_DIRECTION                  1  // ԭ������Ļ���Ͻ� X*Y=320*480
//#define LCD_DIRECTION                  2  // ԭ������Ļ���Ͻ� X*Y=480*320
//#define LCD_DIRECTION                  3  // ԭ������Ļ���½� X*Y=320*480
//#define LCD_DIRECTION                  4  // ԭ������Ļ���½� X*Y=480*320

/******** ILI934 ��ʾ��ȫ��Ĭ�ϣ�ɨ�跽��Ϊ1ʱ������Ⱥ����߶�*************/
#if (LCD_DIRECTION==1)||(LCD_DIRECTION==3)

 #define LCD_DEFAULT_WIDTH		         320  // X�᳤��
 #define LCD_DEFAULT_HEIGTH         	 480  // Y�᳤��
 
#else

 #define LCD_DEFAULT_WIDTH		         480  // X�᳤��
 #define LCD_DEFAULT_HEIGTH         	 320  // Y�᳤��
 
#endif


/* ��չ���� ------------------------------------------------------------------*/
extern SRAM_HandleTypeDef hlcd;
extern __IO uint32_t lcd_id;

/* �������� ------------------------------------------------------------------*/
uint32_t BSP_LCD_Init(void);

#endif /* __BSP_LCD_H__ */

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
