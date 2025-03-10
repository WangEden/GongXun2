#ifndef __USART_H
#define __USART_H

#include "robot.h"	


#define USART_MAX_RECV_LEN		8	
#define USART_MAX_SEND_LEN		11					

//extern uint8_t USART_RXPacket[]; 
//extern uint8_t USART_TXPacket[]; 


void uart1_init(uint32_t bound);


#endif


