#ifndef _KEY_H
#define _KEY_H

#include "robot.h"
extern float Motor_Angle[4];
extern int Motor_Id;

/*****************************************************************************
 ** ��ֲ������
****************************************************************************/
#define KEY_0_CLK                      RCC_AHB1Periph_GPIOF  // �˿�ʱ��
#define KEY_0_GPIO                     GPIOF                 // �������ö˿�
#define KEY_0_PIN                      GPIO_Pin_6            // ���ű��
#define KEY_0_PUPD                     GPIO_PuPd_UP        // ��ʱ�ڲ�������״̬
#define KEY_0_EXTI_PORT                EXTI_PortSourceGPIOF  // ���ŵ��ж϶˿�  
#define KEY_0_EXTI_PIN                 EXTI_PinSource6       // ���ŵ��жϱ��
#define KEY_0_EXTI_LINE                EXTI_Line6            // �ⲿ�ж��߱�� 
#define KEY_0_EXTI_TRIGGER             EXTI_Trigger_Falling   // ������ʽ; �����ش���:EXTI_Trigger_Rising�� �½��ش���:EXTI_Trigger_Falling
#define KEY_0_INTERRUPT_NUMBER         EXTI9_5_IRQn            // �жϱ�ţ������ж��������еı��
// KEY_2, ��ʱ����������ʱ���õ͵�ƽ 
#define KEY_1_CLK                      RCC_AHB1Periph_GPIOF  // �˿�ʱ��
#define KEY_1_GPIO                     GPIOF                 // �������ö˿�
#define KEY_1_PIN                      GPIO_Pin_7            // ���ű��
#define KEY_1_PUPD                     GPIO_PuPd_UP          // ��ʱ�ڲ�������״̬
#define KEY_1_EXTI_PORT                EXTI_PortSourceGPIOF  // ���ŵ��ж϶˿�  
#define KEY_1_EXTI_PIN                 EXTI_PinSource7       // ���ŵ��жϱ��
#define KEY_1_EXTI_LINE                EXTI_Line7           // �ⲿ�ж��߱�� 
#define KEY_1_EXTI_TRIGGER             EXTI_Trigger_Falling  // ������ʽ; �����ش���:EXTI_Trigger_Rising�� �½��ش���:EXTI_Trigger_Falling
#define KEY_1_INTERRUPT_NUMBER         EXTI9_5_IRQn            // �жϱ�ţ������ж��������еı��
// KEY_2, ��ʱ����������ʱ���õ͵�ƽ
#define KEY_2_CLK                      RCC_AHB1Periph_GPIOF  // �˿�ʱ��
#define KEY_2_GPIO                     GPIOF                 // �������ö˿�
#define KEY_2_PIN                      GPIO_Pin_8            // ���ű��
#define KEY_2_PUPD                     GPIO_PuPd_UP          // ��ʱ�ڲ�������״̬
#define KEY_2_EXTI_PORT                EXTI_PortSourceGPIOF  // ���ŵ��ж϶˿�  
#define KEY_2_EXTI_PIN                 EXTI_PinSource8       // ���ŵ��жϱ��
#define KEY_2_EXTI_LINE                EXTI_Line8            // �ⲿ�ж��߱�� 
#define KEY_2_EXTI_TRIGGER             EXTI_Trigger_Falling  // ������ʽ; �����ش���:EXTI_Trigger_Rising�� �½��ش���:EXTI_Trigger_Falling
#define KEY_2_INTERRUPT_NUMBER         EXTI9_5_IRQn            // �жϱ�ţ������ж��������еı��
   


/*****************************************************************************
 ** ����ȫ�ֺ���
****************************************************************************/
void    Key_Init(void);  // ʹ��h�ļ��еĲ�������ʼ������

#endif

