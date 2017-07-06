#include "bsp_can.h"
#include "global.h"
#include "bsp.h"

static Frame_DefType receive_Frame;

static void CAN_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	CAN1_GPIO_CLK_ENABLE();//使能CAN1_GPIO时钟	                   											

    GPIO_InitStructure.GPIO_Pin = CAN1_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
    GPIO_Init(CAN1_GPIO, &GPIO_InitStructure);			//初始化IO
   
    GPIO_InitStructure.GPIO_Pin = CAN1_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
    GPIO_Init(CAN1_GPIO, &GPIO_InitStructure);		 //初始化IO	
}
/**************************************************************************************************************
函数描述 : CAN初始化
***************************************************************************************************************/
void CAN_Configuration(void)
{
	CAN_InitTypeDef  CAN_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 

	CAN_DeInit(CAN1);/* CAN register init */
	CAN_StructInit(&CAN_InitStructure);
	
	CAN_GPIO_Init();
	CAN1_CLK_ENABLE();//使能CAN1时钟
	
	CAN_InitStructure.CAN_TTCM = DISABLE; /*禁止时间触发通讯模式*/
//	CAN_InitStructure.CAN_ABOM = ENABLE; /*自动退出离线状态方式，0-有条件手动离线，1-自动退出离线状态*/
	CAN_InitStructure.CAN_ABOM = DISABLE; /*自动退出离线状态方式，0-有条件手动离线，1-自动退出离线状态*/
	CAN_InitStructure.CAN_AWUM = DISABLE; /*0-由软件通过清0唤醒，1-检测到报文时，自动唤醒*/
	CAN_InitStructure.CAN_NART = DISABLE; /*0-一直重复发送直到成功，1-不论成功以否只发送一次*///自动重传											
	CAN_InitStructure.CAN_RFLM = DISABLE; /*0-溢出时FIFO未锁定，新报文盖掉旧报文，1-锁定，溢出后新报文直接丢失*/
	CAN_InitStructure.CAN_TXFP = DISABLE; /*0-报文发送优先级由标志符决定，1-报文发送优先级由请求先后顺序决定*/
//	CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;/*模式-测试模式-正常模式*/
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
/*-----------------------------------------------
//BaudRate = APB1 / ((BS1 + BS2 + 1) * Prescaler)
//--------------------------------------------------------
//   速率   |  SJW  |  BS1  |  BS2  | prescaler | Sample |
//--------------------------------------------------------
// 1000kbps |  1tq  | 2tq   |  1tq  |     9     |  75.0% |
//--------------------------------------------------------
//  800kbps |  1tq  | 3tq   |  1tq  |     9     |  80.0% |
//--------------------------------------------------------
//  500kbps |  1tq  | 6tq   |  1tq  |     9     |  87.5% |
//--------------------------------------------------------
//  250kbps |  1tq  | 6tq   |  1tq  |     18    |  87.5% |
//--------------------------------------------------------
//  125kbps |  1tq  | 13tq  |  2tq  |     18    |  87.5% |
//--------------------------------------------------------
//  100kbps |  1tq  | 6tq   |  1tq  |     45    |  87.5% |  
//--------------------------------------------------------*/
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;//重同步时间宽度
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;//CAN_BS1_5tq;//时间1宽度
	CAN_InitStructure.CAN_BS2 = CAN_BS2_1tq;//时间2宽度
	CAN_InitStructure.CAN_Prescaler = 9;//分频值(时间单元长度)
	CAN_Init(CAN1, &CAN_InitStructure);
	
	//CAN_Filter_Reconfig(BOARD);
//	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	
	/* Enable the CAN Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	CAN_Filter_Config();
}
/*
 * 函数名：CAN_Filter_Config
 * 描述  ：CAN的过滤器 配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
void CAN_Filter_Config(void)
{
   CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/*CAN过滤器初始化*/
	CAN_FilterInitStructure.CAN_FilterNumber=0;						//过滤器组0
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//工作在标识列表模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//过滤器位宽为单个32位。
	/* 使能报文标示符过滤器按照标示符的内容进行比对过滤，扩展ID不是如下的就抛弃掉，是的话，会存入FIFO0。 */

    CAN_FilterInitStructure.CAN_FilterIdHigh= ((uint32_t)(MAU_CAN_ID<<21)&0xffff0000)>>16;				//要过滤的ID高位 
    CAN_FilterInitStructure.CAN_FilterIdLow= (CAN_ID_STD|CAN_RTR_DATA)&0xffff; //要过滤的ID低位 
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xEFFF;//0x7FFF;			//过滤器高16位每位必须匹配
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xFFFF;//0xFFFF;			//过滤器低16位每位必须匹配
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;				//过滤器被关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//使能过滤器
	CAN_FilterInit(&CAN_FilterInitStructure);
	/*CAN通信中断使能*/
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

//发送CAN数据帧--PDU发送--单帧
uint8_t Send_CAN_DataFrame(uint8_t *ptr,uint8_t len) //ptr: 数据指针. len: 数据长度 
{
	uint16_t i=0;
	uint8_t mailbox_num, can_tx_fail=0;
	uint32_t SENDID;
	CanTxMsg TxMessage;

	
	SENDID = (0x01<<7)|MAU_CAN_ID;
	
	TxMessage.StdId = SENDID;	
	TxMessage.IDE = CAN_ID_STD;//标准帧     //CAN_ID_EXT;
	TxMessage.RTR = CAN_RTR_DATA;//数据帧
    
	if((len<=8)&&(len>0))
	{
		TxMessage.DLC = len;
		
		for(i=0; i<len; i++)
		{
			TxMessage.Data[i] = *ptr++;
		}	
	}
	else
	{
		TxMessage.DLC = 0;
	}
	
	mailbox_num = CAN_Transmit(CAN1, &TxMessage);
	
	while((CAN_TransmitStatus(CAN1, mailbox_num) != CANTXOK)&&(i<1000)) //发送状态,等到发送完成  
	{
		i++;
	}
	if(i==1000)
	{
		CAN_CancelTransmit(CAN1, mailbox_num);//超时关闭发送
		can_tx_fail = 1;
	}
	return can_tx_fail;
}

//接收CAN数据帧，作功能识别
void Receive_CAN_DataFrame(CanRxMsg* canRx,uint8_t num)
{
	uint8_t i;
	uint8_t *prt_rx;
	if(num!=5)
	{
		ev_CanRespone=CAN_EV_ResFail;
		return;
	}
	prt_rx=(uint8_t *)(&receive_Frame);
	for(i=0; i<num; i++)
	{
		*(prt_rx+i)= canRx->Data[i];
	}
	if(DataCheckSum((uint8_t *)(&receive_Frame),4)!=receive_Frame.check_sum)
	{
		ev_CanRespone=CAN_EV_ResFail;
		return;
	}
	switch(receive_Frame.fun_code)
	{
		case FUN_CAN:
			MAU_FunCan_Respone_Deal(&receive_Frame);
			break;
		case FUN_RESULT:
			
			break;
		case FUN_END:
			
			break;
		default:
			ev_CanRespone=CAN_EV_ResFail;
			break;
	}

}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

void USB_LP_CAN1_RX0_IRQHandler(void)       //CAN接收中断函数
{
	uint8_t i;
	CanRxMsg RxMessage;
 	

	RxMessage.StdId = 0x00;  //标准标识符（取值 0~0x7FF）
	RxMessage.ExtId = 0x00;  //扩展标识符（取值 0~0x3FFFF）
	RxMessage.IDE = 0x00;    //标识符的类型
	RxMessage.DLC = 0x00;    //消息的帧长度（取值 0~0x8）
	RxMessage.FMI = 0x00;    //要通过的过滤器索引，这些消息存储于邮箱中（取值为0~0xFF）
	for(i=0; i<8; i++)
	{
		RxMessage.Data[i] = 0x00;//待传输数据（取值为0~0xFF）
	}
	
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//接收
	
	if(RxMessage.IDE==CAN_ID_STD)
	{
		if(RxMessage.DLC>0)
		{
			Receive_CAN_DataFrame(&RxMessage,RxMessage.DLC);//接收CAN数据,执行相应操作	
		}
		if(RxMessage.DLC==0)
		{

		}
		
	}
}

