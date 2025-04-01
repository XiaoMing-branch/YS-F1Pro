#ifndef __IAP_H
#define __IAP_H
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"

extern uint8_t USART_RX_BUF[255];
extern uint8_t USART_RX_CNT;
extern uint8_t CodeUpdateFlag;

void BoardDisableIrq(void);
void BoardEnableIrq(void);
__asm void MSR_MSP(uint32_t addr);
void iap_load_app(uint32_t AppAddr);
#ifdef __cplusplus
}
#endif

#endif /* __YMODEM_H */
