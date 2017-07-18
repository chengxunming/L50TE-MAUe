#include "key.h"
#include "global.h"

uint8_t keylin[11];
uint16_t KEYx=0;
//uint8_t key_filter[11]={0,0,0,0,0,0,0,0,0,0,0};
//uint8_t KeyValGet(uint8_t keyNum)
//{
//	uint8_t key_state;
//	switch(keyNum)
//	{
//		case 0:
//			key_state=KEY_NUMFN;
//			break;
//		case 1:
//			key_state=KEY_NUMY;
//			break;
//		case 2:
//			key_state=KEY_NUMAR;
//			break;
//		case 3:
//			key_state=KEY_NUM1;
//			break;
//		case 4:
//			key_state=KEY_NUM2;
//			break;
//		case 5:
//			key_state=KEY_NUM3;
//			break;
//		case 6:
//			key_state=KEY_NUM4;
//			break;
//		case 7:
//			key_state=KEY_NUM5;
//			break;
//		case 8:
//			key_state=KEY_NUM6;
//			break;
//		case 9:
//			key_state=KEY_NUM7;
//			break;
//		case 10:
//			key_state=KEY_NUM8;
//			break;
//	}
//	return key_state;
//}

//uint8_t KEY_Check(uint8_t keyNum)
//{
//	uint8_t CurrentKeyVal;
//	CurrentKeyVal=KeyValGet(keyNum);
//	if(CurrentKeyVal==0)//检测到按键按下
//	{
//		if(key_filter[keyNum]>=1)//10ms按键去抖
//		{
//			if(keyNum==KEY_FN)
//			{
//				
//			}
//		}
//		else
//		{
//			key_filter[keyNum]++;
//		}
//	}
//}

//uint8_t read_keyVal(void)
//{
//	uint8_t i;
//	KeyValGet();
//	for(i=0;i<11;i++)
//	{
//		KEY_Check(i);
//	}
//}

void ReadKeyboard(void)
{
	KEYx=0;

	keylin[0] = KEY_NUMFN;
	keylin[1] = KEY_NUMYF;
	keylin[2] = KEY_NUMAR;
	keylin[3] = KEY_NUM1;
	keylin[4] = KEY_NUM2;
	keylin[5] = KEY_NUM3;
	keylin[6] = KEY_NUM4;
	keylin[7] = KEY_NUM5;
	keylin[8] = KEY_NUM6;
	keylin[9] = KEY_NUM7;
	keylin[10] = KEY_NUM8;


	KEYx  = keylin[0];
	KEYx |= keylin[1]<<1;
	KEYx |= keylin[2]<<2;
	KEYx |= keylin[3]<<3;
	KEYx |= keylin[4]<<4;
	KEYx |= keylin[5]<<5;
	KEYx |= keylin[6]<<6;
	KEYx |= keylin[7]<<7;	
	KEYx |= keylin[8]<<8;
	KEYx |= keylin[9]<<9;
	KEYx |= keylin[10]<<10;
}

uint8_t Read_KeyValue(void)
{
	uint8_t KeyValue=0x0;
	
	ReadKeyboard();
	if ((KEYx & 0x7ff)!= 0x7ff)
	{
		ReadKeyboard();
		switch (KEYx & 0x7ff)
		{
			case 0x7fe:
				KeyValue=KEY_FN;
				break;
			case 0x7fd:
				KeyValue=KEY_YF;
				break;
			case 0x7fb:
				KeyValue=KEY_AR;
				break;
			case 0x7f7:
				KeyValue=KEY_1;
				break;
			case 0x7ef:
				KeyValue=KEY_2;
				break;
			case 0x7df:
				KeyValue=KEY_3;
				break;
			case 0x7bf:
				KeyValue=KEY_4;
				break;
			case 0x77f:
				KeyValue=KEY_5;
				break;
			case 0x6ff:
				KeyValue=KEY_6;
				break;
			case 0x5ff:
				KeyValue=KEY_7;
				break;
			case 0x3ff:
				KeyValue=KEY_8;
				break;
		}
	}
	
	return KeyValue;
}



