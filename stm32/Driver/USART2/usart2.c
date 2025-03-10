#include "usart2.h"

/**
 * @brief 解析结果变量，如需跨文件调用，需要外部声明
 */
float pos_x=0;
float pos_y=0;
float zangle=0;
float xangle=0;
float yangle=0;
float w_z=0;


void uart2_init(u32 bound)
{
  //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
 
	//串口2对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOD8复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOD9复用为USART2
	
	//USART2端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); 

    //USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART2, &USART_InitStructure); //初始化串口2
	
    USART_Cmd(USART2, ENABLE);  //使能串口2
	
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

	//USART2 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=7;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
}

/**
 * @brief 数据解析函数  如更换MCU平台或更换软件库，只需将串口接收到的值传入该函数即可解析
 * @param  rec 串口接收到的字节数据
 */
void Data_Analyse(uint8_t rec)
{
	static uint8_t ch;
	static union
	{
		uint8_t date[24];
		float ActVal[6];
	}posture;
	static uint8_t count=0;
	static uint8_t i=0;

	ch=rec;
	switch(count)
	{
		case 0:
			if(ch==0x0d)
				count++;
			else
				count=0;
			break;
		case 1:
			if(ch==0x0a)
			{
				i=0;
				count++;
			}
			else if(ch==0x0d);
			else
				count=0;
			break;
		case 2:
			posture.date[i]=ch;
			i++;
			if(i>=24)
			{
				i=0;
				count++;
			}
			break;
		case 3:
			if(ch==0x0a)
				count++;
			else
				count=0;
			break;
		case 4:
			if(ch==0x0d)
			{
				zangle=posture.ActVal[0];
				xangle=posture.ActVal[1];
				yangle=posture.ActVal[2];
				pos_x=posture.ActVal[3];
				pos_y=posture.ActVal[4];
				w_z=posture.ActVal[5];
			}
			count=0;
			break;
		default:
			count=0;
		break;
	}
}

void Stract(char strDestination[],char strSource[],int num)
{
	int i =0,j = 0;
	while(strDestination[i]!='\0') i++;
	for(j = 0;j<num;j++)
	  strDestination[i++] = strSource[j];
}

void Update_X(float New_X)
{
	int i = 0;
	char Update_x[8] = "ACTX";
	static union
	{
		float X;
		char data[4];
	}New_set;
	
	New_set.X = New_X;
	Stract(Update_x,New_set.data,4);
	for(i = 0;i<8;i++)
	{
		while(USART_GetFlagStatus(USART2 , USART_FLAG_TXE) == RESET)
			USART_SendData(USART2,Update_x[i]);
	}
}

void Update_Y(float New_Y)
{
	int i = 0;
	char Update_y[8] = "ACTY";
	static union
	{
		float Y;
		char data[4];
	}New_set;
	
	New_set.Y = New_Y;
	Stract(Update_y,New_set.data,4);
	for(i = 0;i<8;i++)
	{
		while(USART_GetFlagStatus(USART2 , USART_FLAG_TXE) == RESET)
			USART_SendData(USART2,Update_y[i]);
	}
}

/*******Send Message Function*******/

/*send one data*/
void USART2_Send_oneData(uint8_t Byte)
{
	USART_SendData(USART2,Byte);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET); 
}
/*send CharArray*/
void USART2_Send_Array(uint8_t *Array,uint16_t Length)
{
	uint16_t i;
	for(i = 0;i < Length; i++)
	{
	USART2_Send_oneData(Array[i]);
	}
}
/*send String*/
void USART2_Send_String(char *String) //"String",if you want to change the line,then printf "String\r\n"
{
	uint8_t i;
	for(i = 0; String[i] != '\0'; i++)
	USART2_Send_oneData(String[i]);
}

/*Another way to take place of printf*/
void USART2_printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	USART2_Send_String(String);
}


void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断
	{
		Res =USART_ReceiveData(USART2);//(USART2->DR);	//读取接收到的数据
		Data_Analyse(Res);
  }
} 







