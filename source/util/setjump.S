/* setjmp implementation, entirely stolen from newlib */
.globl setjump
.type setjump, %function
setjump:
	stmia	r0!, { r4-r10, fp, sp, lr }
	mov		r0, #0
    tst		lr, #1
	moveq	pc, lr
    .inst   0xe12fff1e	/* bx lr */
    b .

/* longjmp implementation, entirely stolen from newlib */
.globl lngjump
.type lngjump, %function
lngjump:
	ldmia	r0!, { r4-r10, fp, sp, lr }
	movs	r0, r1
	it		eq
	moveq	r0, #1
    tst		lr, #1
	moveq	pc, lr
    .inst   0xe12fff1e	/* bx lr */
    b .
