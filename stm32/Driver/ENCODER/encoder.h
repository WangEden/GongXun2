#ifndef __ENCODER_H
#define __ENCODER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

//X-SOFT �ӿں���
void ENCODER_A_Init(void);          //��������ʼ��
uint16_t ENCODER_A_GetCounter(void);          //��������ȡ��������ֵ
void ENCODER_A_SetCounter(uint16_t count);    //���������ü�������ֵ

void ENCODER_B_Init(void);          //��������ʼ��
uint16_t ENCODER_B_GetCounter(void);          //��������ȡ��������ֵ
void ENCODER_B_SetCounter(uint16_t count);    //���������ü�������ֵ

void ENCODER_C_Init(void);          //��������ʼ��
uint16_t ENCODER_C_GetCounter(void);          //��������ȡ��������ֵ
void ENCODER_C_SetCounter(uint16_t count);    //���������ü�������ֵ

void ENCODER_D_Init(void);          //��������ʼ��
uint16_t ENCODER_D_GetCounter(void);          //��������ȡ��������ֵ
void ENCODER_D_SetCounter(uint16_t count);    //���������ü�������ֵ

#endif

