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
#ifndef OTP_H_
#define OTP_H_

/**
 * @brief      This function serves to read IEEE address from OTP.
 * @param[out] buf  - Pointer to IEEE address buffer(IEEE address is 8bytes)
 * @return     none
 */
void otp_get_ieee_addr(unsigned char *buf);

/**
 * @brief      This function is used to calib ADC 1.2V vref.
 * @param[in]  none
 * @return     DRV_API_SUCCESS - the calibration value update, DRV_API_FAILURE - the calibration value is not update.
 */
drv_api_status_e otp_calib_adc_vref(void);


#endif /* OTP_H_ */
