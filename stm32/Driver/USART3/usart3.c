#include "usart3.h"

//串口接收缓存区 	
uint8_t USART3_RXPacket[USART3_MAX_RECV_LEN];//identify the RXPacket and the Max Length
uint8_t USART3_TXPacket[USART3_MAX_SEND_LEN];//identify the TXPacket and the Max Length




void uart3_init(uint32_t bound)
{  
  
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //串口3时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); 
	
 	USART_DeInit(USART3);                         
    //USART3_xx 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);   

	USART_InitStructure.USART_BaudRate = bound;                    
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_InitStructure.USART_Parity = USART_Parity_No;             //无奇偶校验位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;          //一个停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;     //no paiity to 8	
	USART_Init(USART3, &USART_InitStructure); //初始化串口3
	USART_Cmd(USART3, ENABLE);                 
	
    /*Through NVIC to get data*/
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//start the NVIC
	
 	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 10;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream1_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15; //
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //
	NVIC_Init(&NVIC_InitStructure);


	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);  	
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);  	
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE); 
	
}
/*Calculation Function*/
uint32_t USART3_PowCalculation(uint32_t x,uint32_t y)
{
	uint32_t Init=1;
	while(y--) //circulation y times
	{
		Init *= x;
	}
	return Init;
}
/*******Send Message Function*******/

/*send one data*/
void USART3_Send_oneData(uint8_t Byte)
{
	USART_SendData(USART3,Byte);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET); 
}
/*send CharArray*/
void USART3_Send_Array(uint8_t *Array,uint16_t Length)
{
	uint16_t i;
	for(i = 0;i < Length; i++)
	{
	USART3_Send_oneData(Array[i]);
	}
}
/*send String*/
void USART3_Send_String(char *String) //"String",if you want to change the line,then printf "String\r\n"
{
	uint8_t i;
	for(i = 0; String[i] != '\0'; i++)
	USART3_Send_oneData(String[i]);
}
/*send a number*/
void USART3_Send_Number(uint32_t Number,uint8_t Length)
{
	uint8_t i;
	for (i = 0;i < Length; i ++)
	USART3_Send_oneData(Number / USART3_PowCalculation( 10 , Length - i - 1 ) % 10 + '0');
}
/*Another way to take place of printf*/
void USART3_printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	USART3_Send_String(String);
}





