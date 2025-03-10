#ifndef __DMA_H
#define	__DMA_H	   

#include "robot.h"

extern uint8_t uart1RxBuffer[200];
extern uint8_t uart3RxBuffer[200];
extern uint8_t uart6TxBuffer[200];
extern uint8_t uart6RxBuffer[200];


extern uint8_t uart6TxDataSize;
extern uint8_t uart6RxDataSize;
extern uint8_t uart1RxDataSize;
extern uint8_t uart3RxDataSize;

// �洢ƫ����
extern int deltaX;
extern int deltaY;
// �洢����
extern char command[4];


void MYDMA_Config1(DMA_Stream_TypeDef *DMA_Streamx);
void MYDMA_Config3(DMA_Stream_TypeDef *DMA_Streamx);
void MYDMA_Config6(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr);//����DMAx_CHx
void DMA_USART6_Send(DMA_Stream_TypeDef *DMA_Streamx,u8 *data,u16 size);//�������ƿ��Զ���

#endif






























