/********************************************************************************************************
 * @file     factory_reset.h
 *
 * @brief	 Factory reset header
 *
 * @author
 * @date     Dec. 1, 2017
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#ifndef _FACTORY_RESET_H_
#define _FACTORY_RESET_H_

u8 factory_reset_handle(void);
u8 factory_reset_cnt_check(void);
void factory_reset(void);

#endif	/* _FACTORY_RESET_H_ */
