#ifndef __UART4_H
#define __UART4_H

/* Includes ------------------------------------------------------------------*/
#include "robot.h"

extern uint8_t USART_RX_BUF[64];     //接收缓冲,最大63个字节.末字节为换行符 
extern uint8_t USART_RX_STA;         //接收状态标记	
//OpenCTR驱动接口函数
void uart4_init(uint32_t baud);  //UART 调试串口初始化
void uartWriteBuf(uint8_t *buf, uint8_t len);


#endif







