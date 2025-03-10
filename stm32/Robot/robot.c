#include "robot.h"

Car_Wheel Wheel_A,Wheel_B,Wheel_C,Wheel_D;
int xoy=1;
static void Look(void)
{
	//看物块
	Sum_Control(94.34,71.59,228.17,98.03,720,720,1200,1500);
}
static void Catch_Disk(void)
{
	Sum_Control(94.34,65.904,241.76,110.816,360,360,360,360);//抓取圆盘
	vTaskDelay(500);
	runActionGroup(2,1);//抓
    vTaskDelay(1000);
}
static void Red_Get(float v1,float v2,float v3,float v4)
{
	Sum_Control(108.801,77.61,42.02,262.483,v1,v2,v3,v4);
	vTaskDelay(500);
	runActionGroup(2,1);//抓
    vTaskDelay(1000);
}
static void Red_Set_Mid(float v1,float v2,float v3,float v4)
{
	 Sum_Control(109.656,80.139,59.073,280.041,v1,v2,v3,v4);
}
static void Red_Set(float v1,float v2,float v3,float v4)
{
	Sum_Control(109.801,77.639,46.573,268.041,v1,v2,v3,v4);	
	vTaskDelay(500);
	runActionGroup(1,1);//放
	vTaskDelay(1000);
}
static void Red_Set_Round(float v1,float v2,float v3,float v4)
{
	Sum_Control(55.970, 5.417, 167.188, 102.015, v1,v2,v3,v4);
	vTaskDelay(2000);
	runActionGroup(1,1);//放
	vTaskDelay(1000);
}
static void Red_Get_Round(float v1,float v2,float v3,float v4)
{
	Sum_Control(56.339, 25.589, 169.669, 88.529,720,720,720,720);
	vTaskDelay(1000);
	Sum_Control(55.970, 5.417, 167.188, 102.015, v1,v2,v3,v4);
	vTaskDelay(500);
	runActionGroup(2,1);
	vTaskDelay(1000);
}
static void Green_Get(float v1,float v2,float v3,float v4)
{
	Sum_Control(73.389,78.81,42.63,260.0,v1,v2,v3,v4);
	vTaskDelay(500);
	runActionGroup(2,1);//抓
    vTaskDelay(1000);
}
static void Green_Set_Mid(float v1,float v2,float v3,float v4)
{
	Sum_Control(71.195,76.632,50.958,270.097,v1,v2,v3,v4);
}
static void Green_Set(float v1,float v2,float v3,float v4)
{
	Sum_Control(70.909,77.557,46.41,265.436,v1,v2,v3,v4);
	vTaskDelay(500);
	runActionGroup(1,1);//放
	vTaskDelay(1000);
}
static void Green_Set_Round(float v1,float v2,float v3,float v4)
{
	Sum_Control(92.25, 40.843, 241.904, 139.338,v1,v2,v3,v4);
	vTaskDelay(2000);
	runActionGroup(1,1);//放
	vTaskDelay(1000);
}
static void Green_Get_Round(float v1,float v2,float v3,float v4)
{
	Sum_Control(94.339, 65.089, 238.169, 113.529,720,720,720,720);
	vTaskDelay(1000);
	Sum_Control(92.25, 40.843, 241.904, 139.338,v1,v2,v3,v4);
	vTaskDelay(500);
	runActionGroup(2,1);
	vTaskDelay(1000);
}
static void Blue_Get(float v1,float v2,float v3,float v4)
{
	Sum_Control(32.203,78.202,42.462,258.756,v1,v2,v3,v4);
	vTaskDelay(500);
	runActionGroup(2,1);//抓
    vTaskDelay(1000);
}
static void Blue_Set_Mid(float v1,float v2,float v3,float v4)
{
	Sum_Control(31.203,81.324,57.206,275.349,v1,v2,v3,v4);	
}
static void Blue_Set(float v1,float v2,float v3,float v4)
{
	Sum_Control(31.203,77.50,45.69,265.91,v1,v2,v3,v4);	
	vTaskDelay(500);
	runActionGroup(1,1);//放
	vTaskDelay(1000);
}
static void Blue_Set_Round(float v1,float v2,float v3,float v4)
{
	Sum_Control(137.278, 34.316, 227.847, 131.777,v1,v2,v3,v4);
	vTaskDelay(2000);
	runActionGroup(1,1);//放
	vTaskDelay(1000);
}
static void Blue_Get_Round(float v1,float v2,float v3,float v4)
{
	Sum_Control(137.839, 54.589, 222.169, 109.029,720,720,720,720);
	vTaskDelay(1000);
	Sum_Control(137.278, 34.316, 227.847, 131.777,v1,v2,v3,v4);
	vTaskDelay(500);
	runActionGroup(2,1);
	vTaskDelay(1000);
}
/**
  * @简  述  机器人管理任务
  * @参  数  无
  * @返回值  无
  */
void Robot_Task(void* parameter)
{	
	

	Init_Parmetre(22,120,22,120,22,120,22,120);
	Clear_Angle();
	setAllActionGroupSpeed(300);
	vTaskDelay(1000);
	//run2
	Look();  
	
	//test
//	xoy =2;
//	while(1)
//	 {
//	     vTaskDelay(50);	  
//		 if((command[0]=='r')&&(command[1]=='w')&&(command[2]=='w')&&(command[3]=='c'))
//         break;
//	 }
	/*OUT*/
	while(1)
	{
		vTaskDelay(10);
		ROBOT_Kinematics();
		Set_TargetSpeed_A(15.0f);
		Set_TargetSpeed_C(15.0f);
		MOTORB_SetSpeed(0);	
		MOTORD_SetSpeed(0);

		if(pos_y >= 130)
			break;
	}
/*********************************************************************************
 任务一：扫二维码
**********************************************************************************/
	 Arive_Destination(Right , 0 ,15.0f , 80.0 , 80.0 , 200,  650.0 , 0 );	
	 while(1)
	{ 
	   vTaskDelay(50);
	   if((command[0]=='r')&&(command[1]=='w')&&(command[2]=='w')&&(command[3]=='c'))
	   {command[0]='0';break;}  
	}
/*********************************************************************************
 任务二：原料区(圆盘)取三个物块
**********************************************************************************/
	Arive_Destination(Right , 1 ,20.0f , 100.0 , 80.0 , 300, 1485 , 130);   
     /*通知树莓派到达圆盘*/
	 printf("wait");	
	 while(1)
	 {	
	     vTaskDelay(50);
		  if((command[0]=='r')&&(command[1]=='w')&&(command[2]=='w')&&(command[3]=='c'))
		  {command[0]='0';break;}      
	 }
	 Arive_Destination(Front , 0 ,10.0f , 40.0 , 40.0 , 50, 0 , 165);
/*********************************************************************************
 任务三：粗加工区(1x3)先放置三个物块后搬走三个物块
**********************************************************************************/
	 Arive_Destination(Right , 0 ,15.0f , 80.0 , 80.0 , 200, 1840.0 , 0 );
	 Anti_Turnto_90();
	 xoy = 2;
	 Arive_Destination2(Front , 1 ,20.0f , 100.0 , 80.0 , 300,  1870.0 , 1055.0 );
	 vTaskDelay(50);
     printf("wait");
	 while(1)
	 {
	     vTaskDelay(50);	  
		 if((command[0]=='r')&&(command[1]=='w')&&(command[2]=='w')&&(command[3]=='c'))
         {command[0]='0';break;}  
	 }
	 Init_Parmetre(22,120,22,120,22,120,22,120);
/*********************************************************************************
  任务四：暂存区(2x3)放置三个物块
**********************************************************************************/
	Arive_Destination2(Front , 0 ,20.0f , 100.0 , 80.0 , 300,  0 , 1670.0 );
    Anti90_Turnto_180();
	xoy = 3;
    Arive_Destination3(Left , 1 ,20.0f , 100.0 , 80.0 , 300, 1050.0 , 1745.0 );
	vTaskDelay(50);
	 while(1)
	 {		 
	     vTaskDelay(50);
		  if((command[0]=='r')&&(command[1]=='w')&&(command[2]=='w')&&(command[3]=='c'))
	     {command[0]='0';break;}  
	 }
/*********************************************************************************
  任务五：再次返回原料区(圆盘)取三个物块
**********************************************************************************/
    Arive_Destination3(Right , 0 ,20.0f , 100.0 , 80.0 , 300,  1830.0 , 0 );
	Turn180_to_90();	 
	 xoy = 2;
	Arive_Destination2(Back , 0 ,20.0f , 100.0 , 80.0 , 300, 0 , 180.0 );
	Turn90_to_0();
	 xoy = 1;	 
	Arive_Destination(Left , 1 ,15.0f , 80.0 , 80.0 , 200, 1450.0 , 170.0 );
	vTaskDelay(50);
    while(1)
	 {
	     vTaskDelay(50);
		  if((command[0]=='r')&&(command[1]=='w')&&(command[2]=='w')&&(command[3]=='c'))
	      {command[0]='0';break;}  
	 }
/*********************************************************************************
 任务六：粗加工区(1x3)先放置三个物块后搬走三个物块
**********************************************************************************/	 
	 Arive_Destination(Right , 0 ,15.0f , 80.0 , 80.0 , 200, 1850.0 , 0 );
	 Anti_Turnto_90();
	 	 xoy = 2;
	 Arive_Destination2(Front , 1 ,20.0f , 100.0 , 80.0 , 300,  1830.0 , 1060.0 );
	 vTaskDelay(50);
	 while(1)
	 {
	     vTaskDelay(50);
		  if((command[0]=='r')&&(command[1]=='w')&&(command[2]=='w')&&(command[3]=='c'))
	      {command[0]='0';break;}  
	 }
/*********************************************************************************
  任务七：暂存区(2x3)放置三个物块
**********************************************************************************/
	Arive_Destination2(Front , 0 ,20.0f , 100.0 , 80.0 , 300,  0 , 1670.0 );
	Anti90_Turnto_180();
	xoy = 3;
	Arive_Destination3(Left , 1 ,20.0f , 100.0 , 80.0 , 300, 1035.0 , 1700.0 );
	vTaskDelay(50);
	 while(1)
	 {
	     vTaskDelay(50);
		  if((command[0]=='r')&&(command[1]=='w')&&(command[2]=='w')&&(command[3]=='c'))
	      {command[0]='0';break;}  
	 }
/*********************************************************************************
  任务八：回零点
**********************************************************************************/
    Arive_Destination3(Left , 0 ,20.0f , 100.0 , 80.0 , 300, 100.0 , 0.0 );
	Arive_Destination3(Back , 1 ,20.0f , 100.0 , 80.0 , 300, 5.0 , 25.0 ); 
	 while(1)
	 {
		  vTaskDelay(50);
	 }
 }

void Adjust_Task(void *pvParameters)
{
	static portTickType WakeTime;
	const portTickType Timement = pdMS_TO_TICKS(10); 
	WakeTime = xTaskGetTickCount();
	Clear_Angle();
	static int adjust_x;
	static int adjust_y;
	//只调节一次
    while(1)
    {  
	  vTaskDelay(50);
	  if((command[0]=='k')&&(command[1]=='s')&&(command[2]=='t')&&(command[3]=='z'))
	  {
		  command[0]= '0';
		  Clear_Angle();
		  adjust_x = pos_x;
		  adjust_y = pos_y;
		  Init_Parmetre(22,600,22,600,22,600,22,600);
		  while(1)
          {				
              			  
			  vTaskDelayUntil(&WakeTime,Timement); 	  
	          if(deltaX > 0)
	          {
				  if((adjust_x-pos_x) > (deltaX - 25)) 
				      {MOTOR_Stop(); Clear_Angle(); break;}
					  
				  else if(xoy == 1)
				  {	
                      ROBOT_Kinematics();					  
					  Cal_AngleErr(); 
					  Set_TargetSpeed_A(-3.2f - 0.0f);	
			          Set_TargetSpeed_C(-3.2f + 0.0f);	
			          Set_TargetSpeed_B(-3.2f + 0.0f);	
			          Set_TargetSpeed_D(-3.2f - 0.0f);

				  }
				  else if(xoy == 2)
				  {
					  ROBOT_Kinematics();
					  Cal_AngleErr2();
					  Set_TargetSpeed_A(3.2f - 0.0f);	
					 Set_TargetSpeed_C(3.2f + 0.0f);	
					 Set_TargetSpeed_B(-3.2f + 0.0f);	
					 Set_TargetSpeed_D(-3.2f - 0.0f);
				  }
				  else if(xoy == 3)
				  {
					  ROBOT_Kinematics();
					  Cal_AngleErr3();
					  Set_TargetSpeed_A(3.2f - 0.0f);	
					 Set_TargetSpeed_C(3.2f + 0.0f);	
					 Set_TargetSpeed_B(3.2f + 0.0f);	
					 Set_TargetSpeed_D(3.2f - 0.0f);
					  break;
				  }	    
	          }
			  else if(deltaX < 0)
			  {
				  if((adjust_x - pos_x) < (deltaX + 25) ) 
				      {MOTOR_Stop(); Clear_Angle(); break;}
					
                  else if(xoy == 1)
				  {	
					 ROBOT_Kinematics();
                      Cal_AngleErr(); 	
					  Set_TargetSpeed_A(3.2f - 0.0f);	
					 Set_TargetSpeed_C(3.2f + 0.0f);	
					 Set_TargetSpeed_B(3.2f + 0.0f);	
					 Set_TargetSpeed_D(3.2f - 0.0f);
				  }
                  else if(xoy == 2)
				  {
					  ROBOT_Kinematics();
					  Cal_AngleErr2();
					  Set_TargetSpeed_A(-3.2f - 0.0f);	
					 Set_TargetSpeed_C(-3.2f + 0.0f);	
					 Set_TargetSpeed_B(3.2f + 0.0f);	
					 Set_TargetSpeed_D(3.2f - 0.0f);
				  }	
				  else if(xoy == 3)
				  {
					  ROBOT_Kinematics();
	                  Cal_AngleErr3();
					  Set_TargetSpeed_A(-3.2f - 0.0f);	
					 Set_TargetSpeed_C(-3.2f + 0.0f);	
					 Set_TargetSpeed_B(-3.2f + 0.0f);	
					 Set_TargetSpeed_D(-3.2f - 0.0f);
                   }			    
			  }
              else
				 {MOTOR_Stop(); Clear_Angle(); break;}			  
		  }
		  while(1)
		 {
			  vTaskDelayUntil(&WakeTime,Timement); 
			  if(deltaY > 0)
			  {
				  if(adjust_y - pos_y > (deltaY - 10))
				  {MOTOR_Stop(); Clear_Angle(); break;}
				  
				  else if(xoy == 1)
				  {	
					  ROBOT_Kinematics();
                      Cal_AngleErr();					  	
					   Set_TargetSpeed_A(-3.2f - 0.0f);	
					 Set_TargetSpeed_C(-3.2f + 0.0f);	
					 Set_TargetSpeed_B(3.2f + 0.0f);	
					 Set_TargetSpeed_D(3.2f - 0.0f);
				  }
                  else if(xoy == 2)
				  {
					  ROBOT_Kinematics();
					  Cal_AngleErr2();
					  Set_TargetSpeed_A(-3.2f - 0.0f);	
					 Set_TargetSpeed_C(-3.2f + 0.0f);	
					 Set_TargetSpeed_B(-3.2f + 0.0f);	
					 Set_TargetSpeed_D(-3.2f - 0.0f);
				  }	
				  else if(xoy == 3 )
				  {
					  ROBOT_Kinematics();
	                  Cal_AngleErr3();
					  Set_TargetSpeed_A(3.2f - 0.0f);	
					 Set_TargetSpeed_C(3.2f + 0.0f);	
					 Set_TargetSpeed_B(-3.2f + 0.0f);	
					 Set_TargetSpeed_D(-3.2f - 0.0f);
                   }		    
			  }
			  else if(deltaY < 0)
			  {
				  if(adjust_y - pos_y < (deltaY + 10))
				  {MOTOR_Stop(); Clear_Angle(); break;}
				  
				  else if(xoy == 1)
				  {	
					  ROBOT_Kinematics();
					  Cal_AngleErr(); 	
					  Set_TargetSpeed_A(3.2f - 0.0f);	
					 Set_TargetSpeed_C(3.2f + 0.0f);	
					 Set_TargetSpeed_B(-3.2f + 0.0f);	
					 Set_TargetSpeed_D(-3.2f - 0.0f);
				  }
                  else if(xoy == 2)
				  {
					  ROBOT_Kinematics();
					  Cal_AngleErr2();
					  Set_TargetSpeed_A(3.2f - 0.0f);	
					 Set_TargetSpeed_C(3.2f + 0.0f);	
					 Set_TargetSpeed_B(3.2f + 0.0f);	
					 Set_TargetSpeed_D(3.2f - 0.0f);
				  }	
				  else if(xoy == 3 )
				  {
					   ROBOT_Kinematics();
					  Cal_AngleErr3();              
					 Set_TargetSpeed_A(-3.2f - 0.0f );	
					 Set_TargetSpeed_C(-3.2f + 0.0f );	
					 Set_TargetSpeed_B(3.2f + 0.0f );	
					 Set_TargetSpeed_D(3.2f - 0.0f );
                   }	    
			  }
			  else
			  	 {MOTOR_Stop(); Clear_Angle(); break;}
		 }
		 //printf("wtok");
	  }	  
	} 
}	

void Inform_Task(void *pvParameters)
{
	while(1)
	{
		vTaskDelay(1000);
		USART3_printf("pos_x = %f   pos_y = %f   zangle = %f \r\n", pos_x , pos_y , zangle );
	    USART3_printf("A_speed = %f   B_speed = %f   C_speed = %f  D_speed = %f\r\n", Wheel_A.NS , Wheel_B.NS , Wheel_C.NS ,Wheel_D.NS );		
		USART3_printf("Angle1 = %f , Angle2 = %f ,Angle3 = %f ,Angle4 = %f ,Id = %d \r\n",Motor_Angle[0],Motor_Angle[1],Motor_Angle[2],Motor_Angle[3],Motor_Id);
	    USART3_printf("command = %s, deltaX = %d , deltaY = %d \r\n",command,deltaX,deltaY);
		USART3_printf("\r\n");
	}
}
void Arm_Task(void* parameter)
{
	while(1)
	{

		if((command[0] == 'r')&&(command[1] == 'u')&&(command[2] == 'n')&&(command[3] == '2'))
		{
			//看物块
			command[0] = '0';
			Sum_Control(94.34,71.59,228.17,98.03,720,720,1200,1500);
		}
		else if((command[0] == 's')&&(command[1] == 'e')&&(command[2] == 't')&&(command[3] == 'r'))
		{
			//放红色到圆盘
			command[0] = '0';
			Catch_Disk();
			Red_Set_Mid(720,720,1000,1500);//中间
			vTaskDelay(2000);
			Red_Set(360,360,360,360);       
			Red_Set_Mid(360,360,360,360);//中间
			vTaskDelay(500);
			Look();			
		}
		else if((command[0] == 's')&&(command[1] == 'e')&&(command[2] == 't')&&(command[3] == 'R'))
		{
			//放红色到圆环
			command[0] = '0';
			Red_Set_Mid(720,720,1000,1500);//中间
			vTaskDelay(2000);
			Red_Get(360,360,360,360);       
			Red_Set_Mid(360,360,360,360);//中间
			vTaskDelay(500);
			Red_Set_Round(720,720,1000,1500);
			Sum_Control(56.339, 25.589, 169.669, 88.529,360,360,360,360);//中间态
			vTaskDelay(500);
			Look();			
		}
		else if((command[0] == 's')&&(command[1] == 'e')&&(command[2] == 't')&&(command[3] == 'g'))
		{
			//放绿色到圆盘
			command[0] = '0';
			Catch_Disk();
			Green_Set_Mid(720,720,1000,1500);//中间
			vTaskDelay(2000);
			Green_Set(360,360,360,360);       
			Green_Set_Mid(360,360,360,360);//中间
			vTaskDelay(500);
			Look();	
		}
		else if((command[0] == 's')&&(command[1] == 'e')&&(command[2] == 't')&&(command[3] == 'G'))
		{
			//放绿色到圆环
			command[0] = '0';
			Green_Set_Mid(720,720,1000,1500);//中间
			vTaskDelay(2000);
			Green_Get(360,360,360,360);       
			Green_Set_Mid(360,360,360,360);//中间
			vTaskDelay(500);
			Green_Set_Round(720,720,1000,1500);
			Sum_Control(94.339, 65.089, 238.169, 113.529,360,360,360,360);
			vTaskDelay(500);
			Look();	
		}
		else if((command[0] == 's')&&(command[1] == 'e')&&(command[2] == 't')&&(command[3] == 'b'))
		{
			//放蓝色到圆盘
			command[0] = '0';
			Catch_Disk();
			Blue_Set_Mid(720,720,1000,1500);//中间
			vTaskDelay(2000);
			Blue_Set(360,360,360,360);       
			Blue_Set_Mid(360,360,360,360);//中间
			vTaskDelay(500);
			Look();			
		}
		else if((command[0] == 's')&&(command[1] == 'e')&&(command[2] == 't')&&(command[3] == 'B'))
		{
			//放蓝色到圆环
			command[0] = '0';
			Blue_Set_Mid(720,720,1000,1500);//中间
			vTaskDelay(2000);
		    Blue_Get(360,360,360,360);       
			Blue_Set_Mid(360,360,360,360);//中间
			vTaskDelay(500);
			Blue_Set_Round(720,720,1000,1500);
			Sum_Control(137.839, 54.589, 222.169, 109.029,360,360,360,360);
			vTaskDelay(500);
			Look();	
		}		
		else if((command[0] == 'q')&&(command[1] == 'r')&&(command[2] == 's')&&(command[3] == 'h'))
		{ // 红色环取
			command[0] = '0';
			Red_Get_Round(360,360,360,360);
			Sum_Control(56.339, 25.589, 169.669, 88.529,360,360,360,360);//中间
	        vTaskDelay(500);	
			Red_Set_Mid(720,720,1000,1500);//中间
			vTaskDelay(2000);
			Red_Set(360,360,360,360);       
			Red_Set_Mid(360,360,360,360);//中间
			vTaskDelay(500);
			Look();	  			
		}
		else if((command[0] == 'q')&&(command[1] == 'g')&&(command[2] == 's')&&(command[3] == 'h'))
		{ // 绿色环取
			command[0] = '0';
			Green_Get_Round(360,360,360,360);
			Sum_Control(94.339, 65.089, 238.169, 113.529,360,360,360,360);//中间
	        vTaskDelay(500);	
			Green_Set_Mid(720,720,1000,1500);//中间
			vTaskDelay(2000);
			Green_Set(360,360,360,360);       
			Green_Set_Mid(360,360,360,360);//中间
			vTaskDelay(500);
			Look();			
		}
		else if((command[0] == 'q')&&(command[1] == 'b')&&(command[2] == 's')&&(command[3] == 'h'))
		{ // 蓝色环取
			command[0] = '0';
			Blue_Get_Round(360,360,360,360);
			Sum_Control(137.839, 54.589, 222.169, 109.029,360,360,360,360);//中间
	        vTaskDelay(500);	
			Blue_Set_Mid(720,720,1000,1500);//中间
			vTaskDelay(2000);
			Blue_Set(360,360,360,360);       
			Blue_Set_Mid(360,360,360,360);//中间
			vTaskDelay(500);
			Look();			
		}
		else if((command[0] == 'r')&&(command[1] == '1'))
		{ // red取
			command[0] = '0';
            Sum_Control(108.801,77.61,42.02,262.483,360,360,360,360);			
		}
		else if((command[0] == 'r')&&(command[1] == '2'))
		{ // red中
			command[0] = '0';
            Sum_Control(110.656,80.139,59.073,280.041,360,360,360,360);			
		}
		else if((command[0] == 'r')&&(command[1] == '3'))
		{ 	//red放
           command[0] = '0';
            Sum_Control(110.801,77.639,46.573,268.041,360,360,360,360);			
		}
		else if((command[0] == 'g')&&(command[1] == '1'))
		{ // green取
			command[0] = '0';
            Sum_Control(73.389,78.81,42.63,260.0,360,360,360,360);			
		}
		else if((command[0] == 'g')&&(command[1] == '2'))
		{ // green中
			command[0] = '0';
            Sum_Control(72.195,76.632,50.958,270.097,360,360,360,360);			
		}
		else if((command[0] == 'g')&&(command[1] == '3'))
		{ 	//green放
           command[0] = '0';
            Sum_Control(71.909,77.557,46.41,265.436,360,360,360,360);			
		}
		else if((command[0] == 'b')&&(command[1] == '1'))
		{ // blue取
			command[0] = '0';
            Sum_Control(32.203,78.202,42.462,258.756,360,360,360,360);			
		}
		else if((command[0] == 'b')&&(command[1] == '2'))
		{ // blue中
			command[0] = '0';
            Sum_Control(32.203,81.324,57.206,275.349,360,360,360,360);			
		}
		else if((command[0] == 'b')&&(command[1] == '3'))
		{ 	//blue放
           command[0] = '0';
            Sum_Control(32.203,77.50,45.69,265.91,360,360,360,360);			
		}		
		else if((command[0] == 'f')&&(command[1] == 'r'))
		{ 	//红色环放
           command[0] = '0';
            Sum_Control(55.970, 5.417, 167.188, 102.015, 360,360,360,360);			
		}
		else if((command[0] == 'f')&&(command[1] == 'g'))
		{ 	//绿色环放
           command[0] = '0';
            Sum_Control(92.25, 40.843, 241.904, 139.338,360,360,360,360);			
		}
		else if((command[0] == 'f')&&(command[1] == 'b'))
		{ 	//蓝色环放
           command[0] = '0';
            Sum_Control(137.278, 34.316, 227.847, 131.777,360,360,360,360);			
		}
		else if((command[0] == 'm')&&(command[1] == 'd')&&(command[2] == 'r'))
		{ 	//码垛红
           command[0] = '0';
            Sum_Control(56.339, 25.589, 169.669, 88.529,360,360,360,360);			
		}
		else if((command[0] == 'm')&&(command[1] == 'd')&&(command[2] == 'g'))
		{ 	//码垛绿
           command[0] = '0';
            Sum_Control(94.339, 65.089, 238.169, 113.529,360,360,360,360);			
		}
		else if((command[0] == 'm')&&(command[1] == 'd')&&(command[2] == 'b'))
		{ 	//码垛蓝
           command[0] = '0';
            Sum_Control(137.839, 54.589, 222.169, 109.029,360,360,360,360);			
		}
        else if((command[0] == 'z')&&(command[1] == 'a'))
		{ 	//blue放
           command[0] = '0';
           	runActionGroup(2,1);//抓
            vTaskDelay(1000);		
		}
		else if((command[0] == 'f')&&(command[1] == 'a'))
		{ 	//blue放
           command[0] = '0';
           	runActionGroup(1,1);//抓
            vTaskDelay(1000);		
		}
			vTaskDelay(100);
	}
}
