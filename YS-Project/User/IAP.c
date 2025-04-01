#include "IAP.h"

uint8_t USART_RX_BUF[255];
uint8_t USART_RX_CNT;
uint8_t CodeUpdateFlag;

/* 功  能: 关闭全局中断 */
void BoardDisableIrq(void)
{
    __disable_irq();
	
}

/* 功  能: 使能全局中断 */
void BoardEnableIrq(void)
{
    __enable_irq();
}

/* 初始化堆栈指针 */
__asm void MSR_MSP(uint32_t addr) 
{
    msr msp, r0  // r0是函数的第一个参数
    bx lr
}

typedef void (*IapFun)(void); // 声明一个函数指针，用于跳转到绝对地址执行程序
IapFun JumpToApp; 

/*!
 *  功  能: 跳转到应用程序 
 *  param1: 用户代码起始地址
 *  retval: 无返回值
 */
void iap_load_app(uint32_t AppAddr)
{
    /*
        应用程序APP中设置把 中断向量表 放置在0x08003000 开始的位置；而中断向量表里第一个放的就是栈顶地址的值
        也就是说，这句话即通过判断栈顶地址值是否正确（是否在0x2000 0000 - 0x 2000 2000之间） 来判断是否应用程序
        已经下载了，因为应用程序的启动文件刚开始就去初始化化栈空间，如果栈顶值对了，说应用程已经下载了启动文件,初始化也执行了；
    */
  if( ((*(uint32_t*)AppAddr) & 0x2FFE0000) == 0x20000000 )// 检查栈顶地址是否合法,查看参考手册内存章节的SRAM小节
  { 
	BoardDisableIrq();  // 禁止中断
	JumpToApp = (IapFun)*(uint32_t*)(AppAddr+4);    // 用户代码区第二个字为程序开始地址(新程序复位地址)		
	MSR_MSP(*(uint32_t*)AppAddr);		                // 初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
								 
	JumpToApp();	                                  // 设置PC指针为bootload复位中断函数的地址，往下执行
  }
}

