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



#ifdef WIN32
extern u16 my_random(void);
#endif



void generateRandomData(u8 *pData, u8 len)
{
	u8 i;
	for (i=0; i<2; i++) {
#ifdef WIN32
		*((u16*)pData) = (u16)my_random(); //randomT();
#else
		*pData = rand(); //randomT();
		*(pData + 1) = rand();
#endif

	}
	for (i=2; i<len; i+=2) {
#ifdef WIN32
		*((u16*)(pData + i)) = (u16)my_random(); //randomT();
#else
		*((u16*)(pData + i)) = (u16)rand(); //randomT();
#endif

	}
}
