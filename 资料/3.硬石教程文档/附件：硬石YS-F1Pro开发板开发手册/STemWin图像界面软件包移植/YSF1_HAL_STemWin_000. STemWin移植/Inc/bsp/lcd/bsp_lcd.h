#ifndef __BSP_LCD_H__
#define	__BSP_LCD_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
/******************************************************************************
2^26 =0X0400 0000 = 64MB,每个 BANK 有4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

选择BANK1-BORSRAM4 连接 TFT，地址范围为0X6C00 0000 ~ 0X6FFF FFFF
YS-F1P开发板选择 FSMC_A0 接LCD的DC(寄存器/数据选择)脚
寄存器基地址 = 0X6C00 0000
RAM基地址 = 0X6C00 0002 = 0X6C00 0000+(1<<(0+1))
如果电路设计时选择不同的地址线时，地址要重新计算  
*******************************************************************************/
/******************************* ILI9488 显示屏的 FSMC 参数定义 ***************/
#define FSMC_LCD_CMD                   ((uint32_t)0x6C000000)	    //FSMC_Bank1_NORSRAM1用于LCD命令操作的地址
#define FSMC_LCD_DATA                  ((uint32_t)0x6C000002)      //FSMC_Bank1_NORSRAM1用于LCD数据操作的地址      
#define LCD_WRITE_CMD(x)               *(__IO uint16_t *)FSMC_LCD_CMD  = x 
#define LCD_WRITE_DATA(x)              *(__IO uint16_t *)FSMC_LCD_DATA = x
#define LCD_READ_DATA()                *(__IO uint16_t *)FSMC_LCD_DATA

#define FSMC_LCD_BANKx                 FSMC_NORSRAM_BANK4

/************************* ILI9488 显示屏8080通讯引脚定义 *********************/
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

/**************** 显示方向选择，可选(1,2,3,4)四个方向 *************************/
#define LCD_DIRECTION                  1  // 原点在屏幕左上角 X*Y=320*480
//#define LCD_DIRECTION                  2  // 原点在屏幕右上角 X*Y=480*320
//#define LCD_DIRECTION                  3  // 原点在屏幕右下角 X*Y=320*480
//#define LCD_DIRECTION                  4  // 原点在屏幕左下角 X*Y=480*320

/******** ILI934 显示屏全屏默认（扫描方向为1时）最大宽度和最大高度*************/
#if (LCD_DIRECTION==1)||(LCD_DIRECTION==3)

 #define LCD_DEFAULT_WIDTH		         320  // X轴长度
 #define LCD_DEFAULT_HEIGTH         	 480  // Y轴长度
 
#else

 #define LCD_DEFAULT_WIDTH		         480  // X轴长度
 #define LCD_DEFAULT_HEIGTH         	 320  // Y轴长度
 
#endif


/* 扩展变量 ------------------------------------------------------------------*/
extern SRAM_HandleTypeDef hlcd;
extern __IO uint32_t lcd_id;

/* 函数声明 ------------------------------------------------------------------*/
uint32_t BSP_LCD_Init(void);

#endif /* __BSP_LCD_H__ */

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
