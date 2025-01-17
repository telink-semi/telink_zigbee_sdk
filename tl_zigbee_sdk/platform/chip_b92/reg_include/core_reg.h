/********************************************************************************************************
 * @file    core_reg.h
 *
 * @brief   This is the header file for B92
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
#ifndef CORE_REG_H
#define CORE_REG_H

/* If using Andes toolchain, this Macro is defined in it's toolchain */
/* If using std gcc toolchain, this core register(not standard risc-v core register)
    should be defined here. */
#ifdef STD_GCC
    #define mmisc_ctl 0x7D0
#endif

typedef enum
{
    /* User Trap Setup */
    NDS_USTATUS = 0x000,
    NDS_UIE     = 0x004,
    NDS_UTVEC   = 0x005,

    /* User Trap Handling */
    NDS_USCRATCH = 0x040,
    NDS_UEPC     = 0x041,
    NDS_UCAUSE   = 0x042,
    NDS_UTVAL    = 0x043,
    NDS_UBADADDR = 0x043,
    NDS_UIP      = 0x044,

    /* User Floating-Point CSRs */
    NDS_FFLAGS = 0x001,
    NDS_FRM    = 0x002,
    NDS_FCSR   = 0x003,

    /* User Floating-Point CSRs */
    NDS_CYCLE         = 0xC00,
    NDS_TIME          = 0xC01,
    NDS_INSTRET       = 0xC02,
    NDS_HPMCOUNTER3   = 0xC03,
    NDS_HPMCOUNTER4   = 0xC04,
    NDS_HPMCOUNTER5   = 0xC05,
    NDS_HPMCOUNTER6   = 0xC06,
    NDS_HPMCOUNTER7   = 0xC07,
    NDS_HPMCOUNTER8   = 0xC08,
    NDS_HPMCOUNTER9   = 0xC09,
    NDS_HPMCOUNTER10  = 0xC0A,
    NDS_HPMCOUNTER11  = 0xC0B,
    NDS_HPMCOUNTER12  = 0xC0C,
    NDS_HPMCOUNTER13  = 0xC0D,
    NDS_HPMCOUNTER14  = 0xC0E,
    NDS_HPMCOUNTER15  = 0xC0F,
    NDS_HPMCOUNTER16  = 0xC10,
    NDS_HPMCOUNTER17  = 0xC11,
    NDS_HPMCOUNTER18  = 0xC12,
    NDS_HPMCOUNTER19  = 0xC13,
    NDS_HPMCOUNTER20  = 0xC14,
    NDS_HPMCOUNTER21  = 0xC15,
    NDS_HPMCOUNTER22  = 0xC16,
    NDS_HPMCOUNTER23  = 0xC17,
    NDS_HPMCOUNTER24  = 0xC18,
    NDS_HPMCOUNTER25  = 0xC19,
    NDS_HPMCOUNTER26  = 0xC1A,
    NDS_HPMCOUNTER27  = 0xC1B,
    NDS_HPMCOUNTER28  = 0xC1C,
    NDS_HPMCOUNTER29  = 0xC1D,
    NDS_HPMCOUNTER30  = 0xC1E,
    NDS_HPMCOUNTER31  = 0xC1F,
    NDS_CYCLEH        = 0xC80,
    NDS_TIMEH         = 0xC81,
    NDS_INSTRETH      = 0xC82,
    NDS_HPMCOUNTER3H  = 0xC83,
    NDS_HPMCOUNTER4H  = 0xC84,
    NDS_HPMCOUNTER5H  = 0xC85,
    NDS_HPMCOUNTER6H  = 0xC86,
    NDS_HPMCOUNTER7H  = 0xC87,
    NDS_HPMCOUNTER8H  = 0xC88,
    NDS_HPMCOUNTER9H  = 0xC89,
    NDS_HPMCOUNTER10H = 0xC8A,
    NDS_HPMCOUNTER11H = 0xC8B,
    NDS_HPMCOUNTER12H = 0xC8C,
    NDS_HPMCOUNTER13H = 0xC8D,
    NDS_HPMCOUNTER14H = 0xC8E,
    NDS_HPMCOUNTER15H = 0xC8F,
    NDS_HPMCOUNTER16H = 0xC90,
    NDS_HPMCOUNTER17H = 0xC91,
    NDS_HPMCOUNTER18H = 0xC92,
    NDS_HPMCOUNTER19H = 0xC93,
    NDS_HPMCOUNTER20H = 0xC94,
    NDS_HPMCOUNTER21H = 0xC95,
    NDS_HPMCOUNTER22H = 0xC96,
    NDS_HPMCOUNTER23H = 0xC97,
    NDS_HPMCOUNTER24H = 0xC98,
    NDS_HPMCOUNTER25H = 0xC99,
    NDS_HPMCOUNTER26H = 0xC9A,
    NDS_HPMCOUNTER27H = 0xC9B,
    NDS_HPMCOUNTER28H = 0xC9C,
    NDS_HPMCOUNTER29H = 0xC9D,
    NDS_HPMCOUNTER30H = 0xC9E,
    NDS_HPMCOUNTER31H = 0xC9F,

    /* Supervisor Trap Setup */
    NDS_SSTATUS    = 0x100,
    NDS_SEDELEG    = 0x102,
    NDS_SIDELEG    = 0x103,
    NDS_SIE        = 0x104,
    NDS_STVEC      = 0x105,
    NDS_SCOUNTEREN = 0x106,

    /* Supervisor Configuration */
    NDS_SENVCFG = 0x10A,

    /* Supervisor State Management CSRs */
    NDS_SSTATEEN0 = 0x10C,
    NDS_SSTATEEN1 = 0x10D,
    NDS_SSTATEEN2 = 0x10E,
    NDS_SSTATEEN3 = 0x10F,

    /* SUPERVISOR TRAP HANDLING */
    NDS_SSCRATCH = 0x140,
    NDS_SEPC     = 0x141,
    NDS_SCAUSE   = 0x142,
    NDS_STVAL    = 0x143,
    NDS_SBADADDR = 0x143, /* alias NDS_STVAL */
    NDS_SIP      = 0x144,

    /* Supervisor Protection and Translation */
    NDS_SATP  = 0x180,
    NDS_SPTBR = 0x180, /* alias NDS_SPTBR */

    /* Counter related CSRs */
    NDS_SCOUNTOVF = 0xDA0,

    /* Machine Information Registers */
    NDS_MVENDORID  = 0xF11,
    NDS_MARCHID    = 0xF12,
    NDS_MIMPID     = 0xF13,
    NDS_MHARTID    = 0xF14,
    NDS_MCONFIGPTR = 0xF15,

    /* Machine Trap Setup */
    NDS_MSTATUS    = 0x300,
    NDS_MISA       = 0x301,
    NDS_MEDELEG    = 0x302,
    NDS_MIDELEG    = 0x303,
    NDS_MIE        = 0x304,
    NDS_MTVEC      = 0x305,
    NDS_MCOUNTEREN = 0x306,

    /* Machine Trap Handling */
    NDS_MSCRATCH = 0x340,
    NDS_MEPC     = 0x341,
    NDS_MCAUSE   = 0x342,
    NDS_MTVAL    = 0x343,
    NDS_MIP      = 0x344,

    /* Machine Configuration */
    NDS_MENVCFG  = 0x30A,
    NDS_MENVCFGH = 0x31A,
    NDS_MSECCFG  = 0x747,
    NDS_MSECCFGH = 0x757,

    /* Machine State Management CSRs */
    NDS_MSTATEEN0  = 0x30C,
    NDS_MSTATEEN1  = 0x30D,
    NDS_MSTATEEN2  = 0x30E,
    NDS_MSTATEEN3  = 0x30F,
    NDS_MSTATEEN0H = 0x31C,
    NDS_MSTATEEN1H = 0x31D,
    NDS_MSTATEEN2H = 0x31E,
    NDS_MSTATEEN3H = 0x31F,

    /* Machine Protection and Translation */
    NDS_PMPCFG0   = 0x3A0,
    NDS_PMPCFG2   = 0x3A2,
    NDS_PMPCFG1   = 0x3A1,
    NDS_PMPCFG3   = 0x3A3,
    NDS_PMPADDR0  = 0x3B0,
    NDS_PMPADDR1  = 0x3B1,
    NDS_PMPADDR2  = 0x3B2,
    NDS_PMPADDR3  = 0x3B3,
    NDS_PMPADDR4  = 0x3B4,
    NDS_PMPADDR5  = 0x3B5,
    NDS_PMPADDR6  = 0x3B6,
    NDS_PMPADDR7  = 0x3B7,
    NDS_PMPADDR8  = 0x3B8,
    NDS_PMPADDR9  = 0x3B9,
    NDS_PMPADDR10 = 0x3BA,
    NDS_PMPADDR11 = 0x3BB,
    NDS_PMPADDR12 = 0x3BC,
    NDS_PMPADDR13 = 0x3BD,
    NDS_PMPADDR14 = 0x3BE,
    NDS_PMPADDR15 = 0x3BF,
    NDS_PMPADDR16 = 0X3C0,
    NDS_PMPADDR17 = 0X3C1,
    NDS_PMPADDR18 = 0X3C2,
    NDS_PMPADDR19 = 0X3C3,
    NDS_PMPADDR20 = 0X3C4,
    NDS_PMPADDR21 = 0X3C5,
    NDS_PMPADDR22 = 0X3C6,
    NDS_PMPADDR23 = 0X3C7,
    NDS_PMPADDR24 = 0X3C8,
    NDS_PMPADDR25 = 0X3C9,
    NDS_PMPADDR26 = 0X3CA,
    NDS_PMPADDR27 = 0X3CB,
    NDS_PMPADDR28 = 0X3CC,
    NDS_PMPADDR29 = 0X3CD,
    NDS_PMPADDR30 = 0X3CE,
    NDS_PMPADDR31 = 0X3CF,
    NDS_PMPADDR32 = 0X3D0,
    NDS_PMPADDR33 = 0X3D1,
    NDS_PMPADDR34 = 0X3D2,
    NDS_PMPADDR35 = 0X3D3,
    NDS_PMPADDR36 = 0X3D4,
    NDS_PMPADDR37 = 0X3D5,
    NDS_PMPADDR38 = 0X3D6,
    NDS_PMPADDR39 = 0X3D7,
    NDS_PMPADDR40 = 0X3D8,
    NDS_PMPADDR41 = 0X3D9,
    NDS_PMPADDR42 = 0X3DA,
    NDS_PMPADDR43 = 0X3DB,
    NDS_PMPADDR44 = 0X3DC,
    NDS_PMPADDR45 = 0X3DD,
    NDS_PMPADDR46 = 0X3DE,
    NDS_PMPADDR47 = 0X3DF,
    NDS_PMPADDR48 = 0X3E0,
    NDS_PMPADDR49 = 0X3E1,
    NDS_PMPADDR50 = 0X3E2,
    NDS_PMPADDR51 = 0X3E3,
    NDS_PMPADDR52 = 0X3E4,
    NDS_PMPADDR53 = 0X3E5,
    NDS_PMPADDR54 = 0X3E6,
    NDS_PMPADDR55 = 0X3E7,
    NDS_PMPADDR56 = 0X3E8,
    NDS_PMPADDR57 = 0X3E9,
    NDS_PMPADDR58 = 0X3EA,
    NDS_PMPADDR59 = 0X3EB,
    NDS_PMPADDR60 = 0X3EC,
    NDS_PMPADDR61 = 0X3ED,
    NDS_PMPADDR62 = 0X3EE,
    NDS_PMPADDR63 = 0X3EF,

    /* Machine Counter and Timers */
    NDS_MCYCLE         = 0xB00,
    NDS_MINSTRET       = 0xB02,
    NDS_MHPMCOUNTER3   = 0xB03,
    NDS_MHPMCOUNTER4   = 0xB04,
    NDS_MHPMCOUNTER5   = 0xB05,
    NDS_MHPMCOUNTER6   = 0xB06,
    NDS_MHPMCOUNTER7   = 0xB07,
    NDS_MHPMCOUNTER8   = 0xB08,
    NDS_MHPMCOUNTER9   = 0xB09,
    NDS_MHPMCOUNTER10  = 0xB0A,
    NDS_MHPMCOUNTER11  = 0xB0B,
    NDS_MHPMCOUNTER12  = 0xB0C,
    NDS_MHPMCOUNTER13  = 0xB0D,
    NDS_MHPMCOUNTER14  = 0xB0E,
    NDS_MHPMCOUNTER15  = 0xB0F,
    NDS_MHPMCOUNTER16  = 0xB10,
    NDS_MHPMCOUNTER17  = 0xB11,
    NDS_MHPMCOUNTER18  = 0xB12,
    NDS_MHPMCOUNTER19  = 0xB13,
    NDS_MHPMCOUNTER20  = 0xB14,
    NDS_MHPMCOUNTER21  = 0xB15,
    NDS_MHPMCOUNTER22  = 0xB16,
    NDS_MHPMCOUNTER23  = 0xB17,
    NDS_MHPMCOUNTER24  = 0xB18,
    NDS_MHPMCOUNTER25  = 0xB19,
    NDS_MHPMCOUNTER26  = 0xB1A,
    NDS_MHPMCOUNTER27  = 0xB1B,
    NDS_MHPMCOUNTER28  = 0xB1C,
    NDS_MHPMCOUNTER29  = 0xB1D,
    NDS_MHPMCOUNTER30  = 0xB1E,
    NDS_MHPMCOUNTER31  = 0xB1F,
    NDS_MCYCLEH        = 0xB80,
    NDS_MINSTRETH      = 0xB82,
    NDS_MHPMCOUNTER3H  = 0xB83,
    NDS_MHPMCOUNTER4H  = 0xB84,
    NDS_MHPMCOUNTER5H  = 0xB85,
    NDS_MHPMCOUNTER6H  = 0xB86,
    NDS_MHPMCOUNTER7H  = 0xB87,
    NDS_MHPMCOUNTER8H  = 0xB88,
    NDS_MHPMCOUNTER9H  = 0xB89,
    NDS_MHPMCOUNTER10H = 0xB8A,
    NDS_MHPMCOUNTER11H = 0xB8B,
    NDS_MHPMCOUNTER12H = 0xB8C,
    NDS_MHPMCOUNTER13H = 0xB8D,
    NDS_MHPMCOUNTER14H = 0xB8E,
    NDS_MHPMCOUNTER15H = 0xB8F,
    NDS_MHPMCOUNTER16H = 0xB90,
    NDS_MHPMCOUNTER17H = 0xB91,
    NDS_MHPMCOUNTER18H = 0xB92,
    NDS_MHPMCOUNTER19H = 0xB93,
    NDS_MHPMCOUNTER20H = 0xB94,
    NDS_MHPMCOUNTER21H = 0xB95,
    NDS_MHPMCOUNTER22H = 0xB96,
    NDS_MHPMCOUNTER23H = 0xB97,
    NDS_MHPMCOUNTER24H = 0xB98,
    NDS_MHPMCOUNTER25H = 0xB99,
    NDS_MHPMCOUNTER26H = 0xB9A,
    NDS_MHPMCOUNTER27H = 0xB9B,
    NDS_MHPMCOUNTER28H = 0xB9C,
    NDS_MHPMCOUNTER29H = 0xB9D,
    NDS_MHPMCOUNTER30H = 0xB9E,
    NDS_MHPMCOUNTER31H = 0xB9F,

    /* Machine Counter Setup */
    NDS_MHPMEVENT3   = 0x323,
    NDS_MHPMEVENT4   = 0x324,
    NDS_MHPMEVENT5   = 0x325,
    NDS_MHPMEVENT6   = 0x326,
    NDS_MHPMEVENT7   = 0x327,
    NDS_MHPMEVENT8   = 0x328,
    NDS_MHPMEVENT9   = 0x329,
    NDS_MHPMEVENT10  = 0x32A,
    NDS_MHPMEVENT11  = 0x32B,
    NDS_MHPMEVENT12  = 0x32C,
    NDS_MHPMEVENT13  = 0x32D,
    NDS_MHPMEVENT14  = 0x32E,
    NDS_MHPMEVENT15  = 0x32F,
    NDS_MHPMEVENT16  = 0x330,
    NDS_MHPMEVENT17  = 0x331,
    NDS_MHPMEVENT18  = 0x332,
    NDS_MHPMEVENT19  = 0x333,
    NDS_MHPMEVENT20  = 0x334,
    NDS_MHPMEVENT21  = 0x335,
    NDS_MHPMEVENT22  = 0x336,
    NDS_MHPMEVENT23  = 0x337,
    NDS_MHPMEVENT24  = 0x338,
    NDS_MHPMEVENT25  = 0x339,
    NDS_MHPMEVENT26  = 0x33A,
    NDS_MHPMEVENT27  = 0x33B,
    NDS_MHPMEVENT28  = 0x33C,
    NDS_MHPMEVENT29  = 0x33D,
    NDS_MHPMEVENT30  = 0x33E,
    NDS_MHPMEVENT31  = 0x33F,
    NDS_MHPMEVENT3H  = 0x723,
    NDS_MHPMEVENT4H  = 0x724,
    NDS_MHPMEVENT5H  = 0x725,
    NDS_MHPMEVENT6H  = 0x726,
    NDS_MHPMEVENT7H  = 0x727,
    NDS_MHPMEVENT8H  = 0x728,
    NDS_MHPMEVENT9H  = 0x729,
    NDS_MHPMEVENT10H = 0x72A,
    NDS_MHPMEVENT11H = 0x72B,
    NDS_MHPMEVENT12H = 0x72C,
    NDS_MHPMEVENT13H = 0x72D,
    NDS_MHPMEVENT14H = 0x72E,
    NDS_MHPMEVENT15H = 0x72F,
    NDS_MHPMEVENT16H = 0x730,
    NDS_MHPMEVENT17H = 0x731,
    NDS_MHPMEVENT18H = 0x732,
    NDS_MHPMEVENT19H = 0x733,
    NDS_MHPMEVENT20H = 0x734,
    NDS_MHPMEVENT21H = 0x735,
    NDS_MHPMEVENT22H = 0x736,
    NDS_MHPMEVENT23H = 0x737,
    NDS_MHPMEVENT24H = 0x738,
    NDS_MHPMEVENT25H = 0x739,
    NDS_MHPMEVENT26H = 0x73A,
    NDS_MHPMEVENT27H = 0x73B,
    NDS_MHPMEVENT28H = 0x73C,
    NDS_MHPMEVENT29H = 0x73D,
    NDS_MHPMEVENT30H = 0x73E,
    NDS_MHPMEVENT31H = 0x73F,

    /* Debug/ Trace Registers (shared with Debug Mode) */
    NDS_TSELECT = 0x7A0,
    NDS_TDATA1  = 0x7A1,
    NDS_TDATA2  = 0x7A2,
    NDS_TDATA3  = 0x7A3,

    /* Debug Mode Registers */
    NDS_DCSR     = 0x7B0,
    NDS_DPC      = 0x7B1,
    NDS_DSCRATCH = 0x7B2,

    /* User Mode CSRs */
    NDS_UITB           = 0x800,
    NDS_UCODE          = 0x801,
    NDS_UDCAUSE        = 0x809,
    NDS_UCCTLBEGINADDR = 0x80B,
    NDS_UCCTLCOMMAND   = 0x80C,
    NDS_WFE            = 0x810,
    NDS_SLEEPVALUE     = 0x811,
    NDS_TXEVT          = 0x812,
    NDS_UMISC_CTL      = 0x813,

    /* Supervisor Mode Trap CSRs */
    NDS_SLIE    = 0x9C4,
    NDS_SLIP    = 0x9C5,
    NDS_SDCAUSE = 0x9C9,

    /* Supervisor Mode Counter CSRs */
    NDS_SCOUNTERINTEN  = 0x9CF,
    NDS_SCOUNTERMASK_M = 0x9D1,
    NDS_SCOUNTERMASK_S = 0x9D2,
    NDS_SCOUNTERMASK_U = 0x9D3,
    NDS_SCOUNTEROVF    = 0x9D4,
    NDS_SCOUNTINHIBIT  = 0x9E0,
    NDS_SHPMEVENT3     = 0x9E3,
    NDS_SHPMEVENT4     = 0x9E4,
    NDS_SHPMEVENT5     = 0x9E5,
    NDS_SHPMEVENT6     = 0x9E6,

    /* Supervisor Mode Control CSRs */
    NDS_SCCTLDATA = 0x9CD,
    NDS_SMISC_CTL = 0x9D0,

    /* Machine Mode Configuration CSRs */
    NDS_MICM_CFG  = 0xFC0,
    NDS_MDCM_CFG  = 0xFC1,
    NDS_MMSC_CFG  = 0xFC2,
    NDS_MMSC_CFG2 = 0xFC3,
    NDS_MMSC_CFG3 = 0xFC4,
    NDS_MVEC_CFG  = 0xFC7,

    /* Machine Mode Crash Debug CSRs */
    NDS_MCRASH_STATESAVE  = 0xFC8,
    NDS_MSTATUS_CRASHSAVE = 0xFC9,

    /* HVM Configuration CSRs */
    NDS_MHVM_CFG = 0xFD0,
    NDS_MHVMB    = 0xFD1,

    /* Machine Mode Memory and Miscellaneous CSRs */
    NDS_MILMB          = 0x7C0,
    NDS_MDLMB          = 0x7C1,
    NDS_MECC_CODE      = 0x7C2,
    NDS_MNVEC          = 0x7C3,
    NDS_MPFT_CTL       = 0x7C5,
    NDS_MCACHE_CTL     = 0x7CA,
    NDS_MMISC_CTL      = 0x7D0,
    NDS_MCLK_CTL       = 0x7DF,
    NDS_MCCTLBEGINADDR = 0x7CB,
    NDS_MCCTLCOMMAND   = 0x7CC,
    NDS_MCCTLDATA      = 0x7CD,
    NDS_MPPIB          = 0x7F0,
    NDS_MFIOB          = 0x7F1,

    /* Machine Mode Trap CSRs */
    NDS_MXSTATUS     = 0x7C4,
    NDS_MDCAUSE      = 0x7C9,
    NDS_MSLIDELEG    = 0x7D5,
    NDS_MSAVESTATUS  = 0x7D6,
    NDS_MSAVEEPC1    = 0x7D7,
    NDS_MSAVECAUSE1  = 0x7D8,
    NDS_MSAVEEPC2    = 0x7D9,
    NDS_MSAVECAUSE2  = 0x7DA,
    NDS_MSAVEDCAUSE1 = 0x7DB,
    NDS_MSAVEDCAUSE2 = 0x7DC,
    NDS_MNDSX_RDATA  = 0x7DD,
    NDS_MNDSX_WDATA  = 0x7DE,

    /* Machine Mode Hardware Stack Protection and Recording CSRs */
    NDS_MHSP_CTL  = 0x7C6,
    NDS_MSP_BOUND = 0x7C7,
    NDS_MSP_BASE  = 0x7C8,

    /* Machine Mode Counter CSRs */
    NDS_MCOUNTERWEN    = 0x7CE,
    NDS_MCOUNTERMASK_M = 0x7D1,
    NDS_MCOUNTERMASK_S = 0x7D2,
    NDS_MCOUNTERMASK_U = 0x7D3,
    NDS_MCOUNTERINTEN  = 0x7CF,
    NDS_MCOUNTEROVF    = 0x7D4,
    NDS_MCOUNTINHIBIT  = 0x320,

    /* Machine Mode Enhanced CLIC CSRs */
    NDS_MIRQ_ENTRY   = 0x7EC,
    NDS_MINTSEL_JAL  = 0x7ED,
    NDS_PUSHMCAUSE   = 0x7EE,
    NDS_PUSHMEPC     = 0x7EF,
    NDS_PUSHMXSTATUS = 0x7EB,

    /* Machine Mode Physical Memory Attribute Unit CSRs */
    NDS_PMACFG0   = 0xBC0,
    NDS_PMACFG1   = 0xBC1,
    NDS_PMACFG2   = 0xBC2,
    NDS_PMACFG3   = 0xBC3,
    NDS_PMAADDR0  = 0xBD0,
    NDS_PMAADDR1  = 0xBD1,
    NDS_PMAADDR2  = 0xBD2,
    NDS_PMAADDR3  = 0xBD3,
    NDS_PMAADDR4  = 0xBD4,
    NDS_PMAADDR5  = 0xBD5,
    NDS_PMAADDR6  = 0xBD6,
    NDS_PMAADDR7  = 0xBD7,
    NDS_PMAADDR8  = 0xBD8,
    NDS_PMAADDR9  = 0xBD9,
    NDS_PMAADDR10 = 0xBDA,
    NDS_PMAADDR11 = 0xBDB,
    NDS_PMAADDR12 = 0xBDC,
    NDS_PMAADDR13 = 0xBDD,
    NDS_PMAADDR14 = 0xBDE,
    NDS_PMAADDR15 = 0xBDF,

    /* Debug/ Trace Registers (shared with Debug Mode) */
    NDS_MCONTROL = 0x7A1,
    NDS_ICOUNT   = 0x7A1,
    NDS_ITRIGGER = 0x7A1,
    NDS_ETRIGGER = 0x7A1,
    NDS_TEXTRA32 = 0x7A3,
    NDS_TEXTRA64 = 0x7A3,
    NDS_TINFO    = 0x7A4,
    NDS_TCONTROL = 0x7A5,
    NDS_MCONTEXT = 0x7A8,
    NDS_SCONTEXT = 0x7AA,

    /* Debug Mode CSRs */
    NDS_DSCRATCH0 = 0x7B2,
    NDS_DSCRATCH1 = 0x7B3,
    NDS_DEXC2DBG  = 0x7E0,
    NDS_DDCAUSE   = 0x7E1,

    /* CLIC Extension CSRs */
    NDS_MTVT         = 0x307,
    NDS_MNXTI        = 0x345,
    NDS_MINTTHRESH   = 0x347,
    NDS_MSCRATCHCSW  = 0x348,
    NDS_MSCRATCHCSWL = 0x349,

    /* Vector CSRs */
    NDS_VSTART = 0x008,
    NDS_VXSAT  = 0x009,
    NDS_VXRM   = 0x00A,
    NDS_VCSR   = 0x00F,
    NDS_VL     = 0xC20,
    NDS_VTYPE  = 0xC21,
    NDS_VLENB  = 0xC22,

    /* RISC-V Architecture */
    NDS_MRVARCH_CFG  = 0XFCA,
    NDS_MRVARCH_CFG2 = 0XFCB,

    /* L2 cache control base address */
    NDS_ML2C_CTL_BASE    = 0XFCF,
    NDS_MCCACHE_CTL_BASE = 0xFCF,
} riscv_core;

enum riscv_fence
{
    FENCE_W = 1,
    FENCE_R,
    FENCE_RW,
    FENCE_O,
    FENCE_OW,
    FENCE_OR,
    FENCE_ORW,
    FENCE_I,
    FENCE_IW,
    FENCE_IR,
    FENCE_IRW,
    FENCE_IO,
    FENCE_IOW,
    FENCE_IOR,
    FENCE_IORW
};

#endif
