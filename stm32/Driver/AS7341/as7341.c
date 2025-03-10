#include "as7341.h"

eMode_t measureMode;
sModeOneData_t data1;
sModeTwoData_t data2;

/******************************************************************************
function:	Send one byte of data to AS7341 via I2C
parameter:
            Addr: Register address
           Value: Write to the value of the register
Info:
******************************************************************************/
static void AS7341_Write_Byte(uint16_t Addr, uint8_t Value)
{
    u8 buff_data[1] = {0};
    buff_data[0] = Value;
    IIC_Start();
    IIC_Send_Byte(AS7341_ADDRESS << 1 | AS_write); 
    IIC_Wait_Ack();
    IIC_Send_Byte(Addr); 
    IIC_Wait_Ack();
    IIC_Send_Byte(buff_data[0]);
    IIC_Wait_Ack();
    IIC_Stop();
}
/******************************************************************************
function:	Read one byte of data to AS7341 via I2C 
parameter:
            Addr: Register address
Info:
******************************************************************************/
static uint16_t AS7341_Read_Byte(uint16_t Addr)
{
    u8 buff_data[1] = {0};
    IIC_Start();
    IIC_Send_Byte(AS7341_ADDRESS << 1 | AS_write); 
    IIC_Wait_Ack();
    IIC_Send_Byte(Addr); 
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(AS7341_ADDRESS << 1 | AS_read); 
    IIC_Wait_Ack();
    buff_data[0] = IIC_Read_Byte(0);
    IIC_Stop();
    return buff_data[0];
}

static void F1F4_Clear_NIR() 
{
  AS7341_Write_Byte(0x00, 0x30); 
  AS7341_Write_Byte(0x01, 0x01); 
  AS7341_Write_Byte(0x02, 0x00); 
  AS7341_Write_Byte(0x03, 0x00); 
  AS7341_Write_Byte(0x04, 0x00); 
  AS7341_Write_Byte(0x05, 0x42); 
  AS7341_Write_Byte(0x06, 0x00); 
  AS7341_Write_Byte(0x07, 0x00); 
  AS7341_Write_Byte(0x08, 0x50); 
  AS7341_Write_Byte(0x09, 0x00); 
  AS7341_Write_Byte(0x0A, 0x00); 
  AS7341_Write_Byte(0x0B, 0x00); 
  AS7341_Write_Byte(0x0C, 0x20); 
  AS7341_Write_Byte(0x0D, 0x04); 
  AS7341_Write_Byte(0x0E, 0x00); 
  AS7341_Write_Byte(0x0F, 0x30); 
  AS7341_Write_Byte(0x10, 0x01); 
  AS7341_Write_Byte(0x11, 0x50); 
  AS7341_Write_Byte(0x12, 0x00); 
  AS7341_Write_Byte(0x13, 0x06); 
}
/******************************************************************************
function:	Configure SMUX for sensors F5-8, Clear and NIR
info:	
******************************************************************************/
static void F5F8_Clear_NIR() 
{
  AS7341_Write_Byte(0x00, 0x00); 
  AS7341_Write_Byte(0x01, 0x00); 
  AS7341_Write_Byte(0x02, 0x00); 
  AS7341_Write_Byte(0x03, 0x40); 
  AS7341_Write_Byte(0x04, 0x02); 
  AS7341_Write_Byte(0x05, 0x00); 
  AS7341_Write_Byte(0x06, 0x10); 
  AS7341_Write_Byte(0x07, 0x03); 
  AS7341_Write_Byte(0x08, 0x50); 
  AS7341_Write_Byte(0x09, 0x10); 
  AS7341_Write_Byte(0x0A, 0x03); 
  AS7341_Write_Byte(0x0B, 0x00); 
  AS7341_Write_Byte(0x0C, 0x00); 
  AS7341_Write_Byte(0x0D, 0x00); 
  AS7341_Write_Byte(0x0E, 0x24); 
  AS7341_Write_Byte(0x0F, 0x00); 
  AS7341_Write_Byte(0x10, 0x00); 
  AS7341_Write_Byte(0x11, 0x50); 
  AS7341_Write_Byte(0x12, 0x00); 
  AS7341_Write_Byte(0x13, 0x06); 
}

/******************************************************************************
function:  Gets data for all channels
info:		
******************************************************************************/
static UWORD AS7341_GetChannelData(UBYTE channel)
{
  UWORD data[2];
  UWORD channelData = 0x0000;
  data[0] = AS7341_Read_Byte(AS7341_CH0_DATA_L + channel*2); 
  data[1] = AS7341_Read_Byte(AS7341_CH0_DATA_H + channel*2); 
  channelData = data[1];
  channelData = (channelData<<8) | data[0];
  vTaskDelay(50);
  return channelData;
}
/******************************************************************************
function:  Use SMUX to read data from the high channel
info:		
******************************************************************************/
static sModeOneData_t AS7341_ReadSpectralDataOne()
{
  sModeOneData_t data;
  data.channel1 = AS7341_GetChannelData(0);
  data.channel2 = AS7341_GetChannelData(1);
  data.channel3 = AS7341_GetChannelData(2);
  data.channel4 = AS7341_GetChannelData(3);
  data.CLEAR = AS7341_GetChannelData(4);
  data.NIR = AS7341_GetChannelData(5);
  return data;
}

/******************************************************************************
function:  Use SMUX to read data from the high channel
info:		
******************************************************************************/

static sModeTwoData_t AS7341_ReadSpectralDataTwo()
{
  sModeTwoData_t data;
  data.channel5 = AS7341_GetChannelData(0);
  data.channel6 = AS7341_GetChannelData(1);
  data.channel7 = AS7341_GetChannelData(2);
  data.channel8 = AS7341_GetChannelData(3);
  data.CLEAR = AS7341_GetChannelData(4);
  data.NIR = AS7341_GetChannelData(5);
  return data;
}


static int AS7341_MeasureComplete(){
	UBYTE status;
	status = AS7341_Read_Byte(AS7341_STATUS_2); 
	if((status & (1<<6))){
		return true;
	}
	else{
		return false;
	}
}


static void AS7341_Enable(int flag)
{
	UBYTE data,data1;
	data=AS7341_Read_Byte(AS7341_ENABLE);
    if(flag == true){
    data = data | (1<<0);
    } else {
    data = data & (~1);
    }
	AS7341_Write_Byte(AS7341_ENABLE,data);
    delay_ms(500);
    data1=AS7341_Read_Byte(AS7341_ENABLE);
    if(data1 == data)
        USART3_printf("Initialization is complete !\r\n");
    else
    {
        USART3_printf("initialization is failure !\r\n");
        USART3_printf("Please check whether the IIC pin is connected correctly and whether the IIC function is normal \r\n");
        USART3_printf("Please press the reset button or run the program again after checking \r\n");
        while(1);
    }
    AS7341_Write_Byte(0x00, 0x30);
}

/******************************************************************************
function: enable Spectral measure
info:	
******************************************************************************/
static void AS7341_EnableSpectralMeasure(int flag)//Enable spectral measurement
{
    UBYTE data;
    data=AS7341_Read_Byte(AS7341_ENABLE);
    if(flag == true){
      data = data | (1<<1);
    } else {
      data = data & (~(1<<1));
    }
    AS7341_Write_Byte(AS7341_ENABLE,data);
}

/******************************************************************************
function: enable SMUX
info:	
******************************************************************************/
static void AS7341_EnableSMUX(int flag)//Enable multiplexer
/*The other available channels can be accessed by amultiplexer (SMUX) connecting them to one of the internal ADCs.*/
{
  UBYTE data;
    data=AS7341_Read_Byte(AS7341_ENABLE);
  if(flag == true){
    data = data | (1<<4);
  } else {
    data = data & (~(1<<4));
  }
    AS7341_Write_Byte(AS7341_ENABLE,data);
}



/******************************************************************************
function:	set REG_BANK
info:	0: Register access to register 0x80 and above
		1: Register access to register 0x60 to 0x74
******************************************************************************/
static void AS7341_SetBank(UBYTE addr)
{
  UBYTE data=0;
  data = AS7341_Read_Byte(AS7341_CFG_0);
  if(addr == 1){
  
    data = data | (1<<4);
  }
  
  if(addr == 0){
  
    data = data & (~(1<<4));
  }
  AS7341_Write_Byte(AS7341_CFG_0,data);
}


/******************************************************************************
function:	Control the brightness of the LED
info:   LED 			 :   true  : LED ON
											 false : LED OFF
         current   :  intensity control
******************************************************************************/
static void AS7341_ControlLed(UBYTE LED,UBYTE current)
{
  UBYTE data=0;
  if(current < 1) current = 1;
    current--;
  if(current > 19) current = 19;
  AS7341_SetBank(1); 
	if(LED == true)	
		data = 0x80 | current;
	else	
		data = current;
  AS7341_Write_Byte(AS7341_LED,data);
  vTaskDelay(100);
  AS7341_SetBank(0);
}

/******************************************************************************
function:	choose model for spectral measurement
info:	
******************************************************************************/
static void AS7341_Config(eMode_t mode)
{
  UBYTE data;
  AS7341_SetBank(1);
  data=AS7341_Read_Byte(AS7341_CONFIG);
  switch(mode){
    case eSpm : {
      data = (data & (~3)) | eSpm;
    };
	break;
    case eSyns : {
      data = (data & (~3)) | eSyns;
    };
	break;
    case eSynd : {
      data = (data & (~3)) | eSynd;
    };
	break;
    default : break;
  }
  AS7341_Write_Byte(AS7341_CONFIG,data);
  AS7341_SetBank(0);
}

/******************************************************************************
function:	Set GPIO to input or output mode
info:
******************************************************************************/
static void AS7341_SetGpioMode(UBYTE mode)
{
  UBYTE data;

  data = AS7341_Read_Byte(AS7341_GPIO_2);  
  if(mode == INPUT){
     data = data | (1<<2);
  }
  
  if(mode == OUTPUT){
     data = data & (~(1<<2));
  }
  AS7341_Write_Byte(AS7341_GPIO_2,data);
}





/******************************************************************************
function:	Start the measurement
info:		This function only handles SPM and SYNS modes.
******************************************************************************/
static void AS7341_startMeasure(eChChoose_t mode)
{
	  UBYTE data=0;
	  data = AS7341_Read_Byte(AS7341_CFG_0);  
	  data = data & (~(1<<4));

	  AS7341_Write_Byte(AS7341_CFG_0,data);
	  
	  AS7341_EnableSpectralMeasure(false);
	  AS7341_Write_Byte(0xAF,0x10);//SMUX Command config
	  
	  if(mode  == eF1F4ClearNIR)
      F1F4_Clear_NIR();
	  else if(mode  == eF5F8ClearNIR)
	  F5F8_Clear_NIR();
	  AS7341_EnableSMUX(true);
	  if(measureMode == eSyns){
	  AS7341_SetGpioMode(INPUT);
      AS7341_Config(eSyns);
      }
	  else if(measureMode == eSpm){
      AS7341_Config(eSpm);
	  }
	  AS7341_EnableSpectralMeasure(true);
      if(measureMode == eSpm){
        while(!AS7341_MeasureComplete()){
        vTaskDelay(1);
        }
      }
}

/************************************************/
/************************************************/
/************************************************/
/************************************************/
/************************************************/

UBYTE AS7341_Init(eMode_t mode)
{
	IIC_Init();//8-bit address
	AS7341_Enable(true);
	measureMode=mode;
	return 0;
}

void AS7341_ATIME_config(UBYTE value)
{
  AS7341_Write_Byte(AS7341_ATIME,value);
}

void AS7341_ASTEP_config(UWORD value)
{
  UBYTE highValue,lowValue;
  lowValue = value & 0x00ff;
  highValue = value >> 8 ;
  AS7341_Write_Byte(AS7341_ASTEP_L,lowValue);
  AS7341_Write_Byte(AS7341_ASTEP_H,highValue);
}


void AS7341_AGAIN_config(UBYTE value)
{
  if(value > 10) value = 10;
  AS7341_Write_Byte(AS7341_CFG_1,value);
}

/******************************************************************************
function:	enable led
info:true		: Enable
			false		: Anergy
******************************************************************************/
void AS7341_EnableLED(int flag)
{
  UBYTE data=0;
  UBYTE data1=0;
  AS7341_SetBank(1);
  data = AS7341_Read_Byte(AS7341_CONFIG);
  data1 = AS7341_Read_Byte(AS7341_LED);
  if(flag== true)
	{
    data = data | 0x08;
	} 
	else 
	{
    data = data & 0xf7;
    data1 = data1 & 0x7f;
		AS7341_Write_Byte(AS7341_LED,data1);
  }
  AS7341_Write_Byte(AS7341_CONFIG,data);
  AS7341_SetBank(0);
}


void Data_Convert(void)
{

	AS7341_ControlLed(true,5);//Turn on or off the LED and set the brightness of the LED
	AS7341_startMeasure(eF1F4ClearNIR);
	data1 = AS7341_ReadSpectralDataOne();
	AS7341_startMeasure(eF5F8ClearNIR);
	data2 =AS7341_ReadSpectralDataTwo();

        USART3_printf("1. Channel:\r\n");
		USART3_printf("dark purple = %d\r\n",data1.channel1);
		USART3_printf("blue purple = %d\r\n",data1.channel2);
		USART3_printf("blue sky = %d\r\n",data1.channel3);   
		USART3_printf("green = %d\r\n",data1.channel4);
		USART3_printf("yellow = %d\r\n",data2.channel5);
		USART3_printf("orange = %d\r\n",data2.channel6);
		USART3_printf("red = %d\r\n",data2.channel7);
		USART3_printf("red black = %d\r\n",data2.channel8);
		USART3_printf("2. Clear:\r\n");
		USART3_printf("%d\r\n",data2.CLEAR);
		USART3_printf("3. NIR:\r\n");
		USART3_printf("%d\r\n",data2.NIR);
	
		USART3_printf("--------------------------\r\n");

}






