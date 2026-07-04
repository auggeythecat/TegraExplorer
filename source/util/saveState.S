.section .text.saveState
.arm

#define MODE_SYS  0x1F
#define IRQ       0x80

.extern irq_handler
.type irq_handler, %function

_TEIRQHandler: /* TODO: harden IRQ handling to allow other events to pass through without exploding my console */
    MSR CPSR_c, #(MODE_SYS | IRQ) /* SYSTEM mode, IRQ disabled                          */
    LDR R12, =irq_handler         /* Load C IRQ handler address                         */
    MOV LR, PC                    /* Set valid return point in case the handler returns */
    BX R12                        /* Call the C IRQ handler (ARM/THUMB)                 */

#define EXCP_VEC_BASE      0x6000F000
#define EVP_COP_IRQ_VECTOR 0x218

.globl overrideIRQ
.type overrideIRQ, %function
overrideIRQ:
    LDR R0, =EXCP_VEC_BASE            /* Load Exception base address  */
    LDR R1, =_TEIRQHandler            /* Load ARM IRQ handler address */
    STR R1, [R0, #EVP_COP_IRQ_VECTOR] /* Store address to IRQ offset  */

    BX LR                             /* Return from subroutine       */

.globl saveState
.type saveState, %function
saveState: /* ARG1: saveState_t defined in 'timeout.h' */
    STMIA R0!, {r1-r12} /* Store all General Purpose registers in struct   */

    STR   SP , [R0]     /* Store Stack Pointer directly                    */
    ADD   R0 , #4       /* Increment pointer manually                      */
    STR   LR , [R0]     /* Store Link Register directly                    */
    ADD   R0 , #4       /* Increment pointer manually                      */
    MRS   R1 , CPSR     /* Load Current Program Status Register            */
    STR   R1 , [R0]     /* Store Current Program Status Register in struct */

    MOV   R0 , #0       /* Load 'success' value into return register       */
    BX LR               /* Return from subroutine                          */


.globl restoreState
.type restoreState, %function
restoreState:
    LDMIA R0!, {r1-r12} /* Load all General Purpose registers from struct   */

    LDR   SP , [R0]     /* Load Stack Pointer directly                      */
    ADD   R0 , #4       /* Increment pointer manually                       */
    LDR   LR , [R0]     /* Load Link Register directly                      */
    ADD   R0 , #4       /* Increment pointer manually                       */
    LDR   R0 , [R0]     /* Load Current Program Status Register temporarily */
    MSR   CPSR_cxsf, R0 /* Restore Current Program Status Register          */
    MOV   R0 , #1       /* Load 'failed' value into return register         */
    BX LR               /* Return from subroutine                           */
