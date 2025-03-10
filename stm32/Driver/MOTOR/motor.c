#include "motor.h" 

static void MOTOR_IOInit(void)
{ 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE); 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); 		
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 /*AIN1,AIN2,BIN1,CIN1,CIN2,DIN2*/; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 /*BIN2*/; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
}

static void MOTOR_TIMInit(void)
{
  TIM1_PWM_Init();
  TIM12_PWM_Init(MOTOR_Period,MOTOR_Prescaler);
}

void MOTORA_SetSpeed(int speed)
{
	int temp;

    if(speed > 0)
	{
	 AIN1=0;
	 AIN2=1;
	 temp = speed;	
	}
	else if(speed < 0)
	{
	 AIN1=1;
	 AIN2=0;
		temp = -speed;
	}
	else
	{
	 AIN1=1;
	 AIN2=1;
		temp = 0;
	}
	
	if(temp>1999)
		temp = 1999;
	
	TIM_SetCompare1(TIM1,temp);
}

void MOTORB_SetSpeed(int speed)
{
	int temp;

    if(speed > 0)
	{
	 BIN1=1;
	 BIN2=0;
		temp = speed;	
	}
	else if(speed < 0)
	{	
	 BIN1=0;
	 BIN2=1;
		temp = -speed;
	}
	else
	{
	 BIN1=1;
	 BIN2=1;
	temp = 0;
	}
	
	if(temp>1999)
		temp = 1999;
	
	TIM_SetCompare2(TIM1,temp);
}

void MOTORC_SetSpeed(int speed)
{
	int temp;

    if(speed > 0)
	{
	 CIN1=1;
	 CIN2=0;
		temp = speed;	
	}
	else if(speed < 0)
	{	
	 CIN1=0;
	 CIN2=1;
		temp = -speed;
	}
	else
	{
	 CIN1=1;
	 CIN2=1;
		temp = 0;
	}
	
	if(temp > 1999)
		temp = 1999;
	
	TIM_SetCompare3(TIM1,temp);
}
void MOTORD_SetSpeed(int speed)
{
	int temp;

    if(speed > 0)
	{
	 DIN1=0;
	 DIN2=1;
		temp = speed;	
	}
	else if(speed < 0)
	{	
	 DIN1=1;
	 DIN2=0;
	 temp = -speed;
	}
	else
	{
	 DIN1=1;
	 DIN2=1;
	 temp = 0;
	}
	
	if(temp > 1999)
		temp = 1999;
	
	TIM_SetCompare4(TIM1,temp);
}

void MOTOR_Init(void)
{
	MOTOR_IOInit();
	MOTOR_TIMInit();
}

void MOTOR_Stop(void)
{
	  MOTORA_SetSpeed(0);	
	  MOTORB_SetSpeed(0);
   	  MOTORC_SetSpeed(0);	
	  MOTORD_SetSpeed(0);
}




