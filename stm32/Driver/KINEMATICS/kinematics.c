#include "kinematics.h"

//���PID���Ʋ���
int16_t a_motor_kp;      
int16_t a_motor_kd; 

int16_t b_motor_kp;      
int16_t b_motor_kd; 

int16_t c_motor_kp;      
int16_t c_motor_kd; 

int16_t d_motor_kp;      
int16_t d_motor_kd; 

float angle_kp=0.012;      
float angle_kd=1.9; 
float change_angle_speed;
float angle_bias,angle_bias_last;


int16_t A_motor_pwm_out = 0,B_motor_pwm_out = 0 ,C_motor_pwm_out = 0 ,D_motor_pwm_out = 0 ;
float A_bias = 0.0f,A_bias_last = 0.0f,B_bias = 0.0f,B_bias_last = 0.0f,
	  C_bias = 0.0f,C_bias_last = 0.0f,D_bias = 0.0f,D_bias_last = 0.0f;

void ROBOT_Kinematics(void)
{
	//ͨ����������ȡ����ʵʱת��m/s
	Wheel_A.NS = (float) ((int16_t)ENCODER_A_GetCounter()*WHEEL_SCALE);
	ENCODER_A_SetCounter(0);
	
	Wheel_B.NS = (float) ((int16_t)ENCODER_B_GetCounter()*WHEEL_SCALE);
	ENCODER_B_SetCounter(0);
	
	Wheel_C.NS = (float) ((int16_t)ENCODER_C_GetCounter()*WHEEL_SCALE);
	ENCODER_C_SetCounter(0);
	
	Wheel_D.NS = (float) ((int16_t)ENCODER_D_GetCounter()*WHEEL_SCALE);
	ENCODER_D_SetCounter(0);
}


/**
  * @��  ��  ����ABCD���ӷֱ��������˶��ܹ����д
  * @��  ��  spd_target:�������ٶ�Ŀ��ֵ ,��Χ����250��
  *          spd_current: �������ٶȵ�ǰֵ
  * @����ֵ  ���PWM�ٶ�
  */
int16_t SPEED_PidCtlA(float spd_target, float spd_current)
{


	//���ƫ��ֵ
	A_bias = spd_target - spd_current;
	
	//PID���������PWMֵ
	A_motor_pwm_out += a_motor_kp*A_bias + a_motor_kd*(A_bias-A_bias_last);
	
	//��¼�ϴ�ƫ��
	A_bias_last = A_bias;
	
	//����������
	if(A_motor_pwm_out > 1999)                       
		A_motor_pwm_out = 1999;
	if(A_motor_pwm_out < -1999)
		A_motor_pwm_out = -1999;
	
	//����PWM����ֵ
	return A_motor_pwm_out;
}	

int16_t SPEED_PidCtlB(float spd_target, float spd_current)
{
	//���ƫ��ֵ
	B_bias = spd_target - spd_current;
	
	//PID���������PWMֵ
	B_motor_pwm_out += b_motor_kp*B_bias + b_motor_kd*(B_bias-B_bias_last);
	
	//��¼�ϴ�ƫ��
	B_bias_last = B_bias;
	
	//����������
	if(B_motor_pwm_out > 1999)
		B_motor_pwm_out = 1999;
	if(B_motor_pwm_out < -1999)
		B_motor_pwm_out = -1999;
	
	//����PWM����ֵ
	return B_motor_pwm_out;
}	
int16_t SPEED_PidCtlC(float spd_target, float spd_current)
{
	//���ƫ��ֵ
	C_bias = spd_target - spd_current;
	
	//PID���������PWMֵ
	C_motor_pwm_out += c_motor_kp*C_bias + c_motor_kd*(C_bias-C_bias_last);
	
	//��¼�ϴ�ƫ��
	C_bias_last = C_bias;
	
	//����������
	if(C_motor_pwm_out > 1999)
		C_motor_pwm_out = 1999;
	if(C_motor_pwm_out < -1999)
		C_motor_pwm_out = -1999;
	
	//����PWM����ֵ
	return C_motor_pwm_out;
}	

int16_t SPEED_PidCtlD(float spd_target, float spd_current)
{

	//���ƫ��ֵ
	D_bias = spd_target - spd_current;
	
	//PID���������PWMֵ
	D_motor_pwm_out += d_motor_kp*D_bias + d_motor_kd*(D_bias-D_bias_last);
	
	//��¼�ϴ�ƫ��
	D_bias_last = D_bias;
	
	//����������
	if(D_motor_pwm_out > 1999)
		D_motor_pwm_out = 1999;
	if(D_motor_pwm_out < -1999)
		D_motor_pwm_out = -1999;
	
	//����PWM����ֵ
	return D_motor_pwm_out;
}	

void Set_TargetSpeed_A(float Target_speed_A)
{
	//���õ��PWMֵ
	Wheel_A.PWM = SPEED_PidCtlA(Target_speed_A, Wheel_A.NS);   
	MOTORA_SetSpeed( Wheel_A.PWM);
}

void Set_TargetSpeed_B(float Target_speed_B)
{
	//���õ��PWMֵ
	Wheel_B.PWM = SPEED_PidCtlB(Target_speed_B, Wheel_B.NS);   
	MOTORB_SetSpeed( Wheel_B.PWM);
}

void Set_TargetSpeed_C(float Target_speed_C)
{
	//���õ��PWMֵ
	Wheel_C.PWM = SPEED_PidCtlC(Target_speed_C, Wheel_C.NS);   
	MOTORC_SetSpeed( Wheel_C.PWM);
}

void Set_TargetSpeed_D(float Target_speed_D)
{	
	//���õ��PWMֵ
	Wheel_D.PWM = SPEED_PidCtlD(Target_speed_D, Wheel_D.NS);   
	MOTORD_SetSpeed( Wheel_D.PWM);
}

void Cal_AngleErr(void)
{
		angle_bias = zangle;
		//PID���������PWMֵ
		change_angle_speed += angle_kp*angle_bias + angle_kd*(angle_bias-angle_bias_last);
		//��¼�ϴ�ƫ��
		angle_bias_last = angle_bias;
		//����������
		if(change_angle_speed > 18.0f)                       
			change_angle_speed = 18.0f;
		else if(change_angle_speed < -18.0f)
			change_angle_speed = -18.0f;		
}
void Cal_AngleErr2(void)
{
		angle_bias = zangle - 90.0f;
		//PID���������PWMֵ
		change_angle_speed += angle_kp*angle_bias + angle_kd*(angle_bias-angle_bias_last);
		//��¼�ϴ�ƫ��
		angle_bias_last = angle_bias;
		//����������
		if(change_angle_speed > 18.0f)                       
			change_angle_speed = 18.0f;
		else if(change_angle_speed < -18.0f)
			change_angle_speed = -18.0f;		
}
void Cal_AngleErr3(void)
{
	if(zangle >= 0)
		angle_bias = zangle - 180.0f;
	else
		angle_bias = zangle + 180.0f;
		//PID���������PWMֵ
		change_angle_speed += angle_kp*angle_bias + angle_kd*(angle_bias-angle_bias_last);
		//��¼�ϴ�ƫ��
		angle_bias_last = angle_bias;
		//����������
		if(change_angle_speed > 18.0f)                       
			change_angle_speed = 18.0f;
		else if(change_angle_speed < -18.0f)
			change_angle_speed = -18.0f;		
}
// mm
void Arive_Destination(Direction direction, int adjust_flag ,float Speed , float UP_Time, float Stop_Time , int Down_Distance, float X , float Y)
 {
	static portTickType PreTime;
	const portTickType Timet = pdMS_TO_TICKS(10);
	PreTime = xTaskGetTickCount();
	//1.5s to speeed up 1500/10 = 150;
	//1.0s to speeed down 1000/10 = 100;
	float psc;
	float True_Speed,Interval_Speed;
	float Stop_Speed,Stop_Interval_Speed;
	int Stop_Flag = 0;
	    Interval_Speed = Speed / UP_Time;
	Stop_Speed = Speed;
	Stop_Interval_Speed = (Speed - 4.5f)/Stop_Time;
	Init_Parmetre(22,120,22,120,22,120,22,120);
	Clear_Angle();
	if(Speed <= 10.0f)
		psc = 0.444f;
	else if((Speed >10.0f)&&(Speed <= 15.0f))
		psc = 0.722f;
	else
		psc = 1.0f;
	while(1)
	{
		if(True_Speed < Speed)
			True_Speed+=Interval_Speed;
		if((Stop_Flag ==1) && (Stop_Speed > 4.5f))
			Stop_Speed -= Stop_Interval_Speed;
		ROBOT_Kinematics();
		Cal_AngleErr();
        vTaskDelayUntil(&PreTime,Timet); 	//10ms running time	
		
		if(direction == Front)
		{
            if(pos_y < (Y - Down_Distance))
			{
		         Set_TargetSpeed_A(True_Speed - psc*change_angle_speed);	
			     Set_TargetSpeed_C(True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_B(-True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_D(-True_Speed - psc*change_angle_speed);
			}
			else 
			{
				Stop_Flag = 1;
				 if((pos_y > (Y - 5)) && (pos_y < (Y + 5)))
					{MOTOR_Stop(); Clear_Angle(); break;}
					
				 Set_TargetSpeed_A(Stop_Speed - 0.1f*change_angle_speed);	
			     Set_TargetSpeed_C(Stop_Speed + 0.1f*change_angle_speed);	
			     Set_TargetSpeed_B(-Stop_Speed + 0.1f*change_angle_speed);	
			     Set_TargetSpeed_D(-Stop_Speed - 0.1f*change_angle_speed);						
			}			
		}
		else if(direction == Back)
		{
			if(pos_y > (Y + Down_Distance))
			{

                 Set_TargetSpeed_A(-True_Speed - psc*change_angle_speed);	
			     Set_TargetSpeed_C(-True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_B(True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_D(True_Speed - psc*change_angle_speed);				
			}
			else 
			{
				Stop_Flag = 1;
				if((pos_y < (Y + 5))&&(pos_y > (Y - 5)))
					{MOTOR_Stop(); Clear_Angle(); break;}
					
				 Set_TargetSpeed_A(-Stop_Speed -  0.1f*change_angle_speed);	
			     Set_TargetSpeed_C(-Stop_Speed +  0.1f*change_angle_speed);	
			     Set_TargetSpeed_B(Stop_Speed +  0.1f*change_angle_speed);	
			     Set_TargetSpeed_D(Stop_Speed -  0.1f*change_angle_speed);			
			}
		}
		 else if(direction == Right)
		{
			if(pos_x < (X - Down_Distance))
			{
				 Set_TargetSpeed_A(True_Speed - psc*change_angle_speed);	
			     Set_TargetSpeed_C(True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_B(True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_D(True_Speed - psc*change_angle_speed);	
			}
			else 
			{
				 Stop_Flag = 1;
				 if((pos_x > (X - 5)) && (pos_x < (X + 5)))
					 {MOTOR_Stop(); Clear_Angle(); break;}
				       					 
				 Set_TargetSpeed_A(Stop_Speed - 0.1f*change_angle_speed);	
			     Set_TargetSpeed_C(Stop_Speed + 0.1f*change_angle_speed);	
			     Set_TargetSpeed_B(Stop_Speed + 0.1f*change_angle_speed);	
			     Set_TargetSpeed_D(Stop_Speed - 0.1f*change_angle_speed);				
			}	
		}
		else if(direction == Left)
		{
			if(pos_x > (X + Down_Distance))
			{
		         Set_TargetSpeed_A(-True_Speed - psc*change_angle_speed);	
			     Set_TargetSpeed_C(-True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_B(-True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_D(-True_Speed - psc*change_angle_speed);	
			}
			else 
			{
				 Stop_Flag = 1;
				if((pos_x < (X + 5))&&(pos_x > (X - 5)))
					{MOTOR_Stop(); Clear_Angle(); break;}
				 Set_TargetSpeed_A(-Stop_Speed - 0.1f*change_angle_speed);	
			     Set_TargetSpeed_C(-Stop_Speed + 0.1f*change_angle_speed);	
			     Set_TargetSpeed_B(-Stop_Speed + 0.1f*change_angle_speed);	
			     Set_TargetSpeed_D(-Stop_Speed - 0.1f*change_angle_speed);
			}
		}
	}
	//Adjust The Other Error
	while(1)
	{
		if (adjust_flag == 0)
			{MOTOR_Stop(); Clear_Angle(); break;}
		ROBOT_Kinematics();
		Cal_AngleErr();
		vTaskDelayUntil(&PreTime,Timet);
		if((direction == Front) || (direction == Back))
		{
			if(pos_x > X)
			{
				if(pos_x < (X+5)) 
				{MOTOR_Stop(); Clear_Angle(); break;}
				
				 Set_TargetSpeed_A(-4.5f - 0.20f*change_angle_speed);	
			     Set_TargetSpeed_C(-4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_B(-4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_D(-4.5f - 0.20f*change_angle_speed);
			}
			else
			{
				if(pos_x > (X-5))
				{MOTOR_Stop(); Clear_Angle(); break;}
				
				 Set_TargetSpeed_A(4.5f - 0.20f*change_angle_speed);	
			     Set_TargetSpeed_C(4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_B(4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_D(4.5f - 0.20f*change_angle_speed);
			}
		}
		else if((direction == Right) || (direction == Left))
		{
			if(pos_y > Y)
			{
				if( pos_y < (Y+5))
				{MOTOR_Stop(); Clear_Angle(); break;}
				
				 Set_TargetSpeed_A(-4.5f - 0.20f*change_angle_speed);	
			     Set_TargetSpeed_C(-4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_B(4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_D(4.5f - 0.20f*change_angle_speed);
			}
			else
			{
				if(pos_y > (Y-5))
				{MOTOR_Stop(); Clear_Angle();  break;}
				
				 Set_TargetSpeed_A(4.5f - 0.20f*change_angle_speed);	
			     Set_TargetSpeed_C(4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_B(-4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_D(-4.5f - 0.20f*change_angle_speed);
			}
		}
	}
}

void Arive_Destination2(Direction direction, int adjust_flag ,float Speed , float UP_Time, float Stop_Time , int Down_Distance, float X , float Y)
{
	static portTickType PreTime2;
	const portTickType Timet2 = pdMS_TO_TICKS(10);
	PreTime2 = xTaskGetTickCount();
	//1.5s to speeed up 1500/10 = 150;
	//1.0s to speeed down 1000/10 = 100;
	float psc;
	float True_Speed,Interval_Speed;
	float Stop_Speed,Stop_Interval_Speed;
	int Stop_Flag = 0;
	    Interval_Speed = Speed / UP_Time;
	Stop_Speed = Speed;
	Stop_Interval_Speed = (Speed - 4.5f)/Stop_Time;
	Init_Parmetre(22,120,22,120,22,120,22,120);
	Clear_Angle();
	if(Speed <= 10.0f)
		psc = 0.444f;
	else if((Speed >10.0f)&&(Speed <= 15.0f))
		psc = 0.722f;
	else
		psc = 1.0f;
	while(1)
	{
		if(True_Speed < Speed)
			True_Speed+=Interval_Speed;
		if((Stop_Flag ==1) && (Stop_Speed > 4.5f))
			Stop_Speed -= Stop_Interval_Speed;
		ROBOT_Kinematics();
		Cal_AngleErr2();
        vTaskDelayUntil(&PreTime2,Timet2); 	//10ms running time	
		
		if(direction == Front)
		{
            if(pos_y < (Y - Down_Distance))
			{
		         Set_TargetSpeed_A(True_Speed - psc*change_angle_speed);	
			     Set_TargetSpeed_C(True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_B(True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_D(True_Speed - psc*change_angle_speed);	
			}
			else 
			{
				Stop_Flag = 1;
				 if((pos_y > (Y - 5)) && (pos_y < (Y + 5)))
					{MOTOR_Stop(); Clear_Angle(); break;}
					
				 Set_TargetSpeed_A(Stop_Speed - 0.1f*change_angle_speed);	
			     Set_TargetSpeed_C(Stop_Speed + 0.1f*change_angle_speed);	
			     Set_TargetSpeed_B(Stop_Speed + 0.1f*change_angle_speed);	
			     Set_TargetSpeed_D(Stop_Speed - 0.1f*change_angle_speed);						
			}			
		}
		else if(direction == Back)
		{
			if(pos_y > (Y + Down_Distance))
			{
		         Set_TargetSpeed_A(-True_Speed - psc*change_angle_speed);	
			     Set_TargetSpeed_C(-True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_B(-True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_D(-True_Speed - psc*change_angle_speed);	
			}
			else 
			{
				Stop_Flag = 1;
				if((pos_y < (Y + 5))&&(pos_y > (Y - 5)))
					{MOTOR_Stop(); Clear_Angle(); break;}
					
				 Set_TargetSpeed_A(-Stop_Speed - 0.1f*change_angle_speed);	
			     Set_TargetSpeed_C(-Stop_Speed + 0.1f*change_angle_speed);	
			     Set_TargetSpeed_B(-Stop_Speed + 0.1f*change_angle_speed);	
			     Set_TargetSpeed_D(-Stop_Speed - 0.1f*change_angle_speed);			
			}
		}
		 else if(direction == Right)
		{
			if(pos_x < (X - Down_Distance))
			{
				 Set_TargetSpeed_A(-True_Speed - psc*change_angle_speed);	
			     Set_TargetSpeed_C(-True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_B(True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_D(True_Speed - psc*change_angle_speed);	
			}
			else 
			{
				 Stop_Flag = 1;
				 if((pos_x > (X - 5)) && (pos_x < (X + 5)))
					 {MOTOR_Stop(); Clear_Angle(); break;}
				       					 
				 Set_TargetSpeed_A(-Stop_Speed - 0.1f*change_angle_speed);	
			     Set_TargetSpeed_C(-Stop_Speed + 0.1f*change_angle_speed);	
			     Set_TargetSpeed_B(Stop_Speed + 0.1f*change_angle_speed);	
			     Set_TargetSpeed_D(Stop_Speed - 0.1f*change_angle_speed);			
			}	
		}
		else if(direction == Left)
		{
			if(pos_x > (X + Down_Distance))
			{
		         Set_TargetSpeed_A(True_Speed - psc*change_angle_speed);	
			     Set_TargetSpeed_C(True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_B(-True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_D(-True_Speed - psc*change_angle_speed);	
			}
			else 
			{
				 Stop_Flag = 1;
				if((pos_x < (X + 5))&&(pos_x > (X - 5)))
					{MOTOR_Stop(); Clear_Angle(); break;}
					
				 Set_TargetSpeed_A(Stop_Speed - 0.1f*change_angle_speed);	
			     Set_TargetSpeed_C(Stop_Speed + 0.1f*change_angle_speed);	
			     Set_TargetSpeed_B(-Stop_Speed + 0.1f*change_angle_speed);	
			     Set_TargetSpeed_D(-Stop_Speed - 0.1f*change_angle_speed);
			}
		}
	}
	//Adjust The Other Error
	while(1)
	{
		if (adjust_flag == 0)
			{MOTOR_Stop(); Clear_Angle(); break;}
		ROBOT_Kinematics();
		Cal_AngleErr2();
		vTaskDelayUntil(&PreTime2,Timet2);
		if((direction == Front) || (direction == Back))
		{
			if(pos_x > X)
			{
				if(pos_x < (X+5)) 
				{MOTOR_Stop(); Clear_Angle(); break;}
				
				 Set_TargetSpeed_A(4.5f - 0.20f*change_angle_speed);	
			     Set_TargetSpeed_C(4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_B(-4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_D(-4.5f - 0.20f*change_angle_speed);
			}
			else
			{
				if(pos_x > (X-5))
				{MOTOR_Stop(); Clear_Angle(); break;}
				
				 Set_TargetSpeed_A(-4.5f - 0.20f*change_angle_speed);	
			     Set_TargetSpeed_C(-4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_B(4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_D(4.5f - 0.20f*change_angle_speed);
				
			}
		}
		else if((direction == Right) || (direction == Left))
		{
			if(pos_y > Y)
			{
				if( pos_y < (Y+5))
				{MOTOR_Stop(); Clear_Angle(); break;}
					
                 Set_TargetSpeed_A(-4.5f - 0.20f*change_angle_speed);	
			     Set_TargetSpeed_C(-4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_B(-4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_D(-4.5f - 0.20f*change_angle_speed);				
			}
			else
			{
				if(pos_y > (Y-5))
				{MOTOR_Stop(); Clear_Angle();  break;}
				
				 Set_TargetSpeed_A(4.5f - 0.20f*change_angle_speed);	
			     Set_TargetSpeed_C(4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_B(4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_D(4.5f - 0.20f*change_angle_speed);
				
			}
		}
	}
}

void Arive_Destination3(Direction direction, int adjust_flag ,float Speed , float UP_Time, float Stop_Time , int Down_Distance, float X , float Y)
{
	static portTickType PreTime3;
	const portTickType Timet3 = pdMS_TO_TICKS(10);
	PreTime3 = xTaskGetTickCount();
	//1.5s to speeed up 1500/10 = 150;
	//1.0s to speeed down 1000/10 = 100;
	float psc;
	float True_Speed,Interval_Speed;
	float Stop_Speed,Stop_Interval_Speed;
	int Stop_Flag = 0;
	    Interval_Speed = Speed / UP_Time;
	Stop_Speed = Speed;
	Stop_Interval_Speed = (Speed - 4.5f)/Stop_Time;
	Init_Parmetre(22,120,22,120,22,120,22,120);
	Clear_Angle();
	if(Speed <= 10.0f)
		psc = 0.444f;
	else if((Speed >10.0f)&&(Speed <= 15.0f))
		psc = 0.722f;
	else
		psc = 1.0f;
	while(1)
	{
		if(True_Speed < Speed)
			True_Speed+=Interval_Speed;
		if((Stop_Flag ==1) && (Stop_Speed > 4.5f))
			Stop_Speed -= Stop_Interval_Speed;
		ROBOT_Kinematics();
		Cal_AngleErr3();
        vTaskDelayUntil(&PreTime3,Timet3); 	//10ms running time	
		
		if(direction == Front)
		{
            if(pos_y < (Y - Down_Distance))
			{
		         Set_TargetSpeed_A(-True_Speed - psc*change_angle_speed);	
			     Set_TargetSpeed_C(-True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_B(True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_D(True_Speed - psc*change_angle_speed);	
			}
			else 
			{
				Stop_Flag = 1;
				 if((pos_y > (Y - 5)) && (pos_y < (Y + 5)))
					{MOTOR_Stop(); Clear_Angle(); break;}
					
				 Set_TargetSpeed_A(-Stop_Speed - 0.1f*change_angle_speed);	
			     Set_TargetSpeed_C(-Stop_Speed + 0.1f*change_angle_speed);	
			     Set_TargetSpeed_B(Stop_Speed + 0.1f*change_angle_speed);	
			     Set_TargetSpeed_D(Stop_Speed - 0.1f*change_angle_speed);						
			}			
		}
		else if(direction == Back)
		{
			if(pos_y > (Y + Down_Distance))
			{
		         Set_TargetSpeed_A(True_Speed - psc*change_angle_speed);	
			     Set_TargetSpeed_C(True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_B(-True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_D(-True_Speed - psc*change_angle_speed);	
			}
			else 
			{
				Stop_Flag = 1;
				if((pos_y < (Y + 5))&&(pos_y > (Y - 5)))
					{MOTOR_Stop(); Clear_Angle(); break;}
					
				 Set_TargetSpeed_A(Stop_Speed - 0.1f*change_angle_speed);	
			     Set_TargetSpeed_C(Stop_Speed + 0.1f*change_angle_speed);	
			     Set_TargetSpeed_B(-Stop_Speed + 0.1f*change_angle_speed);	
			     Set_TargetSpeed_D(-Stop_Speed - 0.1f*change_angle_speed);			
			}
		}
		 else if(direction == Right)
		{
			if(pos_x < (X - Down_Distance))
			{
				 Set_TargetSpeed_A(-True_Speed - psc*change_angle_speed);	
			     Set_TargetSpeed_C(-True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_B(-True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_D(-True_Speed - psc*change_angle_speed);	
			}
			else 
			{
				 Stop_Flag = 1;
				 if((pos_x > (X - 5)) && (pos_x < (X + 5)))
					 {MOTOR_Stop(); Clear_Angle(); break;}
				       					 
				 Set_TargetSpeed_A(-Stop_Speed - 0.1f*change_angle_speed);	
			     Set_TargetSpeed_C(-Stop_Speed + 0.1f*change_angle_speed);	
			     Set_TargetSpeed_B(-Stop_Speed + 0.1f*change_angle_speed);	
			     Set_TargetSpeed_D(-Stop_Speed - 0.1f*change_angle_speed);				
			}	
		}
		else if(direction == Left)
		{
			if(pos_x > (X + Down_Distance))
			{
		         Set_TargetSpeed_A(True_Speed - psc*change_angle_speed);	
			     Set_TargetSpeed_C(True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_B(True_Speed + psc*change_angle_speed);	
			     Set_TargetSpeed_D(True_Speed - psc*change_angle_speed);	
			}
			else 
			{
				 Stop_Flag = 1;
				if((pos_x < (X + 5))&&(pos_x > (X - 5)))
					{MOTOR_Stop(); Clear_Angle(); break;}
					
				 Set_TargetSpeed_A(Stop_Speed - 0.1f*change_angle_speed);	
			     Set_TargetSpeed_C(Stop_Speed + 0.1f*change_angle_speed);	
			     Set_TargetSpeed_B(Stop_Speed + 0.1f*change_angle_speed);	
			     Set_TargetSpeed_D(Stop_Speed - 0.1f*change_angle_speed);
			}
		}
	}
	//Adjust The Other Error
	while(1)
	{
		if (adjust_flag == 0)
			{MOTOR_Stop(); Clear_Angle(); break;}
		ROBOT_Kinematics();
		Cal_AngleErr3();
		vTaskDelayUntil(&PreTime3,Timet3);
	    if((direction == Front) || (direction == Back))
		{
			if(pos_x > X)
			{
				if(pos_x < (X+5)) 
				{MOTOR_Stop(); Clear_Angle(); break;}
					
				 Set_TargetSpeed_A(4.5f - 0.20f*change_angle_speed);	
			     Set_TargetSpeed_C(4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_B(4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_D(4.5f - 0.20f*change_angle_speed);
			}
			else
			{
				if(pos_x > (X-5))
				{MOTOR_Stop(); Clear_Angle(); break;}
				
				 Set_TargetSpeed_A(-4.5f - 0.20f*change_angle_speed);	
			     Set_TargetSpeed_C(-4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_B(-4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_D(-4.5f - 0.20f*change_angle_speed);
			}
		}
		else if((direction == Right) || (direction == Left))
		{
			if(pos_y > Y)
			{
				if( pos_y < (Y+5))
				{MOTOR_Stop(); Clear_Angle(); break;}
				
				 Set_TargetSpeed_A(4.5f - 0.20f*change_angle_speed);	
			     Set_TargetSpeed_C(4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_B(-4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_D(-4.5f - 0.20f*change_angle_speed);
			}
			else
			{
				if(pos_y > (Y-5))
				{MOTOR_Stop(); Clear_Angle();  break;}
				
				 Set_TargetSpeed_A(-4.5f - 0.20f*change_angle_speed);	
			     Set_TargetSpeed_C(-4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_B(4.5f + 0.20f*change_angle_speed);	
			     Set_TargetSpeed_D(4.5f - 0.20f*change_angle_speed);
			}
		}
	}
}
void Anti_Turnto_90(void)
{
	while(1)
	 {
		ROBOT_Kinematics();
		vTaskDelay(10);
	    Set_TargetSpeed_A(15.0f );
		Set_TargetSpeed_C(-15.0f );
		Set_TargetSpeed_B(-15.0f );
		Set_TargetSpeed_D(15.0f );
		 if(zangle >= 85)
			 break;
	 }
}
void Anti90_Turnto_180(void)
{
	while(1)
	 {
		ROBOT_Kinematics();
		vTaskDelay(10);
	    Set_TargetSpeed_A(15.0f );
		Set_TargetSpeed_C(-15.0f );
		Set_TargetSpeed_B(-15.0f );
		Set_TargetSpeed_D(15.0f );
		 if(zangle >= 175)
			 break;
	 }
}
void Turn180_to_90(void)
{
	 while(1)
	 {
		ROBOT_Kinematics();
		vTaskDelay(10);
	    Set_TargetSpeed_A(-15.0f );
		Set_TargetSpeed_C(15.0f );
		Set_TargetSpeed_B(15.0f );
		Set_TargetSpeed_D(-15.0f );
		 if(fabs(zangle) <= 95)
			 break;
	 }
}
void Turn90_to_0(void)
{
	while(1)
	 {
		ROBOT_Kinematics();
		vTaskDelay(10);
	    Set_TargetSpeed_A(-15.0f );
		Set_TargetSpeed_C(15.0f );
		Set_TargetSpeed_B(15.0f );
		Set_TargetSpeed_D(-15.0f );
		 if(zangle <= 5)
			 break;
	 }
}
void Init_Parmetre(int A_P,int A_D,int B_P,int B_D,int C_P,int C_D,int D_P,int D_D)
{
	 
     A_bias = 0.0;A_bias_last= 0.0;B_bias= 0.0;B_bias_last= 0.0;
	 C_bias= 0.0;C_bias_last= 0.0;D_bias= 0.0;D_bias_last= 0.0;
	 a_motor_kp=A_P;      
     a_motor_kd=A_D; 

	 b_motor_kp=B_P;      
	 b_motor_kd=B_D; 

	 c_motor_kp=C_P;      
	 c_motor_kd=C_D; 

	 d_motor_kp=D_P;      
	 d_motor_kd=D_D; 
}

void Clear_Angle(void)
{
	change_angle_speed = 0;
     angle_bias = 0;
	 angle_bias_last =0;
}




