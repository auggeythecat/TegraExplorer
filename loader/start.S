/*
* Copyright (c) 2018 naehrwert
*
* This program is free software; you can redistribute it and/or modify it
* under the terms and conditions of the GNU General Public License,
* version 2, as published by the Free Software Foundation.
*
* This program is distributed in the hope it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

.section .text._start
.arm

.extern _relocIpl
.type _relocIpl, %function

.extern memset
.type memset, %function

.extern loaderMain
.type loaderMain, %function

.globl _start
.type _start, %function
_start:
	ADR R0, _start
	LDR R1, =__iplStart
	CMP R0, R1
	BEQ _realStart

	/* If we are not in the right location already, copy a relocator to upper IRAM. */
	ADR R2, _relocIpl
	LDR R3, =0x4003FF00
	MOV R4, #(_realStart - _relocIpl)
_copyLoop:
	LDMIA R2!, {R5}
	STMIA R3!, {R5}
	SUBS R4, #4
	BNE _copyLoop

	/* Use the relocator to copy ourselves into the right place. */
	LDR R2, =__iplEnd
	SUB R2, R2, R1
	LDR R3, =_realStart
	LDR R4, =0x4003FF00
	BX R4

_relocIpl:
	LDMIA R0!, {R4-R7}
	STMIA R1!, {R4-R7}
	SUBS R2, #0x10
	BNE _relocIpl
	/* Jump to the relocated entry. */
	BX R3

_realStart:
	/* Initially, we place our stack in IRAM but will move it to SDRAM later. */
	LDR SP, =0x40007000
	LDR R0, =__ldrEnd
	BL loaderMain
	B .
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0