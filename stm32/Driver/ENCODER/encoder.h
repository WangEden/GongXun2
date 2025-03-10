#ifndef __ENCODER_H
#define __ENCODER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

//X-SOFT 接口函数
void ENCODER_A_Init(void);          //编码器初始化
uint16_t ENCODER_A_GetCounter(void);          //编码器获取计数器数值
void ENCODER_A_SetCounter(uint16_t count);    //编码器设置计数器数值

void ENCODER_B_Init(void);          //编码器初始化
uint16_t ENCODER_B_GetCounter(void);          //编码器获取计数器数值
void ENCODER_B_SetCounter(uint16_t count);    //编码器设置计数器数值

void ENCODER_C_Init(void);          //编码器初始化
uint16_t ENCODER_C_GetCounter(void);          //编码器获取计数器数值
void ENCODER_C_SetCounter(uint16_t count);    //编码器设置计数器数值

void ENCODER_D_Init(void);          //编码器初始化
uint16_t ENCODER_D_GetCounter(void);          //编码器获取计数器数值
void ENCODER_D_SetCounter(uint16_t count);    //编码器设置计数器数值

#endif

