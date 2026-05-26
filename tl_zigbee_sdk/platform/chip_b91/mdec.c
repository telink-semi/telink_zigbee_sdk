/********************************************************************************************************
 * @file    mdec.c
 *
 * @brief   This is the source file for B91
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "mdec.h"
#include "compiler.h"
#include "reg_include/mdec_reg.h"
#include "analog.h"
#include "clock.h"

/**
 * @brief       This function is used to initialize the MDEC module,include clock setting and input IO select.
 * @param[in]   pin - mdec pin.
 *                    In order to distinguish which pin the data is input from,only one input pin can be selected one time.
 * @return      none.
 */
void mdec_init(mdec_pin_e pin)
{
    analog_write_reg8(mdec_rst_addr, (analog_read_reg8(mdec_rst_addr) & (~FLD_CLS_MDEC)) | pin); //A0/B7/C4/D0/E0
}

/**
 * @brief       This function is used to read the receive data of MDEC module's IO.
 * @param[out]  dat     - The array to store date.
 * @return      1 decode success,  0 decode failure.
 */
unsigned char mdec_read_dat(unsigned char *dat)
{
    unsigned char m0, m1, m2, data_crc;

    dat[0] = analog_read_reg8(0x6a);
    dat[1] = analog_read_reg8(0x6b);
    dat[2] = analog_read_reg8(0x6c);
    dat[3] = analog_read_reg8(0x6d);
    dat[4] = analog_read_reg8(0x6e);

    m0       = ((dat[0] >> 5) << 4);
    m1       = dat[0] & 0x07;
    m2       = m0 + m1;
    data_crc = (((m2 + dat[1]) ^ dat[2]) + dat[3]) ^ 0xa5;

    if (data_crc == dat[4]) {
        return 1;
    }
    return 0;
}
