/********************************************************************************************************
 * @file	div_mod.S
 *
 * @brief	This is the div & mod file for b85m
 *
 * @author	Driver Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#define 	UDIV		#0
#define 	SDIV		#1
#define 	UMOD		#2
#define 	SMOD		#3

	.code	16
	.text

	.align	2
	.global	__modsi3
	.code 16
	.thumb_func
	.type	__modsi3, %function
__modsi3:
	tmov	r2, SMOD
	tj	div
	.size	__modsi3, .-__modsi3

	.align	2
	.global	__divsi3
	.code 16
	.thumb_func
	.type	__divsi3, %function
__divsi3:
	tmov	r2, SDIV
	tj	div
	.size	__divsi3, .-__divsi3

	.align	2
	.global	__umodsi3
	.code 16
	.thumb_func
	.type	__umodsi3, %function
__umodsi3:
	tmov	r2, UMOD
	tj	div
	.size	__umodsi3, .-__umodsi3

	.align	2
	.global	__udivsi3
	.code 16
	.thumb_func
	.type	__udivsi3, %function
__udivsi3:
	tmov	r2, UDIV
	tj	div
	.size	__udivsi3, .-__udivsi3

	.align	2
	.global	div
	.code 16
	.thumb_func
	.type	div, %function
div:
	tmrcs	r3
	tpush	{r3, r4}
	tmov	r4, #0x80
	tor	r3, r4
	tmcsr	r3
	
	tloadr	r3, .L11
	tstorer	r0, [r3]
	tadd	r3, r3, #4
	tstorer	r1, [r3]
	tsub	r3, r3, #8
	tstorerb	r2, [r3]

.L2:
	tloadrb	r0, [r3]
	tcmp	r0, #0
	tjne	.L2
	tcmp	r2, #1
	tjls	.L4
	tadd	r3, r3, #8
	tloadr	r0, [r3]
	tj	.L6
.L4:
	tadd	r3, r3, #4
	tloadr	r0, [r3]

.L6:
	tpop	{r3, r4}
	tmcsr	r3
	tjex	lr

	.align	4
.L11:
	.word(0x800664)
	.word(0x800660)
	.word(0x800668)
	.size	div, .-div
