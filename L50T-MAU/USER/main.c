/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "global.h"
#include "SysTick.h"

/* Private variables ---------------------------------------------------------*/

//uint32_t TestCnt = 10000;

extern LED LEDStatus;

/*********************************************************************************************************
** �������ƣ�myDelay
** �������������Լus��ʱ������ȷ
** ���������ulTime
** ����ֵ  ����
*********************************************************************************************************/
void myDelay (uint32_t  ulTime)
{
  uint32_t i;   

  while (ulTime--)
	{
    for (i = 0; i < 19192; i++);//120M��Լ1us����ȷ
  }
}



/**********************************************************************
* ��    �ƣ�IWDG_INIT()
* ��    �ܣ���ʱ
* ��ڲ�����cnt
* ���ڲ�����
-----------------------------------------------------------------------
* ˵����
***********************************************************************/
void IWDG_INIT(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_64);
	IWDG_SetReload(0x4e2);
	IWDG_ReloadCounter();
	IWDG_Enable();
}


/**********************************************************************
 * ��������CAN_Filter_Config
 * ����  ��CAN�Ĺ����� ����
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 ********************************************************************/
static void CAN_Filter_Config(void)
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/*CAN��������ʼ��*/
	CAN_FilterInitStructure.CAN_FilterNumber=0;						//��������0
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//�����ڱ�ʶ�б�ģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//������λ��Ϊ����32λ��
	/* ʹ�ܱ��ı�ʾ�����������ձ�ʾ�������ݽ��бȶԹ��ˣ���չID�������µľ����������ǵĻ��������FIFO0�� */

    CAN_FilterInitStructure.CAN_FilterIdHigh= ((uint32_t)(Heartbeat_ID<<21)&0xffff0000)>>16;				//Ҫ���˵�ID��λ 
    CAN_FilterInitStructure.CAN_FilterIdLow= (CAN_ID_STD|CAN_RTR_DATA)&0xffff; //Ҫ���˵�ID��λ 
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x6FFF;//0x7FFF;			//��������16λ������֮֡�����ƥ��
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xFFFF;//0xFFFF;			//��������16λÿλ����ƥ��
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;				//��������������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	//CAN��������ʼ��
	CAN_FilterInitStructure.CAN_FilterNumber=1;						//��������0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//�����ڱ�ʶ�б�ģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//������λ��Ϊ����32λ��
	// ʹ�ܱ��ı�ʾ�����������ձ�ʾ�������ݽ��бȶԹ��ˣ���չID�������µľ����������ǵĻ��������FIFO0��

	CAN_FilterInitStructure.CAN_FilterIdHigh= ((uint32_t)(MAU_CAN_ID<<21)&0xffff0000)>>16;				//Ҫ���˵�ID��λ
	CAN_FilterInitStructure.CAN_FilterIdLow= (CAN_ID_STD|CAN_RTR_DATA)&0xffff; //Ҫ���˵�ID��λ
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x6FFF;//0x7FFF;			//��������16λ������֮֡�����ƥ��
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xFFFF;//0xFFFF;			//��������16λÿλ����ƥ��
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;				//��������������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);
	//CANͨ���ж�ʹ��
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}



int main(void)
{


	SystemInit();         //ϵͳʱ�ӳ�ʼ��

	RCC_Configuration();  //�ⲿʱ������

	GPIOInit();           //GPIO�ڳ�ʼ��


	NVIC_Configuration(); //�ж�����

	CAN_Configuration();  //CAN1�ӿڳ�ʼ��

	CAN_Filter_Config();  //CAN1����������
	
	myDelay(100);
	
	
	TIM_Configuration();  //��ʱ����ʼ��

	IWDG_INIT();          //�������Ź�����

	while (1)
	{

		if (SendHeartbeatFlag)//����������־
		{
			Send_CAN_HeartbeatFrame();//CAN����֡����
			SendHeartbeatFlag=0;
			IWDG_ReloadCounter();
		}
		if (passwordchangeFlag)//�����޸ı�־
		{
			flashstor(password);//�������Flash
			passwordchangeFlag=0;
		}
		RUNLED();
	}
}

/******************* (C) COPYRIGHT 2015 Lopu *****END OF FILE****/
