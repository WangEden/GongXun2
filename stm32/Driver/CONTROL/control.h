#ifndef __CONTROL_H__
#define __CONTROL_H__
#include "robot.h"


#ifdef __cplusplus
extern "C" {
#endif


// ��С֡��
#define LEAST_FRAME_SIZE					  5		// ��С֡���� header + cmd + id + dataLength + headCheckSum										
// HEAD                                 	
#define CMD_HEAD							      0x3E	// ͷ


#define CMD_OPEN_CONTROL						0xA0	// ��������MS
#define CMD_TORQUE_CONTROL					0xA1	// ���ؿ���MF MG  -2000-2000
#define CMD_SPEED_CONTROL						0xA2	// �ٶȿ��� -24000-24000
#define CMD_MULTI_ANGLE_CONTROL1					0xA3	// λ�ÿ���
#define CMD_MULTI_ANGLE_CONTROL2                   0xA4
#define CMD_SINGLE_ANGLE_CONTROL2          0xA6  // ��Ȧλ�ñջ���������2
#define CMD_SETTING_ZERO_POSITION          0x19  // д�뵱ǰλ�õ�ROM��Ϊ������
	
extern uint8_t ctlCmd;
extern uint8_t motorId;
extern int64_t ctlValue;
extern uint32_t commandSendCount;
extern uint32_t commandUnreceivedCount;

extern int8_t motorTemperature;
extern int16_t motorPowerOrTorque;
extern int16_t motorSpeed;
extern uint16_t motorEncoder;


void command_Run(void);
void status_Run(void);
void control_Send(uint8_t cmd, uint8_t id, int64_t value);
void control_Receive(void);
uint8_t control_CheckReceivedData(void);
void control_PackCmd(uint8_t *buffer, uint8_t cmd, uint8_t id, uint8_t dataSize, uint8_t *data);
void Control_Single_Angle2(uint8_t id, uint8_t spinDirection,uint16_t angle, uint16_t speed);
void Control_Multi_Angle2(uint8_t id,float angle, float speed);
void Sum_Control(float angle1, float angle2, float angle3, float angle4, float speed1,float speed2,float speed3,float speed4);


#ifdef __cplusplus
}
#endif
#endif /* __CONTROL_H__ */




