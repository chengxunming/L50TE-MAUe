/* Includes ------------------------------------------------------------------*/

#include "global.h"

/* Private variables ---------------------------------------------------------*/



/**************************************************************************************************************
                            �������� : GPIO��ʼ��
***************************************************************************************************************/
void GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//�ϵ���������GPIOΪ��������
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|
	                               GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|
	                               GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|
	                               GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|
	                               GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD,&GPIO_InitStructure);


	/***************************************************************************
	                      KeyBoard ������̳�ʼ��
	****************************************************************************/
	//��Ϊ��������
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_SetBits(GPIOB,GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);	//��1,Ĭ����������
	GPIO_SetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);
	
	/***************************************************************************
	                      KeyBoard ����LED��ʼ��
	****************************************************************************/
	//���� KeyBoard ����LED Ĭ�����������Ĭ����1
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);	//��1�ر�LED
	GPIO_SetBits(GPIOC,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);

	/***************************************************************************
	                      ����ָʾ��LED��ʼ��
	****************************************************************************/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_SetBits(GPIOA,GPIO_Pin_8);	//��1�ر�LED
	GPIO_SetBits(GPIOC,GPIO_Pin_14|GPIO_Pin_15);
	
	/***************************************************************************
	                      ״ָ̬ʾ��LED��ʼ��
	****************************************************************************/
	//����״ָ̬ʾ��LED���������Ĭ����1
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);

	GPIO_SetBits(GPIOB,GPIO_Pin_6);	//��1�ر�LED
	GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
	GPIO_SetBits(GPIOD,GPIO_Pin_2);

	/***************************************************************************
	                      ��������״̬����ʼ��
	****************************************************************************/
	//��������״̬��⣬��������Ĭ����1
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_SetBits(GPIOB,GPIO_Pin_0); //��1��Ĭ����������
	GPIO_SetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_5); //��1��Ĭ����������

	/***************************************************************************
	                      ������Beep������Ƴ�ʼ��
	****************************************************************************/
	//����������Beep���������Ĭ����0
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_ResetBits(GPIOC,GPIO_Pin_0);	//��0���رշ��������

	/***************************************************************************
	                      CANͨ�����ų�ʼ��
	****************************************************************************/
	//CAN GPIO CONFIG
	// Configure CAN pin: RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//Configure CAN pin: TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

}

/*--------------------------------������̳�ʼ��----------------------------------------
*  ����:  ��ʼ��stm32��Ƭ��GPIO  //PA0-PA7
*  �������ݣ�
*  ���룺
*  ����ֵ:
--------------------------------------------------------------------------------------*/
/*
 void KeyBoard_Init(void){

   GPIO_InitTypeDef GPIO_InitStructure;

   GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	 GPIO_Init(GPIOA,&GPIO_InitStructure);

   GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;
	 GPIO_Init(GPIOA,&GPIO_InitStructure);

   GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
   GPIO_ResetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);

}
*/
/*--------------------------------------------------------------------------------------
*    �����������
* �ļ�:     keyboard.c
* ��д��:   Demos
* ����:     ɨ��4x4����������룬�����ؼ�ֵ
* ���÷�Χ������ΪST3.5���д��������STM32F10xϵ�е�Ƭ��
* �������ţ�PA0-PA7
* ��дʱ��: 2015��10��15��
* �汾:     1.0
--------------------------------------------------------------------------------------*//*

#include"stm32f10x.h"
#include"keyboard.h"
#include"dealy.h"
*/
/*------------------------------�������ɨ��--------------------------------------------
*  ����:  ɨ�������̣������ؼ�ֵ
*  ����:
*  ����:  ��
*  ����:  �м����·��ظü�ֵ
*         �޼�����ʱ�򷵻�0
--------------------------------------------------------------------------------------*/
/*
u8 Read_KeyValue(void){
  u8 KeyValue=0;
	if((GPIO_ReadInputData(GPIOA)&0xff)!=0x0f){
		Delay_ms(10);
		if((GPIO_ReadInputData(GPIOA)&0xff)!=0x0f){
			GPIO_SetBits(GPIOA,GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
			switch(GPIO_ReadInputData(GPIOA) & 0xff){
				case 0x11:
            KeyValue=1;
            break;
        case 0x21:
            KeyValue=5;
            break;
        case 0x41:
            KeyValue=9;
            break;
        case 0x81:
            KeyValue=13;
            break;
      }
      GPIO_SetBits(GPIOA,GPIO_Pin_1);
      GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3);
			switch(GPIO_ReadInputData(GPIOA)&0xff){
				case 0x12:
					KeyValue=2;
          break;
        case 0x22:
          KeyValue=6;
          break;
        case 0x42:
          KeyValue=10;
          break;
        case 0x82:
          KeyValue=14;
          break;
      }
      GPIO_SetBits(GPIOA,GPIO_Pin_2);
      GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3);
			switch(GPIO_ReadInputData(GPIOA)&0xff){
				case 0x14:
          KeyValue=3;
          break;
        case 0x24:
          KeyValue=7;
          break;
        case 0x44:
          KeyValue=11;
          break;
        case 0x84:
          KeyValue=15;
          break;
      }
      GPIO_SetBits(GPIOA,GPIO_Pin_3);
      GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
			switch(GPIO_ReadInputData(GPIOA)&0xff){
				case 0x18:
					KeyValue=4;
          break;
        case 0x28:
        	KeyValue=8;
          break;
        case 0x48:
	        KeyValue=12;
          break;
        case 0x88:
	        KeyValue=16;
          break;
      }
      GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
      GPIO_ResetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
      while((GPIO_ReadInputData(GPIOA)&0xff)!=0x0f);
      return KeyValue;
      }
   }

   return 0;
}
*/

/*--------------------------------THEEND--------------------------------------------*/
/*--------------------------------------------------------------------------------------
*   �����������
* �ļ�:     keyboard.h
* ��д��:   Demos
* ����:     ɨ��4x4����������룬�����ؼ�ֵ
* ���÷�Χ������ΪST3.5���д��������STM32F10xϵ�е�Ƭ��
* �������ţ�PA0-PA7
* ��дʱ��: 2015��10��15��
* �汾:     1.0
--------------------------------------------------------------------------------------*//*
#ifndef__KEYBOARD_H
#define__KEYBOARD_H

voidKeyBoard_Init(void);

u8Read_KeyValue(void);

#endif
*/
/*----------------------------------THEEND------------------------------------------*/



