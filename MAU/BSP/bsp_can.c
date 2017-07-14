#include "bsp_can.h"
#include "global.h"
#include "bsp.h"

static Frame_DefType receive_Frame;
volatile uint8_t Rx_Message[8];
volatile uint16_t FuncNumber;
volatile uint8_t MAUFunFlag;

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
	CAN_InitStructure.CAN_ABOM = ENABLE; /*自动退出离线状态方式，0-有条件手动离线，1-自动退出离线状态*/
//	CAN_InitStructure.CAN_ABOM = DISABLE; /*自动退出离线状态方式，0-有条件手动离线，1-自动退出离线状态*/
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
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

    CAN_FilterInitStructure.CAN_FilterIdHigh= ((uint32_t)(Heartbeat_ID<<21)&0xffff0000)>>16;				//要过滤的ID高位 
    CAN_FilterInitStructure.CAN_FilterIdLow= (CAN_ID_STD|CAN_RTR_DATA)&0xffff; //要过滤的ID低位 
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x6FFF;//0x7FFF;			//过滤器高16位除心跳帧之外必须匹配
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xFFFF;//0xFFFF;			//过滤器低16位每位必须匹配
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;				//过滤器被关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//使能过滤器
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	//CAN过滤器初始化
	CAN_FilterInitStructure.CAN_FilterNumber=1;						//过滤器组0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//工作在标识列表模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//过滤器位宽为单个32位。
	// 使能报文标示符过滤器按照标示符的内容进行比对过滤，扩展ID不是如下的就抛弃掉，是的话，会存入FIFO0。

	CAN_FilterInitStructure.CAN_FilterIdHigh= ((uint32_t)(MAU_CAN_ID<<21)&0xffff0000)>>16;				//要过滤的ID高位
	CAN_FilterInitStructure.CAN_FilterIdLow= (CAN_ID_STD|CAN_RTR_DATA)&0xffff; //要过滤的ID低位
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x6FFF;//0x7FFF;			//过滤器高16位除心跳帧之外必须匹配
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xFFFF;//0xFFFF;			//过滤器低16位每位必须匹配
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;				//过滤器被关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//使能过滤器
	CAN_FilterInit(&CAN_FilterInitStructure);
	//CAN通信中断使能
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

/*
 * 函数名：	Send_CAN_HeartbeatFrame
 * 描述  ：	发送CAN心跳桢
 * 输入  ：	无
 * 输出  : 	can_tx_fail  CAN发送状态 0：成功     1：失败
 */
uint8_t Send_CAN_HeartbeatFrame(void)
{
	uint16_t i=0;
	uint8_t mailbox_num, can_tx_fail=0;
	uint32_t SENDID;
	CanTxMsg TxMessage;

	SENDID = (0x01<<10)|(0x01<<7) | MAU_CAN_ID;//将ID.28置1和ID.25置1,ID28数据帧类型设置，1为心跳帧，0为信息帧

	TxMessage.StdId = SENDID;
	TxMessage.IDE = CAN_ID_STD;   //标准ID
	TxMessage.RTR = CAN_RTR_DATA;

	TxMessage.DLC = 0;

	mailbox_num = CAN_Transmit(CAN1, &TxMessage);


	while ((CAN_TransmitStatus(CAN1, mailbox_num) != CANTXOK)&&(i<2000))   //发送状态,等到发送完成
	{
		i++;
	}
	if (i==2000)
	{
		CAN_CancelTransmit(CAN1, mailbox_num);   //超时关闭发送
		can_tx_fail = 1;
	}
	return can_tx_fail;
}

/*
 * 函数名：	Send_CAN_DataFrame_Single
 * 描述  ：	发送CAN数据帧--单帧
 * 输入  ：	ptr: 数据指针
 * 			len: 数据长度
 * 输出  :	can_tx_fail  CAN发送状态 0：成功     1：失败
 */
uint8_t Send_CAN_DataFrame_Single(uint8_t *ptr, uint8_t len)
{
	uint16_t i=0,FuncH,FuncL;
	uint8_t mailbox_num, can_tx_fail=0;
	uint32_t SENDID;
	CanTxMsg TxMessage;

	SENDID = (0x01<<7) | MAU_CAN_ID; //将ID.25设置为1，1为发送地址，0为接收地址

	switch (MAUFunFlag)
	{
		case 21:
			FuncNumber = 21;//MLMC编号0x15
			break;
		case 22:
			FuncNumber = 22;//MMAC编号0x16
			break;
		case 23:
			FuncNumber = 23;//MFSC编号0x17
			break;
		case 24:
			FuncNumber = 24;//MRTC编号0x18
			break;
		case 25:
			FuncNumber = 25;//MKYC编号0x19
			break;
		case 26:
			FuncNumber = 26;//MKYC编号0x1A
			break;
		case 27:
			FuncNumber = 341;//MAUS编号0x0155
			break;
		default:
			break;
	}

	FuncH = (FuncNumber&0xFF00)>>8;
	FuncL = (uint8_t)(FuncNumber&0x00FF);

	TxMessage.StdId = SENDID;
	TxMessage.IDE = CAN_ID_STD;//CAN_ID_EXT;
	TxMessage.RTR = CAN_RTR_DATA;

	if ((len<=8)&&(len>0))
	{
		TxMessage.DLC = len+3;

		TxMessage.Data[0] = 0x00;   //开始帧,单帧传输
		TxMessage.Data[1] = FuncL;
		TxMessage.Data[2] = FuncH;

		for (i=0; i<len; i++)
		{
			TxMessage.Data[i+3] = *ptr++;
		}
	}
	else
	{
		TxMessage.DLC = 0;
	}

	mailbox_num = CAN_Transmit(CAN1, &TxMessage);

	while ((CAN_TransmitStatus(CAN1, mailbox_num) != CANTXOK)&&(i<2000)) //发送状态,等到发送完成
	{
		i++;
	}
	if (i==2000)
	{
		CAN_CancelTransmit(CAN1, mailbox_num);//超时关闭发送
		can_tx_fail = 1;
	}
	return can_tx_fail;
}





//接收CAN数据帧，作功能识别
/*
 * 函数名：	Send_CAN_DataFrame_Single
 * 描述  ：	发送CAN数据帧--单帧
 * 输入  ：	canRx: CAN接收消息数据指针
 * 			num	 : 接收数据长度
 * 输出  :	can_tx_fail  CAN发送状态 0：成功     1：失败
 */
//接收CAN数据帧，作功能识别
void Receive_CAN_DataFrame(CanRxMsg* canRx,uint8_t num)
{
	uint8_t i;
	uint16_t ID_temp;
//	for (i=0; i<num; i++)
//	{
//		Receive_Buf[i] = canRx->Data[i];
//	}
//	ID_temp = Receive_Buf[2]<<8 | Receive_Buf[1];
//	switch (ID_temp)
//	{
//	case 11:
//		PASC();//介入密码及使能命令,命令0x0B(11)

//		break;
//	case 321:
//		WARN();//警告故障状态,命令0x141(321)

//		break;
//	case 322:
//		FATA();//严重故障状态,命令0x142(322)

//		break;
//	case 323:
//		LMPS();//外灯通道输出开关状态,命令0x143(323)

//		break;
//	case 324:
//		CSOR();//控制源状态,命令0x144(324)

//		break;
//	case 325:
//		FLSS();//闪灯状态,命令0x145(325)

//		break;

//	case 326:
//		REMT();//信号机遥控状态,命令0x146(326)

//		break;
//	case 327:
//		KNMS();//手动步进点状态,命令0x147(327)

//		break;
//	case 328:
//		KSPS();//手动步进点状态,命令0x148(328)

//		break;
//	case 329:
//		KARS();//手动步进点状态,命令0x149(329)

//		break;
//	case 330:
//		KLAS();//手动步进点状态,命令0x14A(330)

//		break;
//	case 331:
//		KLBS();//手动步进点状态,命令0x14B(331)

//		break;
//	case 332:
//		KLCS();//手动步进点状态,命令0x14C(332)

//		break;
//	case 333:
//		KLDS();//手动步进点状态,命令0x14D(333)

//		break;
//	case 334:
//		KLES();//手动步进点状态,命令0x14E(334)

//		break;
//	case 335:
//		KLSS();//手动步进点状态,命令0x14F(335)

//		break;

////自行增加测试备用命令
//	case 336:
//		Test();//键盘指示灯全亮,命令0x150(336)
//		break;
//	default:
//		memset(Receive_Buf,0x00,10);
//		break;
//	}
}

/*
 * 函数名：	IRQHandler_CAN1_RX0_Callback
 * 描述  ：	CAN接收中断回调函数
 * 输入  ：	无
 * 输出  :	无
 */
void IRQHandler_CAN1_RX0_Callback(void)
{
	uint8_t i;
	CanRxMsg RxMessage;

	//初始化接收
	RxMessage.StdId = 0x00;  //标准标识符（取值 0~0x7FF）
	RxMessage.ExtId = 0x00;  //扩展标识符（取值 0~0x3FFFF）
	RxMessage.IDE = 0x00;    //标识符的类型
	RxMessage.DLC = 0x00;    //消息的帧长度（取值 0~0x8）
	RxMessage.FMI = 0x00;    //要通过的过滤器索引，这些消息存储于邮箱中（取值为0~0xFF）
	for (i=0; i<8; i++)
	{
		RxMessage.Data[i] = 0x00;//待传输数据（取值为0~0xFF）
		Rx_Message[i] = 0;
	}
	//接收总线上的数据释放FIFO
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//接收
	//处理接收的数据
	for (i=0; i<RxMessage.DLC; i++)
	{
		Rx_Message[i] = RxMessage.Data[i];
	}

	if (RxMessage.IDE==CAN_ID_STD)
	{
		if (RxMessage.DLC>0)
		{
			Receive_CAN_DataFrame(&RxMessage,RxMessage.DLC);//接收CAN数据
		}
		if (RxMessage.DLC==0)
		{			
//			if(CPUOFFLINEFLAG)
//			{
//				MAVS();
//			}
//			CPUOFFLINEFLAG = 0;
//			RUNledstatus = !RUNledstatus;
//			GetHeartCnt = 12;
		}

	}
}
