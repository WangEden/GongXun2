#include "dma.h"				

uint8_t uart1RxBuffer[200];
uint8_t uart3RxBuffer[200];
uint8_t uart6TxBuffer[200];
uint8_t uart6RxBuffer[200];

uint8_t uart6TxDataSize = 0;
uint8_t uart6RxDataSize = 0;
uint8_t uart1RxDataSize = 0;
uint8_t uart3RxDataSize = 0;

// 存储偏移量
int deltaX = 0;
int deltaY = 0;
// 存储命令
char command[4];


void MYDMA_Config1(DMA_Stream_TypeDef *DMA_Streamx)
{ 
 
	DMA_InitTypeDef  DMA_InitStructure;
	if((u32)DMA_Streamx>(u32)DMA2)//得到当前stream是属于DMA2还是DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能 		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1时钟使能 
	}
  DMA_DeInit(DMA_Streamx);
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//等待DMA可配置 
			
	/*配置串口DMA接收*/
	DMA_DeInit(DMA2_Stream5);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4; 							//通道选择  USART6_RX
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;		//DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)uart1RxBuffer;		//DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;   				//存储器到外设模式
	DMA_InitStructure.DMA_BufferSize = 200;						//数据传输量
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//使用普通模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;				    //高等优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                  //不开启FIFO模式
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;           //FIFO阈值
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//外设突发单次传输
	DMA_Init(DMA2_Stream5, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream5, ENABLE); 
}  

void USART1_IRQHandler(void)  												//串口1中断服务程序
{
    if(USART_GetITStatus(USART1,USART_IT_IDLE)!=RESET) 	//空闲中断触发
    {
    	DMA_Cmd(DMA2_Stream5, DISABLE);  					   /* 暂时关闭dma，数据尚未处理 */
    	uart1RxDataSize = 200 - DMA_GetCurrDataCounter(DMA2_Stream5);/* 获取接收到的数据长度 单位为字节*/
    	DMA_ClearFlag(DMA2_Stream5,DMA_FLAG_TCIF5);  		/* 清DMA标志位 */
    	DMA_SetCurrDataCounter(DMA2_Stream5,200);	/* 重新赋值计数值，必须大于等于最大可能接收到的数据帧数目 */
    	DMA_Cmd(DMA2_Stream5, ENABLE);      				/*打开DMA*/
    	USART_ReceiveData(USART1);   						//清除空闲中断标志位（接收函数有清标志位的作用）
    }
  	if(USART_GetFlagStatus(USART1,USART_IT_TXE)==RESET)	//串口发送完成
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
	if((u32)DMA_Streamx>(u32)DMA2)//得到当前stream是属于DMA2还是DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能 		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1时钟使能 
	}
  DMA_DeInit(DMA_Streamx);
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//等待DMA可配置 
			
	/*配置串口DMA接收*/
	DMA_DeInit(DMA1_Stream1);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4; 							//通道选择  USART6_RX
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;		//DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)uart3RxBuffer;		//DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;   				//存储器到外设模式
	DMA_InitStructure.DMA_BufferSize = 200;						//数据传输量
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//使用普通模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;				    //高等优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                  //不开启FIFO模式
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;           //FIFO阈值
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//外设突发单次传输
	DMA_Init(DMA1_Stream1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Stream1, ENABLE); 
}  

void USART3_IRQHandler(void)  												//串口1中断服务程序
{
    if(USART_GetITStatus(USART3,USART_IT_IDLE)!=RESET) 	//空闲中断触发
    {
    	DMA_Cmd(DMA1_Stream1, DISABLE);  					   /* 暂时关闭dma，数据尚未处理 */
    	uart3RxDataSize = 200 - DMA_GetCurrDataCounter(DMA1_Stream1);/* 获取接收到的数据长度 单位为字节*/
    	DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_TCIF1);  		/* 清DMA标志位 */
    	DMA_SetCurrDataCounter(DMA1_Stream1,200);	/* 重新赋值计数值，必须大于等于最大可能接收到的数据帧数目 */
    	DMA_Cmd(DMA1_Stream1, ENABLE);      				/*打开DMA*/
    	USART_ReceiveData(USART3);   						//清除空闲中断标志位（接收函数有清标志位的作用）
    }
  	if(USART_GetFlagStatus(USART3,USART_IT_TXE)==RESET)	//串口发送完成
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
	if((u32)DMA_Streamx>(u32)DMA2)//得到当前stream是属于DMA2还是DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能 		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1时钟使能 
	}
  DMA_DeInit(DMA_Streamx);
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//等待DMA可配置 
	
  /* 配置 DMA Stream */
  DMA_InitStructure.DMA_Channel = chx;  //通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA 存储器0地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
  DMA_InitStructure.DMA_BufferSize = ndtr;//数据传输量 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
  DMA_Init(DMA_Streamx, &DMA_InitStructure);//初始化DMA Stream
		
	/*配置串口DMA接收*/
	DMA_DeInit(DMA2_Stream1);
	DMA_InitStructure.DMA_Channel = DMA_Channel_5; 							//通道选择  USART6_RX
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART6->DR;		//DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)uart6RxBuffer;		//DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;   				//存储器到外设模式
	DMA_InitStructure.DMA_BufferSize = 200;						//数据传输量
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//使用普通模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;				    //高等优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                  //不开启FIFO模式
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;           //FIFO阈值
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//外设突发单次传输
	DMA_Init(DMA2_Stream1, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream1, ENABLE); //使能DMA2_Stream2通道
}  

void USART6_IRQHandler(void)  												//串口1中断服务程序
{
    if(USART_GetITStatus(USART6,USART_IT_IDLE)!=RESET) 	//空闲中断触发
    {
    	DMA_Cmd(DMA2_Stream1, DISABLE);  					   /* 暂时关闭dma，数据尚未处理 */
    	uart6RxDataSize = 200 - DMA_GetCurrDataCounter(DMA2_Stream1);/* 获取接收到的数据长度 单位为字节*/
    	DMA_ClearFlag(DMA2_Stream1,DMA_FLAG_TCIF1);  		/* 清DMA标志位 */
    	DMA_SetCurrDataCounter(DMA2_Stream1,200);	/* 重新赋值计数值，必须大于等于最大可能接收到的数据帧数目 */
    	DMA_Cmd(DMA2_Stream1, ENABLE);      				/*打开DMA*/
    	USART_ReceiveData(USART6);   						//清除空闲中断标志位（接收函数有清标志位的作用）
    }

  	if(USART_GetFlagStatus(USART6,USART_IT_TXE)==RESET)	//串口发送完成
  	{
    	USART_ITConfig(USART6,USART_IT_TC,DISABLE);
 	}
}


void DMA_USART6_Send(DMA_Stream_TypeDef *DMA_Streamx,u8 *data,u16 size)//函数名称可自定义
{
	memcpy(uart6TxBuffer,data,size);				//复制数据到DMA发送缓存区	 
	DMA_Cmd(DMA_Streamx, DISABLE);                      //关闭DMA传输 
    while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//确保DMA可以被设置	
	DMA_SetCurrDataCounter(DMA_Streamx,size);          //数据传输量  	
	DMA_Cmd(DMA_Streamx, ENABLE);                      //开size启DMA传输 		
	while(1){
		if(DMA_GetFlagStatus(DMA2_Stream6,DMA_FLAG_TCIF6)!=RESET){
			DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF6);
			break; 
		}
	}
}
