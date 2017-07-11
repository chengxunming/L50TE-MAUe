#include "bsp_key.h"
#include "bsp.h"

static KeyHandle_DefType  key_rst,key_mfu;

uint8_t Time_KeyCheck_Count=0;//Ϊ������ѯ����ṩʱ��

//��λ�������
__weak void KEY_RST_Down(void)
{

}
//��λ�����������
__weak void KEY_RST_DownLong(void)
{
	
}
//MFU���԰������
__weak void KEY_MFU_Down(void)
{
	
}
//MFU���԰����������
__weak void KEY_MFU_DownLong(void)
{

}

//�жϰ����Ƿ���
static uint8_t IsKeyRstDown(void)
{
	if(KEY_RST_STATE==0)
		return 1;
	else 
		return 0;
}
static uint8_t IsKeyMfuDown(void)
{
	if(KEY_MFU_STATE==0)
		return 1;
	else 
		return 0;
}

//�����ṹ���ʼ��
static void KEY_InitVar(void)
{
	key_rst.FilterTime=1;//20ms ʱ��20ms
	key_rst.PressTime=0;//Ϊ0����ⰴ������
	key_rst.FilterCount=0;
	key_rst.PressCount=0;
	key_rst.IsPress=false;//����ȱʡ״̬Ϊδ����
	key_rst.event=EV_KEY_NONE;//�ް����¼�
	key_rst.IsKeyDownFunc = IsKeyRstDown;//�󶨰���������⺯��
	key_rst.KeyEventDownFunc=KEY_RST_Down;
	key_rst.KeyEventDownLongFunc=KEY_RST_DownLong;

	key_mfu.FilterTime=1;//20ms ʱ��20ms
	key_mfu.PressTime=50;//1s  ʱ��20ms
	key_mfu.FilterCount=0;
	key_mfu.PressCount=0;
	key_mfu.IsPress=false;//����ȱʡ״̬Ϊδ����
	key_mfu.event=EV_KEY_NONE;//�ް����¼�
	key_mfu.IsKeyDownFunc = IsKeyMfuDown;//�󶨰���������⺯��
	key_mfu.KeyEventDownFunc=KEY_MFU_Down;
	key_mfu.KeyEventDownLongFunc=KEY_MFU_DownLong;
}
//������⴦��
static void KEY_Check(KeyHandle_DefType *pCurrentKey)
{
	if(pCurrentKey->IsKeyDownFunc())
	{
		if(pCurrentKey->IsPress==false)
		{
			if(pCurrentKey->FilterCount>=pCurrentKey->FilterTime)
			{
				pCurrentKey->IsPress=true;
				pCurrentKey->event=EV_KEY_DOWN;
				pCurrentKey->KeyEventDownFunc();
			}
			else pCurrentKey->FilterCount++;
		}
		
		if(pCurrentKey->IsPress)
		{
			if(pCurrentKey->PressTime!=0)
			{
				if(pCurrentKey->PressCount<pCurrentKey->PressTime)
				{
					pCurrentKey->PressCount++;
				}
				else if(pCurrentKey->PressCount==pCurrentKey->PressTime)
				{
					pCurrentKey->PressCount++;
					pCurrentKey->event=EV_KEY_LONG;
					pCurrentKey->KeyEventDownLongFunc();
				}
			}
		}
	}
	else
	{
		if(pCurrentKey->IsPress==true)
		{
			if(pCurrentKey->FilterCount==0)
			{
				pCurrentKey->IsPress=false;
				pCurrentKey->event=EV_KEY_UP;
				pCurrentKey->PressCount=0;
			}
			else pCurrentKey->FilterCount--;
		}

	}
}
//������ʼ��
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	KEY_GPIO_CLK_ENABLE();
	
	GPIO_InitStructure.GPIO_Pin = KEY_MFU_PIN | KEY_RST_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(KEY_GPIO,&GPIO_InitStructure);
	
	KEY_InitVar();
}
//������ѯ
void KEY_Scan(void)
{
	if(Time_KeyCheck_Count>=20)
	{
		KEY_Check(&key_rst);
		KEY_Check(&key_mfu);
		Time_KeyCheck_Count=0;
	}
}




