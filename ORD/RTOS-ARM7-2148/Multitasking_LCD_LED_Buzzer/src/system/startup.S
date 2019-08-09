
#define dc32 .word
 
	.equ	Top_Stack,	0x40008000

# Standard definitions of Mode bits and Interrupt (I & F) flags in PSRs

	.equ	Mode_USR,	0x10
	.equ	Mode_FIQ,	0x11
	.equ	Mode_IRQ,	0x12
	.equ	Mode_SVC,	0x13
	.equ	Mode_ABT,	0x17
	.equ	Mode_UND,	0x1B
	.equ	Mode_SYS,	0x1F

	.equ	I_Bit,	0x80	/* when I bit is set, IRQ is disabled */
	.equ	F_Bit,	0x40	/* when F bit is set, FIQ is disabled */

	.equ	UND_Stack_Size,	0x00000016
	.equ	SVC_Stack_Size,	0x00000200
	.equ	ABT_Stack_Size,	0x00000016
	.equ	FIQ_Stack_Size,	0x00000100
	.equ	IRQ_Stack_Size,	0x00000100
	.equ	USR_Stack_Size,	0x00000100

	.text
	.arm
	.global Vectors
		.global	undef_handler, swi_handler, prefetch_handler
//		.global	data_handler, irq_handler, fiq_handler

Vectors:
    B       CallMain
    B       Loop            /* undefined interrupt handler */
    B       Loop            /* software interrupt handler */
    B       Loop            /* pre-fetch interrupt handler */
    B       Loop            /* data abort interrupt handler */
    NOP                     /* Reserved Vector */
    LDR PC,       IrqHandlerAddr  /* IRQ handler */
    LDR PC,       FiqHandlerAddr  /* FIQ handler */

IrqHandlerAddr:         .word   OS_CPU_IRQ_ISR    /*IrqHandler*/
FiqHandlerAddr:         .word   OS_CPU_FIQ_ISR    /*FiqHandler*/

CallMain:
cstartup:

	LDR	R0, =Top_Stack

# Enter Undefined Instruction Mode and set its Stack Pointer
	MSR	CPSR_c, #Mode_UND|I_Bit|F_Bit
	MOV	SP, R0
	SUB	R0, R0, #UND_Stack_Size

# Enter Abort Mode and set its Stack Pointer
	MSR	CPSR_c, #Mode_ABT|I_Bit|F_Bit
	MOV	SP, R0
	SUB	R0, R0, #ABT_Stack_Size

# Enter FIQ Mode and set its Stack Pointer
	MSR	CPSR_c, #Mode_FIQ|I_Bit|F_Bit
	MOV	SP, R0
	SUB	R0, R0, #FIQ_Stack_Size

# Enter IRQ Mode and set its Stack Pointer
	MSR	CPSR_c, #Mode_IRQ|I_Bit|F_Bit
	MOV	SP, R0
	SUB	R0, R0, #IRQ_Stack_Size

# Enter Supervisor Mode and set its Stack Pointer
	MSR	CPSR_c, #Mode_SVC|I_Bit|F_Bit
	MOV	SP, R0
	SUB	R0, R0, #SVC_Stack_Size


	LDR	R1, =_etext
	LDR	R2, =_data
	LDR	R3, =_edata
LoopRel:CMP	R2, R3
	LDRLO	R0, [R1], #4
	STRLO	R0, [R2], #4
	BLO	LoopRel

# Clear .bss section (Zero init)
	MOV	R0, #0
	LDR	R1, =__bss_start__
	LDR	R2, =__bss_end__
LoopZI:	CMP	R1, R2
	STRLO	R0, [R1], #4
	BLO	LoopZI




#ifdef __ARMVFP__
// Enable the VFP coprocessor.
                mov     r0, #0x40000000                 // Set EN bit in VFP
                fmxr    fpexc, r0                       // FPEXC, clear others.

// Disable underflow exceptions by setting flush to zero mode.
// For full IEEE 754 underflow compliance this code should be removed
// and the appropriate exception handler installed.
                mov     r0, #0x01000000		        // Set FZ bit in VFP
                fmxr    fpscr, r0                       // FPSCR, clear others.
#endif

// Add more initialization here


// Continue to ?main for more IAR specific system startup

                ldr     r0,=main
                bx      r0
Loop:	B	Loop

//                LTORG

//                ENDMOD

//                END



