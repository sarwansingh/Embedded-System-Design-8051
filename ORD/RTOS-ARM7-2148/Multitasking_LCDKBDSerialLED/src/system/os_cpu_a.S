//********************************************************************************************************
//                                               uC/OS-II
//                                         The Real-Time Kernel
//
//                               (c) Copyright 1992-2005, Micrium, Weston, FL
//                                          All Rights Reserved
//
//                                           Generic ARM Port
//
// File      : OS_CPU_A.ASM
// Version   : V1.61
// By        : Jean J. Labrosse
//
// For       : ARM7 or ARM9
// Mode      : ARM or Thumb
// Toolchain : IAR's EWARM V4.11a and higher
//********************************************************************************************************
#define PUBLIC .global
/*            EXTERN  OSRunning                    // External references
            EXTERN  OSPrioCur
            EXTERN  OSPrioHighRdy
            EXTERN  OSTCBCur
            EXTERN  OSTCBHighRdy
            EXTERN  OSIntNesting
            EXTERN  OSIntExit
            EXTERN  OSTaskSwHook
            EXTERN  OS_CPU_IRQ_ISR_Handler
            EXTERN  OS_CPU_FIQ_ISR_Handler*/


            PUBLIC  OS_CPU_SR_Save               // Functions declared in this file
            PUBLIC  OS_CPU_SR_Restore
            PUBLIC  OSStartHighRdy
            PUBLIC  OSCtxSw
            PUBLIC  OSIntCtxSw
            PUBLIC  OS_CPU_IRQ_ISR
            PUBLIC  OS_CPU_FIQ_ISR


//NO_INT      EQU     0xC0                         // Mask used to disable interrupts (Both FIR and IRQ)
//SVC32_MODE  EQU     0x13
//FIQ32_MODE  EQU     0x11
//IRQ32_MODE  EQU     0x12
.equ NO_INT,        0xC0                         // Mask used to disable interrupts (Both FIR and IRQ)
.equ SVC32_MODE,    0x13
.equ FIQ32_MODE,    0x11
.equ IRQ32_MODE,    0x12


//*********************************************************************************************************
//                                   CRITICAL SECTION METHOD 3 FUNCTIONS
//
// Description: Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking you
//              would store the state of the interrupt disable flag in the local variable 'cpu_sr' and then
//              disable interrupts.  'cpu_sr' is allocated in all of uC/OS-II's functions that need to
//              disable interrupts.  You would restore the interrupt disable state by copying back 'cpu_sr'
//              into the CPU's status register.
//
// Prototypes :     OS_CPU_SR  OS_CPU_SR_Save(void)//
//                  void       OS_CPU_SR_Restore(OS_CPU_SR cpu_sr)//
//
//
// Note(s)    : 1) These functions are used in general like this:
//
//                 void Task (void *p_arg)
//                 {
//                 #if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
//                     OS_CPU_SR  cpu_sr//
//                 #endif
//
//                          :
//                          :
//                     OS_ENTER_CRITICAL()//             /* cpu_sr = OS_CPU_SaveSR()//                */
//                          :
//                          :
//                     OS_EXIT_CRITICAL()//              /* OS_CPU_RestoreSR(cpu_sr)//                */
//                          :
//                          :
//                 }
//
//              2) OS_CPU_SaveSR() is implemented as recommended by Atmel's application note:
//
//                    "Disabling Interrupts at Processor Level"
//*********************************************************************************************************

/*        RSEG CODE:CODE:NOROOT(2)
        CODE32*/

OS_CPU_SR_Save:
        MRS     R0,CPSR                     // Set IRQ and FIQ bits in CPSR to disable all interrupts
        ORR     R1,R0,#NO_INT
        MSR     CPSR_c,R1
        MRS     R1,CPSR                     // Confirm that CPSR contains the proper interrupt disable flags
        AND     R1,R1,#NO_INT
        CMP     R1,#NO_INT
        BNE     OS_CPU_SR_Save              // Not properly disabled (try again)
        BX      LR                          // Disabled, return the original CPSR contents in R0


OS_CPU_SR_Restore:
        MSR     CPSR_c,R0
        BX      LR


//*********************************************************************************************************
//                                          START MULTITASKING
//                                       void OSStartHighRdy(void)
//
// Note(s) : 1) OSStartHighRdy() MUST:
//              a) Call OSTaskSwHook() then,
//              b) Set OSRunning to TRUE,
//              c) Switch to the highest priority task.
//*********************************************************************************************************

        /*RSEG CODE:CODE:NOROOT(2)
        CODE32*/

OSStartHighRdy:

        LDR     R0, OS_TaskSwHook     // OSTaskSwHook()//
        MOV     LR, PC
        BX      R0

        MSR     CPSR_cxsf, #0xD3        // Switch to SVC mode with IRQ and FIQ disabled

        LDR     R4, OS_Running        // OSRunning = TRUE
        MOV     R5, #1
        STRB    R5, [R4]

                                        // SWITCH TO HIGHEST PRIORITY TASK
        LDR     R4, OS_TCBHighRdy     //    Get highest priority task TCB address
        LDR     R4, [R4]                //    get stack pointer
        LDR     SP, [R4]                //    switch to the new stack

        LDR     R4,  [SP], #4           //    pop new task's CPSR
        MSR     SPSR_cxsf,R4
        LDMFD   SP!, {R0-R12,LR,PC}^    //    pop new task's context


//*********************************************************************************************************
//                         PERFORM A CONTEXT SWITCH (From task level) - OSCtxSw()
//
// Note(s) : 1) OSCtxSw() is called in SYS mode with BOTH FIQ and IRQ interrupts DISABLED
//
//           2) The pseudo-code for OSCtxSw() is:
//              a) Save the current task's context onto the current task's stack
//              b) OSTCBCur->OSTCBStkPtr = SP//
//              c) OSTaskSwHook()//
//              d) OSPrioCur             = OSPrioHighRdy//
//              e) OSTCBCur              = OSTCBHighRdy//
//              f) SP                    = OSTCBHighRdy->OSTCBStkPtr//
//              g) Restore the new task's context from the new task's stack
//              h) Return to new task's code
//
//           3) Upon entry:
//              OSTCBCur      points to the OS_TCB of the task to suspend
//              OSTCBHighRdy  points to the OS_TCB of the task to resume
//*********************************************************************************************************
/*
        RSEG CODE:CODE:NOROOT(2)
        CODE32*/

OSCtxSw:
                                        // SAVE CURRENT TASK'S CONTEXT
        STMFD   SP!, {LR}               //     Push return address
        STMFD   SP!, {LR}
        STMFD   SP!, {R0-R12}           //     Push registers
        MRS     R4,  CPSR               //     Push current CPSR
        TST     LR, #1                  //     See if called from Thumb mode
        ORRNE   R4,  R4, #0x20          //     If yes, Set the T-bit
        STMFD   SP!, {R4}

        LDR     R4, OS_TCBCur         // OSTCBCur->OSTCBStkPtr = SP//
        LDR     R5, [R4]
        STR     SP, [R5]

        LDR     R0, OS_TaskSwHook     // OSTaskSwHook()//
        MOV     LR, PC
        BX      R0

        LDR     R4, OS_PrioCur        // OSPrioCur = OSPrioHighRdy
        LDR     R5, OS_PrioHighRdy
        LDRB    R6, [R5]
        STRB    R6, [R4]

        LDR     R4, OS_TCBCur         // OSTCBCur  = OSTCBHighRdy//
        LDR     R6, OS_TCBHighRdy
        LDR     R6, [R6]
        STR     R6, [R4]

        LDR     SP, [R6]                // SP = OSTCBHighRdy->OSTCBStkPtr//

                                        // RESTORE NEW TASK'S CONTEXT
        LDMFD   SP!, {R4}               //    Pop new task's CPSR
        MSR     SPSR_cxsf, R4

        LDMFD   SP!, {R0-R12,LR,PC}^    //    Pop new task's context


//*********************************************************************************************************
//                   PERFORM A CONTEXT SWITCH (From interrupt level) - OSIntCtxSw()
//
// Note(s) : 1) OSIntCtxSw() is called in SYS mode with BOTH FIQ and IRQ interrupts DISABLED
//
//           2) The pseudo-code for OSCtxSw() is:
//              a) OSTaskSwHook()//
//              b) OSPrioCur             = OSPrioHighRdy//
//              c) OSTCBCur              = OSTCBHighRdy//
//              d) SP                    = OSTCBHighRdy->OSTCBStkPtr//
//              e) Restore the new task's context from the new task's stack
//              f) Return to new task's code
//
//           3) Upon entry:
//              OSTCBCur      points to the OS_TCB of the task to suspend
//              OSTCBHighRdy  points to the OS_TCB of the task to resume
//*********************************************************************************************************

/*        RSEG CODE:CODE:NOROOT(2)
        CODE32
*/
OSIntCtxSw:
        LDR     R0, OS_TaskSwHook     // OSTaskSwHook()//
        MOV     LR, PC
        BX      R0

        LDR     R4,OS_PrioCur         // OSPrioCur = OSPrioHighRdy
        LDR     R5,OS_PrioHighRdy
        LDRB    R6,[R5]
        STRB    R6,[R4]

        LDR     R4,OS_TCBCur          // OSTCBCur  = OSTCBHighRdy//
        LDR     R6,OS_TCBHighRdy
        LDR     R6,[R6]
        STR     R6,[R4]

        LDR     SP,[R6]                 // SP = OSTCBHighRdy->OSTCBStkPtr//

                                        // RESTORE NEW TASK'S CONTEXT
        LDMFD   SP!, {R4}               //    Pop new task's CPSR
        MSR     SPSR_cxsf, R4

        LDMFD   SP!, {R0-R12,LR,PC}^    //    Pop new task's context


//*********************************************************************************************************
//                                      IRQ Interrupt Service Routine
//*********************************************************************************************************

/*        RSEG CODE:CODE:NOROOT(2)
        CODE32*/

OS_CPU_IRQ_ISR:

        STMFD   SP!, {R1-R3}                   // PUSH WORKING REGISTERS ONTO IRQ STACK

        MOV     R1, SP                         // Save   IRQ stack pointer

        ADD     SP, SP,#12                     // Adjust IRQ stack pointer

        SUB     R2, LR,#4                      // Adjust PC for return address to task

        MRS     R3, SPSR                       // Copy SPSR (i.e. interrupted task's CPSR) to R3

        MSR     CPSR_c, #(NO_INT | SVC32_MODE) // Change to SVC mode

                                               // SAVE TASK'S CONTEXT ONTO TASK'S STACK
        STMFD   SP!, {R2}                      //    Push task's Return PC
        STMFD   SP!, {LR}                      //    Push task's LR
        STMFD   SP!, {R4-R12}                  //    Push task's R12-R4

        LDMFD   R1!, {R4-R6}                   //    Move task's R1-R3 from IRQ stack to SVC stack
        STMFD   SP!, {R4-R6}
        STMFD   SP!, {R0}                      //    Push task's R0    onto task's stack
        STMFD   SP!, {R3}                      //    Push task's CPSR (i.e. IRQ's SPSR)

                                               // HANDLE NESTING COUNTER
        LDR     R0, OS_IntNesting            // OSIntNesting++;
        LDRB    R1, [R0]
        ADD     R1, R1,#1
        STRB    R1, [R0]

        CMP     R1, #1                         // if (OSIntNesting == 1) {
        BNE     OS_CPU_IRQ_ISR_1

        LDR     R4, OS_TCBCur                //     OSTCBCur->OSTCBStkPtr = SP
        LDR     R5, [R4]
        STR     SP, [R5]                       // }

OS_CPU_IRQ_ISR_1:
        MSR     CPSR_c, #(NO_INT | IRQ32_MODE) // Change to IRQ mode (to use the IRQ stack to handle interrupt)

        LDR     R0, OS_CPU_IRQ_ISR_HandlerP   // OS_CPU_IRQ_ISR_Handler()//
        MOV     LR, PC
        BX      R0

        MSR     CPSR_c, #(NO_INT | SVC32_MODE) // Change to SVC mode

        LDR     R0, OS_IntExit               // OSIntExit()//
        MOV     LR, PC
        BX      R0

                                               // RESTORE NEW TASK'S CONTEXT
        LDMFD   SP!, {R4}                      //    Pop new task's CPSR
        MSR     SPSR_cxsf, R4

        LDMFD   SP!, {R0-R12,LR,PC}^           //    Pop new task's context


//*********************************************************************************************************
//                                      FIQ Interrupt Service Routine
//*********************************************************************************************************
/*
        RSEG CODE:CODE:NOROOT(2)
        CODE32*/

OS_CPU_FIQ_ISR:

        STMFD   SP!, {R1-R3}                   // PUSH WORKING REGISTERS ONTO FIQ STACK

        MOV     R1, SP                         // Save   FIQ stack pointer

        ADD     SP, SP,#12                     // Adjust FIQ stack pointer

        SUB     R2, LR,#4                      // Adjust PC for return address to task

        MRS     R3, SPSR                       // Copy SPSR (i.e. interrupted task's CPSR) to R3

        MSR     CPSR_c, #(NO_INT | SVC32_MODE) // Change to SVC mode

                                               // SAVE TASK'S CONTEXT ONTO TASK'S STACK
        STMFD   SP!, {R2}                      //    Push task's Return PC
        STMFD   SP!, {LR}                      //    Push task's LR
        STMFD   SP!, {R4-R12}                  //    Push task's R12-R4

        LDMFD   R1!, {R4-R6}                   //    Move task's R1-R3 from FIQ stack to SVC stack
        STMFD   SP!, {R4-R6}
        STMFD   SP!, {R0}                      //    Push task's R0    onto task's stack
        STMFD   SP!, {R3}                      //    Push task's CPSR (i.e. FIQ's SPSR)

                                               // HANDLE NESTING COUNTER
        LDR     R0, OS_IntNesting            // OSIntNesting++//
        LDRB    R1, [R0]
        ADD     R1, R1,#1
        STRB    R1, [R0]

        CMP     R1, #1                         // if (OSIntNesting == 1) {
        BNE     OS_CPU_FIQ_ISR_1

        LDR     R4, OS_TCBCur                //     OSTCBCur->OSTCBStkPtr = SP
        LDR     R5, [R4]
        STR     SP, [R5]                       // }

OS_CPU_FIQ_ISR_1:
        MSR     CPSR_c, #(NO_INT | FIQ32_MODE) // Change to FIQ mode (to use the FIQ stack to handle interrupt)

        LDR     R0, OS_CPU_FIQ_ISR_HandlerP   // OS_CPU_FIQ_ISR_Handler()//
        MOV     LR, PC
        BX      R0

        MSR     CPSR_c, #(NO_INT | SVC32_MODE) // Change to SVC mode

        LDR     R0, OS_IntExit               // OSIntExit()//
        MOV     LR, PC
        BX      R0

                                               // RESTORE NEW TASK'S CONTEXT
        LDMFD   SP!, {R4}                      //    Pop new task's CPSR
        MSR     SPSR_cxsf, R4

        LDMFD   SP!, {R0-R12,LR,PC}^           //    Pop new task's context


//*********************************************************************************************************
//                                     POINTERS TO VARIABLES
//*********************************************************************************************************

//        DATA
#define DC32 .word
    .global OS_CPU_FIQ_ISR_Handler

OS_TaskSwHook:
      DC32    OSTaskSwHook

OS_CPU_IRQ_ISR_HandlerP:
      DC32    OS_CPU_IRQ_ISR_Handler

OS_CPU_FIQ_ISR_HandlerP:
      DC32    OS_CPU_FIQ_ISR_Handler

OS_IntExit:
      DC32    OSIntExit

OS_IntNesting:
      DC32    OSIntNesting

OS_PrioCur:
      DC32    OSPrioCur

OS_PrioHighRdy:
      DC32    OSPrioHighRdy

OS_Running:
      DC32    OSRunning

OS_TCBCur:
      DC32    OSTCBCur

OS_TCBHighRdy:
        DC32    OSTCBHighRdy

//        END
