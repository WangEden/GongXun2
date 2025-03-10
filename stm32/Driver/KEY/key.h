#ifndef _KEY_H
#define _KEY_H

#include "robot.h"
extern float Motor_Angle[4];
extern int Motor_Id;

/*****************************************************************************
 ** 移植配置区
****************************************************************************/
#define KEY_0_CLK                      RCC_AHB1Periph_GPIOF  // 端口时钟
#define KEY_0_GPIO                     GPIOF                 // 引脚所用端口
#define KEY_0_PIN                      GPIO_Pin_6            // 引脚编号
#define KEY_0_PUPD                     GPIO_PuPd_UP        // 闲时内部上下拉状态
#define KEY_0_EXTI_PORT                EXTI_PortSourceGPIOF  // 引脚的中断端口  
#define KEY_0_EXTI_PIN                 EXTI_PinSource6       // 引脚的中断编号
#define KEY_0_EXTI_LINE                EXTI_Line6            // 外部中断线编号 
#define KEY_0_EXTI_TRIGGER             EXTI_Trigger_Falling   // 触发方式; 上升沿触发:EXTI_Trigger_Rising， 下降沿触发:EXTI_Trigger_Falling
#define KEY_0_INTERRUPT_NUMBER         EXTI9_5_IRQn            // 中断编号，即在中断向量表中的编号
// KEY_2, 闲时上拉，按下时被置低电平 
#define KEY_1_CLK                      RCC_AHB1Periph_GPIOF  // 端口时钟
#define KEY_1_GPIO                     GPIOF                 // 引脚所用端口
#define KEY_1_PIN                      GPIO_Pin_7            // 引脚编号
#define KEY_1_PUPD                     GPIO_PuPd_UP          // 闲时内部上下拉状态
#define KEY_1_EXTI_PORT                EXTI_PortSourceGPIOF  // 引脚的中断端口  
#define KEY_1_EXTI_PIN                 EXTI_PinSource7       // 引脚的中断编号
#define KEY_1_EXTI_LINE                EXTI_Line7           // 外部中断线编号 
#define KEY_1_EXTI_TRIGGER             EXTI_Trigger_Falling  // 触发方式; 上升沿触发:EXTI_Trigger_Rising， 下降沿触发:EXTI_Trigger_Falling
#define KEY_1_INTERRUPT_NUMBER         EXTI9_5_IRQn            // 中断编号，即在中断向量表中的编号
// KEY_2, 闲时上拉，按下时被置低电平
#define KEY_2_CLK                      RCC_AHB1Periph_GPIOF  // 端口时钟
#define KEY_2_GPIO                     GPIOF                 // 引脚所用端口
#define KEY_2_PIN                      GPIO_Pin_8            // 引脚编号
#define KEY_2_PUPD                     GPIO_PuPd_UP          // 闲时内部上下拉状态
#define KEY_2_EXTI_PORT                EXTI_PortSourceGPIOF  // 引脚的中断端口  
#define KEY_2_EXTI_PIN                 EXTI_PinSource8       // 引脚的中断编号
#define KEY_2_EXTI_LINE                EXTI_Line8            // 外部中断线编号 
#define KEY_2_EXTI_TRIGGER             EXTI_Trigger_Falling  // 触发方式; 上升沿触发:EXTI_Trigger_Rising， 下降沿触发:EXTI_Trigger_Falling
#define KEY_2_INTERRUPT_NUMBER         EXTI9_5_IRQn            // 中断编号，即在中断向量表中的编号
   


/*****************************************************************************
 ** 声明全局函数
****************************************************************************/
void    Key_Init(void);  // 使用h文件中的参数，初始化引脚

#endif

