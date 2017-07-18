;/****************************************Copyright (c)**************************************************
;**                               Guangzou ZLG-MCU Development Co.,LTD.
;**                                      graduate school
;**                                 http://www.zlgmcu.com
;**
;**--------------File Info-------------------------------------------------------------------------------
;** File name: 			Startup.s
;** Last modified Date:	2004-09-17
;** Last Version: 		1.0
;** Descriptions: 		The start up codes for LPC2100, including the initializing codes for the entry point of exceptions and the stacks of user tasks.
;**						Every project should have a independent copy of this file for related modifications
;**------------------------------------------------------------------------------------------------------
;** Created by:			Chenmingji
;** Created date:		2004-02-02
;** Version:			1.0
;** Descriptions: 		The original version
;**
;**------------------------------------------------------------------------------------------------------
;** Modified by: 		Chenmingji
;** Modified date:		2004-09-17
;** Version:			1.01
;** Descriptions: 		Modified the bus setting to adapt for many common situations 
;**
;**------------------------------------------------------------------------------------------------------
;** Modified by: 		Chenmingji
;** Modified date:		2004-09-17
;** Version:			1.02
;** Descriptions: 		Added codes to support the enciphering of the chip
;**
;**------------------------------------------------------------------------------------------------------
;** Modified by: 		Chenmingji
;** Modified date:		2004-09-17
;** Version:			1.04
;** Descriptions: 		Renewed the template, added codes to support more compilers 
;**
;**------------------------------------------------------------------------------------------------------
;** Modified by: 		Chenxibing	
;** Modified date:		2004-12-09
;** Version:			1.05
;** Descriptions: 
;**
;********************************************************************************************************/

;define the stack size
;�����ջ�Ĵ�С
SVC_STACK_LEGTH         EQU         0
FIQ_STACK_LEGTH         EQU         0
IRQ_STACK_LEGTH         EQU         256
ABT_STACK_LEGTH         EQU         0
UND_STACK_LEGTH         EQU         0



NoInt       EQU 0x80
NoFIQ		EQU	0x40

USR32Mode   EQU 0x10
SVC32Mode   EQU 0x13
SYS32Mode   EQU 0x1f
IRQ32Mode   EQU 0x12
FIQ32Mode   EQU 0x11

    IMPORT __use_no_semihosting_swi

;The imported labels
;������ⲿ�����������
    IMPORT  FIQ_Exception                   ;Fast interrupt exceptions handler �����ж��쳣�������
    IMPORT  __main                          ;The entry point to the main function C������������� 
    IMPORT  TargetResetInit                 ;initialize the target board Ŀ��������ʼ��
	
;The emported labels
;���ⲿʹ�õı����������
    EXPORT  bottom_of_heap
    EXPORT  StackUsr
    
    EXPORT  Reset
    EXPORT __user_initial_stackheap

    ARM
    PRESERVE8
    AREA    vectors,CODE,READONLY


;interrupt vectors
;�ж�������
Reset
        LDR     PC, ResetAddr
        LDR     PC, UndefinedAddr
        LDR     PC, SWI_Addr
        LDR     PC, PrefetchAddr
        LDR     PC, DataAbortAddr
        DCD     0xb9205f80
        LDR     PC, [PC, #-0xff0]
        LDR     PC, FIQ_Addr

ResetAddr           DCD     ResetInit
UndefinedAddr       DCD     Undefined
SWI_Addr            DCD     SoftwareInterrupt
PrefetchAddr        DCD     PrefetchAbort
DataAbortAddr       DCD     DataAbort
Nouse               DCD     0
IRQ_Addr            DCD     0
FIQ_Addr            DCD     FIQ_Handler


;δ����ָ��
Undefined
        B       Undefined

;���ж�
SoftwareInterrupt			
;        B       SoftwareInterrupt
;//���ӿ�/���жϴ��� Chenxibing-2004-02-09                                 
        CMP     R0, #4
        LDRLO   PC, [PC, R0, LSL #2]
        MOVS    PC, LR

SwiFunction
        DCD     IRQDisable       ;0
        DCD     IRQEnable        ;1
        DCD		FIQDisable		 ;2
        DCD		FIQEnable		 ;3

IRQDisable
        ;��IRQ�ж�
        MRS     R0, SPSR
        ORR     R0, R0, #NoInt
        MSR     SPSR_c, R0
        MOVS    PC, LR

IRQEnable
        ;��IRQ�ж�
        MRS   R0, SPSR
        BIC   R0, R0, #NoInt
        MSR   SPSR_c, R0
        MOVS    PC, LR
        
FIQDisable
        ;��FIQ�ж�
        MRS     R0, SPSR
        ORR     R0, R0, #NoFIQ
        MSR     SPSR_c, R0
        MOVS    PC, LR

FIQEnable
        ;��FIQ�ж�
        MRS   R0, SPSR
        BIC   R0, R0, #NoFIQ
        MSR   SPSR_c, R0
        MOVS    PC, LR
;// Changed 2004-12-09
      
;ȡָ����ֹ
PrefetchAbort
        B       PrefetchAbort

;ȡ������ֹ
DataAbort
        B       DataAbort

;�����ж�
FIQ_Handler
        STMFD   SP!, {R0-R3, LR}
        BL      FIQ_Exception
        LDMFD   SP!, {R0-R3, LR}
        SUBS    PC,  LR,  #4

            
;/*********************************************************************************************************
;** unction name: 			InitStack
;** Descriptions: 			Initialize the stacks
;** input parameters:   	None
;** Returned value:  		None
;** Used global variables: 	None
;** Calling modules: 		None
;** 
;** Created by: 			Chenmingji
;** Created Date: 			2004/02/02
;**-------------------------------------------------------------------------------------------------------
;** Modified by:        	yuanguiyou
;** Modified date:        	2008/4/23  
;** reason:   				use "BX R0" to replace "MOV PC,R0"
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
InitStack    
        MOV     R0, LR
;Build the SVC stack
;���ù���ģʽ��ջ
        MSR     CPSR_c, #0xd3		
        LDR     SP, StackSvc	
;Build the IRQ stack	
;�����ж�ģʽ��ջ
        MSR     CPSR_c, #0xd2
        LDR     SP, StackIrq
;Build the FIQ stack
;���ÿ����ж�ģʽ��ջ
        MSR     CPSR_c, #0xd1
        LDR     SP, StackFiq
;Build the DATAABORT stack
;������ֹģʽ��ջ
        MSR     CPSR_c, #0xd7
        LDR     SP, StackAbt
;Build the UDF stack
;����δ����ģʽ��ջ
        MSR     CPSR_c, #0xdb
        LDR     SP, StackUnd
;Build the SYS stack
;����ϵͳģʽ��ջ
        MSR     CPSR_c, #0xdf
        LDR     SP, =StackUsr

        BX R0     ;MODIFY BY yuanguiyou 2008/4/23

;/*********************************************************************************************************
;** unction name: 			ResetInit
;** Descriptions: 			RESET
;** input parameters:   	None
;** Returned value:  		None
;** Used global variables: 	None
;** Calling modules: 		None
;** 
;** Created by: 			Chenmingji
;** Created Date: 			2004/02/02
;**-------------------------------------------------------------------------------------------------------
;** Modified by : 			Chenmingji
;** Modified date: 			2004/02/02
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
ResetInit
        
        BL      InitStack               ;Initialize the stack
        BL      TargetResetInit         ;Initialize the target board
                                        ;Jump to the entry point of C program
        B       __main

;/*********************************************************************************************************
;** unction name: 			__user_initial_stackheap
;** Descriptions: 			Initial the function library stacks and heaps, can not deleted!
;** input parameters:   	reference by function library
;** Returned value:  		reference by function library
;** Used global variables: 	None
;** Calling modules: 		None
;** 
;** Created by: 			Chenmingji
;** Created Date: 			2004/02/02
;**-------------------------------------------------------------------------------------------------------
;** Modified by:  			yuanguiyou	
;** Modified date: 			2008/04/23
;** reanson: 				talk the project to realview MDK	
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
      EXPORT  __user_initial_stackheap
__user_initial_stackheap
      LDR     R0, =  bottom_of_heap
      LDR     R1, =StackUsr
      LDR     R2, = (bottom_of_heap + HEAP_SIZE)
      LDR     R3, = (StackUsr- STACKUSER_SIZE )
      BX      LR

StackSvc           DCD     SvcStackSpace + (SVC_STACK_LEGTH - 1)* 4
StackIrq           DCD     IrqStackSpace + (IRQ_STACK_LEGTH - 1)* 4
StackFiq           DCD     FiqStackSpace + (FIQ_STACK_LEGTH - 1)* 4
StackAbt           DCD     AbtStackSpace + (ABT_STACK_LEGTH - 1)* 4
StackUnd           DCD     UndtStackSpace + (UND_STACK_LEGTH - 1)* 4

;/*********************************************************************************************************
;** unction name: 			CrpData
;** Descriptions: 			encrypt the chip
;** input parameters:   	None
;** Returned value:  		None
;** Used global variables: 	None
;** Calling modules: 		None
;** 
;** Created by: 			Chenmingji
;** Created Date: 			2004/03/27
;**-------------------------------------------------------------------------------------------------------
;** Modified by: 
;** Modified date: 
;**-------------------------------------------------------------------------------------------------------
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
    DCD     0x87654321          				;/*When the Data is Ϊ0x87654321,user code be protected.*/
    ENDIF
    
;/* �����ջ�ռ� */
        AREA    MyStacks, DATA, NOINIT, ALIGN=2
SvcStackSpace      SPACE   SVC_STACK_LEGTH * 4  ;Stack spaces for Administration Mode
IrqStackSpace      SPACE   IRQ_STACK_LEGTH * 4  ;Stack spaces for Interrupt ReQuest Mode
FiqStackSpace      SPACE   FIQ_STACK_LEGTH * 4  ;Stack spaces for Fast Interrupt reQuest Mode
AbtStackSpace      SPACE   ABT_STACK_LEGTH * 4  ;Stack spaces for Suspend Mode
UndtStackSpace     SPACE   UND_STACK_LEGTH * 4  ;Stack spaces for Undefined Mode


HEAP_SIZE       EQU   0x00000001
        AREA    Heap, DATA, NOINIT
bottom_of_heap    SPACE   HEAP_SIZE

STACKUSER_SIZE  EQU 0x00000000
        AREA    Stacks, DATA, NOINIT
StackUsr    SPACE    STACKUSER_SIZE   

    END
;/*********************************************************************************************************
;**                            End Of File
;********************************************************************************************************/
