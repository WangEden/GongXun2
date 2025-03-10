#ifndef __USART3_H
#define __USART3_H

#include "robot.h"	

#define USART3_MAX_RECV_LEN		8	
#define USART3_MAX_SEND_LEN		11					

extern uint8_t USART3_RXPacket[]; 
extern uint8_t USART3_TXPacket[]; 


void uart3_init(uint32_t bound);
uint32_t USART3_PowCalculation(uint32_t x,uint32_t y);
void USART3_Send_oneData(uint8_t Byte);
void USART3_Send_Array(uint8_t *Array,uint16_t Length);
void USART3_Send_String(char *String);
void USART3_Send_Number(uint32_t Number,uint8_t Length);
void USART3_printf(char *format, ...);



#endif
