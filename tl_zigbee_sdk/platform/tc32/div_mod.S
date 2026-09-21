/********************************************************************************************************
 * @file    div_mod.S
 *
 * @brief   This is the div & mod file for b85m
 *
 * @author  Driver Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
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
    .size div, .-div


///////// asm crc24 function 2
    .section	.ram_code,"ax" //in ram code
    .align	2
    .global	blt_packet_crc24_opt
    .code 16
    .thumb_func
    .type	blt_packet_crc24_opt, %function
blt_packet_crc24_opt:
    tpush	{r3, r4, r5, r6, r7, lr}
    tmov	r5, r8
    tpush	{r5}
    tmov	r5, r1
    tneg	r1, r0
    tmov	r4, #3
    tand	r1, r4			//number of byte CRC of pre_process to align CRC to word boundary
    tsub	r5, r1
    tjge	CRC24_SAVE_WORD_NUM
    tadd	r1, r5
    tmov	r5, #0
CRC24_SAVE_WORD_NUM:
    tmov	r8, r5			//save to r8
    //tloadr	r3, CRC24_DAT
    tadd	r4, r0, #0
    tmov	r0, #0
    tmov	r7, #60			//r7 = 15 * 4
CRC24_BYTE_LOOP:			//r4: src; r6: dat; r2: crc; r5: tmp
    tcmp	r0, r1
    tjeq	CRC24_BYTE_END
    tloadrb	r6, [r4, r0]	//r6 = dat[r0]
    txor	r6, r2			//r6 = crc ^ dat
    tshftl	r5, r6, #2		//r5 = r6 << 2
    tand	r5, r7			//r2 = r2 & 60
    tloadr	r5, [r5, r3]	//load table
    tasr	r2, r2, #4		//r2 >> 4 (crc >> 4)
    tshftr	r6, r6, #2		// r6 = r6 >> 2
    txor	r2, r5			//r2 = r5 ^  r2
    tand	r6, r7			//r6 = r6 & 60

    tloadr	r6, [r6, r3]
    tasr	r2, r2, #4		//r2 >> 4 (crc >> 4)
    tadd	r0, #1
    txor	r2, r6			//r2 = r6 ^  r2
    tjne	CRC24_BYTE_LOOP
CRC24_BYTE_END:
    tmov	r1, r8
    tcmp	r1, #0
    tjeq	CRC24_END
    tmov	r5, #0
    tmov	r8, r5
    tadd	r4, r0
    tmov	r0, #0
CRC24_WORD_LOOP:
    tsub	r1, #4
    tjlt	CRC24_WORD_END
    tloadr	r0, [r4, #0]	//r0 = dat[r0]
    tadd	r4, #4
    tshftr	r6, r0, #0		// r6 = r0 >> 0
CRC24_WORD_nib0:
    txor	r6, r2			//r6 = crc ^ dat
    tshftl	r5, r6, #2		//r5 = r6 << 2
    tand	r5, r7			//r2 = r2 & 60
    tloadr	r5, [r5, r3]	//load table
    tasr	r2, r2, #4		//r2 >> 4 (crc >> 4)
    tshftr	r6, r6, #2		// r6 = r6 >> 2
    tand	r6, r7			//r6 = r6 & 60
    tloadr	r6, [r6, r3]
    txor	r2, r5			//r2 = r5 ^  r2
CRC24_WORD_nib1:
    tasr	r2, r2, #4		//r2 >> 4 (crc >> 4)
    tshftr	r5, r0, #8		//dat >> 8
    txor	r2, r6			//r2 = r6 ^  r2
CRC24_WORD_nib2:
    txor	r5, r2			//r6 = crc ^ dat
    tshftl	r6, r5, #2		//r5 << 2
    tand	r6, r7
    tloadr	r6, [r6, r3]
    tasr	r2, r2, #4		//r2 >> 4 (crc >> 4)
    tshftr	r5, r5, #2
    tand	r5, r7			//r6 = r6 & 60
    tloadr	r5, [r5, r3]
    txor	r2, r6			//r2 = r6 ^  r2
CRC24_WORD_nib3:
    tasr	r2, r2, #4		//r2 >> 4 (crc >> 4)
    tshftr	r6, r0, #16		//dat >> 8
    txor	r2, r5			//r2 = r6 ^  r2
CRC24_WORD_nib4:
    txor	r6, r2			//r6 = crc ^ dat
    tshftl	r5, r6, #2		//r5 = r6 << 2
    tand	r5, r7			//r2 = r2 & 60
    tloadr	r5, [r5, r3]	//load table
    tasr	r2, r2, #4		//r2 >> 4 (crc >> 4)
    tshftr	r6, r6, #2		// r6 = r6 >> 2
    tand	r6, r7			//r6 = r6 & 60
    tloadr	r6, [r6, r3]
    txor	r2, r5			//r2 = r5 ^  r2
CRC24_WORD_nib5:
    tasr	r2, r2, #4		//r2 >> 4 (crc >> 4)
    tshftr	r5, r0, #24		//dat >> 8
    txor	r2, r6			//r2 = r6 ^  r2
CRC24_WORD_nib6:
    txor	r5, r2			//r6 = crc ^ dat
    tshftl	r6, r5, #2		//r5 << 2
    tand	r6, r7
    tloadr	r6, [r6, r3]
    tasr	r2, r2, #4		//r2 >> 4 (crc >> 4)
    tshftr	r5, r5, #2
    tand	r5, r7			//r5 = r5 & 60
    tloadr	r5, [r5, r3]
    txor	r2, r6			//r2 = r6 ^  r2
CRC24_WORD_nib7:
    tasr	r2, r2, #4		//r2 >> 4 (crc >> 4)
    tmov	r0, #0
    txor	r2, r5			//r2 = r6 ^  r2
    tj		CRC24_WORD_LOOP
CRC24_WORD_END:
    tadd	r1, #4
    tj		CRC24_BYTE_LOOP
CRC24_END:
    tadd	r0, r2, #0
    tpop	{r5}
    tmov	r8, r5
    tpop	{r3, r4, r5, r6, r7, pc}
    tnop

