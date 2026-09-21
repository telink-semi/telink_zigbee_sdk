/********************************************************************************************************
 * @file    bsp.c
 *
 * @brief   This is the source file for B85
 *
 * @author  Driver & Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
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
#include "bsp.h"
#include "analog.h"
#include "timer.h"

/**
 * @brief      This function performs a series of operations of writing digital or analog registers
 *             according to a command table
 * @param[in]  pt - pointer to a command table containing several writing commands
 * @param[in]  size  - number of commands in the table
 * @return     number of commands are carried out
 */
int LoadTblCmdSet(const TBLCMDSET *pt, int size)
{
    int l = 0;

    while (l < size) {
        unsigned int  cadr = ((unsigned int)0x800000) | pt[l].adr;
        unsigned char cdat = pt[l].dat;
        unsigned char ccmd = pt[l].cmd;
        unsigned char cvld = (ccmd & TCMD_UNDER_WR);
        ccmd &= TCMD_MASK;
        if (cvld) {
            if (ccmd == TCMD_WRITE) {
                write_reg8(cadr, cdat);
            } else if (ccmd == TCMD_WAREG) {
                analog_write(cadr, cdat);
            } else if (ccmd == TCMD_WAIT) {
                sleep_us(pt[l].adr * 256 + cdat);
            }
        }
        l++;
    }
    return size;
}

/**
 * @brief      This function writes a byte data to analog register
 * @param[in]  addr - the address of the analog register needs to write
 * @param[in]  value  - the data will be written to the analog register
 * @param[in]  e - the end address of value
 * @param[in]  s - the start address of the value
 * @return     none
 */
void sub_wr_ana(unsigned int addr, unsigned char value, unsigned char e, unsigned char s)
{
    unsigned char v, mask, tmp1, target, tmp2;

    v = analog_read(addr);
    mask = BIT_MASK_LEN(e - s + 1);
    tmp1 = value & mask;

    tmp2 = v & (~BIT_RNG(s, e));

    target = (tmp1 << s) | tmp2;
    analog_write(addr, target);
}

/**
 * @brief      This function writes a byte data to a specified analog register
 * @param[in]  addr - the address of the analog register needs to write
 * @param[in]  value  - the data will be written to the analog register
 * @param[in]  e - the end address of value
 * @param[in]  s - the start address of the value
 * @return     none
 */
void sub_wr(unsigned int addr, unsigned char value, unsigned char e, unsigned char s)
{
    unsigned char v, mask, tmp1, target, tmp2;

    v = read_reg8(addr);
    mask = BIT_MASK_LEN(e - s + 1);
    tmp1 = value & mask;

    tmp2 = v & (~BIT_RNG(s, e));

    target = (tmp1 << s) | tmp2;
    write_reg8(addr, target);
}
