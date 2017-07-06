#include "bsp.h"
#include "bsp_can.h"
#include "bsp_io.h"
#include "bsp_led.h"
#include "delay.h"

Frame_DefType send_Frame;

static void ChannelX_ResultSend(uint8_t chNum,uint8_t chState);
//CAN接口测试
void MAU_CanTest(void)
{
	uint8_t can_test_num=0;
	while(1)
	{
		can_test_num++; //can测试次数累加
		
		send_Frame.module_id=MAU_CAN_ID;
		send_Frame.fun_code=FUN_CAN;
		send_Frame.dataH=CIRCUIT_CAN;
		send_Frame.dataL=0xff;
		send_Frame.check_sum=DataCheckSum((uint8_t *)(&send_Frame),4);
		
		Send_CAN_DataFrame((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧
		Time_CanRespone_Count=0;
		ev_CanRespone=CAN_EV_ResReady;
		while(ev_CanRespone==CAN_EV_ResReady);
		if(ev_CanRespone!=CAN_EV_ResTimeOut)
		{
			fault_canTest=false;
			break;
		}
		else if(can_test_num>=3)
		{
			fault_canTest=true;
			break;
		}
	}

	send_Frame.module_id=MAU_CAN_ID;
	send_Frame.fun_code=FUN_RESULT;
	send_Frame.dataH=CIRCUIT_CAN;
	if( ev_CanRespone==CAN_EV_ResOk )send_Frame.dataL=0x01;
	else send_Frame.dataL=0x02;
	
	send_Frame.check_sum=DataCheckSum((uint8_t *)(&send_Frame),4);
		
	Send_CAN_DataFrame((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧
	
}
//逻辑通道检测
void MAU_SwitchTest(void)
{
	ChannelX_ResultSend(CIRCUIT_SW1,SIGNAL1);
	ChannelX_ResultSend(CIRCUIT_SW2,SIGNAL2);
	ChannelX_ResultSend(CIRCUIT_SW3,SIGNAL3);
	ChannelX_ResultSend(CIRCUIT_SW4,SIGNAL4);
}
//蜂鸣器电路测试，连续响3声
void MAU_BuzzerTest(void)
{
	BEEPON;
	delay_ms(200);
	BEEPOFF;
	delay_ms(200);
	BEEPON;
	delay_ms(200);
	BEEPOFF;
	delay_ms(200);
	BEEPON;
	delay_ms(200);
	BEEPOFF;
}
//发送测试结束帧
void MAU_TestEnd(void)
{
	send_Frame.module_id=MAU_CAN_ID;
	send_Frame.fun_code=FUN_END;
	send_Frame.dataH=0xff;
	send_Frame.dataL=0xff;
	send_Frame.check_sum=DataCheckSum((uint8_t *)(&send_Frame),4);
		
	Send_CAN_DataFrame((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧	
}

//CAN接口测试命令响应处理
void MAU_FunCan_Respone_Deal(Frame_DefType * data)
{
	if( (data->dataH == CIRCUIT_CAN ) && (data->dataL==0xff) && (data->module_id==MAU_CAN_ID) )
	{
		ev_CanRespone=CAN_EV_ResOk;
	}
	else 
	{
		ev_CanRespone=CAN_EV_ResFail;
	}
}

//计算和校验
uint8_t DataCheckSum(uint8_t *data,uint8_t len)
{
	uint8_t sum=0;
	uint8_t i;
	for(i=0;i<len;i++)
	{
		sum+=data[i];
	}
	sum=0xff-sum;
	return sum;
}

//通道测试结果发送到测试板
static void ChannelX_ResultSend(uint8_t chNum,uint8_t chState)
{
	send_Frame.module_id=MAU_CAN_ID;
	send_Frame.fun_code=FUN_RESULT;
	send_Frame.dataH=chNum;
	if(chState==0)send_Frame.dataL=0x01;
	else send_Frame.dataL=0x02;

	send_Frame.check_sum=DataCheckSum((uint8_t *)(&send_Frame),4);
			
	Send_CAN_DataFrame((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧
}
