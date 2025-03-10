#include "usart.h"	

////串口接收缓存区 
//uint8_t USART_RXPacket[USART_MAX_RECV_LEN];//identify the RXPacket and the Max Length
//uint8_t USART_TXPacket[USART_MAX_SEND_LEN];//identify the TXPacket and the Max Length


void uart1_init(uint32_t bound)
{  
	  //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1

	//USART1端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
    USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=5;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream5_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6; //
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //
	NVIC_Init(&NVIC_InitStructure);


	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);  	
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);  	
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);  
}


//void Analyze_data(void)
//{
//	command[0] = USART_RXPacket[0];
//	command[1] = USART_RXPacket[1];
//	command[2] = USART_RXPacket[2];
//	command[3] = USART_RXPacket[3];
//	deltaX = ((int16_t)( (uint8_t)USART_RXPacket[4] + ((uint8_t)USART_RXPacket[5]<<8) ));
//	deltaY = ((int16_t)( (uint8_t)USART_RXPacket[6] + ((uint8_t)USART_RXPacket[7]<<8) ));
//}


/*******Receive Message Function*******/


//void USART1_IRQHandler(void)
//{
//	static int USART1RxCount = 0;
//	static u8 RXstate = 0;
//	
//	if(USART_GetITStatus(USART1, USART_IT_RXNE))
//	{
//		uint8_t tmp;
//		tmp = USART_ReceiveData(USART1);
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//		switch (RXstate)
//		{
//			case 0:
//			{
//				if (tmp == 0x2C) // , 帧头1
//					RXstate = 1;
//				else
//					RXstate = 0;
//			}break;
//			case 1:
//			{
//				if (tmp == 0x3C) // < 帧头2
//					RXstate = 2;
//				else
//					RXstate = 0;
//			}break;
//			case 2:
//			{
//				USART_RXPacket[USART1RxCount++] = tmp;
//				if (USART1RxCount == 8)
//				{
//					RXstate = 3;
//					USART1RxCount = 0;
//					
//				}
//	
//			}break;
//			case 3:
//			{
//				if (tmp == 0x3E) // > 帧尾
//				{
//					Analyze_data();
//					Uart_flag = 1;
//				}
//				RXstate = 0;
//			}break;
//			default:
//				RXstate = 0;
//			break;
//		}
//	}

//}


/**************************串口打印相关函数重定义********************************/
/**
  * @简  述  重定义putc函数（USART1）	
  */
int fputc(int ch, FILE *f)
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART1, (uint8_t) ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}

	return ch;
}

/**
  * @简  述  重定义getc函数（USART1）	
  */
int fgetc(FILE *f)
{
	/* 等待串口1输入数据 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
	{}

	return (int)USART_ReceiveData(USART1);
}
 




