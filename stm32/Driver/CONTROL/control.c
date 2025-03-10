#include "control.h"

uint8_t ctlCmd = CMD_SPEED_CONTROL;
uint8_t motorId = 1;
int64_t ctlValue = 30000;
uint32_t commandSendCount = 0;
uint32_t commandUnreceivedCount = 0;
uint8_t spinDirection = 2;//0x00����˳ʱ�룬0x01������ʱ��

int8_t motorTemperature = 0;
int16_t motorPowerOrTorque = 0;
int16_t motorSpeed = 0;
uint16_t motorEncoder = 0;

uint32_t angleControl; 
uint32_t maxSpeed;

uint64_t multi_angleControl; 
uint32_t multi_maxSpeed;

int16_t controlquantity1;
int16_t controlquantity2;
int16_t controlquantity3;
int16_t controlquantity4;

uint8_t data[200];

/*********************************************************************************
* @brief	��������
* @param	None
* @retval	None
*********************************************************************************/
void command_Run(void)
{
	static uint16_t tick = 0;
	uint8_t receivedFlag = 0;	// �յ��ظ���־
	
	if (tick < 50)
	{
		if (tick == 0)
		{
			control_Send(ctlCmd, motorId, ctlValue);
		}
		else if (tick == 20)
		{
			receivedFlag = control_CheckReceivedData();
			
			commandSendCount++;
			if (receivedFlag == 0)
			{
				commandUnreceivedCount++;	// δ�յ��ظ�����
			}		
		}
		tick++;
	}
	else
	{
		tick = 0;
	}
}

/*********************************************************************************
* @brief	���Ϳ�������
* @param	None
* @retval	None
*********************************************************************************/
void control_Send(uint8_t cmd, uint8_t id, int64_t value)
{
	uint8_t dataSize = 0;	// �������ݳ���
	int16_t openCtlData = 0;	// ������������
	int16_t torqueCtlData = 0;	// ���ػ���������
	int32_t speedCtlData = 0;	// �ٶȻ���������
	int64_t angleCtlData = 0;	// λ�û���������
	
	if (cmd == CMD_OPEN_CONTROL)
	{
		openCtlData = value;
		dataSize = 2;
		
		control_PackCmd(uart6TxBuffer, cmd, id, dataSize, (uint8_t *)&openCtlData);
		DMA_USART6_Send(DMA2_Stream6,uart6TxBuffer, uart6TxDataSize);//��������
		uart6RxDataSize = LEAST_FRAME_SIZE + 7 + 1;
	}
	else if (cmd == CMD_TORQUE_CONTROL)
	{
		torqueCtlData = value;
		dataSize = 2;
		
		control_PackCmd(uart6TxBuffer, cmd, id, dataSize, (uint8_t *)&torqueCtlData);
		DMA_USART6_Send(DMA2_Stream6,uart6TxBuffer, uart6TxDataSize);
		uart6RxDataSize = LEAST_FRAME_SIZE + 7 + 1;
	}
	else if (cmd == CMD_SPEED_CONTROL)
	{
		speedCtlData = value;
		dataSize = 0x04;
		
		control_PackCmd(uart6TxBuffer, cmd, id, dataSize, (uint8_t *)&speedCtlData);
		DMA_USART6_Send(DMA2_Stream6,uart6TxBuffer, uart6TxDataSize);
		uart6RxDataSize = LEAST_FRAME_SIZE + 7 + 1;
	}	
	else if (cmd == CMD_MULTI_ANGLE_CONTROL1)
	{
		angleCtlData = value;
		dataSize = 8;
		
		control_PackCmd(uart6TxBuffer, cmd, id, dataSize, (uint8_t *)&angleCtlData);
		DMA_USART6_Send(DMA2_Stream6,uart6TxBuffer, uart6TxDataSize);
		uart6RxDataSize = LEAST_FRAME_SIZE + 7 + 1;
	}
}

/*********************************************************************************
* @brief	������ظ�����
* @param	None
* @retval	None
*********************************************************************************/
uint8_t control_CheckReceivedData(void)
{
	uint8_t receiveSuccess = 0;
	uint8_t temp = 0;
	uint8_t i = 0;

	if (uart6RxBuffer[0] == CMD_HEAD)
	{
		temp = uart6RxBuffer[0] + uart6RxBuffer[1] + uart6RxBuffer[2] + uart6RxBuffer[3];
		if (uart6RxBuffer[4] == temp)
		{
			temp = uart6RxBuffer[5] + uart6RxBuffer[6] + uart6RxBuffer[7] + uart6RxBuffer[8] + uart6RxBuffer[9] + uart6RxBuffer[10] + uart6RxBuffer[11];
			if (uart6RxBuffer[12] == temp)
			{
				motorTemperature = (int8_t)uart6RxBuffer[5];
				motorPowerOrTorque = (int16_t)(uart6RxBuffer[6] + (uart6RxBuffer[7]<<8));
				motorSpeed = (int16_t)(uart6RxBuffer[8] + (uart6RxBuffer[9]<<8));		
				motorEncoder = (int16_t)(uart6RxBuffer[10] + (uart6RxBuffer[11]<<8));			
				receiveSuccess = 1;
			}
		}
	}
	
	for (i=0; i<uart6RxDataSize; i++)
		uart6RxBuffer[i] = 0;
	uart6RxDataSize = 0;
	
	return receiveSuccess;
}

/*********************************************************************************
* @brief	���������
* @param	buffer[i]:���ڴ������Ҫ�������ݵ�����
					cmd:֡ͷʶ���ֽڣ�0x3E
          id:���id��
					size:֡���ݵĸ���������У��λ��
					data[i]:�������ݴ�ŵ�����
* @retval	None
*********************************************************************************/
void control_PackCmd(uint8_t *buffer, uint8_t cmd, uint8_t id, uint8_t size, uint8_t *data)
{
	uint8_t i = 0;
	buffer[0] = CMD_HEAD;
	buffer[1] = cmd;
	buffer[2] = id;
	buffer[3] = size;
	buffer[4] = 0;	// ��Ҫ����0
	for (i=0; i<4; i++)
		buffer[4] += buffer[i];
	
	if (size != 0){
		buffer[LEAST_FRAME_SIZE+size] = 0;	// ��Ҫ����0
		for (i=0; i<size; i++)	// �������ݲ�����У��ֵ
		{
			buffer[LEAST_FRAME_SIZE+i] = data[i];
			buffer[LEAST_FRAME_SIZE+size] += buffer[LEAST_FRAME_SIZE+i];
		}
		uart6TxDataSize = i + LEAST_FRAME_SIZE + 1;	// ��Ҫ���͵������ܳ���
	}
	else
		uart6TxDataSize = LEAST_FRAME_SIZE ;	// ��Ҫ���͵������ܳ���
}
/*********************************************************************************
* @brief	��Ȧλ�ñջ���������2
* @param	id:���id��
          spinDirection:0x00����˳ʱ�룬0x01������ʱ��
* @retval	
*********************************************************************************/
void Control_Single_Angle2(uint8_t id, uint8_t spinDirection,uint16_t angle, uint16_t speed)
{
	angleControl = angle*1000; 
	maxSpeed = speed*100;
	data[0] = spinDirection;
	data[1] = *(uint8_t*)(&angleControl);
	data[2] = *((uint8_t*)(&angleControl) + 1);
	data[3] = *((uint8_t*)(&angleControl) + 2);
	data[4] = *(uint8_t*)(&maxSpeed);
	data[5] = *((uint8_t*)(&maxSpeed) + 1);
	data[6] = *((uint8_t*)(&maxSpeed) + 2);
	data[7] = *((uint8_t*)(&maxSpeed) + 3);
	
	control_PackCmd(uart6TxBuffer,CMD_SINGLE_ANGLE_CONTROL2,id,8,data);
	DMA_USART6_Send(DMA2_Stream6,uart6TxBuffer, uart6TxDataSize);
	uart6RxDataSize = LEAST_FRAME_SIZE + 9;

}
/*********************************************************************************
* @brief	��Ȧλ�ñջ���������2
* @param	id:���id��
* @retval	
*********************************************************************************/
void Control_Multi_Angle2(uint8_t id,float angle, float speed)
{
	multi_angleControl = angle*1000; 
	multi_maxSpeed = speed*100;
	
	data[0] = *(uint8_t*)(&multi_angleControl);
	data[1] = *((uint8_t*)(&multi_angleControl) + 1);
	data[2] = *((uint8_t*)(&multi_angleControl) + 2);
	data[3] = *((uint8_t*)(&multi_angleControl) + 3);
	data[4] = *((uint8_t*)(&multi_angleControl) + 4);
	data[5] = *((uint8_t*)(&multi_angleControl) + 5);
	data[6] = *((uint8_t*)(&multi_angleControl) + 6);
	data[7] = *((uint8_t*)(&multi_angleControl) + 7);
	data[8] = *(uint8_t*)(&multi_maxSpeed);
	data[9] = *((uint8_t*)(&multi_maxSpeed) + 1);
	data[10] = *((uint8_t*)(&multi_maxSpeed) + 2);
	data[11] = *((uint8_t*)(&multi_maxSpeed) + 3);
	
	control_PackCmd(uart6TxBuffer,CMD_MULTI_ANGLE_CONTROL2,id,12,data);
	DMA_USART6_Send(DMA2_Stream6,uart6TxBuffer, uart6TxDataSize);
	uart6RxDataSize = LEAST_FRAME_SIZE + 13;
	Motor_Angle[id-1] = angle;
}
/*********************************************************************************
* @brief	���ŷ�����
* @param	ǰ�ĸ������ֱ���id��Ϊ1/2/3/4��λ�ز������ٶ�Ϊͳһ���ٶ�
* @retval	
*********************************************************************************/
void Sum_Control(float angle1, float angle2, float angle3, float angle4, float speed1,float speed2,float speed3,float speed4)
{
	Control_Multi_Angle2(4,angle4,speed4);
	vTaskDelay(50);
	Control_Multi_Angle2(3,angle3,speed3);
    vTaskDelay(50);
	Control_Multi_Angle2(2,angle2,speed2);
    vTaskDelay(50);
    Control_Multi_Angle2(1,angle1,speed1);
    vTaskDelay(50);
  
	
  

}
