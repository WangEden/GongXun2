#ifndef __MOTOR_H
#define __MOTOR_H

#include "robot.h"
/*
test
*/
#define MOTOR_Period  1999
#define MOTOR_Prescaler  4

#define AIN1 PFout(12)
#define AIN2 PFout(11)
#define BIN1 PFout(14)
#define BIN2 PFout(13)
#define CIN1 PGout(2)
#define CIN2 PFout(15)
#define DIN1 PGout(3)
#define DIN2 PGout(4)


void MOTOR_Init(void);
void MOTORA_SetSpeed(int speed);
void MOTORB_SetSpeed(int speed);
void MOTORC_SetSpeed(int speed);
void MOTORD_SetSpeed(int speed);
void MOTOR_Stop(void);



#endif

