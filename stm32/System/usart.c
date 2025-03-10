#include "usart.h"	

////���ڽ��ջ����� 
//uint8_t USART_RXPacket[USART_MAX_RECV_LEN];//identify the RXPacket and the Max Length
//uint8_t USART_TXPacket[USART_MAX_SEND_LEN];//identify the TXPacket and the Max Length


void uart1_init(uint32_t bound)
{  
	  //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1

	//USART1�˿�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
    USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=5;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
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
//				if (tmp == 0x2C) // , ֡ͷ1
//					RXstate = 1;
//				else
//					RXstate = 0;
//			}break;
//			case 1:
//			{
//				if (tmp == 0x3C) // < ֡ͷ2
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
//				if (tmp == 0x3E) // > ֡β
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


/**************************���ڴ�ӡ��غ����ض���********************************/
/**
  * @��  ��  �ض���putc������USART1��	
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
  * @��  ��  �ض���getc������USART1��	
  */
int fgetc(FILE *f)
{
	/* �ȴ�����1�������� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
	{}

	return (int)USART_ReceiveData(USART1);
}
 




