#ifndef __ROBOT_H
#define __ROBOT_H
	 
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

//C库函数的相关头文件
#include <stdio.h> 
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>	 	

//FreeRTOS头文件
#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
 
//外设相关头文件
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


//机器人轮子速度数据结构体
typedef struct  
{
	double  NS;       //车轮实时速度，单位m/s
	float  TS;       //车轮目标速度，单位m/s
	short  PWM;      //车轮PWM控制速度
	
}Car_Wheel;

extern  Car_Wheel Wheel_A,Wheel_B,Wheel_C,Wheel_D; //机器人轮子数据

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

//杂类
#define  PI           3.1416     //圆周率PI
#define  SQRT3        1.732      //3开平方
#define  PID_RATE     50         //PID频率

#define  WHEEL_DIAMETER	      7.0	 //轮子直径cm
#define  WHEEL_RESOLUTION     245760.0   //编码器分辨率(1024线),减速比60,1204x60x4=245760
#define  WHEEL_SCALE   (float)(PI*WHEEL_DIAMETER*PID_RATE/WHEEL_RESOLUTION)  //轮子速度m/s与编码器转换系数


//任务句柄
extern TaskHandle_t Robot_Task_Handle;
extern TaskHandle_t Arm_Task_Handle;
extern TaskHandle_t Adjust_Task_Handle;
extern TaskHandle_t Inform_Task_Handle;



#endif

