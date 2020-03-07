/********************************************************************************************************
 * @file     utility.c
 *
 * @brief
 *
 * @author
 * @date     Oct. 8, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
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
 *
 *******************************************************************************************************/
#include "../tl_common.h"
#include "utility.h"



void generateRandomData(u8 *pData, u8 len)
{
	int i;
	unsigned int randNums = 0;
	/* if len is odd */
	for(i = 0; i < len; i++){
		if((i & 3) == 0){
			randNums = rand();
		}

		pData[i] = randNums & 0xff;
		randNums >>= 8;
	}
}
