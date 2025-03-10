#include "stm32f4xx.h"
#include "stdio.h"
#include "robot.h"

//启动任务
#define START_TASK_PRIO		1
#define START_STK_SIZE 		128  
TaskHandle_t StartTask_Handler = NULL;
void Start_Task(void *pvParameters);

//机器人运行主任务
#define ROBOT_TASK_PRIO		5    
#define ROBOT_STK_SIZE 		512 
TaskHandle_t Robot_Task_Handle = NULL;
void Robot_Task(void *pvParameters);

#define INFORM_TASK_PRIO		2  
#define INFORM_STK_SIZE 		256   
TaskHandle_t Inform_Task_Handle = NULL;
void Inform_Task(void *pvParameters);

#define ARM_TASK_PRIO		3   
#define ARM_STK_SIZE 		256   
TaskHandle_t Arm_Task_Handle = NULL;
void Arm_Task(void *pvParameters);

//Adjust distance
#define ADJUST_TASK_PRIO		4    
#define ADJUST_STK_SIZE 		512   
TaskHandle_t Adjust_Task_Handle = NULL;
void Adjust_Task(void *pvParameters);



int main(void)
{ 
	//System Init
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	DELAY_Init();					//初始化延时函数
	
	//Hardware Init
	HardWare_Init();

    //创建AppTaskCreate任务
	xTaskCreate((TaskFunction_t )Start_Task,  /* 任务入口函数 */
								 (const char*    )"Start_Task",/* 任务名字 */
								 (uint16_t       )START_STK_SIZE,  /* 任务栈大小 */
								 (void*          )NULL,/* 任务入口函数参数 */
								 (UBaseType_t    )START_TASK_PRIO, /* 任务的优先级 */
								 (TaskHandle_t*  )&StartTask_Handler);/* 任务控制块指针 */ 						
	//启动任务，开启调度						 
	vTaskStartScheduler(); 
}
void Start_Task(void *pvParameters)
{	
	//进入临界区
	taskENTER_CRITICAL();   
	//创建机器人控制任务
	xTaskCreate((TaskFunction_t )Robot_Task, /* 任务入口函数 */
			 (const char*    )"Robot_Task",/* 任务名字 */
			 (uint16_t       )ROBOT_STK_SIZE,   /* 任务栈大小 */
			 (void*          )NULL,	/* 任务入口函数参数 */
			 (UBaseType_t    )ROBOT_TASK_PRIO,	    /* 任务的优先级 */
			 (TaskHandle_t*  )&Robot_Task_Handle);/* 任务控制块指针 */		 	 								 
	xTaskCreate((TaskFunction_t )Arm_Task, /* 任务入口函数 */
			 (const char*    )"Arm_Task",/* 任务名字 */
			 (uint16_t       )ARM_STK_SIZE,   /* 任务栈大小 */
			 (void*          )NULL,	/* 任务入口函数参数 */
			 (UBaseType_t    )ARM_TASK_PRIO,	    /* 任务的优先级 */
			 (TaskHandle_t*  )&Arm_Task_Handle);/* 任务控制块指针 */								 		 
		//Adjust distance	 
	xTaskCreate((TaskFunction_t )Adjust_Task, /* 任务入口函数 */
			 (const char*    )"Adjust_Task",/* 任务名字 */
			 (uint16_t       )ADJUST_STK_SIZE,   /* 任务栈大小 */
			 (void*          )NULL,	/* 任务入口函数参数 */
			 (UBaseType_t    )ADJUST_TASK_PRIO,	    /* 任务的优先级 */
			 (TaskHandle_t*  )&Adjust_Task_Handle);/* 任务控制块指针 */		
	xTaskCreate((TaskFunction_t )Inform_Task, /* 任务入口函数 */
			 (const char*    )"Inform_Task",/* 任务名字 */
			 (uint16_t       )INFORM_STK_SIZE,   /* 任务栈大小 */
			 (void*          )NULL,	/* 任务入口函数参数 */
			 (UBaseType_t    )INFORM_TASK_PRIO,	    /* 任务的优先级 */
			 (TaskHandle_t*  )&Inform_Task_Handle);/* 任务控制块指针 */				 
	//删除AppTaskCreate任务				
	vTaskDelete(StartTask_Handler); 
	//退出临界区
	taskEXIT_CRITICAL();           			 							
}



