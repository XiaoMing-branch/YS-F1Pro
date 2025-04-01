#include "IAP.h"

uint8_t USART_RX_BUF[255];
uint8_t USART_RX_CNT;
uint8_t CodeUpdateFlag;

/* ��  ��: �ر�ȫ���ж� */
void BoardDisableIrq(void)
{
    __disable_irq();
	
}

/* ��  ��: ʹ��ȫ���ж� */
void BoardEnableIrq(void)
{
    __enable_irq();
}

/* ��ʼ����ջָ�� */
__asm void MSR_MSP(uint32_t addr) 
{
    msr msp, r0  // r0�Ǻ����ĵ�һ������
    bx lr
}

typedef void (*IapFun)(void); // ����һ������ָ�룬������ת�����Ե�ִַ�г���
IapFun JumpToApp; 

/*!
 *  ��  ��: ��ת��Ӧ�ó��� 
 *  param1: �û�������ʼ��ַ
 *  retval: �޷���ֵ
 */
void iap_load_app(uint32_t AppAddr)
{
    /*
        Ӧ�ó���APP�����ð� �ж������� ������0x08003000 ��ʼ��λ�ã����ж����������һ���ŵľ���ջ����ַ��ֵ
        Ҳ����˵����仰��ͨ���ж�ջ����ֵַ�Ƿ���ȷ���Ƿ���0x2000 0000 - 0x 2000 2000֮�䣩 ���ж��Ƿ�Ӧ�ó���
        �Ѿ������ˣ���ΪӦ�ó���������ļ��տ�ʼ��ȥ��ʼ����ջ�ռ䣬���ջ��ֵ���ˣ�˵Ӧ�ó��Ѿ������������ļ�,��ʼ��Ҳִ���ˣ�
    */
  if( ((*(uint32_t*)AppAddr) & 0x2FFE0000) == 0x20000000 )// ���ջ����ַ�Ƿ�Ϸ�,�鿴�ο��ֲ��ڴ��½ڵ�SRAMС��
  { 
	BoardDisableIrq();  // ��ֹ�ж�
	JumpToApp = (IapFun)*(uint32_t*)(AppAddr+4);    // �û��������ڶ�����Ϊ����ʼ��ַ(�³���λ��ַ)		
	MSR_MSP(*(uint32_t*)AppAddr);		                // ��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
								 
	JumpToApp();	                                  // ����PCָ��Ϊbootload��λ�жϺ����ĵ�ַ������ִ��
  }
}

