;/****************************************Copyright (c)**************************************************
;**                               Guangzhou ZHIYUAN electronics Co.,LTD.
;**                                 http://www.zyinside.com
;**
;**--------------File Info-------------------------------------------------------------------------------
;** File name:			LPC2294.inc
;** Last modified Date: 2006-02-22
;** Last Version:		1.0
;** Descriptions:		Defines the special registers and firmware programs for 
;**						lpc22xx\lpc212x\lpc211x\lpc210x
;**
;**------------------------------------------------------------------------------------------------------
;** Created by:			Chenmingji
;** Created date:		2006-02-22
;** Version:            1.0
;** Descriptions:		The original version
;**
;**------------------------------------------------------------------------------------------------------
;** Modified by:
;** Modified date:
;** Version:
;** Descriptions:
;**
;********************************************************************************************************/

;/* EXTERNAL MEMORY CONTROLLER (EMC) */
;/* 外部总线控制器 */
BCFG0          EQU          0xFFE00000          ;/* lpc22xx only */
BCFG1          EQU          0xFFE00004          ;/* lpc22xx only */
BCFG2          EQU          0xFFE00008          ;/* lpc22xx only */
BCFG3          EQU          0xFFE0000C          ;/* lpc22xx only */

;/* External Interrupts */
;/* 外部中断控制寄存器 */
EXTINT         EQU          0xE01FC140
EXTWAKE        EQU          0xE01FC144
INTWAKE        EQU          0xE01FC144
EXTMODE        EQU          0xE01FC148          ;/* no in lpc210x*/
EXTPOLAR       EQU          0xE01FC14C          ;/* no in lpc210x*/

;/* SMemory mapping control. */
;/* 内存remap控制寄存器 */
MEMMAP         EQU          0xE01FC040

;/* Phase Locked Loop (PLL) */
;/* PLL控制寄存器 */
PLLCON         EQU          0xE01FC080
PLLCFG         EQU          0xE01FC084
PLLSTAT        EQU          0xE01FC088
PLLFEED        EQU          0xE01FC08C

;/* Power Control */
;/* 功率控制寄存器 */
PCON           EQU          0xE01FC0C0
PCONP          EQU          0xE01FC0C4

;/* VPB Divider */
;/* VLSI外设总线（VPB）分频寄存器 */
VPBDIV         EQU          0xE01FC100

;/* Reset Source Identification Register */ 
;/* 复位源识别寄存器 */
RSIR           EQU          0xE01FC180      ;/* lpc2130 only */

;/* Code Security Protection Register */
;/* 代码安全寄存器 */
CSPR            EQU         0xE01FC184      ;/* lpc2130 only */

;/* Memory Accelerator Module (MAM) */
;/* 存储器加速模块 */
MAMCR          EQU         0xE01FC000
MAMTIM         EQU         0xE01FC004

;/* Vectored Interrupt Controller (VIC) */
;/* 向量中断控制器(VIC)的特殊寄存器 */
VICIRQStatus    EQU         0xFFFFF000
VICFIQStatus    EQU         0xFFFFF004
VICRawIntr      EQU         0xFFFFF008
VICIntSelect    EQU         0xFFFFF00C
VICIntEnable    EQU         0xFFFFF010
VICIntEnClr     EQU         0xFFFFF014
VICSoftInt      EQU         0xFFFFF018
VICSoftIntClear EQU         0xFFFFF01C
VICProtection   EQU         0xFFFFF020
VICVectAddr     EQU         0xFFFFF030
VICDefVectAddr  EQU         0xFFFFF034
VICVectAddr0    EQU         0xFFFFF100
VICVectAddr1    EQU         0xFFFFF104
VICVectAddr2    EQU         0xFFFFF108
VICVectAddr3    EQU         0xFFFFF10C
VICVectAddr4    EQU         0xFFFFF110
VICVectAddr5    EQU         0xFFFFF114
VICVectAddr6    EQU         0xFFFFF118
VICVectAddr7    EQU         0xFFFFF11C
VICVectAddr8    EQU         0xFFFFF120
VICVectAddr9    EQU         0xFFFFF124
VICVectAddr10   EQU         0xFFFFF128
VICVectAddr11   EQU         0xFFFFF12C
VICVectAddr12   EQU         0xFFFFF130
VICVectAddr13   EQU         0xFFFFF134
VICVectAddr14   EQU         0xFFFFF138
VICVectAddr15   EQU         0xFFFFF13C
VICVectCntl0    EQU         0xFFFFF200
VICVectCntl1    EQU         0xFFFFF204
VICVectCntl2    EQU         0xFFFFF208
VICVectCntl3    EQU         0xFFFFF20C
VICVectCntl4    EQU         0xFFFFF210
VICVectCntl5    EQU         0xFFFFF214
VICVectCntl6    EQU         0xFFFFF218
VICVectCntl7    EQU         0xFFFFF21C
VICVectCntl8    EQU         0xFFFFF220
VICVectCntl9    EQU         0xFFFFF224
VICVectCntl10   EQU         0xFFFFF228
VICVectCntl11   EQU         0xFFFFF22C
VICVectCntl12   EQU         0xFFFFF230
VICVectCntl13   EQU         0xFFFFF234
VICVectCntl14   EQU         0xFFFFF238
VICVectCntl15   EQU         0xFFFFF23C

;/* Pin Connect Block */
;/* 管脚连接模块控制寄存器 */
PINSEL0         EQU         0xE002C000
PINSEL1         EQU         0xE002C004
PINSEL2         EQU         0xE002C014          ;/* no in lpc210x*/

;/* General Purpose Input/Output (GPIO) */
;/* 通用并行IO口的特殊寄存器 */
IOPIN           EQU         0xE0028000          ;/* lpc210x only */
IOSET           EQU         0xE0028004          ;/* lpc210x only */
IODIR           EQU         0xE0028008          ;/* lpc210x only */
IOCLR           EQU         0xE002800C          ;/* lpc210x only */

IO0PIN          EQU         0xE0028000          ;/* no in lpc210x*/
IO0SET          EQU         0xE0028004          ;/* no in lpc210x*/
IO0DIR          EQU         0xE0028008          ;/* no in lpc210x*/
IO0CLR          EQU         0xE002800C          ;/* no in lpc210x*/

IO1PIN          EQU         0xE0028010          ;/* no in lpc210x*/
IO1SET          EQU         0xE0028014          ;/* no in lpc210x*/
IO1DIR          EQU         0xE0028018          ;/* no in lpc210x*/
IO1CLR          EQU         0xE002801C          ;/* no in lpc210x*/

IO2PIN          EQU         0xE0028020          ;/* lpc22xx only */
IO2SET          EQU         0xE0028024          ;/* lpc22xx only */
IO2DIR          EQU         0xE0028028          ;/* lpc22xx only */
IO2CLR          EQU         0xE002802C          ;/* lpc22xx only */

IO3PIN          EQU         0xE0028030          ;/* lpc22xx only */
IO3SET          EQU         0xE0028034          ;/* lpc22xx only */
IO3DIR          EQU         0xE0028038          ;/* lpc22xx only */
IO3CLR          EQU         0xE002803C          ;/* lpc22xx only */

SCS				EQU         0xE01FC1A0          ;/* lpc2103 only */

FIO0DIR			EQU         0x3FFFC000          ;/* lpc2103 only */
FIO0DIR0		EQU			0x3FFFC000			;/* lpc2103 only */
FIO0DIR1		EQU         0x3FFFC001          ;/* lpc2103 only */
FIO0DIR2		EQU         0x3FFFC002          ;/* lpc2103 only */
FIO0DIR3		EQU         0x3FFFC003          ;/* lpc2103 only */
FIO0DIRL		EQU         0x3FFFC000          ;/* lpc2103 only */
FIO0DIRU        EQU         0x3FFFC002          ;/* lpc2103 only */

FIO0MASK		EQU         0x3FFFC010          ;/* lpc2103 only */
FIO0MASK0		EQU         0x3FFFC010          ;/* lpc2103 only */
FIO0MASK1	    EQU         0x3FFFC011          ;/* lpc2103 only */
FIO0MASK2	    EQU         0x3FFFC012          ;/* lpc2103 only */
FIO0MASK3	    EQU         0x3FFFC013          ;/* lpc2103 only */
FIO0MASKL	    EQU         0x3FFFC010          ;/* lpc2103 only */
FIO0MASKU       EQU         0x3FFFC012          ;/* lpc2103 only */

FIO0PIN         EQU         0x3FFFC014          ;/* lpc2103 only */
FIO0PIN0        EQU         0x3FFFC014          ;/* lpc2103 only */
FIO0PIN1        EQU         0x3FFFC015          ;/* lpc2103 only */
FIO0PIN2        EQU         0x3FFFC016          ;/* lpc2103 only */
FIO0PIN3        EQU         0x3FFFC017          ;/* lpc2103 only */
FIO0PINL        EQU         0x3FFFC014          ;/* lpc2103 only */
FIO0PINU        EQU         0x3FFFC016          ;/* lpc2103 only */

FIO0SET	        EQU         0x3FFFC018          ;/* lpc2103 only */
FIO0SET0        EQU         0x3FFFC018          ;/* lpc2103 only */
FIO0SET1        EQU         0x3FFFC019          ;/* lpc2103 only */
FIO0SET2        EQU         0x3FFFC01A          ;/* lpc2103 only */
FIO0SET3        EQU         0x3FFFC01B          ;/* lpc2103 only */
FIO0SETL        EQU         0x3FFFC018          ;/* lpc2103 only */
FIO0SETU        EQU         0x3FFFC01A          ;/* lpc2103 only */

FIO0CLR         EQU         0x3FFFC01C          ;/* lpc2103 only */
FIO0CLR0        EQU         0x3FFFC01C          ;/* lpc2103 only */
FIO0CLR1        EQU         0x3FFFC01D          ;/* lpc2103 only */
FIO0CLR2        EQU         0x3FFFC01E          ;/* lpc2103 only */
FIO0CLR3        EQU         0x3FFFC01F          ;/* lpc2103 only */
FIO0CLRL        EQU         0x3FFFC01C          ;/* lpc2103 only */
FIO0CLRU        EQU         0x3FFFC01E          ;/* lpc2103 only */

;/* Universal Asynchronous Receiver Transmitter 0 (UART0) */
;/* 通用异步串行口0(UART0)的特殊寄存器 */
U0RBR          EQU          0xE000C000
U0THR          EQU          0xE000C000
U0DLL          EQU          0xE000C000
U0DLM          EQU          0xE000C004
U0IER          EQU          0xE000C004
U0IIR          EQU          0xE000C008
U0FCR          EQU          0xE000C008
U0LCR          EQU          0xE000C00C
U0LSR          EQU          0xE000C014
U0SCR          EQU          0xE000C01C
U0ACR          EQU          0xE000C020           ;/* lpc2103 only */
U0FDR	         EQU          0xE000C028		       ;/* lpc2103 only */
U0TER          EQU          0xE000C030

;/* Universal Asynchronous Receiver Transmitter 1 (UART1) */
;/* 通用异步串行口1(UART1)的特殊寄存器 */
U1RBR          EQU          0xE0010000
U1THR          EQU          0xE0010000
U1DLL          EQU          0xE0010000
U1DLM          EQU          0xE0010004
U1IER          EQU          0xE0010004
U1IIR          EQU          0xE0010008
U1FCR          EQU          0xE0010008
U1LCR          EQU          0xE001000C
U1LSR          EQU          0xE0010014
U1SCR          EQU          0xE001001C
U1ACR          EQU          0xE0010020		       ;/* lpc2103 only */
U1FDR          EQU          0xE0010028		       ;/* lpc2103 only */
U1TER          EQU          0xE0010030
U1MCR          EQU          0xE0010010
U1MSR          EQU          0xE0010018

;/* I2C (8/16 bit data bus) */
;/* 芯片间总线（I2C）的特殊寄存器 */
I2CONSET        EQU         0xE001C000
I2STAT          EQU         0xE001C004
I2DAT           EQU         0xE001C008
I2ADR           EQU         0xE001C00C
I2SCLH          EQU         0xE001C010
I2SCLL          EQU         0xE001C014
I2CONCLR        EQU         0xE001C018

;/* I2C0 (8/16 bit data bus) */
;/* 芯片间总线（I2C0）的特殊寄存器 */
I2C0CONSET      EQU         0xE001C000
I2C0STAT        EQU         0xE001C004
I2C0DAT         EQU         0xE001C008
I2C0ADR         EQU         0xE001C00C
I2C0SCLH        EQU         0xE001C010
I2C0SCLL        EQU         0xE001C014
I2C0CONCLR      EQU         0xE001C018

;/* I2C1 (8/16 bit data bus) */
;/* 芯片间总线（I2C1）的特殊寄存器 */
I2C1CONSET      EQU         0xE005C000         ;/* lpc213x only */
I2C1STAT        EQU         0xE005C004         ;/* lpc213x only */
I2C1DAT         EQU         0xE005C008         ;/* lpc213x only */
I2C1ADR         EQU         0xE005C00C         ;/* lpc213x only */
I2C1SCLH        EQU         0xE005C010         ;/* lpc213x only */
I2C1SCLL        EQU         0xE005C014         ;/* lpc213x only */
I2C1CONCLR      EQU         0xE005C018         ;/* lpc213x only */

;/* SPI (Serial Peripheral Interface) */
;/* SPI总线接口的特殊寄存器 */
;        /* only for lpc210x*/
SPI_SPCR       EQU          0xE0020000
SPI_SPSR       EQU          0xE0020004
SPI_SPDR       EQU          0xE0020008
SPI_SPCCR      EQU          0xE002000C
SPI_SPINT      EQU          0xE002001C

S0PCR          EQU          0xE0020000          ;/* no in lpc210x*/
S0PSR          EQU          0xE0020004          ;/* no in lpc210x*/
S0PDR          EQU          0xE0020008          ;/* no in lpc210x*/
S0PCCR         EQU          0xE002000C          ;/* no in lpc210x*/
S0PINT         EQU          0xE002001C          ;/* no in lpc210x*/

S1PCR          EQU          0xE0030000          ;/* no in lpc210x*/
S1PSR          EQU          0xE0030004          ;/* no in lpc210x*/
S1PDR          EQU          0xE0030008          ;/* no in lpc210x*/
S1PCCR         EQU          0xE003000C          ;/* no in lpc210x*/
S1PINT         EQU          0xE003001C          ;/* no in lpc210x*/

;/* for lpc213x */
 
SSPCR0         EQU          0xE0068000          ;/* lpc2132\2138 only */
SSPCR1         EQU          0xE0068004          ;/* lpc2132\2138 only */
SSPDR          EQU          0xE0068008          ;/* lpc2132\2138 only */
SSPSR          EQU          0xE006800C          ;/* lpc2132\2138 only */
SSPCPSR        EQU          0xE0068010          ;/* lpc2132\2138 only */
SSPIMSC        EQU          0xE0068014          ;/* lpc2132\2138 only */
SSPRIS         EQU          0xE0068018          ;/* lpc2132\2138 only */
SSPMIS         EQU          0xE006801C          ;/* lpc2132\2138 only */
SSPICR         EQU          0xE0068020          ;/* lpc2132\2138 only */

;/* CAN CONTROLLERS AND ACCEPTANCE FILTER */
;/* CAN控制器和接收路波器 */
CAN1MOD        EQU          0xE0044000          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN1CMR        EQU          0xE0044004          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN1GSR        EQU          0xE0044008          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN1ICR        EQU          0xE004400C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN1IER        EQU          0xE0044010          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN1BTR        EQU          0xE0044014          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN1EWL        EQU          0xE004401C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN1SR         EQU          0xE0044020          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN1RFS        EQU          0xE0044024          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN1RDA        EQU          0xE0044028          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN1RDB        EQU          0xE004402C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN1TFI1       EQU          0xE0044030          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN1TID1       EQU          0xE0044034          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN1TDA1       EQU          0xE0044038          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN1TDB1       EQU          0xE004403C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN1TFI2       EQU          0xE0044040          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN1TID2       EQU          0xE0044044          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN1TDA2       EQU          0xE0044048          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN1TDB2       EQU          0xE004404C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN1TFI3       EQU          0xE0044050          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN1TID3       EQU          0xE0044054          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN1TDA3       EQU          0xE0044058          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN1TDB3       EQU          0xE004405C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */

CAN2MOD        EQU          0xE0048000          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN2CMR        EQU          0xE0048004          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN2GSR        EQU          0xE0048008          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN2ICR        EQU          0xE004800C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN2IER        EQU          0xE0048010          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN2BTR        EQU          0xE0048014          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN2EWL        EQU          0xE004801C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN2SR         EQU          0xE0048020          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN2RFS        EQU          0xE0048024          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN2RDA        EQU          0xE0048028          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN2RDB        EQU          0xE004802C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN2TFI1       EQU          0xE0048030          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN2TID1       EQU          0xE0048034          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN2TDA1       EQU          0xE0048038          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN2TDB1       EQU          0xE004803C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN2TFI2       EQU          0xE0048040          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN2TID2       EQU          0xE0048044          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN2TDA2       EQU          0xE0048048          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN2TDB2       EQU          0xE004804C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN2TFI3       EQU          0xE0048050          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN2TID3       EQU          0xE0048054          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN2TDA3       EQU          0xE0048058          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN2TDB3       EQU          0xE004805C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */

CAN3MOD        EQU          0xE004C000          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN3CMR        EQU          0xE004C004          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN3GSR        EQU          0xE004C008          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN3ICR        EQU          0xE004C00C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN3IER        EQU          0xE004C010          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN3BTR        EQU          0xE004C014          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN3EWL        EQU          0xE004C01C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN3SR         EQU          0xE004C020          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN3RFS        EQU          0xE004C024          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN3RDA        EQU          0xE004C028          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN3RDB        EQU          0xE004C02C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN3TFI1       EQU          0xE004C030          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN3TID1       EQU          0xE004C034          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN3TDA1       EQU          0xE004C038          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN3TDB1       EQU          0xE004C03C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN3TFI2       EQU          0xE004C040          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN3TID2       EQU          0xE004C044          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN3TDA2       EQU          0xE004C048          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN3TDB2       EQU          0xE004C04C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN3TFI3       EQU          0xE004C050          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN3TID3       EQU          0xE004C054          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN3TDA3       EQU          0xE004C058          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN3TDB3       EQU          0xE004C05C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */

CAN4MOD        EQU          0xE0050000          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN4CMR        EQU          0xE0050004          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN4GSR        EQU          0xE0050008          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN4ICR        EQU          0xE005000C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN4IER        EQU          0xE0050010          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN4BTR        EQU          0xE0050014          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN4EWL        EQU          0xE005001C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN4SR         EQU          0xE0050020          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN4RFS        EQU          0xE0050024          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN4RDA        EQU          0xE0050028          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN4RDB        EQU          0xE005002C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN4TFI1       EQU          0xE0050030          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN4TID1       EQU          0xE0050034          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN4TDA1       EQU          0xE0050038          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN4TDB1       EQU          0xE005003C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN4TFI2       EQU          0xE0050040          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN4TID2       EQU          0xE0050044          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN4TDA2       EQU          0xE0050048          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN4TDB2       EQU          0xE005004C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN4TFI3       EQU          0xE0050050          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN4TID3       EQU          0xE0050054          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN4TDA3       EQU          0xE0050058          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN4TDB3       EQU          0xE005005C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */

CAN5MOD        EQU          0xE0054000          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN5CMR        EQU          0xE0054004          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN5GSR        EQU          0xE0054008          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN5ICR        EQU          0xE005400C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN5IER        EQU          0xE0054010          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN5BTR        EQU          0xE0054014          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN5EWL        EQU          0xE005401C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN5SR         EQU          0xE0054020          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN5RFS        EQU          0xE0054024          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN5RDA        EQU          0xE0054028          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN5RDB        EQU          0xE005402C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN5TFI1       EQU          0xE0054030          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN5TID1       EQU          0xE0054034          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN5TDA1       EQU          0xE0054038          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN5TDB1       EQU          0xE005403C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN5TFI2       EQU          0xE0054040          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN5TID2       EQU          0xE0054044          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN5TDA2       EQU          0xE0054048          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN5TDB2       EQU          0xE005404C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN5TFI3       EQU          0xE0054050          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN5TID3       EQU          0xE0054054          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN5TDA3       EQU          0xE0054058          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CAN5TDB3       EQU          0xE005405C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */

CAN6MOD        EQU          0xE0058000          ;/* lpc2292\lpc2294 only */
CAN6CMR        EQU          0xE0058004          ;/* lpc2292\lpc2294 only */
CAN6GSR        EQU          0xE0058008          ;/* lpc2292\lpc2294 only */
CAN6ICR        EQU          0xE005800C          ;/* lpc2292\lpc2294 only */
CAN6IER        EQU          0xE0058010          ;/* lpc2292\lpc2294 only */
CAN6BTR        EQU          0xE0058014          ;/* lpc2292\lpc2294 only */
CAN6EWL        EQU          0xE005801C          ;/* lpc2292\lpc2294 only */
CAN6SR         EQU          0xE0058020          ;/* lpc2292\lpc2294 only */
CAN6RFS        EQU          0xE0058024          ;/* lpc2292\lpc2294 only */
CAN6RDA        EQU          0xE0058028          ;/* lpc2292\lpc2294 only */
CAN6RDB        EQU          0xE005802C          ;/* lpc2292\lpc2294 only */
CAN6TFI1       EQU          0xE0058030          ;/* lpc2292\lpc2294 only */
CAN6TID1       EQU          0xE0058034          ;/* lpc2292\lpc2294 only */
CAN6TDA1       EQU          0xE0058038          ;/* lpc2292\lpc2294 only */
CAN6TDB1       EQU          0xE005803C          ;/* lpc2292\lpc2294 only */
CAN6TFI2       EQU          0xE0058040          ;/* lpc2292\lpc2294 only */
CAN6TID2       EQU          0xE0058044          ;/* lpc2292\lpc2294 only */
CAN6TDA2       EQU          0xE0058048          ;/* lpc2292\lpc2294 only */
CAN6TDB2       EQU          0xE005804C          ;/* lpc2292\lpc2294 only */
CAN6TFI3       EQU          0xE0058050          ;/* lpc2292\lpc2294 only */
CAN6TID3       EQU          0xE0058054          ;/* lpc2292\lpc2294 only */
CAN6TDA3       EQU          0xE0058058          ;/* lpc2292\lpc2294 only */
CAN6TDB3       EQU          0xE005805C          ;/* lpc2292\lpc2294 only */

CANTxSR        EQU          0xE0040000          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CANRxSR        EQU          0xE0040004          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CANMSR         EQU          0xE0040008          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */

CANAFMR        EQU          0xE003C000          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CANSFF_sa      EQU          0xE003C004          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CANSFF_GRP_sa  EQU          0xE003C008          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CANEFF_sa      EQU          0xE003C00C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CANEFF_GRP_sa  EQU          0xE003C010          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CANENDofTable  EQU          0xE003C014          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CANLUTerrAd    EQU          0xE003C018          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */
CANLUTerr      EQU          0xE003C01C          ;/* lpc2119\lpc2129\lpc2292\lpc2294 only */


;/* Timer 0 */
;/* 定时器0的特殊寄存器 */
T0IR            EQU         0xE0004000
T0TCR           EQU         0xE0004004
T0TC            EQU         0xE0004008
T0PR            EQU         0xE000400C
T0PC            EQU         0xE0004010
T0MCR           EQU         0xE0004014
T0MR0           EQU         0xE0004018
T0MR1           EQU         0xE000401C
T0MR2           EQU         0xE0004020
T0MR3           EQU         0xE0004024
T0CCR           EQU         0xE0004028
T0CR0           EQU         0xE000402C
T0CR1           EQU         0xE0004030
T0CR2           EQU         0xE0004034
T0CR3           EQU         0xE0004038
T0EMR           EQU         0xE000403C
;// 为LPC213x增加定时器0计数控制器  2005-02-22
T0CTCR          EQU         0xE0004070
PWM0CON         EQU         0xE0004074          ;/* lpc2103 only */

;/* Timer 1 */
;/* 定时器1的特殊寄存器 */
T1IR            EQU         0xE0008000
T1TCR           EQU         0xE0008004
T1TC            EQU         0xE0008008
T1PR            EQU         0xE000800C
T1PC            EQU         0xE0008010
T1MCR           EQU         0xE0008014
T1MR0           EQU         0xE0008018
T1MR1           EQU         0xE000801C
T1MR2           EQU         0xE0008020
T1MR3           EQU         0xE0008024
T1CCR           EQU         0xE0008028
T1CR0           EQU         0xE000802C
T1CR1           EQU         0xE0008030
T1CR2           EQU         0xE0008034
T1CR3           EQU         0xE0008038
T1EMR           EQU         0xE000803C
;// 为LPC213x增加定时器1计数控制器  2005-02-22
T1CTCR          EQU         0xE0008070
PWM1CON         EQU         0xE0008074          ;/* lpc2103 only */

;/* Timer 2 */				/* lpc2103 only */
;/* 定时器2的特殊寄存器 */	/* lpc2103 only */
T2IR            EQU         0xE0070000          ;/* lpc2103 only */
T2TCR           EQU         0xE0070004          ;/* lpc2103 only */
T2TC            EQU         0xE0070008          ;/* lpc2103 only */
T2PR            EQU         0xE007000C          ;/* lpc2103 only */
T2PC            EQU         0xE0070010          ;/* lpc2103 only */
T2MCR           EQU         0xE0070014          ;/* lpc2103 only */
T2MR0           EQU         0xE0070018          ;/* lpc2103 only */
T2MR1           EQU         0xE007001C          ;/* lpc2103 only */
T2MR2           EQU         0xE0070020          ;/* lpc2103 only */
T2MR3           EQU         0xE0070024          ;/* lpc2103 only */
T2CCR           EQU         0xE0070028          ;/* lpc2103 only */
T2CR0           EQU         0xE007002C          ;/* lpc2103 only */
T2CR1           EQU         0xE0070030          ;/* lpc2103 only */
T2CR2           EQU         0xE0070034          ;/* lpc2103 only */
T2EMR           EQU         0xE007003C          ;/* lpc2103 only */
T2CTCR          EQU         0xE0070070          ;/* lpc2103 only */
PWM2CON         EQU         0xE0070074          ;/* lpc2103 only */

;/* Timer 3 */				/* lpc2103 only */
;/* 定时器3的特殊寄存器 */	/* lpc2103 only */
T3IR            EQU         0xE0074000          ;/* lpc2103 only */
T3TCR           EQU         0xE0074004          ;/* lpc2103 only */
T3TC            EQU         0xE0074008          ;/* lpc2103 only */
T3PR            EQU         0xE007400C          ;/* lpc2103 only */
T3PC            EQU         0xE0074010          ;/* lpc2103 only */
T3MCR           EQU         0xE0074014          ;/* lpc2103 only */
T3MR0           EQU         0xE0074018          ;/* lpc2103 only */
T3MR1           EQU         0xE007401C          ;/* lpc2103 only */
T3MR2           EQU         0xE0074020          ;/* lpc2103 only */
T3MR3           EQU         0xE0074024          ;/* lpc2103 only */
T3CCR           EQU         0xE0074028          ;/* lpc2103 only */
T3CR0           EQU         0xE007402C          ;/* lpc2103 only */
T3CR1           EQU         0xE0074030          ;/* lpc2103 only */
T3CR2           EQU         0xE0074034          ;/* lpc2103 only */
T3EMR           EQU         0xE007403C          ;/* lpc2103 only */
T3CTCR          EQU         0xE0074070          ;/* lpc2103 only */
PWM3CON         EQU         0xE0074074          ;/* lpc2103 only */

;/* Pulse Width Modulator (PWM) */
;/* 脉宽调制器的特殊寄存器 */
PWMIR           EQU         0xE0014000
PWMTCR          EQU         0xE0014004
PWMTC           EQU         0xE0014008
PWMPR           EQU         0xE001400C
PWMPC           EQU         0xE0014010
PWMMCR          EQU         0xE0014014
PWMMR0          EQU         0xE0014018
PWMMR1          EQU         0xE001401C
PWMMR2          EQU         0xE0014020
PWMMR3          EQU         0xE0014024
PWMMR4          EQU         0xE0014040
PWMMR5          EQU         0xE0014044
PWMMR6          EQU         0xE0014048
PWMPCR          EQU         0xE001404C
PWMLER          EQU         0xE0014050

;/* A/D CONVERTER */
;/* A/D转换器 */
ADCR            EQU         0xE0034000          ;/* no in lpc210x*/
ADDR            EQU         0xE0034004          ;/* no in lpc210x*/

;/* for lpc2138 */
AD0CR           EQU         0xE0034000      
AD0DR           EQU         0xE0034004      
ADGSM           EQU         0xE0034008      
AD1CR           EQU         0xE0060000      
AD1DR           EQU         0xE0060004      

;/* for lpc2103 */
AD0GDR			    EQU         0xE0034004	
AD0STAT			    EQU         0xE0034030	
AD0INTEN	    	EQU         0xE003400C	
AD0DR0		    	EQU         0xE0034010	
AD0DR1			    EQU         0xE0034014	
AD0DR2			    EQU         0xE0034018	
AD0DR3			    EQU         0xE003401C	
AD0DR4			    EQU         0xE0034020	
AD0DR5			    EQU         0xE0034024	
AD0DR6			    EQU         0xE0034028	
AD0DR7			    EQU         0xE003402C	

;/* D/A CONVERTER */
;/* D/A转换器 */
DACR  			    EQU         0xE006C000          ;/* lpc2138 only */

;/* Real Time Clock */
;/* 实时时钟的特殊寄存器 */
ILR            EQU          0xE0024000
CTC            EQU          0xE0024004
CCR            EQU          0xE0024008
CIIR           EQU          0xE002400C
AMR            EQU          0xE0024010
CTIME0         EQU          0xE0024014
CTIME1         EQU          0xE0024018
CTIME2         EQU          0xE002401C
SEC            EQU          0xE0024020
MIN            EQU          0xE0024024
HOUR           EQU          0xE0024028
DOM            EQU          0xE002402C
DOW            EQU          0xE0024030
DOY            EQU          0xE0024034
MONTH          EQU          0xE0024038
YEAR           EQU          0xE002403C
ALSEC          EQU          0xE0024060
ALMIN          EQU          0xE0024064
ALHOUR         EQU          0xE0024068
ALDOM          EQU          0xE002406C
ALDOW          EQU          0xE0024070
ALDOY          EQU          0xE0024074
ALMON          EQU          0xE0024078
ALYEAR         EQU          0xE002407C
PREINT         EQU          0xE0024080
PREFRAC        EQU          0xE0024084

;/* Watchdog */
;/* 看门狗的特殊寄存器 */
WDMOD          EQU          0xE0000000
WDTC           EQU          0xE0000004
WDFEED         EQU          0xE0000008
WDTV           EQU          0xE000000C

;/* Define firmware Functions Addr*/
;/* 定义固件函数地址 */
rm_init_entry               EQU         0x7fffff91
rm_undef_handler            EQU         0x7fffffa0
rm_prefetchabort_handler    EQU         0x7fffffb0
rm_dataabort_handler        EQU         0x7fffffc0
rm_irqhandler               EQU         0x7fffffd0
rm_irqhandler2              EQU         0x7fffffe0
iap_entry                   EQU         0x7ffffff1

    end
;/*********************************************************************************************************
;**                            End Of File
;********************************************************************************************************/
