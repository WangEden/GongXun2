#ifndef __UART4_H
#define __UART4_H

/* Includes ------------------------------------------------------------------*/
#include "robot.h"

extern uint8_t USART_RX_BUF[64];     //���ջ���,���63���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint8_t USART_RX_STA;         //����״̬���	
//OpenCTR�����ӿں���
void uart4_init(uint32_t baud);  //UART ���Դ��ڳ�ʼ��
void uartWriteBuf(uint8_t *buf, uint8_t len);


#endif







