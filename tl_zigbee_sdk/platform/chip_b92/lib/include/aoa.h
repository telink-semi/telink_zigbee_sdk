/********************************************************************************************************
 * @file    aoa.h
 *
 * @brief   This is the header file for B92
 *
 * @author  Driver Group
 * @date    2023
 *
 * @par     Copyright (c) 2023, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef AOA_H_
#define AOA_H_

typedef enum
{
    BYTE_NUM_2P5 = 0,
    BYTE_NUM_4   = 1,
} TypeDefByteNum;

/**
 * @brief       This function servers to find the average of a set of numbers
 * @param[in]   data - the  data.
 * @param[in]   bytenum   - the type of data.
 * @return      abs.
 */
unsigned int abs_value(unsigned int data, TypeDefByteNum bytenum);

/**
 * @brief       This function is used to convert 20bit to 8bit data
 * @param[in]   *data_src               - the ddr of data.
 * @param[in]   *data_has_amplitude     - the data with amplitude.
 * @param[in]   *data_no_amplitude      - the data without amplitude.
 * @return      none.
 */
void frond_end(unsigned char *data_src, unsigned char *data_has_amplitude, unsigned char *data_no_amplitude);

void rf_hadm_frond_end(unsigned char *data_src, int *data_has_amplitude, unsigned int len);

#endif /* DRIVERS_LIB_INCLUDE_AOA_H_ */
