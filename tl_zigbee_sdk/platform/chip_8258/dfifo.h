/********************************************************************************************************
 * @file	dfifo.h
 *
 * @brief	This is the header file for B85
 *
 * @author	Driver & Zigbee Group
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
#ifndef 	DFIFO_H
#define 	DFIFO_H

#include "register.h"


/**
 * @brief      This function performs to enable audio input of DFIFO2.
 * @param[in]  none.
 * @return     none.
 */
static inline void dfifo_enable_dfifo2(void)
{
	reg_dfifo_mode |= FLD_AUD_DFIFO2_IN;
}

/**
 * @brief      This function performs to disable audio input of DFIFO2.
 * @param[in]  none.
 * @return     none.
 */
static inline void dfifo_disable_dfifo2(void)
{
	reg_dfifo_mode &= ~FLD_AUD_DFIFO2_IN;
}

/**
 * @brief      This function performs to start w/r data into/from DFIFO0.
 * @param[in]  pbuff - address in DFIFO0.
 * @param[in]  size_buff - depth of DFIFO0.
 * @return     none.
 */
static inline void dfifo_set_dfifo0(unsigned short* pbuff,unsigned int size_buff)
{
	reg_dfifo0_addr = (unsigned short)((unsigned int)pbuff);
	reg_dfifo0_size = (size_buff>>4)-1;
}

/**
 * @brief      This function performs to start w/r data into/from DFIFO1.
 * @param[in]  pbuff - address in DFIFO1.
 * @param[in]  size_buff - depth of DFIFO1.
 * @return     none.
 */
static inline void dfifo_set_dfifo1(unsigned short* pbuff,unsigned int size_buff)
{
	reg_dfifo1_addr = (unsigned short)((unsigned int)pbuff);
	reg_dfifo1_size = (size_buff>>4)-1;
}

/**
 * @brief      This function performs to start w/r data into/from DFIFO2.
 * @param[in]  pbuff - address in DFIFO2.
 * @param[in]  size_buff - depth of DFIFO2.
 * @return     none.
 */
static inline void dfifo_set_dfifo2(unsigned short* pbuff,unsigned int size_buff)
{
	reg_dfifo2_addr = (unsigned short)((unsigned int)pbuff);
	reg_dfifo2_size = (size_buff>>4)-1;
}

/**
 * @brief      This function performs to set MISC channel.
 * @param[in]  pbuff - address in FIFO2.
 * @param[in]  size_buff - depth of FIFO2.
 * @return     none.
 */
static inline void adc_config_misc_channel_buf(unsigned short* pbuff,unsigned int size_buff)
{
	reg_dfifo_misc_chn_addr = (unsigned short)((unsigned int)pbuff);
	reg_dfifo_misc_chn_size = (size_buff>>4)-1;

	reg_dfifo2_wptr = 0;  //clear dfifo2 write pointer
}

/**
 * @brief     configure the mic buffer's address and size
 * @param[in] pbuff - the first address of SRAM buffer to store MIC data.
 * @param[in] size_buff - the size of pbuff.
 * @return    none
 */
static inline void audio_config_mic_buf(unsigned short* pbuff,unsigned int size_buff)
{
	reg_dfifo_audio_addr = (unsigned short)((unsigned int)pbuff);
	reg_dfifo_audio_size = (size_buff>>4)-1;
}

#endif


