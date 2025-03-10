#ifndef __KINEMATICS_H
#define __KINEMATICS_H

/* Includes ------------------------------------------------------------------*/
#include "robot.h"

typedef enum {
    Front,
    Back,
    Left,
    Right,
  }Direction;
  
void ROBOT_Kinematics(void);

int16_t SPEED_PidCtlA(float spd_target, float spd_current);
int16_t SPEED_PidCtlB(float spd_target, float spd_current);
int16_t SPEED_PidCtlC(float spd_target, float spd_current);
int16_t SPEED_PidCtlD(float spd_target, float spd_current);

void Set_TargetSpeed_A(float Target_speed_A);
void Set_TargetSpeed_B(float Target_speed_B);
void Set_TargetSpeed_C(float Target_speed_C);
void Set_TargetSpeed_D(float Target_speed_D);


void Cal_AngleErr(void);
void Cal_AngleErr2(void);
void Cal_AngleErr3(void);
void Arive_Destination(Direction direction, int adjust_flag ,float Speed , float UP_Time, float Stop_Time , int Down_Distance, float X , float Y);
void Arive_Destination2(Direction direction, int adjust_flag ,float Speed , float UP_Time, float Stop_Time , int Down_Distance, float X , float Y);
void Arive_Destination3(Direction direction, int adjust_flag ,float Speed , float UP_Time, float Stop_Time , int Down_Distance, float X , float Y);
void Anti_Turnto_90(void);
void Anti90_Turnto_180(void);
void Turn180_to_90(void);
void Turn90_to_0(void);
void Init_Parmetre(int A_P,int A_D,int B_P,int B_D,int C_P,int C_D,int D_P,int D_D);
void Clear_Angle(void);



#endif



