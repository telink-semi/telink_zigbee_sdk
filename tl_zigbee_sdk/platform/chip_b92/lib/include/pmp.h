/********************************************************************************************************
 * @file    pmp.h
 *
 * @brief   This is the source file for B92
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
#ifndef __PMP_H__
#define __PMP_H__

/* Check platform support which PMP scheme!
 * CLIC platform only support NAPOT scheme! */

#define SCHEME_NAPOT      3
#define SCHEME_NA4        2
#define SCHEME_TOR        1
#define SCHEME_OFF        0

#define NAPOT(base, size) (unsigned long)(((size) > 0) ? ((((unsigned long)(base) & (~((unsigned long)(size) - 1))) >> 2) | (((unsigned long)(size) - 1) >> 3)) : 0)
#define TOR(top)          (unsigned long)((unsigned long)(top) >> 2)

/*  PMP Configuration Options
 *  *     7       6       5       4       3       2       1       0
 * +-------+-------+-------+-------+-------+-------+-------+-------+
 * |   L   |       0       |       A       |   X   |   W   |   R   | pmp#cfg
 * +-------+-------+-------+-------+-------+-------+-------+-------+
 */
#define PMP_L_OFF   0
#define PMP_L_ON    1
#define PMP_A_OFF   0
#define PMP_A_NA4   SCHEME_NA4
#define PMP_A_TOR   SCHEME_TOR
#define PMP_A_NAPOT SCHEME_NAPOT
#define PMP_X_OFF   0
#define PMP_X_ON    1
#define PMP_W_OFF   0
#define PMP_W_ON    1
#define PMP_R_OFF   0
#define PMP_R_ON    1

/**
   PMPCFG_LAXWR(l, a, x, w, r) is a macro used to generate the configuration value
   for a Physical Memory Protection (PMP) entry. It combines lock (l), address
   matching (a), execute (x), write (w), and read (r) permissions into a single
   configuration value to be written into a PMP configuration register.
 */
#define PMPCFG_LAXWR(l, a, x, w, r) (((l) << 7) | ((a) << 3) | ((x) << 2) | ((w) << 1) | ((r) << 0))


/* Supervisor CSR */
#define SSTATUS_SPP_MSK (1ULL << (8))
/**
 * @brief      This function serves to configure a PMP entry using the TOR (Top of Region) scheme.
 * @param[in]  entry    PMP entry number (0-7) to configure.
 * @param[in]  va       Virtual address representing the start of the address range.
 * @param[in]  pmpcfg   Configuration value for the PMP entry.
 * @return     None
 */
void pmp_tor_config(char entry, void *va, char pmpcfg);
/**
 * @brief      This function serves to configure a PMP entry using the NAPOT (Not A Power Of Two) scheme.
 * @param[in]  entry    PMP entry number (0-7) to configure.
 * @param[in]  va       Virtual address representing the start of the address range.
 * @param[in]  size     Size of the address range.
 * @param[in]  pmpcfg   Configuration value for the PMP entry.
 * @return     None
 */
void pmp_napot_config(char entry, void *va, unsigned long size, char pmpcfg);
#endif // __PMP_H__
