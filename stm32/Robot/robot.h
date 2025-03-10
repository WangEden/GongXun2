#ifndef __ROBOT_H
#define __ROBOT_H
	 
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

//C�⺯�������ͷ�ļ�
#include <stdio.h> 
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>	 	

//FreeRTOSͷ�ļ�
#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
 
//�������ͷ�ļ�
#include "as7341.h"
#include "drive.h"
#include "delay.h"
#include "motor.h"
#include "encoder.h"  
#include "kinematics.h"
#include "uart4.h"
#include "usart2.h"
#include "usart3.h"
#include "usart6.h"
#include "control.h"
#include "usart.h"
#include "dma.h"
#include "key.h"
#include "bool.h"
#include "LobotServoController.h"


//�����������ٶ����ݽṹ��
typedef struct  
{
	double  NS;       //����ʵʱ�ٶȣ���λm/s
	float  TS;       //����Ŀ���ٶȣ���λm/s
	short  PWM;      //����PWM�����ٶ�
	
}Car_Wheel;

extern  Car_Wheel Wheel_A,Wheel_B,Wheel_C,Wheel_D; //��������������

extern int16_t A_motor_pwm_out,B_motor_pwm_out ,C_motor_pwm_out ,D_motor_pwm_out ;
extern float A_bias,A_bias_last,B_bias,B_bias_last,C_bias,C_bias_last,D_bias,D_bias_last;

extern int16_t a_motor_kp;      
extern int16_t a_motor_kd; 

extern int16_t b_motor_kp;      
extern int16_t b_motor_kd; 

extern int16_t c_motor_kp;      
extern int16_t c_motor_kd; 

extern int16_t d_motor_kp;      
extern int16_t d_motor_kd; 


extern float angle_kp;      
extern float angle_kd; 
extern float change_angle_speed;
extern float angle_bias,angle_bias_last;


/*USART*/

//����
#define  PI           3.1416     //Բ����PI
#define  SQRT3        1.732      //3��ƽ��
#define  PID_RATE     50         //PIDƵ��

#define  WHEEL_DIAMETER	      7.0	 //����ֱ��cm
#define  WHEEL_RESOLUTION     245760.0   //�������ֱ���(1024��),���ٱ�60,1204x60x4=245760
#define  WHEEL_SCALE   (float)(PI*WHEEL_DIAMETER*PID_RATE/WHEEL_RESOLUTION)  //�����ٶ�m/s�������ת��ϵ��


//������
extern TaskHandle_t Robot_Task_Handle;
extern TaskHandle_t Arm_Task_Handle;
extern TaskHandle_t Adjust_Task_Handle;
extern TaskHandle_t Inform_Task_Handle;



#endif

