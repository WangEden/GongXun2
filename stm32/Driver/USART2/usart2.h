#ifndef __USART2_H
#define __USART2_H	 


#include "robot.h"



extern float pos_x;
extern float pos_y;
extern float zangle;

void uart2_init(u32 bound);
void Stract(char strDestination[],char strSource[],int num);
void Update_X(float New_X);
void Update_Y(float New_Y);
void USART2_Send_oneData(uint8_t Byte);
void USART2_Send_Array(uint8_t *Array,uint16_t Length);
void USART2_Send_String(char *String);
void USART2_printf(char *format, ...);

#endif













