#include "dma.h"				

uint8_t uart1RxBuffer[200];
uint8_t uart3RxBuffer[200];
uint8_t uart6TxBuffer[200];
uint8_t uart6RxBuffer[200];

uint8_t uart6TxDataSize = 0;
uint8_t uart6RxDataSize = 0;
uint8_t uart1RxDataSize = 0;
uint8_t uart3RxDataSize = 0;

// �洢ƫ����
int deltaX = 0;
int deltaY = 0;
// �洢����
char command[4];


void MYDMA_Config1(DMA_Stream_TypeDef *DMA_Streamx)
{ 
 
	DMA_InitTypeDef  DMA_InitStructure;
	if((u32)DMA_Streamx>(u32)DMA2)//�õ���ǰstream������DMA2����DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1ʱ��ʹ�� 
	}
  DMA_DeInit(DMA_Streamx);
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//�ȴ�DMA������ 
			
	/*���ô���DMA����*/
	DMA_DeInit(DMA2_Stream5);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4; 							//ͨ��ѡ��  USART6_RX
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;		//DMA�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)uart1RxBuffer;		//DMA �洢��0��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;   				//�洢��������ģʽ
	DMA_InitStructure.DMA_BufferSize = 200;						//���ݴ�����
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//�洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�������ݳ���:8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//�洢�����ݳ���:8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//ʹ����ͨģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;				    //�ߵ����ȼ�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                  //������FIFOģʽ
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;           //FIFO��ֵ
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//�洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//����ͻ�����δ���
	DMA_Init(DMA2_Stream5, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream5, ENABLE); 
}  

void USART1_IRQHandler(void)  												//����1�жϷ������
{
    if(USART_GetITStatus(USART1,USART_IT_IDLE)!=RESET) 	//�����жϴ���
    {
    	DMA_Cmd(DMA2_Stream5, DISABLE);  					   /* ��ʱ�ر�dma��������δ���� */
    	uart1RxDataSize = 200 - DMA_GetCurrDataCounter(DMA2_Stream5);/* ��ȡ���յ������ݳ��� ��λΪ�ֽ�*/
    	DMA_ClearFlag(DMA2_Stream5,DMA_FLAG_TCIF5);  		/* ��DMA��־λ */
    	DMA_SetCurrDataCounter(DMA2_Stream5,200);	/* ���¸�ֵ����ֵ��������ڵ��������ܽ��յ�������֡��Ŀ */
    	DMA_Cmd(DMA2_Stream5, ENABLE);      				/*��DMA*/
    	USART_ReceiveData(USART1);   						//��������жϱ�־λ�����պ��������־λ�����ã�
    }
  	if(USART_GetFlagStatus(USART1,USART_IT_TXE)==RESET)	//���ڷ������
  	  {
		command[0] =uart1RxBuffer[0];
		command[1] =uart1RxBuffer[1];
		command[2] = uart1RxBuffer[2];
		command[3] =uart1RxBuffer[3];
		deltaX = ((int16_t)( (uint8_t)uart1RxBuffer[4] + ((uint8_t)uart1RxBuffer[5]<<8) ));
		deltaY = ((int16_t)( (uint8_t)uart1RxBuffer[6] + ((uint8_t)uart1RxBuffer[7]<<8) ));
		 USART_ITConfig(USART1,USART_IT_TC,DISABLE);
 	  }
}

void MYDMA_Config3(DMA_Stream_TypeDef *DMA_Streamx)
{ 
 
	DMA_InitTypeDef  DMA_InitStructure;
	if((u32)DMA_Streamx>(u32)DMA2)//�õ���ǰstream������DMA2����DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1ʱ��ʹ�� 
	}
  DMA_DeInit(DMA_Streamx);
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//�ȴ�DMA������ 
			
	/*���ô���DMA����*/
	DMA_DeInit(DMA1_Stream1);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4; 							//ͨ��ѡ��  USART6_RX
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;		//DMA�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)uart3RxBuffer;		//DMA �洢��0��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;   				//�洢��������ģʽ
	DMA_InitStructure.DMA_BufferSize = 200;						//���ݴ�����
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//�洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�������ݳ���:8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//�洢�����ݳ���:8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//ʹ����ͨģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;				    //�ߵ����ȼ�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                  //������FIFOģʽ
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;           //FIFO��ֵ
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//�洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//����ͻ�����δ���
	DMA_Init(DMA1_Stream1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Stream1, ENABLE); 
}  

void USART3_IRQHandler(void)  												//����1�жϷ������
{
    if(USART_GetITStatus(USART3,USART_IT_IDLE)!=RESET) 	//�����жϴ���
    {
    	DMA_Cmd(DMA1_Stream1, DISABLE);  					   /* ��ʱ�ر�dma��������δ���� */
    	uart3RxDataSize = 200 - DMA_GetCurrDataCounter(DMA1_Stream1);/* ��ȡ���յ������ݳ��� ��λΪ�ֽ�*/
    	DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_TCIF1);  		/* ��DMA��־λ */
    	DMA_SetCurrDataCounter(DMA1_Stream1,200);	/* ���¸�ֵ����ֵ��������ڵ��������ܽ��յ�������֡��Ŀ */
    	DMA_Cmd(DMA1_Stream1, ENABLE);      				/*��DMA*/
    	USART_ReceiveData(USART3);   						//��������жϱ�־λ�����պ��������־λ�����ã�
    }
  	if(USART_GetFlagStatus(USART3,USART_IT_TXE)==RESET)	//���ڷ������
  	  {
		command[0] =uart3RxBuffer[0];
		command[1] = uart3RxBuffer[1];
		command[2] = uart3RxBuffer[2];
		command[3] = uart3RxBuffer[3];
		USART_ITConfig(USART3,USART_IT_TC,DISABLE);
 	  }
}


void MYDMA_Config6(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)
{ 
 
	DMA_InitTypeDef  DMA_InitStructure;
	if((u32)DMA_Streamx>(u32)DMA2)//�õ���ǰstream������DMA2����DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1ʱ��ʹ�� 
	}
  DMA_DeInit(DMA_Streamx);
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//�ȴ�DMA������ 
	
  /* ���� DMA Stream */
  DMA_InitStructure.DMA_Channel = chx;  //ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA �洢��0��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//�洢��������ģʽ
  DMA_InitStructure.DMA_BufferSize = ndtr;//���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ʹ����ͨģʽ 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//�е����ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
  DMA_Init(DMA_Streamx, &DMA_InitStructure);//��ʼ��DMA Stream
		
	/*���ô���DMA����*/
	DMA_DeInit(DMA2_Stream1);
	DMA_InitStructure.DMA_Channel = DMA_Channel_5; 							//ͨ��ѡ��  USART6_RX
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART6->DR;		//DMA�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)uart6RxBuffer;		//DMA �洢��0��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;   				//�洢��������ģʽ
	DMA_InitStructure.DMA_BufferSize = 200;						//���ݴ�����
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//�洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�������ݳ���:8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//�洢�����ݳ���:8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//ʹ����ͨģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;				    //�ߵ����ȼ�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                  //������FIFOģʽ
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;           //FIFO��ֵ
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//�洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//����ͻ�����δ���
	DMA_Init(DMA2_Stream1, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream1, ENABLE); //ʹ��DMA2_Stream2ͨ��
}  

void USART6_IRQHandler(void)  												//����1�жϷ������
{
    if(USART_GetITStatus(USART6,USART_IT_IDLE)!=RESET) 	//�����жϴ���
    {
    	DMA_Cmd(DMA2_Stream1, DISABLE);  					   /* ��ʱ�ر�dma��������δ���� */
    	uart6RxDataSize = 200 - DMA_GetCurrDataCounter(DMA2_Stream1);/* ��ȡ���յ������ݳ��� ��λΪ�ֽ�*/
    	DMA_ClearFlag(DMA2_Stream1,DMA_FLAG_TCIF1);  		/* ��DMA��־λ */
    	DMA_SetCurrDataCounter(DMA2_Stream1,200);	/* ���¸�ֵ����ֵ��������ڵ��������ܽ��յ�������֡��Ŀ */
    	DMA_Cmd(DMA2_Stream1, ENABLE);      				/*��DMA*/
    	USART_ReceiveData(USART6);   						//��������жϱ�־λ�����պ��������־λ�����ã�
    }

  	if(USART_GetFlagStatus(USART6,USART_IT_TXE)==RESET)	//���ڷ������
  	{
    	USART_ITConfig(USART6,USART_IT_TC,DISABLE);
 	}
}


void DMA_USART6_Send(DMA_Stream_TypeDef *DMA_Streamx,u8 *data,u16 size)//�������ƿ��Զ���
{
	memcpy(uart6TxBuffer,data,size);				//�������ݵ�DMA���ͻ�����	 
	DMA_Cmd(DMA_Streamx, DISABLE);                      //�ر�DMA���� 
    while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//ȷ��DMA���Ա�����	
	DMA_SetCurrDataCounter(DMA_Streamx,size);          //���ݴ�����  	
	DMA_Cmd(DMA_Streamx, ENABLE);                      //��size��DMA���� 		
	while(1){
		if(DMA_GetFlagStatus(DMA2_Stream6,DMA_FLAG_TCIF6)!=RESET){
			DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF6);
			break; 
		}
	}
}
