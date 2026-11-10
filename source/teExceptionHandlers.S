/*—————————————————————————————————————————————————————————————————————————————
 — Copyright (c) 2026 auggiethecat!                                           —
 —                                                                            —
 — All code was handwritten without the assistance of a Large Language Model. —
 —                                                                            —
 — This program is free software; you can redistribute it and/or modify it    —
 — under the terms and conditions of the GNU General Public License,          —
 — version 2, as published by the Free Software Foundation.                   —
 —                                                                            —
 — This program is distributed in the hope it will be useful, but WITHOUT     —
 — ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      —
 — FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for  —
 — more details.                                                              —
 —                                                                            —
 — You should have received a copy of the GNU General Public License          —
 — along with this program.  If not, see <http://www.gnu.org/licenses/>.      —
 —————————————————————————————————————————————————————————————————————————————*/

#define EXCEPTION_VECTOR_BASE 0x6000F000
# define EVP_COP_RESET_VECTOR          0x200
# define EVP_COP_UNDEFINED_VECTOR      0x204
# define EVP_COP_SOFTWARE_VECTOR       0x208
# define EVP_COP_PREFETCH_ABORT_VECTOR 0x20C
# define EVP_COP_DATA_ABORT_VECTOR     0x210
# define EVP_COP_RESERVED_VECTOR       0x214
# define EVP_COP_IRQ_VECTOR            0x218
# define EVP_COP_FIQ_VECTOR            0x21C

#define EXCEPTION_LR_ADDRESS     0x4003FF14
#define EXCEPTION_TYPE_ADDRESS   0x4003FF18
# define  RST 0x00545352
# define UNDF 0x46444e55
# define PABT 0x54424150
# define DABT 0x54424144
# define  WDT 0x00544457
#define EXCEPTION_ENABLE_ADDRESS 0x4003FF1C
# define EVP_            0x50564500
# define EXCEPTION_MAGIC EVP_

#define MODE_USR  0x10
#define MODE_FIQ  0x11
#define MODE_IRQ  0x12
#define MODE_SVC  0x13
#define MODE_ABT  0x17
#define MODE_UDF  0x1B
#define MODE_SYS  0x1F
#define MODE_MASK 0x1F

#define FIQ 0x40
#define IRQ 0x80

.globl irqEnableCpuIrqExceptions
.type irqEnableCpuIrqExceptions, %function
irqEnableCpuIrqExceptions:
	MRS R12, CPSR
	BIC R12, R12, #(IRQ | FIQ) /* IRQ/FIQ enabled */
	MSR CPSR, R12
	BX LR

.globl irqDisableCpuIrqExceptions
.type irqDisableCpuIrqExceptions, %function
irqDisableCpuIrqExceptions:
	MRS R12, CPSR
	ORR R12, R12, #(IRQ | FIQ) /* IRQ/FIQ disabled */
	MSR CPSR, R12
	BX LR

.extern iplMain
.type iplMain, %function

.extern fiq_setup
.type fiq_setup, %function

.globl _irqSetup
.type _irqSetup, %function
_irqSetup:
	MRS R0, CPSR
	BIC R0, R0, #MODE_MASK              /* Clear mode bits */
	ORR R0, R0, #(MODE_SVC | IRQ | FIQ) /* SUPERVISOR mode, IRQ/FIQ disabled */
	MSR CPSR, R0

	/* Setup IRQ stack pointer */
	MSR CPSR, #(MODE_IRQ | IRQ | FIQ) /* IRQ mode, IRQ/FIQ disabled */
	LDR SP, =0x40040000

	/* Setup FIQ stack pointer */
	MSR CPSR, #(MODE_FIQ | IRQ | FIQ) /* FIQ mode, IRQ/FIQ disabled */
	LDR SP, =0x40040000

	/* Setup SYS stack pointer */
	MSR CPSR, #(MODE_SYS | IRQ | FIQ) /* SYSTEM mode, IRQ/FIQ disabled */
	LDR SP, =0x4003FF00               /* This will be changed later    */

	MOV LR, PC
	BL setupVectors

	// TODO: FIQs
	/*BL fiq_setup*/

	/* Enable interrupts */
	BL irqEnableCpuIrqExceptions

    /* Branch back to program start */
	B iplMain

	/* Spinloop */
	B .

.extern irq_handler
.type irq_handler, %function

_irqHandler:
	MOV R13, R0    /* Save R0 in R13_IRQ */
	SUB R0, LR, #4 /* Put return address in R0_SYS */
	MOV LR, R1     /* Save R1 in R14_IRQ (LR) */
	MRS R1, SPSR   /* Put the SPSR in R1_SYS */

	MSR CPSR_c, #(MODE_SYS | IRQ)  /* SYSTEM mode, IRQ disabled */
	STMFD SP!, {R0, R1}            /* SPSR and PC */
	STMFD SP!, {R2-R3, R12, LR}    /* AAPCS-clobbered registers */
	MOV R0, SP                     /* Make SP_SYS visible to IRQ mode */
	SUB SP, SP, #8                 /* Make room for stacking R0 and R1 */

	MSR CPSR_c, #(MODE_IRQ | IRQ) /* IRQ mode, IRQ disabled */
	STMFD R0!, {R13, R14}         /* Finish saving the context (R0, R1) */

	MSR CPSR_c, #(MODE_SYS | IRQ) /* SYSTEM mode, IRQ disabled */
	LDR R12, =irq_handler
	MOV LR, PC                    /* Copy the return address to link register */
	BX R12                        /* Call the C IRQ handler (ARM/THUMB) */

	MSR CPSR_c, #(MODE_SYS | IRQ | FIQ) /* SYSTEM mode, IRQ/FIQ disabled */
	MOV R0, SP                          /* Make SP_SYS visible to IRQ mode */
	ADD SP, SP, #32                     /* Fake unstacking 8 registers from SP_SYS */

	MSR CPSR_c, #(MODE_IRQ | IRQ | FIQ) /* IRQ mode, IRQ/FIQ disabled */
	MOV SP, R0                          /* Copy SP_SYS to SP_IRQ */
	LDR R0, [SP, #28]                   /* Load the saved SPSR from the stack */
	MSR SPSR_cxsf, R0                   /* Copy it into SPSR_IRQ */

	LDMFD SP, {R0-R3, R12, LR}^ /* Unstack all saved USER/SYSTEM registers */
	NOP                         /* Cant access barked registers immediately */
	LDR LR, [SP, #24]           /* Load return address from the SYS stack */
	MOVS PC, LR                 /* Return restoring CPSR from SPSR */


.extern fiq_handler
.type fiq_handler, %function
_fiqHandler:
    BL fiq_handler

/* Compatability with BDK_WATCHDOG_FIQ_ENABLE + BDK_RESTART_BL_ON_WDT */
.globl excp_reset
.type excp_reset, %function
excp_reset:
    B exceptionReset

exceptionReset:
    LDR R0, =EXCEPTION_ENABLE_ADDRESS
    LDR R1, [R0]
    ADD R1, R1, #0x01
    AND R1, R1, #0xFF
    LDR R2, =EXCEPTION_MAGIC
    ORR R1, R1, R2
    STR R1, [R0]
    LDR R0, = EXCEPTION_LR_ADDRESS
    MOV R1, LR
    STR R1, [R0]
    LDR R0, =__bssStart
    EOR R1, R1, R1
    LDR R2, =__bssEnd
    SUB R2, R2, R0
    BL memset
    B _irqSetup

_resetHandler:
	LDR R0, =EXCEPTION_TYPE_ADDRESS
	LDR R1, =RST
	STR R1, [R0]
	B exceptionReset

_undefinedHandler:
	LDR R0, =EXCEPTION_TYPE_ADDRESS
	LDR R1, =UNDF
	STR R1, [R0]
	B exceptionReset

_prefetchAbortHandler:
	LDR R0, =EXCEPTION_TYPE_ADDRESS
	LDR R1, =PABT
	STR R1, [R0]
	B exceptionReset

_dataAbortHandler:
	LDR R0, =EXCEPTION_TYPE_ADDRESS
	LDR R1, =DABT
	STR R1, [R0]
	B exceptionReset

setupVectors:
	/* Setup vectors */
	LDR R0, =EXCEPTION_VECTOR_BASE

	LDR R1, =_resetHandler
	STR R1, [R0, #EVP_COP_RESET_VECTOR]

	LDR R1, =_undefinedHandler
	STR R1, [R0, #EVP_COP_UNDEFINED_VECTOR]

	LDR R1, =_resetHandler
	STR R1, [R0, #EVP_COP_SOFTWARE_VECTOR]

	LDR R1, =_prefetchAbortHandler
	STR R1, [R0, #EVP_COP_PREFETCH_ABORT_VECTOR]

	LDR R1, =_dataAbortHandler
	STR R1, [R0, #EVP_COP_DATA_ABORT_VECTOR]

	LDR R1, =_resetHandler
	STR R1, [R0, #EVP_COP_RESET_VECTOR]

	LDR R1, =_irqHandler
	STR R1, [R0, #EVP_COP_IRQ_VECTOR]

    /*
	LDR R1, =_fiqHandler
	STR R1, [R0, #EVP_COP_FIQ_VECTOR]
    */

	BX LR