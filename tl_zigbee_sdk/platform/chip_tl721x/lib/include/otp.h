/********************************************************************************************************
 * @file    otp.h
 *
 * @brief   This is the header file for TL721X
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/** @page OTP
 *
 *  Introduction
 *  ===============
 *  One Time Programmable (OTP)
 *
 *  API Reference
 *  ===============
 *  Header File: otp.h
 *
 *  How to use this driver
 *  ===============
 *   -# Power on OTP using the function otp_set_active_mode().
 *   -# OTP read/write, otp_read(), otp_write().
 *   -# OTP shutdown to save current using the function otp_set_deep_standby_mode().
 *
 */
#ifndef OTP_H_
#define OTP_H_

/**********************************************************************************************************************
 *                                                External interface                                                  *
 *********************************************************************************************************************/
/*!
 * @name External functions
 * @{
 */

/**
 * @brief      This function serves to init otp clk. This interface must be called to initialize the otp clock before using otp.
 * @return     none
 */
void otp_clk_init(void);

/**
 * @brief      This function serves to set otp active mode(auto/manual), if otp is in deep mode, need to operate on the otp, set active mode.
 * @return     none
 */
void otp_set_active_mode(void);

/**
 * @brief      This function serves to otp set deep standby mode, can enter deep to save current.
 * @return     none
 */
void otp_set_deep_standby_mode(void);

/**
 * @brief      This function serves to read data from OTP memory, belong to otp normal read.
 *             otp has three kinds of read mode,in general,just use OTP_READ normal read operation, when the execution of burning operation,
 *             need to use margin read(otp_pgm_margin_read,otp_initial_margin_read),check whether the write is successful.
 * @param[in]  addr - the otp address of the data,it has to be a multiple of 4,the otp memory that can access is from 0x0000-0x3FC,can't access other address.
 * @param[in]  word_len  - the length of the data,the unit is word(4 bytes).
 * @param[out] buff - data buff.
 * @return     none
 */
void otp_read(unsigned int addr, unsigned int word_len, unsigned int *buff);

/**
 * @brief      This function serves to write data to OTP memory.
 *             the minimum unit of otp read-write operation is 4 bytes, that is a word. meanwhile, the otp cannot be burned repeatedly,
 *             this function is limited to writing only once,this function will determine if the otp is 0xffffffff, and if it is 0xffffffff,
 *             it will write the otp.
 * @param[in]  addr - the address of the data,it has to be a multiple of 4,the OTP memory that can access is from 0x0000-0x3FC,can't access other address.
 * @param[in]  word_len  - the length of the data,the unit is word(4 bytes).
 * @param[out] buff - data buff.
 * @return
 *             - 0 it means that the otp operation area is 0xffffffff or the write data,
 *                return 0 not mean that the burning was successful,need to use three kinds of read mode to check whether the writing was successful.
 *             - 1 it means that there is an operation value in the operation area,it is not 0xffffffff or the write data,no burning action is performed.
 */
unsigned char otp_write(unsigned int addr, unsigned int word_len, unsigned int *buff);

/**
 * @brief      This function serves to read data from OTP memory,belong to otp pgm margin read.
 *             otp has three kinds of read mode,in general, just use OTP_READ normal read operation, when the execution of burning operation,
 *             need to use margin read(otp_pgm_margin_read,otp_initial_margin_read),check whether the write is successful.
 * @param[in]  addr - the otp address of the data,it has to be a multiple of 4,the otp memory that can access is from 0x0000-0x3FC,can't access other address.
 * @param[in]  word_len  - the length of the data,the unit is word(4 bytes).
 * @param[out] buff - data buff.
 * @return     none
 */
void otp_pgm_margin_read(unsigned int addr, unsigned int word_len, unsigned int *buff);

/**
 * @brief      This function serves to read data from OTP memory,belong to otp initial margin read.
 *             otp has three kinds of read mode,in general, just use OTP_READ normal read operation, when the execution of burning operation,
 *             need to use margin read(otp_pgm_margin_read,otp_initial_margin_read),check whether the write is successful.
 * @param[in]  addr - the otp address of the data,it has to be a multiple of 4,the otp memory that can access is from 0x0000-0x3FC,can't access other address.
 * @param[in]  word_len  - the length of the data,the unit is word(4 bytes).
 * @param[out] buff - data buff.
 * @return     none
 *
 */
void otp_initial_margin_read(unsigned int addr, unsigned int word_len, unsigned int *buff);

/**
 * @brief        This function serves to check protection code according SDK version.
 * @param[in]    sdk_version, 0:driver sdk  0xff:sdk_version_ignore
 * @return       none.
 */
void otp_check_protection_code(unsigned char sdk_version);

/**
 * @}
 */

#endif /* OTP_H_ */
