#include "key.h"
float angle_psc = 0.5f;
float Motor_Angle[4] = {30.0f,30.0f,30.0f,30.0f};
int Motor_Id = 1;

static void key_0_Init(void);  // ����key_1��ʼ������; static���޸ĺ�������Ч��Χ���Ӷ�������λ����Դ�ļ��ڿɷ���;
static void key_1_Init(void);  // ����key_2��ʼ������; static���޸ĺ�������Ч��Χ���Ӷ�������λ����Դ�ļ��ڿɷ���;
static void key_2_Init(void);  // ����key_3��ʼ������; static���޸ĺ�������Ч��Χ���Ӷ�������λ����Դ�ļ��ڿɷ���;


void Key_Init(void)
{
    key_0_Init();
    key_1_Init();
    key_2_Init();
}

// ����_1�ĳ�ʼ������
static void key_0_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;    // ����GPIO�ṹ�壬�����������Ź���ģʽ
    EXTI_InitTypeDef EXTI_InitStructure;    // ����EXTI�ṹ�壬���������ⲿ�ж��ߣ������ŵ��жϷ�ʽ
    NVIC_InitTypeDef NVIC_InitStructure;    // ����NVIC�ṹ�壬���������ж����ȼ�

    // ʱ��ʹ��
    RCC_AHB1PeriphClockCmd(KEY_0_CLK, ENABLE);                    // ʹ��KEY_1�������Ŷ˿�ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);        // ʹ��ϵͳ������SYSCFG��ʱ�ӣ�ʹ��GPIO�ⲿ�жϱ���ʹ��SYSCFGʱ��

    
    GPIO_InitStructure.GPIO_Pin   = KEY_0_PIN;                    // ѡ��Ҫ���Ƶ����ű��; �˴�ʹ���˺궨�壬�Է�����ֲ�޸�
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;                 // ����ģʽ������ģʽ
    GPIO_InitStructure.GPIO_PuPd  = KEY_0_PUPD;                   // ������״̬
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;              // �������ʣ�2MHz
    GPIO_Init(KEY_0_GPIO, &GPIO_InitStructure);                   // ��ʼ��, ��������Ĳ���д��Ĵ���

    // �������������ж���
    SYSCFG_EXTILineConfig(KEY_0_EXTI_PORT, KEY_0_EXTI_PIN);       // �������������ж���

    // ����EXTI�ж���
    EXTI_InitStructure.EXTI_Line = KEY_0_EXTI_LINE;               // �ж���
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           // �ж�ģʽ
    EXTI_InitStructure.EXTI_Trigger = KEY_0_EXTI_TRIGGER;         // ������ʽ
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     // ʹ��
    EXTI_Init(&EXTI_InitStructure);                               // ��ʼ��, ��������Ĳ���д��Ĵ���

    // ���� NVIC, ���ж����ȼ�
    
    NVIC_InitStructure.NVIC_IRQChannel = KEY_0_INTERRUPT_NUMBER;  // �жϱ�ţ���ʾ�������жϵı�ţ����������ļ��У���оƬ�ڲ���ƶ�Ԥ���趨�õı�ŵġ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 11;     // ��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               // ʹ��
    NVIC_Init(&NVIC_InitStructure);                               // ��ʼ��, ������Ĳ���д��Ĵ���
}



// ����_2�ĳ�ʼ������
static void key_1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;    // ����GPIO�ṹ�壬�����������Ź���ģʽ
    EXTI_InitTypeDef EXTI_InitStructure;    // ����EXTI�ṹ�壬���������ⲿ�ж��ߣ������ŵ��жϷ�ʽ
    NVIC_InitTypeDef NVIC_InitStructure;    // ����NVIC�ṹ�壬���������ж����ȼ�

    // ʱ��ʹ��
    RCC_AHB1PeriphClockCmd(KEY_1_CLK, ENABLE);                    // ʹ��KEY_1�������Ŷ˿�ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);        // ʹ��ϵͳ������SYSCFG��ʱ�ӣ�ʹ��GPIO�ⲿ�жϱ���ʹ��SYSCFGʱ��

    // �������Ź���ģʽ: PA0�� ���롢��ʱ�����������øߵ�ƽ
    GPIO_InitStructure.GPIO_Pin   = KEY_1_PIN;                    // ѡ��Ҫ���Ƶ����ű��; �˴�ʹ���˺궨�壬�Է�����ֲ�޸�
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;                 // ����ģʽ������ģʽ
    GPIO_InitStructure.GPIO_PuPd  = KEY_1_PUPD;                   // ������״̬
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;              // �������ʣ�2MHz
    GPIO_Init(KEY_1_GPIO, &GPIO_InitStructure);                   // ��ʼ��, ��������Ĳ���д��Ĵ���

    // �������������ж���
    SYSCFG_EXTILineConfig(KEY_1_EXTI_PORT, KEY_1_EXTI_PIN);       // �������������ж���

    // ����EXTI�ж���
    EXTI_InitStructure.EXTI_Line = KEY_1_EXTI_LINE;               // �ж���
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           // �ж�ģʽ
    EXTI_InitStructure.EXTI_Trigger = KEY_1_EXTI_TRIGGER;         // ������ʽ
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     // ʹ��
    EXTI_Init(&EXTI_InitStructure);                               // ��ʼ��, ��������Ĳ���д��Ĵ���

    // ���� NVIC, ���ж����ȼ�
   
    NVIC_InitStructure.NVIC_IRQChannel = KEY_1_INTERRUPT_NUMBER;  // �жϱ�ţ���ʾ�������жϵı�ţ����������ļ��У���оƬ�ڲ���ƶ�Ԥ���趨�õı�ŵġ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 12;     // ��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               // ʹ��
    NVIC_Init(&NVIC_InitStructure);                               // ��ʼ��, ������Ĳ���д��Ĵ���
}






static void key_2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;    // ����GPIO�ṹ�壬�����������Ź���ģʽ
    EXTI_InitTypeDef EXTI_InitStructure;    // ����EXTI�ṹ�壬���������ⲿ�ж��ߣ������ŵ��жϷ�ʽ
    NVIC_InitTypeDef NVIC_InitStructure;    // ����NVIC�ṹ�壬���������ж����ȼ�

    // ʱ��ʹ��
    RCC_AHB1PeriphClockCmd(KEY_2_CLK, ENABLE);                    // ʹ��KEY_1�������Ŷ˿�ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);        // ʹ��ϵͳ������SYSCFG��ʱ�ӣ�ʹ��GPIO�ⲿ�жϱ���ʹ��SYSCFGʱ��

    // �������Ź���ģʽ: PA0�� ���롢��ʱ�����������øߵ�ƽ
    GPIO_InitStructure.GPIO_Pin   = KEY_2_PIN;                    // ѡ��Ҫ���Ƶ����ű��; �˴�ʹ���˺궨�壬�Է�����ֲ�޸�
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;                 // ����ģʽ������ģʽ
    GPIO_InitStructure.GPIO_PuPd  = KEY_2_PUPD;                   // ������״̬
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;              // �������ʣ�2MHz
    GPIO_Init(KEY_2_GPIO, &GPIO_InitStructure);                   // ��ʼ��, ��������Ĳ���д��Ĵ���

    // �������������ж���
    SYSCFG_EXTILineConfig(KEY_2_EXTI_PORT, KEY_2_EXTI_PIN);       // �������������ж���

    // ����EXTI�ж���
    EXTI_InitStructure.EXTI_Line = KEY_2_EXTI_LINE;               // �ж���
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           // �ж�ģʽ
    EXTI_InitStructure.EXTI_Trigger = KEY_2_EXTI_TRIGGER;         // ������ʽ
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     // ʹ��
    EXTI_Init(&EXTI_InitStructure);                               // ��ʼ��, ��������Ĳ���д��Ĵ���

    // ���� NVIC, ���ж����ȼ�
    NVIC_InitStructure.NVIC_IRQChannel = KEY_2_INTERRUPT_NUMBER;  // �жϱ�ţ���ʾ�������жϵı�ţ����������ļ��У���оƬ�ڲ���ƶ�Ԥ���趨�õı�ŵġ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 13;     // ��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               // ʹ��
    NVIC_Init(&NVIC_InitStructure);                               // ��ʼ��, ������Ĳ���д��Ĵ���
}



void EXTI9_5_IRQHandler(void)
{
   //���Ƶ��ID
    if(EXTI_GetITStatus(KEY_0_EXTI_LINE) != RESET)
	{
		Motor_Id++;
		if(Motor_Id > 4)
			Motor_Id = 1;
		EXTI_ClearITPendingBit(KEY_0_EXTI_LINE);  // ����жϱ�־λ;
	}
	//���ӽǶ�
	else if(EXTI_GetITStatus(KEY_1_EXTI_LINE) != RESET)
	{
		switch(Motor_Id)
			{
				case 1:
					Motor_Angle[0] = Motor_Angle[0] + angle_psc;
					Control_Multi_Angle2(1,Motor_Angle[0],360);
					break;
				case 2:
					Motor_Angle[1] = Motor_Angle[1] + angle_psc;
					Control_Multi_Angle2(2,Motor_Angle[1],360);
					break;
				case 3:
					Motor_Angle[2] = Motor_Angle[2] + angle_psc;
					Control_Multi_Angle2(3,Motor_Angle[2],360);
					break;
				case 4:
					Motor_Angle[3] = Motor_Angle[3] + angle_psc;
					Control_Multi_Angle2(4,Motor_Angle[3],360);
					break;
				default:
					break;
			}
		EXTI_ClearITPendingBit(KEY_1_EXTI_LINE);  // ����жϱ�־λ;
	}
	//���ٽǶ�
	else if(EXTI_GetITStatus(KEY_2_EXTI_LINE) != RESET)
	{
		switch(Motor_Id)
			{
				case 1:
					Motor_Angle[0] = Motor_Angle[0] - angle_psc;
					Control_Multi_Angle2(1,Motor_Angle[0],360);
					break;
				case 2:
					Motor_Angle[1] = Motor_Angle[1] - angle_psc;
					Control_Multi_Angle2(2,Motor_Angle[1],360);
					break;
				case 3:
					Motor_Angle[2] = Motor_Angle[2] - angle_psc;
					Control_Multi_Angle2(3,Motor_Angle[2],360);
					break;
				case 4:
					Motor_Angle[3] = Motor_Angle[3] - angle_psc;
					Control_Multi_Angle2(4,Motor_Angle[3],360);
					break;
				default:
					break;
			}
		EXTI_ClearITPendingBit(KEY_2_EXTI_LINE);  // ����жϱ�־λ;
	}
	 

}




