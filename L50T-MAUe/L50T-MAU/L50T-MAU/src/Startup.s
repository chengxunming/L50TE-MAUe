;/****************************************Copyright (c)**************************************************
;**                               Guangzhou ZHIYUAN electronics Co.,LTD.
;**                                 http://www.zyinside.com
;**
;**--------------File Info-------------------------------------------------------------------------------
;** File name:          Startup.s
;** Last modified Date: 2006-02-22
;** Last Version:       1.0
;** Descriptions:       The start up codes for LPC2200, including the initializing codes for the entry point of exceptions and the stacks of user tasks.
;**             Every project should have a independent copy of this file for related modifications
;**------------------------------------------------------------------------------------------------------
;** Created by:			Chenmingji
;** Created date:		2005-11-22
;** Version: 			1.0
;** Descriptions:       The original version
;**
;**------------------------------------------------------------------------------------------------------
;** Modified by:       	zhangbin 
;** Modified date:		2006-6-28
;** Version:    		1.1
;** Descriptions: 		Clear VICVectAddr Register In ResetInit.
;**
;********************************************************************************************************/
RAM_SIZE            EQU     0x2000
IRQ_STAXK_SIZE      EQU     0

StackIRQ            EQU     (RAM_SIZE + 0X40000000)
StackUsr            EQU     (StackIRQ - IRQ_STAXK_SIZE * 4)


            INCLUDE     LPC2294.INC         ; Include the head file 引入头文件

NoInt       EQU 0x80
NoFIQ       EQU 0x40

UND32Mode   EQU 0x1b
USR32Mode   EQU 0x10
SVC32Mode   EQU 0x13
SYS32Mode   EQU 0x1f
IRQ32Mode   EQU 0x12
FIQ32Mode   EQU 0x11
TMode       EQU 0x20

;The exported labels
;给外部使用的标号在这声明
    
    EXPORT  Reset

    CODE32

    AREA    vectors,CODE,READONLY
        ENTRY
;interrupt vectors
;中断向量表
Reset
        LDR     PC, Reset_1
Reset_1 DCD     ResetInit
        B       .
        B       .
        B       .
        DCD     -(0xe51ff004 + ResetInit + 0xeafffffe * 3 + 0xe51ffff0 + 0xe25ef004)
        LDR     PC, [PC, #-0xff0]
        SUBS    PC, LR, #4

;********************************************************************************************************/
ResetInit
        MSR     CPSR_c, #(IRQ32Mode | NoInt | NoFIQ)
        LDR     SP, ResetInitData

        MSR     CPSR_c, #(SYS32Mode | NoInt | NoFIQ)
        LDR     SP, ResetInitData+4

		LDR		R0, VICVectAddrData
		MOV		R1, #0x00
		STR		R1, [R0]

        LDR     R1, ResetInitData+8
        BX      R1
ResetInitData
        DCD     StackIRQ
        DCD     StackUsr
        DCD     Main+1
        
VICVectAddrData
		DCD		VICVectAddr

;********************************************************************************************************/
    code16
Main
;add usr code
        B       Main


;********************************************************************************************************/
    IF :DEF: EN_CRP
        IF  . >= 0x1fc
        INFO    1,"\nThe data at 0x000001fc must be 0x87654321.\nPlease delete some source before this line."
        ENDIF
CrpData
    WHILE . < 0x1fc
    NOP
    WEND
CrpData1
    DCD     0x87654321          ;/*When the Data is 为0x87654321,user code be protected. 当此数为0x87654321时，用户程序被保护 */
    ENDIF       

    END
;/*********************************************************************************************************
;**                            End Of File
;********************************************************************************************************/
