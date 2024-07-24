/********************************************************************************************************
 * @file    aes_reg.h
 *
 * @brief   This is the header file for B92
 *
 * @author  Driver Group
 * @date    2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef _AES_REG_H_
#define _AES_REG_H_

#include "soc.h"

//the registers must be accessed by word
#define	aes_base_addr				(0x160000)

#define reg_aes_rwbtcntl           REG_ADDR32(aes_base_addr + 0x00)
enum{
	FLD_AES_SOFT_RESET		= BIT(26),
};

#define	reg_aes_irq_mask			REG_ADDR32(aes_base_addr + 0x0c)

#define reg_aes_irq_status			REG_ADDR32(aes_base_addr + 0x10)

#define reg_aes_clr_irq_status		REG_ADDR32(aes_base_addr + 0x18)
/**
 *  @brief  Define AES IRQ
 */
typedef enum{
	FLD_CRYPT_IRQ		= BIT(7),
}aes_irq_e;

#define reg_aes_mode     			REG_ADDR32(aes_base_addr + 0xb0)
enum{
	FLD_AES_START	=	BIT(0),
	FLD_AES_MODE	=	BIT(1),   /**< 0-cipher  1-decipher */
};

#define reg_aes_key(v)     			REG_ADDR32(aes_base_addr + 0xb4 + (v*4))	// v = 0 1 2 3

#define reg_aes_ptr     			REG_ADDR32(aes_base_addr + 0xc4)	//only the lower 16 bits are used.

#define reg_aes_rpase_cnt     		REG_ADDR32(aes_base_addr + 0x288)
enum{
	FLD_AES_PRAND				=	BIT_RNG(0, 23),
	FLD_AES_IRK_NUM				=	BIT_RNG(24, 27),
	FLD_AES_GEN_RES				=	BIT(29),   /**< W1C */
	FLD_AES_RPASE_START			=	BIT(30), /**< 0-idle 1-running 2-finished */
	FLD_AES_RPASE_EN			=	BIT(31),  /**< 0-unmatched 1-matched */
};

#define reg_aes_hash_status     	REG_ADDR32(aes_base_addr + 0x28c)
enum{
	FLD_AES_HASH_STA			=	BIT_RNG(0, 23),
	FLD_AES_IRK_CNT				=	BIT_RNG(24, 27),
	FLD_AES_RPASE_STA_CLR		=	BIT(28),   /**< W1C */
	FLD_AES_RPASE_STA			=	BIT_RNG(29, 30), /**< 0-idle 1-running 2-finished */
	FLD_AES_HASH_MATCH			=	BIT(31),  /**< 0-unmatched 1-matched */
};

#define reg_aes_irk_ptr     		REG_ADDR32(aes_base_addr + 0x290)	//only the lower 16 bits are used.

#define reg_embase_addr     		REG_ADDR32(0x170304)

#endif /* _AES_REG_H_ */
