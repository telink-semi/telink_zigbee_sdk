/********************************************************************************************************
 * @file     platform.h
 *
 * @brief    This file includes all the modules drivers API
 *
 * @author
 * @date     May. 5, 2016
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
#pragma once


#if defined (MCU_CORE_826x)
#include "chip_826x/driver_826x.h"
#elif defined (MCU_CORE_8258)
#include "chip_8258/driver_8258.h"
#elif defined (MCU_CORE_8278)
#include "chip_8278/driver_8278.h"
#elif defined (MCU_CORE_B91)
#include "chip_b91/driver_b91.h"
#endif

