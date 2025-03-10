#include "stm32f4xx.h"
#include "stdio.h"
#include "robot.h"

//��������
#define START_TASK_PRIO		1
#define START_STK_SIZE 		128  
TaskHandle_t StartTask_Handler = NULL;
void Start_Task(void *pvParameters);

//����������������
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4
	DELAY_Init();					//��ʼ����ʱ����
	
	//Hardware Init
	HardWare_Init();

    //����AppTaskCreate����
	xTaskCreate((TaskFunction_t )Start_Task,  /* ������ں��� */
								 (const char*    )"Start_Task",/* �������� */
								 (uint16_t       )START_STK_SIZE,  /* ����ջ��С */
								 (void*          )NULL,/* ������ں������� */
								 (UBaseType_t    )START_TASK_PRIO, /* ��������ȼ� */
								 (TaskHandle_t*  )&StartTask_Handler);/* ������ƿ�ָ�� */ 						
	//�������񣬿�������						 
	vTaskStartScheduler(); 
}
void Start_Task(void *pvParameters)
{	
	//�����ٽ���
	taskENTER_CRITICAL();   
	//���������˿�������
	xTaskCreate((TaskFunction_t )Robot_Task, /* ������ں��� */
			 (const char*    )"Robot_Task",/* �������� */
			 (uint16_t       )ROBOT_STK_SIZE,   /* ����ջ��С */
			 (void*          )NULL,	/* ������ں������� */
			 (UBaseType_t    )ROBOT_TASK_PRIO,	    /* ��������ȼ� */
			 (TaskHandle_t*  )&Robot_Task_Handle);/* ������ƿ�ָ�� */		 	 								 
	xTaskCreate((TaskFunction_t )Arm_Task, /* ������ں��� */
			 (const char*    )"Arm_Task",/* �������� */
			 (uint16_t       )ARM_STK_SIZE,   /* ����ջ��С */
			 (void*          )NULL,	/* ������ں������� */
			 (UBaseType_t    )ARM_TASK_PRIO,	    /* ��������ȼ� */
			 (TaskHandle_t*  )&Arm_Task_Handle);/* ������ƿ�ָ�� */								 		 
		//Adjust distance	 
	xTaskCreate((TaskFunction_t )Adjust_Task, /* ������ں��� */
			 (const char*    )"Adjust_Task",/* �������� */
			 (uint16_t       )ADJUST_STK_SIZE,   /* ����ջ��С */
			 (void*          )NULL,	/* ������ں������� */
			 (UBaseType_t    )ADJUST_TASK_PRIO,	    /* ��������ȼ� */
			 (TaskHandle_t*  )&Adjust_Task_Handle);/* ������ƿ�ָ�� */		
	xTaskCreate((TaskFunction_t )Inform_Task, /* ������ں��� */
			 (const char*    )"Inform_Task",/* �������� */
			 (uint16_t       )INFORM_STK_SIZE,   /* ����ջ��С */
			 (void*          )NULL,	/* ������ں������� */
			 (UBaseType_t    )INFORM_TASK_PRIO,	    /* ��������ȼ� */
			 (TaskHandle_t*  )&Inform_Task_Handle);/* ������ƿ�ָ�� */				 
	//ɾ��AppTaskCreate����				
	vTaskDelete(StartTask_Handler); 
	//�˳��ٽ���
	taskEXIT_CRITICAL();           			 							
}



