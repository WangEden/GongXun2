#include "key.h"
float angle_psc = 0.5f;
float Motor_Angle[4] = {30.0f,30.0f,30.0f,30.0f};
int Motor_Id = 1;

static void key_0_Init(void);  // 声明key_1初始化函数; static是修改函数的有效范围：从定义声明位置起，源文件内可访问;
static void key_1_Init(void);  // 声明key_2初始化函数; static是修改函数的有效范围：从定义声明位置起，源文件内可访问;
static void key_2_Init(void);  // 声明key_3初始化函数; static是修改函数的有效范围：从定义声明位置起，源文件内可访问;


void Key_Init(void)
{
    key_0_Init();
    key_1_Init();
    key_2_Init();
}

// 按键_1的初始化函数
static void key_0_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;    // 定义GPIO结构体，用于配置引脚工作模式
    EXTI_InitTypeDef EXTI_InitStructure;    // 定义EXTI结构体，用于配置外部中断线，即引脚的中断方式
    NVIC_InitTypeDef NVIC_InitStructure;    // 定义NVIC结构体，用于配置中断优先级

    // 时钟使能
    RCC_AHB1PeriphClockCmd(KEY_0_CLK, ENABLE);                    // 使能KEY_1所用引脚端口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);        // 使能系统配置器SYSCFG的时钟，使用GPIO外部中断必须使能SYSCFG时钟

    
    GPIO_InitStructure.GPIO_Pin   = KEY_0_PIN;                    // 选择要控制的引脚编号; 此处使用了宏定义，以方便移植修改
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;                 // 引脚模式：输入模式
    GPIO_InitStructure.GPIO_PuPd  = KEY_0_PUPD;                   // 上下拉状态
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;              // 引脚速率：2MHz
    GPIO_Init(KEY_0_GPIO, &GPIO_InitStructure);                   // 初始化, 即把上面的参数写入寄存器

    // 把引脚连接至中断线
    SYSCFG_EXTILineConfig(KEY_0_EXTI_PORT, KEY_0_EXTI_PIN);       // 把引脚连接至中断线

    // 配置EXTI中断线
    EXTI_InitStructure.EXTI_Line = KEY_0_EXTI_LINE;               // 中断线
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           // 中断模式
    EXTI_InitStructure.EXTI_Trigger = KEY_0_EXTI_TRIGGER;         // 触发方式
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     // 使能
    EXTI_Init(&EXTI_InitStructure);                               // 初始化, 即把上面的参数写入寄存器

    // 配置 NVIC, 即中断优先级
    
    NVIC_InitStructure.NVIC_IRQChannel = KEY_0_INTERRUPT_NUMBER;  // 中断编号，提示：各种中断的编号，是在启动文件中，按芯片内部设计而预先设定好的编号的。
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 11;     // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               // 使能
    NVIC_Init(&NVIC_InitStructure);                               // 初始化, 即上面的参数写入寄存器
}



// 按键_2的初始化函数
static void key_1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;    // 定义GPIO结构体，用于配置引脚工作模式
    EXTI_InitTypeDef EXTI_InitStructure;    // 定义EXTI结构体，用于配置外部中断线，即引脚的中断方式
    NVIC_InitTypeDef NVIC_InitStructure;    // 定义NVIC结构体，用于配置中断优先级

    // 时钟使能
    RCC_AHB1PeriphClockCmd(KEY_1_CLK, ENABLE);                    // 使能KEY_1所用引脚端口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);        // 使能系统配置器SYSCFG的时钟，使用GPIO外部中断必须使能SYSCFG时钟

    // 配置引脚工作模式: PA0、 输入、闲时下拉、按下置高电平
    GPIO_InitStructure.GPIO_Pin   = KEY_1_PIN;                    // 选择要控制的引脚编号; 此处使用了宏定义，以方便移植修改
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;                 // 引脚模式：输入模式
    GPIO_InitStructure.GPIO_PuPd  = KEY_1_PUPD;                   // 上下拉状态
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;              // 引脚速率：2MHz
    GPIO_Init(KEY_1_GPIO, &GPIO_InitStructure);                   // 初始化, 即把上面的参数写入寄存器

    // 把引脚连接至中断线
    SYSCFG_EXTILineConfig(KEY_1_EXTI_PORT, KEY_1_EXTI_PIN);       // 把引脚连接至中断线

    // 配置EXTI中断线
    EXTI_InitStructure.EXTI_Line = KEY_1_EXTI_LINE;               // 中断线
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           // 中断模式
    EXTI_InitStructure.EXTI_Trigger = KEY_1_EXTI_TRIGGER;         // 触发方式
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     // 使能
    EXTI_Init(&EXTI_InitStructure);                               // 初始化, 即把上面的参数写入寄存器

    // 配置 NVIC, 即中断优先级
   
    NVIC_InitStructure.NVIC_IRQChannel = KEY_1_INTERRUPT_NUMBER;  // 中断编号，提示：各种中断的编号，是在启动文件中，按芯片内部设计而预先设定好的编号的。
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 12;     // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               // 使能
    NVIC_Init(&NVIC_InitStructure);                               // 初始化, 即上面的参数写入寄存器
}






static void key_2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;    // 定义GPIO结构体，用于配置引脚工作模式
    EXTI_InitTypeDef EXTI_InitStructure;    // 定义EXTI结构体，用于配置外部中断线，即引脚的中断方式
    NVIC_InitTypeDef NVIC_InitStructure;    // 定义NVIC结构体，用于配置中断优先级

    // 时钟使能
    RCC_AHB1PeriphClockCmd(KEY_2_CLK, ENABLE);                    // 使能KEY_1所用引脚端口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);        // 使能系统配置器SYSCFG的时钟，使用GPIO外部中断必须使能SYSCFG时钟

    // 配置引脚工作模式: PA0、 输入、闲时下拉、按下置高电平
    GPIO_InitStructure.GPIO_Pin   = KEY_2_PIN;                    // 选择要控制的引脚编号; 此处使用了宏定义，以方便移植修改
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;                 // 引脚模式：输入模式
    GPIO_InitStructure.GPIO_PuPd  = KEY_2_PUPD;                   // 上下拉状态
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;              // 引脚速率：2MHz
    GPIO_Init(KEY_2_GPIO, &GPIO_InitStructure);                   // 初始化, 即把上面的参数写入寄存器

    // 把引脚连接至中断线
    SYSCFG_EXTILineConfig(KEY_2_EXTI_PORT, KEY_2_EXTI_PIN);       // 把引脚连接至中断线

    // 配置EXTI中断线
    EXTI_InitStructure.EXTI_Line = KEY_2_EXTI_LINE;               // 中断线
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           // 中断模式
    EXTI_InitStructure.EXTI_Trigger = KEY_2_EXTI_TRIGGER;         // 触发方式
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     // 使能
    EXTI_Init(&EXTI_InitStructure);                               // 初始化, 即把上面的参数写入寄存器

    // 配置 NVIC, 即中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = KEY_2_INTERRUPT_NUMBER;  // 中断编号，提示：各种中断的编号，是在启动文件中，按芯片内部设计而预先设定好的编号的。
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 13;     // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               // 使能
    NVIC_Init(&NVIC_InitStructure);                               // 初始化, 即上面的参数写入寄存器
}



void EXTI9_5_IRQHandler(void)
{
   //控制电机ID
    if(EXTI_GetITStatus(KEY_0_EXTI_LINE) != RESET)
	{
		Motor_Id++;
		if(Motor_Id > 4)
			Motor_Id = 1;
		EXTI_ClearITPendingBit(KEY_0_EXTI_LINE);  // 清除中断标志位;
	}
	//增加角度
	else if(EXTI_GetITStatus(KEY_1_EXTI_LINE) != RESET)
	{
		switch(Motor_Id)
			{
				case 1:
					Motor_Angle[0] = Motor_Angle[0] + angle_psc;
					Control_Multi_Angle2(1,Motor_Angle[0],360);
					break;
				case 2:
					Motor_Angle[1] = Motor_Angle[1] + angle_psc;
					Control_Multi_Angle2(2,Motor_Angle[1],360);
					break;
				case 3:
					Motor_Angle[2] = Motor_Angle[2] + angle_psc;
					Control_Multi_Angle2(3,Motor_Angle[2],360);
					break;
				case 4:
					Motor_Angle[3] = Motor_Angle[3] + angle_psc;
					Control_Multi_Angle2(4,Motor_Angle[3],360);
					break;
				default:
					break;
			}
		EXTI_ClearITPendingBit(KEY_1_EXTI_LINE);  // 清除中断标志位;
	}
	//减少角度
	else if(EXTI_GetITStatus(KEY_2_EXTI_LINE) != RESET)
	{
		switch(Motor_Id)
			{
				case 1:
					Motor_Angle[0] = Motor_Angle[0] - angle_psc;
					Control_Multi_Angle2(1,Motor_Angle[0],360);
					break;
				case 2:
					Motor_Angle[1] = Motor_Angle[1] - angle_psc;
					Control_Multi_Angle2(2,Motor_Angle[1],360);
					break;
				case 3:
					Motor_Angle[2] = Motor_Angle[2] - angle_psc;
					Control_Multi_Angle2(3,Motor_Angle[2],360);
					break;
				case 4:
					Motor_Angle[3] = Motor_Angle[3] - angle_psc;
					Control_Multi_Angle2(4,Motor_Angle[3],360);
					break;
				default:
					break;
			}
		EXTI_ClearITPendingBit(KEY_2_EXTI_LINE);  // 清除中断标志位;
	}
	 

}




