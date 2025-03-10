#include "encoder.h" 
/**
  * @简  述  编码器初始化
  * @参  数  无
  * @返回值  无
  */
void ENCODER_A_Init(void)
{ 
    GPIO_InitTypeDef GPIO_InitStructure; 
	
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	
	//时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	//复用功能配置
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_TIM2); 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_TIM2); 	

	//配置IO口为复用功能-定时器通道
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //Timer configuration in Encoder mode 
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // No prescaling 
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;  
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
 
    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 6;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);

    //Reset counter
    TIM2->CNT = 0;
  
    TIM_Cmd(TIM2, ENABLE);   	
	
}

/**
  * @简  述  编码器获取计数器数值
  * @参  数  无
  * @返回值  计数器当前值
  */
uint16_t ENCODER_A_GetCounter(void)
{
	return TIM2->CNT;
}

/**
  * @简  述  编码器设置计数器数值
  * @参  数  count  计数器数值
  * @返回值  无
  */
void ENCODER_A_SetCounter(uint16_t count)
{
	TIM2->CNT = count;
}


/**
  * @简  述  编码器初始化
  * @参  数  无
  * @返回值  无
  */
void ENCODER_B_Init(void)
{ 
    GPIO_InitTypeDef GPIO_InitStructure; 
	
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	
	//时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	//复用功能配置
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_TIM3); 
	 	

	//配置IO口为复用功能-定时器通道
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 |  GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    //Timer configuration in Encoder mode 
    TIM_DeInit(TIM3);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // No prescaling 
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;  
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
 
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 6;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    //Reset counter
    TIM3->CNT = 0;
  
    TIM_Cmd(TIM3, ENABLE);   	
	
}

/**
  * @简  述  编码器获取计数器数值
  * @参  数  无
  * @返回值  计数器当前值
  */
uint16_t ENCODER_B_GetCounter(void)
{
	return TIM3->CNT;
}

/**
  * @简  述  编码器设置计数器数值
  * @参  数  count  计数器数值
  * @返回值  无
  */
void ENCODER_B_SetCounter(uint16_t count)
{
	TIM3->CNT = count;
}

/**
  * @简  述  编码器初始化
  * @参  数  无
  * @返回值  无
  */
void ENCODER_C_Init(void)
{ 
    GPIO_InitTypeDef GPIO_InitStructure; 
	
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	
	//时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	//复用功能配置
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5); 	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); 
	

	//配置IO口为复用功能-定时器通道
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 |  GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    //Timer configuration in Encoder mode 
    TIM_DeInit(TIM5);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // No prescaling 
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;  
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
 
    TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 6;
    TIM_ICInit(TIM5, &TIM_ICInitStructure);

    //Reset counter
    TIM5->CNT = 0;
  
    TIM_Cmd(TIM5, ENABLE);   	
	
}

/**
  * @简  述  编码器获取计数器数值
  * @参  数  无
  * @返回值  计数器当前值
  */
uint16_t ENCODER_C_GetCounter(void)
{
	return TIM5->CNT;
}

/**
  * @简  述  编码器设置计数器数值
  * @参  数  count  计数器数值
  * @返回值  无
  */
void ENCODER_C_SetCounter(uint16_t count)
{
	TIM5->CNT = count;
}

/**
  * @简  述  编码器初始化
  * @参  数  无
  * @返回值  无
  */
void ENCODER_D_Init(void)
{ 
    GPIO_InitTypeDef GPIO_InitStructure; 
	
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	
	//时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	//复用功能配置
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4); 	

	//配置IO口为复用功能-定时器通道
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12 |  GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
    GPIO_Init(GPIOD, &GPIO_InitStructure);   
	
    //Timer configuration in Encoder mode 
    TIM_DeInit(TIM4);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // No prescaling 
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;  
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
 
    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 6;
    TIM_ICInit(TIM4, &TIM_ICInitStructure);

    //Reset counter
    TIM4->CNT = 0;
  
    TIM_Cmd(TIM4, ENABLE);   	
	
}

/**
  * @简  述  编码器获取计数器数值
  * @参  数  无
  * @返回值  计数器当前值
  */
uint16_t ENCODER_D_GetCounter(void)
{
	return TIM4->CNT;
}

/**
  * @简  述  编码器设置计数器数值
  * @参  数  count  计数器数值
  * @返回值  无
  */
void ENCODER_D_SetCounter(uint16_t count)
{
	TIM4->CNT = count;
}



    



