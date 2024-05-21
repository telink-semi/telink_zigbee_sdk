/********************************************************************************************************
 * @file    codec_0581_reg.h
 *
 * @brief   This is the header file for B92
 *
 * @author  Driver Group
 * @date    2023
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
#ifndef _CODEC_0581_REG_H_
#define _CODEC_0581_REG_H_


#define CODEC_EQ_PROGRAM_MEM_BASE            (0xA000)                /* start address of eq program memory */
#define CODEC_EQ_PARAM_MEM_BASE              (0xA200)                /* start address of eq param   memory */
#define CODEC_EQ_PARAM0_MEM_BASE             (0xA200)                /* start address of eq param0  memory */
#define CODEC_EQ_PARAM1_MEM_BASE             (0xA400)                /* start address of eq param1  memory */
#define CODEC_8BIT_REG_ADDR_START            (0xC000)                /* start address of 8-bit reg space   */
#define CODEC_8BIT_REG_ADDR_END              (0xCFFF)                /* end   address of 8-bit reg space   */
#define CODEC_FDSP_PROGRAM_MEM_BASE          (0xD000)                /* start address of fdsp code  memory */
#define CODEC_FDSP_PARAM_MEM_BASE            (0xD100)                /* start address of fdsp param memory */
#define CODEC_FDSP_PARAM_SPAN                (0x0100)                /* fdsp param len span */
#define CODEC_FDSP_PARAM_BANKA_BASE          (0xD100)                /* start address of fdsp param bank a */
#define CODEC_FDSP_PARAM_BANKB_BASE          (0xD600)                /* start address of fdsp param bank b */
#define CODEC_FDSP_PARAM_BANKC_BASE          (0xDB00)                /* start address of fdsp param bank c */
#define CODEC_FDSP_STATE_MEM_BASE            (0xE000)                /* start address of fdsp state memory */

/**
 * REG_XXX_ADDR: means codec_0581 register address
 * BF_XXX_INFO:  first  means codec_0581 register address
 *               second means bit field information, byte0 - start bit, byte1 - bits length
 * 
 * for example: #define REG_ADC_DAC_HP_PWR_ADDR 0x0000C004 (register address)
 *              #define BF_ADC2_EN_INFO         0x0000C004 (register address),0x00000102 (byte0-0x02:start bit2, byte1-0x01:bit length 1bit)
 *                                                                               BIT[2] : ADC Channel 2 Enable: 0-Powered Off,1-Powered On
 */

#define REG_VENDOR_ID_ADDR                  0x0000C000
#define BF_VENDOR_INFO                      0x0000C000, 0x00000800

#define REG_DEVICE_ID1_ADDR                 0x0000C001
#define BF_DEVICE1_INFO                     0x0000C001, 0x00000800

#define REG_DEVICE_ID2_ADDR                 0x0000C002
#define BF_DEVICE2_INFO                     0x0000C002, 0x00000800

#define REG_REVISION_ADDR                   0x0000C003
#define BF_REV_INFO                         0x0000C003, 0x00000800

#define REG_ADC_DAC_HP_PWR_ADDR             0x0000C004
#define BF_ADC0_EN_INFO                     0x0000C004, 0x00000100
#define BF_ADC1_EN_INFO                     0x0000C004, 0x00000101
#define BF_ADC2_EN_INFO                     0x0000C004, 0x00000102
#define BF_PB0_EN_INFO                      0x0000C004, 0x00000104

#define REG_PLL_PGA_PWR_ADDR                0x0000C005
#define BF_PLL_EN_INFO                      0x0000C005, 0x00000100
#define BF_RCOSC_PD_INFO                    0x0000C005, 0x00000101
#define BF_RCOSC_EN_INFO                    0x0000C005, 0x00000102
#define BF_PGA0_EN_INFO                     0x0000C005, 0x00000104
#define BF_PGA1_EN_INFO                     0x0000C005, 0x00000105
#define BF_PGA2_EN_INFO                     0x0000C005, 0x00000106
#define BF_MCLKIN_EN_INFO                   0x0000C005, 0x00000107

#define REG_SAI_CLK_PWR_ADDR                0x0000C006
#define BF_SPT0_IN_EN_INFO                  0x0000C006, 0x00000100
#define BF_SPT0_OUT_EN_INFO                 0x0000C006, 0x00000101

#define REG_DSP_PWR_ADDR                    0x0000C007
#define BF_FDSP_EN_INFO                     0x0000C007, 0x00000100

#define REG_ASRC_PWR_ADDR                   0x0000C008
#define BF_ASRCI0_EN_INFO                   0x0000C008, 0x00000100
#define BF_ASRCO0_EN_INFO                   0x0000C008, 0x00000104
#define BF_ASRCO1_EN_INFO                   0x0000C008, 0x00000105
#define BF_ASRCO2_EN_INFO                   0x0000C008, 0x00000106

#define REG_FINT_PWR_ADDR                   0x0000C009
#define BF_FINT0_EN_INFO                    0x0000C009, 0x00000100
#define BF_FINT1_EN_INFO                    0x0000C009, 0x00000101

#define REG_FDEC_PWR_ADDR                   0x0000C00A
#define BF_FDEC0_EN_INFO                    0x0000C00A, 0x00000100
#define BF_FDEC1_EN_INFO                    0x0000C00A, 0x00000101
#define BF_FDEC2_EN_INFO                    0x0000C00A, 0x00000102
#define BF_FDEC3_EN_INFO                    0x0000C00A, 0x00000103

#define REG_KEEPS_ADDR                      0x0000C00B
#define BF_KEEP_MEM_INFO                    0x0000C00B, 0x00000100

#define REG_CHIP_PWR_ADDR                   0x0000C00C
#define BF_POWER_EN_INFO                    0x0000C00C, 0x00000100
#define BF_MASTER_BLOCK_EN_INFO             0x0000C00C, 0x00000101
#define BF_CM_STARTUP_OVER_INFO             0x0000C00C, 0x00000102
#define BF_DLDO_CTRL_INFO                   0x0000C00C, 0x00000304

#define REG_CLK_CTRL1_ADDR                  0x0000C00E
#define BF_PLL_SOURCE_INFO                  0x0000C00E, 0x00000300
#define BF_PLL_TYPE_INFO                    0x0000C00E, 0x00000104
#define BF_PLL_FM_BYPASS_INFO               0x0000C00E, 0x00000105
#define BF_SYNC_SOURCE_INFO                 0x0000C00E, 0x00000206

#define REG_CLK_CTRL2_ADDR                  0x0000C00F
#define BF_PLL_INPUT_PRESCALER_INFO         0x0000C00F, 0x00000300

#define REG_CLK_CTRL3_ADDR                  0x0000C010
#define BF_PLL_INTEGER_DIVIDER_INFO         0x0000C010, 0x00000D00

#define REG_CLK_CTRL4_ADDR                  0x0000C011

#define REG_CLK_CTRL5_ADDR                  0x0000C012
#define BF_PLL_NUMERATOR_INFO               0x0000C012, 0x00001000

#define REG_CLK_CTRL6_ADDR                  0x0000C013

#define REG_CLK_CTRL7_ADDR                  0x0000C014
#define BF_PLL_DENOMINATOR_INFO             0x0000C014, 0x00001000

#define REG_CLK_CTRL8_ADDR                  0x0000C015

#define REG_CLK_CTRL9_ADDR                  0x0000C016
#define BF_PLL_UPDATE_INFO                  0x0000C016, 0x00000100

#define REG_CLK_CTRL10_ADDR                 0x0000C017
#define BF_ADC_FREQ_INFO                    0x0000C017, 0x00000100

#define REG_CLK_CTRL11_ADDR                 0x0000C018
#define BF_FREQ_MULT_EN_INFO                0x0000C018, 0x00000100

#define REG_ADC_CTRL1_ADDR                  0x0000C020
#define BF_ADC01_FS_INFO                    0x0000C020, 0x00000300
#define BF_ADC01_DEC_ORDER_INFO             0x0000C020, 0x00000103
#define BF_ADC2_FS_INFO                     0x0000C020, 0x00000304
#define BF_ADC2_DEC_ORDER_INFO              0x0000C020, 0x00000107

#define REG_ADC_CTRL2_ADDR                  0x0000C021
#define BF_ADC0_IBIAS_INFO                  0x0000C021, 0x00000300
#define BF_ADC1_IBIAS_INFO                  0x0000C021, 0x00000304

#define REG_ADC_CTRL3_ADDR                  0x0000C022
#define BF_ADC2_IBIAS_INFO                  0x0000C022, 0x00000300

#define REG_ADC_CTRL4_ADDR                  0x0000C023
#define BF_ADC0_HPF_EN_INFO                 0x0000C023, 0x00000200
#define BF_ADC1_HPF_EN_INFO                 0x0000C023, 0x00000202
#define BF_ADC2_HPF_EN_INFO                 0x0000C023, 0x00000204

#define REG_ADC_CTRL5_ADDR                  0x0000C024
#define BF_ADC01_FCOMP_INFO                 0x0000C024, 0x00000100
#define BF_ADC2_FCOMP_INFO                  0x0000C024, 0x00000101
#define BF_ADC_HARD_VOL_INFO                0x0000C024, 0x00000104
#define BF_ADC_VOL_LINK_INFO                0x0000C024, 0x00000105
#define BF_ADC_VOL_ZC_INFO                  0x0000C024, 0x00000106

#define REG_ADC_CTRL6_ADDR                  0x0000C025
#define BF_ADC_AIN_CHRG_TIME_INFO           0x0000C025, 0x00000400
#define BF_ADC_CHOP_EN_INFO                 0x0000C025, 0x00000105

#define REG_ADC_CTRL7_ADDR                  0x0000C026
#define BF_ADC_VOC_WKUP_INFO                0x0000C026, 0x00000100
#define BF_AIN0_INPUT_MODE_INFO             0x0000C026, 0x00000104
#define BF_AIN1_INPUT_MODE_INFO             0x0000C026, 0x00000105

#define REG_ADC_MUTES_ADDR                  0x0000C027
#define BF_ADC0_MUTE_INFO                   0x0000C027, 0x00000100
#define BF_ADC1_MUTE_INFO                   0x0000C027, 0x00000101
#define BF_ADC2_MUTE_INFO                   0x0000C027, 0x00000102

#define REG_ADC0_VOL_ADDR                   0x0000C028
#define BF_ADC0_VOL_INFO                    0x0000C028, 0x00000800

#define REG_ADC1_VOL_ADDR                   0x0000C029
#define BF_ADC1_VOL_INFO                    0x0000C029, 0x00000800

#define REG_ADC2_VOL_ADDR                   0x0000C02A
#define BF_ADC2_VOL_INFO                    0x0000C02A, 0x00000800

#define REG_ADC_DITHER_LEV_ADDR             0x0000C02B
#define BF_ADC_DITHER_LEV_INFO              0x0000C02B, 0x00000300

#define REG_PGA0_CTRL1_ADDR                 0x0000C030
#define BF_PGA0_GAIN_INFO                   0x0000C030, 0x00000B00
#define BF_PGA0_SLEW_DIS_INFO               0x0000C030, 0x00000107

#define REG_PGA0_CTRL2_ADDR                 0x0000C031

#define REG_PGA1_CTRL1_ADDR                 0x0000C032
#define BF_PGA1_GAIN_INFO                   0x0000C032, 0x00000B00
#define BF_PGA1_SLEW_DIS_INFO               0x0000C032, 0x00000107

#define REG_PGA1_CTRL2_ADDR                 0x0000C033

#define REG_PGA2_CTRL1_ADDR                 0x0000C034
#define BF_PGA2_GAIN_INFO                   0x0000C034, 0x00000B00
#define BF_PGA2_SLEW_DIS_INFO               0x0000C034, 0x00000107

#define REG_PGA2_CTRL2_ADDR                 0x0000C035

#define REG_PGA_CTRL1_ADDR                  0x0000C036
#define BF_PGA_SLEW_RATE_INFO               0x0000C036, 0x00000200
#define BF_PGA_GAIN_LINK_INFO               0x0000C036, 0x00000104

#define REG_PGA_CTRL2_ADDR                  0x0000C039
#define BF_PGA0_RIN_INFO                    0x0000C039, 0x00000100
#define BF_PGA1_RIN_INFO                    0x0000C039, 0x00000101
#define BF_PGA2_RIN_INFO                    0x0000C039, 0x00000102
#define BF_PGA0_POWER_MODE_INFO             0x0000C039, 0x00000104
#define BF_PGA1_POWER_MODE_INFO             0x0000C039, 0x00000105
#define BF_PGA2_POWER_MODE_INFO             0x0000C039, 0x00000106

#define REG_MBIAS_CTRL_ADDR                 0x0000C030
#define BF_MBIAS0_LEVEL_INFO                0x0000C030, 0x00000100
#define BF_MBIAS1_LEVEL_INFO                0x0000C030, 0x00000101
#define BF_MBIAS_IBIAS_INFO                 0x0000C030, 0x00000204

#define REG_DMIC_CTRL1_ADDR                 0x0000C040
#define BF_DMIC_CLK0_RATE_INFO              0x0000C040, 0x00000300
#define BF_DMIC_CLK1_RATE_INFO              0x0000C040, 0x00000304

#define REG_DMIC_CTRL2_ADDR                 0x0000C041
#define BF_DMIC01_FS_INFO                   0x0000C041, 0x00000300
#define BF_DMIC01_HPF_EN_INFO               0x0000C041, 0x00000103
#define BF_DMIC01_DEC_ORDER_INFO            0x0000C041, 0x00000104
#define BF_DMIC01_FCOMP_INFO                0x0000C041, 0x00000105
#define BF_DMIC01_EDGE_INFO                 0x0000C041, 0x00000106
#define BF_DMIC01_MAP_INFO                  0x0000C041, 0x00000107

#define REG_DMIC_CTRL3_ADDR                 0x0000C042
#define BF_DMIC23_FS_INFO                   0x0000C042, 0x00000300
#define BF_DMIC23_HPF_EN_INFO               0x0000C042, 0x00000103
#define BF_DMIC23_DEC_ORDER_INFO            0x0000C042, 0x00000104
#define BF_DMIC23_FCOMP_INFO                0x0000C042, 0x00000105
#define BF_DMIC23_EDGE_INFO                 0x0000C042, 0x00000106
#define BF_DMIC23_MAP_INFO                  0x0000C042, 0x00000107

#define REG_DMIC_CTRL4_ADDR                 0x0000C043
#define BF_DMIC45_FS_INFO                   0x0000C043, 0x00000300
#define BF_DMIC45_HPF_EN_INFO               0x0000C043, 0x00000103
#define BF_DMIC45_DEC_ORDER_INFO            0x0000C043, 0x00000104
#define BF_DMIC45_FCOMP_INFO                0x0000C043, 0x00000105
#define BF_DMIC45_EDGE_INFO                 0x0000C043, 0x00000106
#define BF_DMIC45_MAP_INFO                  0x0000C043, 0x00000107

#define REG_DMIC_CTRL5_ADDR                 0x0000C044
#define BF_DMIC67_FS_INFO                   0x0000C044, 0x00000300
#define BF_DMIC67_HPF_EN_INFO               0x0000C044, 0x00000103
#define BF_DMIC67_DEC_ORDER_INFO            0x0000C044, 0x00000104
#define BF_DMIC67_FCOMP_INFO                0x0000C044, 0x00000105
#define BF_DMIC67_EDGE_INFO                 0x0000C044, 0x00000106
#define BF_DMIC67_MAP_INFO                  0x0000C044, 0x00000107

#define REG_DMIC_CTRL6_ADDR                 0x0000C045
#define BF_DMIC_HARD_VOL_INFO               0x0000C045, 0x00000100
#define BF_DMIC_VOL_LINK_INFO               0x0000C045, 0x00000101
#define BF_DMIC_VOL_ZC_INFO                 0x0000C045, 0x00000102

#define REG_DMIC_MUTES_ADDR                 0x0000C046
#define BF_DMIC0_MUTE_INFO                  0x0000C046, 0x00000100
#define BF_DMIC1_MUTE_INFO                  0x0000C046, 0x00000101
#define BF_DMIC2_MUTE_INFO                  0x0000C046, 0x00000102
#define BF_DMIC3_MUTE_INFO                  0x0000C046, 0x00000103
#define BF_DMIC4_MUTE_INFO                  0x0000C046, 0x00000104
#define BF_DMIC5_MUTE_INFO                  0x0000C046, 0x00000105
#define BF_DMIC6_MUTE_INFO                  0x0000C046, 0x00000106
#define BF_DMIC7_MUTE_INFO                  0x0000C046, 0x00000107

#define REG_DMIC_VOL0_ADDR                  0x0000C047
#define BF_DMIC0_VOL_INFO                   0x0000C047, 0x00000800

#define REG_DMIC_VOL1_ADDR                  0x0000C048
#define BF_DMIC1_VOL_INFO                   0x0000C048, 0x00000800

#define REG_DMIC_VOL2_ADDR                  0x0000C049
#define BF_DMIC2_VOL_INFO                   0x0000C049, 0x00000800

#define REG_DMIC_VOL3_ADDR                  0x0000C04A
#define BF_DMIC3_VOL_INFO                   0x0000C04A, 0x00000800

#define REG_DMIC_VOL4_ADDR                  0x0000C04B
#define BF_DMIC4_VOL_INFO                   0x0000C04B, 0x00000800

#define REG_DMIC_VOL5_ADDR                  0x0000C04C
#define BF_DMIC5_VOL_INFO                   0x0000C04C, 0x00000800

#define REG_DMIC_VOL6_ADDR                  0x0000C04D
#define BF_DMIC6_VOL_INFO                   0x0000C04D, 0x00000800

#define REG_DMIC_VOL7_ADDR                  0x0000C04E
#define BF_DMIC7_VOL_INFO                   0x0000C04E, 0x00000800

#define REG_EQ_CFG_ADDR                     0x0000C040
#define BF_EQ_RUN_INFO                      0x0000C040, 0x00000100
#define BF_EQ_CLEAR_INFO                    0x0000C040, 0x00000101
#define BF_EQ_BANK_SEL_INFO                 0x0000C040, 0x00000102

#define REG_EQ_ROUTE_ADDR                   0x0000C044
#define BF_EQ_ROUTE_INFO                    0x0000C044, 0x00000700

#define REG_DAC_CTRL1_ADDR                  0x0000C050
#define BF_DAC_FS_INFO                      0x0000C050, 0x00000300
#define BF_DAC_FCOMP_INFO                   0x0000C050, 0x00000103
#define BF_DAC_LPM_INFO                     0x0000C050, 0x00000106
#define BF_DAC_MORE_FILT_INFO               0x0000C050, 0x00000107

#define REG_DAC_CTRL2_ADDR                  0x0000C051
#define BF_DAC_HARD_VOL_INFO                0x0000C051, 0x00000101
#define BF_DAC_VOL_ZC_INFO                  0x0000C051, 0x00000102
#define BF_DAC_LPM_II_INFO                  0x0000C051, 0x00000103
#define BF_DAC0_HPF_EN_INFO                 0x0000C051, 0x00000104
#define BF_DAC0_MUTE_INFO                   0x0000C051, 0x00000106
#define BF_DAC0_FORCE_MUTE_INFO             0x0000C051, 0x00000107

#define REG_DAC_VOL0_ADDR                   0x0000C052
#define BF_DAC0_VOL_INFO                    0x0000C052, 0x00000800

#define REG_DAC_VOL1_ADDR                   0x0000C053

#define REG_DAC_ROUTE0_ADDR                 0x0000C054
#define BF_DAC0_ROUTE_INFO                  0x0000C054, 0x00000700

#define REG_HP_CTRL_ADDR                    0x0000C060
#define BF_HP0_MODE_INFO                    0x0000C060, 0x00000100

#define REG_HP_LVMODE_CTRL1_ADDR            0x0000C061
#define BF_HP_LVMODE_EN_INFO                0x0000C061, 0x00000100
#define BF_HP_LVMODE_CM_EN_INFO             0x0000C061, 0x00000101

#define REG_HP_LVMODE_CTRL2_ADDR            0x0000C062
#define BF_HP_LVMODE_AUTOSW_MODE_INFO       0x0000C062, 0x00000100
#define BF_HP_LVMODE_AUTO_DLY_INFO          0x0000C062, 0x00000301
#define BF_HP_LVMODE_CM_DLY_INFO            0x0000C062, 0x00000204

#define REG_HP_LVMODE_CTRL3_ADDR            0x0000C063
#define BF_HP_LVMODE_SWITCH_INFO            0x0000C063, 0x00000100

#define REG_HPLDO_CTRL_ADDR                 0x0000C066
#define BF_HPLDO_EN_INFO                    0x0000C066, 0x00000100
#define BF_HPLDO_BYPASS_INFO                0x0000C066, 0x00000101
#define BF_HPLDO_VOUT_SET_INFO              0x0000C066, 0x00000202

#define REG_PB_CTRL_ADDR                    0x0000C06F
#define BF_PB_POWER_CTRL_INFO               0x0000C06F, 0x00000200

#define REG_FDEC_CTRL1_ADDR                 0x0000C070
#define BF_FDEC01_IN_FS_INFO                0x0000C070, 0x00000300
#define BF_FDEC01_OUT_FS_INFO               0x0000C070, 0x00000404

#define REG_FDEC_CTRL2_ADDR                 0x0000C071
#define BF_FDEC23_IN_FS_INFO                0x0000C071, 0x00000300
#define BF_FDEC23_OUT_FS_INFO               0x0000C071, 0x00000404

#define REG_FDEC_ROUTE0_ADDR                0x0000C074
#define BF_FDEC0_ROUTE_INFO                 0x0000C074, 0x00000600

#define REG_FDEC_ROUTE1_ADDR                0x0000C075
#define BF_FDEC1_ROUTE_INFO                 0x0000C075, 0x00000600

#define REG_FDEC_ROUTE2_ADDR                0x0000C076
#define BF_FDEC2_ROUTE_INFO                 0x0000C076, 0x00000600

#define REG_FDEC_ROUTE3_ADDR                0x0000C077
#define BF_FDEC3_ROUTE_INFO                 0x0000C077, 0x00000600

#define REG_FINT_CTRL1_ADDR                 0x0000C080
#define BF_FINT01_IN_FS_INFO                0x0000C080, 0x00000400
#define BF_FINT01_OUT_FS_INFO               0x0000C080, 0x00000304

#define REG_FINT_ROUTE0_ADDR                0x0000C084
#define BF_FINT0_ROUTE_INFO                 0x0000C084, 0x00000700

#define REG_FINT_ROUTE1_ADDR                0x0000C085
#define BF_FINT1_ROUTE_INFO                 0x0000C085, 0x00000700

#define REG_ASRCI_CTRL_ADDR                 0x0000C090
#define BF_ASRCI_OUT_FS_INFO                0x0000C090, 0x00000300
#define BF_ASRCI_LPM_II_INFO                0x0000C090, 0x00000103
#define BF_ASRCI_OSCCAL_LOCK_INFO           0x0000C090, 0x00000104
#define BF_ASRCI_LPM_INFO                   0x0000C090, 0x00000105
#define BF_ASRCI_VFILT_INFO                 0x0000C090, 0x00000106
#define BF_ASRCI_MORE_FILT_INFO             0x0000C090, 0x00000107

#define REG_ASRCI_ROUTE01_ADDR              0x0000C091
#define BF_ASRCI0_ROUTE_INFO                0x0000C091, 0x00000400

#define REG_ASRCO_CTRL_ADDR                 0x0000C093
#define BF_ASRCO_IN_FS_INFO                 0x0000C093, 0x00000300
#define BF_ASRCO_LPM_II_INFO                0x0000C093, 0x00000103
#define BF_ASRCO_OSCCAL_LOCK_INFO           0x0000C093, 0x00000104
#define BF_ASRCO_LPM_INFO                   0x0000C093, 0x00000105
#define BF_ASRCO_VFILT_INFO                 0x0000C093, 0x00000106
#define BF_ASRCO_MORE_FILT_INFO             0x0000C093, 0x00000107

#define REG_ASRCO_ROUTE0_ADDR               0x0000C094
#define BF_ASRCO0_ROUTE_INFO                0x0000C094, 0x00000600

#define REG_ASRCO_ROUTE1_ADDR               0x0000C095
#define BF_ASRCO1_ROUTE_INFO                0x0000C095, 0x00000600

#define REG_ASRCO_ROUTE2_ADDR               0x0000C096
#define BF_ASRCO2_ROUTE_INFO                0x0000C096, 0x00000600

#define REG_FDSP_RUN_ADDR                   0x0000C0A0
#define BF_FDSP_RUN_INFO                    0x0000C0A0, 0x00000100

#define REG_FDSP_CTRL1_ADDR                 0x0000C0A1
#define BF_FDSP_BANK_SEL_INFO               0x0000C0A1, 0x00000200
#define BF_FDSP_RAMP_MODE_INFO              0x0000C0A1, 0x00000102
#define BF_FDSP_ZERO_STATE_INFO             0x0000C0A1, 0x00000103
#define BF_FDSP_RAMP_RATE_INFO              0x0000C0A1, 0x00000404

#define REG_FDSP_CTRL2_ADDR                 0x0000C0A2
#define BF_FDSP_LAMBDA_INFO                 0x0000C0A2, 0x00000600

#define REG_FDSP_CTRL3_ADDR                 0x0000C0A3
#define BF_FDSP_COPY_AB_INFO                0x0000C0A3, 0x00000100
#define BF_FDSP_COPY_AC_INFO                0x0000C0A3, 0x00000101
#define BF_FDSP_COPY_BA_INFO                0x0000C0A3, 0x00000102
#define BF_FDSP_COPY_BC_INFO                0x0000C0A3, 0x00000103
#define BF_FDSP_COPY_CA_INFO                0x0000C0A3, 0x00000104
#define BF_FDSP_COPY_CB_INFO                0x0000C0A3, 0x00000105
#define BF_FDSP_COPY_DONE_CLEAR_INFO        0x0000C0A3, 0x00000107

#define REG_FDSP_CTRL4_ADDR                 0x0000C0A4
#define BF_FDSP_RATE_SOURCE_INFO            0x0000C0A4, 0x00000400
#define BF_FDSP_EXP_ATK_SPEED_INFO          0x0000C0A4, 0x00000104
#define BF_FDSP_ONZ_SOURCE_INFO             0x0000C0A4, 0x00000105

#define REG_FDSP_CTRL5_ADDR                 0x0000C0A5
#define BF_FDSP_RATE_DIV_INFO               0x0000C0A5, 0x00001000

#define REG_FDSP_CTRL6_ADDR                 0x0000C0A6

#define REG_FDSP_CTRL7_ADDR                 0x0000C0A7
#define BF_FDSP_MOD_N_INFO                  0x0000C0A7, 0x00000600

#define REG_FDSP_CTRL8_ADDR                 0x0000C0A8
#define BF_FDSP_REG_COND0_INFO              0x0000C0A8, 0x00000100
#define BF_FDSP_REG_COND1_INFO              0x0000C0A8, 0x00000101
#define BF_FDSP_REG_COND2_INFO              0x0000C0A8, 0x00000102
#define BF_FDSP_REG_COND3_INFO              0x0000C0A8, 0x00000103
#define BF_FDSP_REG_COND4_INFO              0x0000C0A8, 0x00000104
#define BF_FDSP_REG_COND5_INFO              0x0000C0A8, 0x00000105
#define BF_FDSP_REG_COND6_INFO              0x0000C0A8, 0x00000106
#define BF_FDSP_REG_COND7_INFO              0x0000C0A8, 0x00000107

#define REG_FDSP_SL_ADDR_ADDR               0x0000C0A9
#define BF_FDSP_SL_ADDR_INFO                0x0000C0A9, 0x00000600

#define REG_FDSP_SL_P0_0_ADDR               0x0000C0AA
#define BF_FDSP_SL_P0_INFO                  0x0000C0AA, 0x00002000

#define REG_FDSP_SL_P0_1_ADDR               0x0000C0AB

#define REG_FDSP_SL_P0_2_ADDR               0x0000C0AC

#define REG_FDSP_SL_P0_3_ADDR               0x0000C0AD

#define REG_FDSP_SL_P1_0_ADDR               0x0000C0AE
#define BF_FDSP_SL_P1_INFO                  0x0000C0AE, 0x00002000

#define REG_FDSP_SL_P1_1_ADDR               0x0000C0AF

#define REG_FDSP_SL_P1_2_ADDR               0x0000C0B0

#define REG_FDSP_SL_P1_3_ADDR               0x0000C0B1

#define REG_FDSP_SL_P2_0_ADDR               0x0000C0B2
#define BF_FDSP_SL_P2_INFO                  0x0000C0B2, 0x00002000

#define REG_FDSP_SL_P2_1_ADDR               0x0000C0B3

#define REG_FDSP_SL_P2_2_ADDR               0x0000C0B4

#define REG_FDSP_SL_P2_3_ADDR               0x0000C0B5

#define REG_FDSP_SL_P3_0_ADDR               0x0000C0B6
#define BF_FDSP_SL_P3_INFO                  0x0000C0B6, 0x00002000

#define REG_FDSP_SL_P3_1_ADDR               0x0000C0B7

#define REG_FDSP_SL_P3_2_ADDR               0x0000C0B8

#define REG_FDSP_SL_P3_3_ADDR               0x0000C0B9

#define REG_FDSP_SL_P4_0_ADDR               0x0000C0BA
#define BF_FDSP_SL_P4_INFO                  0x0000C0BA, 0x00002000

#define REG_FDSP_SL_P4_1_ADDR               0x0000C0BB

#define REG_FDSP_SL_P4_2_ADDR               0x0000C0BC

#define REG_FDSP_SL_P4_3_ADDR               0x0000C0BD

#define REG_FDSP_SL_UPDATE_ADDR             0x0000C0BE
#define BF_FDSP_SL_UPDATE_INFO              0x0000C0BE, 0x00000100

#define REG_FDSP_ONZ_MASK0_ADDR             0x0000C0BF
#define BF_FDSP_ONZ_MASK_INFO               0x0000C0BF, 0x00001800

#define REG_FDSP_ONZ_MASK1_ADDR             0x0000C0C0

#define REG_FDSP_ONZ_MASK2_ADDR             0x0000C0C1

#define REG_RCOSC_ACTION_ADDR               0x0000C0C2
#define BF_RCOSC_STARTCAL_INFO              0x0000C0C2, 0x00000100
#define BF_RCOSC_FXCAL_INFO                 0x0000C0C2, 0x00000101
#define BF_RCOSC_STATUS_INFO                0x0000C0C2, 0x00000204

#define REG_RCOSC_CALCFG0_ADDR              0x0000C0C3
#define BF_RCOSC_REFSEL_INFO                0x0000C0C3, 0x00000400
#define BF_RCOSC_ERR_TH_INFO                0x0000C0C3, 0x00000404

#define REG_RCOSC_CALCFG1_ADDR              0x0000C0C4
#define BF_RCOSC_TASK_TH_INFO               0x0000C0C4, 0x00000400
#define BF_RCOSC_JOB_TH_INFO                0x0000C0C4, 0x00000404

#define REG_RCOSC_FERR0_ADDR                0x0000C0C5
#define BF_RCOSC_FERR_INFO                  0x0000C0C5, 0x00000E00

#define REG_RCOSC_FERR1_ADDR                0x0000C0C6

#define REG_RCOSC_FRQTRIM_ADDR              0x0000C0C7
#define BF_RCOSC_FRQTRIM_INFO               0x0000C0C7, 0x00000700

#define REG_RCOSC_FXTRIM_ADDR               0x0000C0C8
#define BF_RCOSC_FXTRIM_INFO                0x0000C0C8, 0x00000700
#define BF_RCOSC_DISTRIM_INFO               0x0000C0C8, 0x00000107

#define REG_RCOSC_TCTRIM_ADDR               0x0000C0C9
#define BF_RCOSC_TCTRIM_INFO                0x0000C0C9, 0x00000700

#define REG_MCLKOUT_IRQ_CTRL_ADDR           0x0000C0D2
#define BF_IRQ_OUT_INFO                     0x0000C0D2, 0x00000400
#define BF_CLOCKOUT_AND_IRQ_OUT_INFO        0x0000C0D2, 0x00000404

#define REG_MP_CTRL7_ADDR                   0x0000C0D6
#define BF_GPI_DB_INFO                      0x0000C0D6, 0x00000300
#define BF_MCLKO_RATE_INFO                  0x0000C0D6, 0x00000304

#define REG_MP_CTRL9_ADDR                   0x0000C0D8

#define REG_MP_CTRL10_ADDR                  0x0000C0D9
#define BF_MP12_MODE_INFO                   0x0000C0D9, 0x00000400

#define REG_SELFBOOT_CTRL_ADDR              0x0000C0DA
#define BF_SELFBOOT_DRIVE_INFO              0x0000C0DA, 0x00000200
#define BF_SELFBOOT_PULL_EN_INFO            0x0000C0DA, 0x00000104
#define BF_SELFBOOT_PULL_SEL_INFO           0x0000C0DA, 0x00000105
#define BF_SELFBOOT_SLEW_INFO               0x0000C0DA, 0x00000106

#define REG_SW_EN_CTRL_ADDR                 0x0000C0DB
#define BF_SWEN_DRIVE_INFO                  0x0000C0DB, 0x00000200
#define BF_SWEN_PULL_EN_INFO                0x0000C0DB, 0x00000104
#define BF_SWEN_PULL_SEL_INFO               0x0000C0DB, 0x00000105
#define BF_SWEN_SLEW_INFO                   0x0000C0DB, 0x00000106

#define REG_FSYNC0_CTRL_ADDR                0x0000C0E0
#define BF_FSYNC0_DRIVE_INFO                0x0000C0E0, 0x00000200
#define BF_FSYNC0_SLEW_INFO                 0x0000C0E0, 0x00000102
#define BF_FSYNC0_PULL_EN_INFO              0x0000C0E0, 0x00000104
#define BF_FSYNC0_PULL_SEL_INFO             0x0000C0E0, 0x00000105

#define REG_BCLK0_CTRL_ADDR                 0x0000C0E1
#define BF_BCLK0_DRIVE_INFO                 0x0000C0E1, 0x00000200
#define BF_BCLK0_SLEW_INFO                  0x0000C0E1, 0x00000102
#define BF_BCLK0_PULL_EN_INFO               0x0000C0E1, 0x00000104
#define BF_BCLK0_PULL_SEL_INFO              0x0000C0E1, 0x00000105

#define REG_SDATAO0_CTRL_ADDR               0x0000C0E2
#define BF_SDATAO0_DRIVE_INFO               0x0000C0E2, 0x00000200
#define BF_SDATAO0_SLEW_INFO                0x0000C0E2, 0x00000102
#define BF_SDATAO0_PULL_EN_INFO             0x0000C0E2, 0x00000104
#define BF_SDATAO0_PULL_SEL_INFO            0x0000C0E2, 0x00000105

#define REG_SDATAI0_CTRL_ADDR               0x0000C0E3
#define BF_SDATAI0_DRIVE_INFO               0x0000C0E3, 0x00000200
#define BF_SDATAI0_SLEW_INFO                0x0000C0E3, 0x00000102
#define BF_SDATAI0_PULL_EN_INFO             0x0000C0E3, 0x00000104
#define BF_SDATAI0_PULL_SEL_INFO            0x0000C0E3, 0x00000105

#define REG_IRQ_CTRL_ADDR                   0x0000C0E4
#define BF_IRQ_DRIVE_INFO                   0x0000C0E4, 0x00000200
#define BF_IRQ_SLEW_INFO                    0x0000C0E4, 0x00000102
#define BF_IRQ_PULL_EN_INFO                 0x0000C0E4, 0x00000104
#define BF_IRQ_PULL_SEL_INFO                0x0000C0E4, 0x00000105

#define REG_MCLKO_CTRL_ADDR                 0x0000C0E5
#define BF_MCLKO_DRIVE_INFO                 0x0000C0E5, 0x00000200
#define BF_MCLKO_SLEW_INFO                  0x0000C0E5, 0x00000102
#define BF_MCLKO_PULL_EN_INFO               0x0000C0E5, 0x00000104
#define BF_MCLKO_PULL_SEL_INFO              0x0000C0E5, 0x00000105

#define REG_I2C_SPI_CTRL_ADDR               0x0000C0EC
#define BF_SDA_MISO_DRIVE_INFO              0x0000C0EC, 0x00000200
#define BF_SDA_MISO_SLEW_INFO               0x0000C0EC, 0x00000102
#define BF_SCL_SCLK_DRIVE_INFO              0x0000C0EC, 0x00000204
#define BF_SCL_SCLK_SLEW_INFO               0x0000C0EC, 0x00000106

#define REG_IRQ_CTRL1_ADDR                  0x0000C100
#define BF_IRQ1_CLEAR_INFO                  0x0000C100, 0x00000100
#define BF_IRQ2_CLEAR_INFO                  0x0000C100, 0x00000101
#define BF_IRQ1_PINOUT_POL_INFO             0x0000C100, 0x00000104
#define BF_IRQ2_PINOUT_POL_INFO             0x0000C100, 0x00000105

#define REG_IRQ1_MASK1_ADDR                 0x0000C101
#define BF_IRQ1_DAC0_CLIP_MASK_INFO         0x0000C101, 0x00000100
#define BF_IRQ1_ADC0_CLIP_MASK_INFO         0x0000C101, 0x00000104
#define BF_IRQ1_ADC1_CLIP_MASK_INFO         0x0000C101, 0x00000105
#define BF_IRQ1_ADC2_CLIP_MASK_INFO         0x0000C101, 0x00000106

#define REG_IRQ1_MASK2_ADDR                 0x0000C102
#define BF_IRQ1_PLL_LOCKED_MASK_INFO        0x0000C102, 0x00000100
#define BF_IRQ1_PLL_UNLOCKED_MASK_INFO      0x0000C102, 0x00000101
#define BF_IRQ1_AVDD_UVW_MASK_INFO          0x0000C102, 0x00000102
#define BF_IRQ1_PRAMP_MASK_INFO             0x0000C102, 0x00000103
#define BF_IRQ1_ASRCI_LOCKED_MASK_INFO      0x0000C102, 0x00000104
#define BF_IRQ1_ASRCI_UNLOCKED_MASK_INFO    0x0000C102, 0x00000105
#define BF_IRQ1_ASRCO_LOCKED_MASK_INFO      0x0000C102, 0x00000106
#define BF_IRQ1_ASRCO_UNLOCKED_MASK_INFO    0x0000C102, 0x00000107

#define REG_IRQ1_MASK3_ADDR                 0x0000C103
#define BF_IRQ1_SPT0_UNLOCKED_MASK_INFO     0x0000C103, 0x00000100
#define BF_IRQ1_SYNC_LOCKED_MASK_INFO       0x0000C103, 0x00000101
#define BF_IRQ1_SYNC_UNLOCKED_MASK_INFO     0x0000C103, 0x00000102
#define BF_IRQ1_POWER_UP_COMPLETE_MASK_INFO 0x0000C103, 0x00000104
#define BF_IRQ1_EQ_CLEAR_DONE_MASK_INFO     0x0000C103, 0x00000105
#define BF_IRQ1_FDSP_MASK_ONZ_MASK_INFO     0x0000C103, 0x00000106

#define REG_IRQ2_MASK1_ADDR                 0x0000C104
#define BF_IRQ2_DAC0_CLIP_MASK_INFO         0x0000C104, 0x00000100
#define BF_IRQ2_ADC0_CLIP_MASK_INFO         0x0000C104, 0x00000104
#define BF_IRQ2_ADC1_CLIP_MASK_INFO         0x0000C104, 0x00000105
#define BF_IRQ2_ADC2_CLIP_MASK_INFO         0x0000C104, 0x00000106

#define REG_IRQ2_MASK2_ADDR                 0x0000C105
#define BF_IRQ2_PLL_LOCKED_MASK_INFO        0x0000C105, 0x00000100
#define BF_IRQ2_PLL_UNLOCKED_MASK_INFO      0x0000C105, 0x00000101
#define BF_IRQ2_AVDD_UVW_MASK_INFO          0x0000C105, 0x00000102
#define BF_IRQ2_PRAMP_MASK_INFO             0x0000C105, 0x00000103
#define BF_IRQ2_ASRCI_LOCKED_MASK_INFO      0x0000C105, 0x00000104
#define BF_IRQ2_ASRCI_UNLOCKED_MASK_INFO    0x0000C105, 0x00000105
#define BF_IRQ2_ASRCO_LOCKED_MASK_INFO      0x0000C105, 0x00000106
#define BF_IRQ2_ASRCO_UNLOCKED_MASK_INFO    0x0000C105, 0x00000107

#define REG_IRQ2_MASK3_ADDR                 0x0000C106
#define BF_IRQ2_SPT0_UNLOCKED_MASK_INFO     0x0000C106, 0x00000100
#define BF_IRQ2_SYNC_LOCKED_MASK_INFO       0x0000C106, 0x00000101
#define BF_IRQ2_SYNC_UNLOCKED_MASK_INFO     0x0000C106, 0x00000102
#define BF_IRQ2_POWER_UP_COMPLETE_MASK_INFO 0x0000C106, 0x00000104
#define BF_IRQ2_EQ_CLEAR_DONE_MASK_INFO     0x0000C106, 0x00000105
#define BF_IRQ2_FDSP_MASK_ONZ_MASK_INFO     0x0000C106, 0x00000106

#define REG_RESETS_ADDR                     0x0000C120
#define BF_SOFT_FULL_RESET_INFO             0x0000C120, 0x00000100
#define BF_SOFT_RESET_INFO                  0x0000C120, 0x00000104

#define REG_READ_LAMBDA_ADDR                0x0000C130
#define BF_FDSP_CURRENT_LAMBDA_INFO         0x0000C130, 0x00000600

#define REG_STATUS1_ADDR                    0x0000C131
#define BF_DAC0_CLIP_INFO                   0x0000C131, 0x00000100
#define BF_ADC0_CLIP_INFO                   0x0000C131, 0x00000104
#define BF_ADC1_CLIP_INFO                   0x0000C131, 0x00000105
#define BF_ADC2_CLIP_INFO                   0x0000C131, 0x00000106

#define REG_STATUS2_ADDR                    0x0000C132
#define BF_PLL_LOCK_INFO                    0x0000C132, 0x00000100
#define BF_AVDD_UVW_INFO                    0x0000C132, 0x00000101
#define BF_ASRCI_LOCK_INFO                  0x0000C132, 0x00000102
#define BF_ASRCO_LOCK_INFO                  0x0000C132, 0x00000103
#define BF_SPT0_LOCK_INFO                   0x0000C132, 0x00000104
#define BF_SYNC_LOCK_INFO                   0x0000C132, 0x00000106
#define BF_POWER_UP_COMPLETE_INFO           0x0000C132, 0x00000107

#define REG_STATUS3_ADDR                    0x0000C133
#define BF_FDSP_MASK_ONZ_INFO               0x0000C133, 0x00000100
#define BF_FDSP_BANK_COPY_DONE_INFO         0x0000C133, 0x00000104

#define REG_GPI2_ADDR                       0x0000C135

#define REG_EQ_STATUS_ADDR                  0x0000C136
#define BF_EQ_CLEAR_DONE_INFO               0x0000C136, 0x00000100

#define REG_IRQ1_STATUS1_ADDR               0x0000C138
#define BF_IRQ1_DAC0_CLIP_INFO              0x0000C138, 0x00000100
#define BF_IRQ1_ADC0_CLIP_INFO              0x0000C138, 0x00000104
#define BF_IRQ1_ADC1_CLIP_INFO              0x0000C138, 0x00000105
#define BF_IRQ1_ADC2_CLIP_INFO              0x0000C138, 0x00000106

#define REG_IRQ1_STATUS2_ADDR               0x0000C139
#define BF_IRQ1_PLL_LOCKED_INFO             0x0000C139, 0x00000100
#define BF_IRQ1_PLL_UNLOCKED_INFO           0x0000C139, 0x00000101
#define BF_IRQ1_AVDD_UVW_INFO               0x0000C139, 0x00000102
#define BF_IRQ1_PRAMP_INFO                  0x0000C139, 0x00000103
#define BF_IRQ1_ASRCI_LOCKED_INFO           0x0000C139, 0x00000104
#define BF_IRQ1_ASRCI_UNLOCKED_INFO         0x0000C139, 0x00000105
#define BF_IRQ1_ASRCO_LOCKED_INFO           0x0000C139, 0x00000106
#define BF_IRQ1_ASRCO_UNLOCKED_INFO         0x0000C139, 0x00000107

#define REG_IRQ1_STATUS3_ADDR               0x0000C13A
#define BF_IRQ1_SYNC_LOCKED_INFO            0x0000C13A, 0x00000100
#define BF_IRQ1_SYNC_UNLOCKED_INFO          0x0000C13A, 0x00000101
#define BF_IRQ1_SPT0_UNLOCKED_INFO          0x0000C13A, 0x00000102
#define BF_IRQ1_POWER_UP_COMPLETE_INFO      0x0000C13A, 0x00000104
#define BF_IRQ1_EQ_CLEAR_DONE_INFO          0x0000C13A, 0x00000105
#define BF_IRQ1_FDSP_MASK_ONZ_INFO          0x0000C13A, 0x00000106

#define REG_IRQ2_STATUS1_ADDR               0x0000C13B
#define BF_IRQ2_DAC0_CLIP_INFO              0x0000C13B, 0x00000100
#define BF_IRQ2_ADC0_CLIP_INFO              0x0000C13B, 0x00000104
#define BF_IRQ2_ADC1_CLIP_INFO              0x0000C13B, 0x00000105
#define BF_IRQ2_ADC2_CLIP_INFO              0x0000C13B, 0x00000106

#define REG_IRQ2_STATUS2_ADDR               0x0000C13C
#define BF_IRQ2_PLL_LOCKED_INFO             0x0000C13C, 0x00000100
#define BF_IRQ2_PLL_UNLOCKED_INFO           0x0000C13C, 0x00000101
#define BF_IRQ2_AVDD_UVW_INFO               0x0000C13C, 0x00000102
#define BF_IRQ2_PRAMP_INFO                  0x0000C13C, 0x00000103
#define BF_IRQ2_ASRCI_LOCKED_INFO           0x0000C13C, 0x00000104
#define BF_IRQ2_ASRCI_UNLOCKED_INFO         0x0000C13C, 0x00000105
#define BF_IRQ2_ASRCO_LOCKED_INFO           0x0000C13C, 0x00000106
#define BF_IRQ2_ASRCO_UNLOCKED_INFO         0x0000C13C, 0x00000107

#define REG_IRQ2_STATUS3_ADDR               0x0000C13D
#define BF_IRQ2_SYNC_LOCKED_INFO            0x0000C13D, 0x00000100
#define BF_IRQ2_SYNC_UNLOCKED_INFO          0x0000C13D, 0x00000101
#define BF_IRQ2_SPT0_UNLOCKED_INFO          0x0000C13D, 0x00000102
#define BF_IRQ2_POWER_UP_COMPLETE_INFO      0x0000C13D, 0x00000104
#define BF_IRQ2_EQ_CLEAR_DONE_INFO          0x0000C13D, 0x00000105
#define BF_IRQ2_FDSP_MASK_ONZ_INFO          0x0000C13D, 0x00000106

#define REG_SPT0_CTRL1_ADDR                 0x0000C150
#define BF_SPT0_SAI_MODE_INFO               0x0000C150, 0x00000100
#define BF_SPT0_DATA_FORMAT_INFO            0x0000C150, 0x00000301
#define BF_SPT0_SLOT_WIDTH_INFO             0x0000C150, 0x00000204
#define BF_SPT0_TRI_STATE_INFO              0x0000C150, 0x00000106

#define REG_SPT0_CTRL2_ADDR                 0x0000C151
#define BF_SPT0_BCLK_SRC_INFO               0x0000C151, 0x00000300
#define BF_SPT0_BCLK_POL_INFO               0x0000C151, 0x00000103

#define REG_SPT0_CTRL3_ADDR                 0x0000C152
#define BF_SPT0_LRCLK_SRC_INFO              0x0000C152, 0x00000400
#define BF_SPT0_LRCLK_POL_INFO              0x0000C152, 0x00000104

#define REG_SPT0_ROUTE0_ADDR                0x0000C153
#define BF_SPT0_OUT_ROUTE0_INFO             0x0000C153, 0x00000600

#define REG_SPT0_ROUTE1_ADDR                0x0000C154
#define BF_SPT0_OUT_ROUTE1_INFO             0x0000C154, 0x00000600

#define REG_SPT0_ROUTE2_ADDR                0x0000C155
#define BF_SPT0_OUT_ROUTE2_INFO             0x0000C155, 0x00000600

#define REG_SPT0_ROUTE3_ADDR                0x0000C156
#define BF_SPT0_OUT_ROUTE3_INFO             0x0000C156, 0x00000600

#define REG_SPT0_ROUTE4_ADDR                0x0000C157
#define BF_SPT0_OUT_ROUTE4_INFO             0x0000C157, 0x00000600

#define REG_SPT0_ROUTE5_ADDR                0x0000C158
#define BF_SPT0_OUT_ROUTE5_INFO             0x0000C158, 0x00000600

#define REG_SPT0_ROUTE6_ADDR                0x0000C159
#define BF_SPT0_OUT_ROUTE6_INFO             0x0000C159, 0x00000600

#define REG_SPT0_ROUTE7_ADDR                0x0000C15A
#define BF_SPT0_OUT_ROUTE7_INFO             0x0000C15A, 0x00000600

#define REG_SPT0_ROUTE8_ADDR                0x0000C15B
#define BF_SPT0_OUT_ROUTE8_INFO             0x0000C15B, 0x00000600

#define REG_SPT0_ROUTE9_ADDR                0x0000C15C
#define BF_SPT0_OUT_ROUTE9_INFO             0x0000C15C, 0x00000600

#define REG_SPT0_ROUTE10_ADDR               0x0000C15D
#define BF_SPT0_OUT_ROUTE10_INFO            0x0000C15D, 0x00000600

#define REG_SPT0_ROUTE11_ADDR               0x0000C15E
#define BF_SPT0_OUT_ROUTE11_INFO            0x0000C15E, 0x00000600

#define REG_SPT0_ROUTE12_ADDR               0x0000C15F
#define BF_SPT0_OUT_ROUTE12_INFO            0x0000C15F, 0x00000600

#define REG_SPT0_ROUTE13_ADDR               0x0000C160
#define BF_SPT0_OUT_ROUTE13_INFO            0x0000C160, 0x00000600

#define REG_SPT0_ROUTE14_ADDR               0x0000C161
#define BF_SPT0_OUT_ROUTE14_INFO            0x0000C161, 0x00000600

#define REG_SPT0_ROUTE15_ADDR               0x0000C162
#define BF_SPT0_OUT_ROUTE15_INFO            0x0000C162, 0x00000600

#define REG_PDM_CTRL1_ADDR                  0x0000C180
#define BF_PDM_FS_INFO                      0x0000C180, 0x00000300
#define BF_PDM_FCOMP_INFO                   0x0000C180, 0x00000103
#define BF_PDM_RATE_INFO                    0x0000C180, 0x00000104
#define BF_PDM_MORE_FILT_INFO               0x0000C180, 0x00000107

#define REG_PDM_CTRL2_ADDR                  0x0000C181
#define BF_PDM_VOL_LINK_INFO                0x0000C181, 0x00000100
#define BF_PDM_HARD_VOL_INFO                0x0000C181, 0x00000101
#define BF_PDM_VOL_ZC_INFO                  0x0000C181, 0x00000102
#define BF_PDM0_HPF_EN_INFO                 0x0000C181, 0x00000104
#define BF_PDM1_HPF_EN_INFO                 0x0000C181, 0x00000105
#define BF_PDM0_MUTE_INFO                   0x0000C181, 0x00000106
#define BF_PDM1_MUTE_INFO                   0x0000C181, 0x00000107

#define REG_PDM_VOL0_ADDR                   0x0000C182
#define BF_PDM0_VOL_INFO                    0x0000C182, 0x00000800

#define REG_PDM_VOL1_ADDR                   0x0000C183
#define BF_PDM1_VOL_INFO                    0x0000C183, 0x00000800

#define REG_PDM_ROUTE0_ADDR                 0x0000C184
#define BF_PDM0_ROUTE_INFO                  0x0000C184, 0x00000700

#define REG_PDM_ROUTE1_ADDR                 0x0000C185
#define BF_PDM1_ROUTE_INFO                  0x0000C185, 0x00000700

#define REG_KEY_SELECT_ADDR                 0x0000C190
#define BF_AUTH_DONE_INFO                   0x0000C190, 0x00000104
#define BF_DECRYPT_READY_INFO               0x0000C190, 0x00000105

#define REG_NONCE_1_ADDR                    0x0000C192

#define REG_AUTH_CHALLENGE_2_ADDR           0x0000C194

#define REG_AUTH_CHALLENGE_1_ADDR           0x0000C195

#define REG_AUTH_RESPONSE_2_ADDR            0x0000C197

#define REG_AUTH_RESPONSE_1_ADDR            0x0000C198

#define REG_READBACK_KEY_15_ADDR            0x0000C19A

#define REG_READBACK_KEY_14_ADDR            0x0000C19B

#define REG_READBACK_KEY_13_ADDR            0x0000C19C

#define REG_READBACK_KEY_12_ADDR            0x0000C19D

#define REG_READBACK_KEY_11_ADDR            0x0000C19E

#define REG_READBACK_KEY_10_ADDR            0x0000C19F

#define REG_READBACK_KEY_9_ADDR             0x0000C1A0

#define REG_READBACK_KEY_8_ADDR             0x0000C1A1

#define REG_READBACK_KEY_7_ADDR             0x0000C1A2

#define REG_READBACK_KEY_6_ADDR             0x0000C1A3

#define REG_READBACK_KEY_5_ADDR             0x0000C1A4

#define REG_READBACK_KEY_4_ADDR             0x0000C1A5

#define REG_READBACK_KEY_3_ADDR             0x0000C1A6

#define REG_READBACK_KEY_2_ADDR             0x0000C1A7

#define REG_READBACK_KEY_1_ADDR             0x0000C1A8

#define REG_DAC_NOISE_CTRL2_ADDR            0x0000CC04
#define BF_DAC_LOW_NOISE_EN2_INFO           0x0000CC04, 0x00000101

#define REG_DAC_NOISE_CTRL1_ADDR            0x0000CC13
#define BF_DAC_LOW_NOISE_EN1_INFO           0x0000CC13, 0x00000104

#define REG_XTAL_TEST_ADDR                  0x0000CC18

#define REG_EXTRA3_ADDR                     0x0000CC36

#define REG_PMU_CTRL2_ADDR                  0x0000CC3F


#endif /* _CODEC_0581_REG_H_ */

/************************** end of file *****************************/
