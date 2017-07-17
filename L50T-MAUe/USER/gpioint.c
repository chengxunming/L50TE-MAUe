/* Includes ------------------------------------------------------------------*/

#include "global.h"

/* Private variables ---------------------------------------------------------*/



/**************************************************************************************************************
                            函数描述 : GPIO初始化
***************************************************************************************************************/
void GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//上电设置所有GPIO为输入上拉
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|
	                               GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
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
	                      KeyBoard 矩阵键盘初始化
	****************************************************************************/
	//设为上拉输入
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_SetBits(GPIOB,GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);	//置1,默认上拉输入
	GPIO_SetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);
	
	/***************************************************************************
	                      KeyBoard 背光LED初始化
	****************************************************************************/
	//设置 KeyBoard 背光LED 默认推挽输出，默认置1
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);	//置1关闭LED
	GPIO_SetBits(GPIOC,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);

	/***************************************************************************
	                      开关指示灯LED初始化
	****************************************************************************/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_SetBits(GPIOA,GPIO_Pin_8);	//置1关闭LED
	GPIO_SetBits(GPIOC,GPIO_Pin_14|GPIO_Pin_15);
	
	/***************************************************************************
	                      状态指示灯LED初始化
	****************************************************************************/
	//设置状态指示灯LED推挽输出，默认置1
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

	GPIO_SetBits(GPIOB,GPIO_Pin_6);	//置1关闭LED
	GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
	GPIO_SetBits(GPIOD,GPIO_Pin_2);

	/***************************************************************************
	                      拨动开关状态检测初始化
	****************************************************************************/
	//拨动开关状态检测，上拉输入默认置1
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_SetBits(GPIOB,GPIO_Pin_0); //置1，默认上拉输入
	GPIO_SetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_5); //置1，默认上拉输入

	/***************************************************************************
	                      蜂鸣器Beep输出控制初始化
	****************************************************************************/
	//蜂鸣器驱动Beep推挽输出，默认置0
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_ResetBits(GPIOC,GPIO_Pin_0);	//置0，关闭蜂鸣器输出

	/***************************************************************************
	                      CAN通信引脚初始化
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

/*--------------------------------矩阵键盘初始化----------------------------------------
*  功能:  初始化stm32单片机GPIO  //PA0-PA7
*  参数传递：
*  输入：
*  返回值:
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
*    矩阵键盘驱动
* 文件:     keyboard.c
* 编写人:   Demos
* 描述:     扫描4x4矩阵键盘输入，并返回键值
* 适用范围：驱动为ST3.5库编写，适用于STM32F10x系列单片机
* 所用引脚：PA0-PA7
* 编写时间: 2015年10月15日
* 版本:     1.0
--------------------------------------------------------------------------------------*//*

#include"stm32f10x.h"
#include"keyboard.h"
#include"dealy.h"
*/
/*------------------------------矩阵键盘扫描--------------------------------------------
*  功能:  扫描矩阵键盘，并返回键值
*  参数:
*  输入:  无
*  返回:  有键按下返回该键值
*         无键按下时则返回0
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
*   矩阵键盘驱动
* 文件:     keyboard.h
* 编写人:   Demos
* 描述:     扫描4x4矩阵键盘输入，并返回键值
* 适用范围：驱动为ST3.5库编写，适用于STM32F10x系列单片机
* 所用引脚：PA0-PA7
* 编写时间: 2015年10月15日
* 版本:     1.0
--------------------------------------------------------------------------------------*//*
#ifndef__KEYBOARD_H
#define__KEYBOARD_H

voidKeyBoard_Init(void);

u8Read_KeyValue(void);

#endif
*/
/*----------------------------------THEEND------------------------------------------*/



