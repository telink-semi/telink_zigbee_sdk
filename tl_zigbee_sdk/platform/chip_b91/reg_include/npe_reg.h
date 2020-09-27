/********************************************************************************************************
 * @file     app.c
 * @brief    This is the source file for TLSR9518
 *
 * @author	 Driver Group
 * @date     April 1, 2020
 *
 * @par      Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 * @par      History:
 * 			 1.initial release(Mar. 16, 2020)
 *
 * @version  A001
 *
 *******************************************************************************************************/
#ifndef NPE_REG_H
#define NPE_REG_H
#include "../sys.h"
#include "bit.h"

#define  NPE_BASE_ADDR			         0x112000
#define reg_npe_addr(addr)		((addr)-CPU_DLM_BASE+DLM_BASE)
#define reg_npe_0x40			REG_ADDR32(NPE_BASE_ADDR+0x40)
#define reg_npe_0x6c			REG_ADDR32(NPE_BASE_ADDR+0x6c)
#define reg_npe_0x84			REG_ADDR32(NPE_BASE_ADDR+0x84)
#define reg_npe_0x74			REG_ADDR32(NPE_BASE_ADDR+0x74)
#define reg_npe_0x80			REG_ADDR32(NPE_BASE_ADDR+0x80)
#define reg_npe_0x84			REG_ADDR32(NPE_BASE_ADDR+0x84)


#define reg_npe_irq_0		    REG_ADDR8(STIMER_BASE_ADDR+0x20)
enum{

	FLD_IRQ_NPE_BUS0 			    =   BIT(0),
	FLD_IRQ_NPE_BUS1 			    =   BIT(1),
	FLD_IRQ_NPE_BUS2 			    =   BIT(2),
	FLD_IRQ_NPE_BUS3 			    =   BIT(3),
	FLD_IRQ_NPE_BUS4 			    =   BIT(4),
	FLD_IRQ_NPE_BUS7 			    =   BIT(7),

};

#define reg_npe_irq_1		    REG_ADDR8(STIMER_BASE_ADDR+0x21)
enum{

	FLD_IRQ_NPE_BUS8 			    =   BIT(0),
	FLD_IRQ_NPE_BUS13 			    =   BIT(5),
	FLD_IRQ_NPE_BUS14 			    =   BIT(6),
	FLD_IRQ_NPE_BUS15 			    =   BIT(7),
};

#define reg_npe_irq_2		    REG_ADDR8(STIMER_BASE_ADDR+0x22)
enum{

	FLD_IRQ_NPE_BUS17 			    =   BIT(1),
	FLD_IRQ_NPE_BUS21 			    =   BIT(5),
	FLD_IRQ_NPE_BUS22 			    =   BIT(6),
	FLD_IRQ_NPE_BUS23			    =   BIT(7),
};

#define reg_npe_irq_3		    REG_ADDR8(STIMER_BASE_ADDR+0x23)
enum{

	FLD_IRQ_NPE_BUS24 			    =   BIT(0),
	FLD_IRQ_NPE_BUS25 			    =   BIT(1),
	FLD_IRQ_NPE_BUS26 			    =   BIT(2),
	FLD_IRQ_NPE_BUS27			    =   BIT(3),
	FLD_IRQ_NPE_BUS28 			    =   BIT(4),
	FLD_IRQ_NPE_BUS29 			    =   BIT(5),
	FLD_IRQ_NPE_BUS30 			    =   BIT(6),
	FLD_IRQ_NPE_BUS31			    =   BIT(7),
};
#endif
