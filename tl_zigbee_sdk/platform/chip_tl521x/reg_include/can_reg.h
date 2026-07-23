/********************************************************************************************************
 * @file    can_reg.h
 *
 * @brief   This is the header file for tl521x
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef CAN_REG_H_
#define CAN_REG_H_
#include "soc.h"
/*******************************      can0 registers: 0x80144000      ******************************/

#define can_base_addr(i)                     ((void)(i), (0x80144000)) /* can[0] */
#define can_mb_addr_start(i)                 can_base_addr(i)+0x80
enum{
    TX_MAILBOX_CODE               = BIT_RNG(24,27),
};

#define can_legacy_rx_fifo_filter_addr_start(i)      can_base_addr(i)+0xe0

#define can_enhanced_rx_fifo_filter_addr_start(i)    can_base_addr(i)+0x3000
#define can_enhanced_rx_fifo_addr_start(i)           (can_base_addr(i)+(0x2000))


#define can_individual_mask_addr_start(i)            can_base_addr(i)+0x880

#define reg_can_mcr0(i)                              REG_ADDR8(can_base_addr(i))

enum{
    FLD_CAN_MAXMB             =    BIT_RNG(0,6),
    FLD_CAN_DOZE_REQ          =    BIT(7),
};


#define reg_can_mcr1(i)                       REG_ADDR8(can_base_addr(i)+0x01)

enum{
    FLD_CAN_IDAM      =   BIT_RNG(0,1), // Identifies the format of the Legacy Rx FIFO ID Filter Table elements:

    FLD_CAN_STOP_REQ  =   BIT(2),

    FLD_CAN_FDEN      =   BIT(3),  //CAN FD Operation Enable

    FLD_CAN_AEN       =   BIT(4),  //abort enable,only tx

    FLD_CAN_LPRIO_EN  =   BIT(5),   //LPRIOEN controls whether the Local Priority feature is enabled or not.

    FLD_CAN_PNET_EN   =   BIT(6),  //Pretended Networking Enable

    FLD_CAN_DMA       =   BIT(7),  //The DMA bit enables or disables the DMA feature. The DMA feature can only be used with the Legacy Rx FIFO or the Enhanced Rx FIFO;
                                   //consequently, either CAN_MCR.RFEN or CAN_ERFCR.ERFEN must be asserted.
};

#define reg_can_mcr2(i)                      REG_ADDR8(can_base_addr(i)+0x02)
enum{
    FLD_CAN_IRMQ      =  BIT(0),    //Individual Rx Masking And Queue Enable:

    FLD_CAN_SRXDIS    =  BIT(1),    //SRXDIS controls whether the CAN is allowed to receive frames transmitted by itself.

    FLD_CAN_DOZE      = BIT(2),     //Doze Mode Enable

    FLD_CAN_WAKSRC    = BIT(3),     //1: CAN uses the filtered Rx input to detect recessive-to- dominant edges on the CAN bus.
                                    //0: CAN uses the unfiltered Rx input to detect recessive-to- dominant edges on the CAN bus.

    FLD_CAN_LPMACK    = BIT(4),    //1: CAN is in a low-power mode.
                                   //0: CAN is not in a low-power mode.

    FLD_CAN_WRN_EN    = BIT(5),    //Warning Interrupt Switch Enable

    FLD_CAN_SLF_WAK   = BIT(6),    //Enables the Self Wake Up feature when the CAN is in a low-power mode other than Module Disable:

    FLD_CAN_SUPV      = BIT(7),    //Configures the CAN to be either in Supervisor or User mode
};

#define  reg_can_mcr3(i)                 REG_ADDR8(can_base_addr(i)+0x03)
enum{
    FLD_CAN_FRZACK     =  BIT(0),    // 1: CAN in Freeze mode, prescaler stopped.
                                     // 0: CAN not in Freeze mode, prescaler running.

    FLD_CAN_SOFT_RST   =  BIT(1),    //soft reset

    FLD_CAN_WAK_MASK   =  BIT(2),    //Wake Up Interrupt Mask - Enables Wake Up interrupt generation under the Self Wake Up mechanism

    FLD_CAN_NOT_READY  =  BIT(3),    //1: CAN is either in Module Disable, Doze, Stop, or Freeze mode.
                                     //0: CAN is either in Normal, Listen-Only, or Loop-Back mode.

    FLD_CAN_HALT       =  BIT(4),     //1: Enter Freeze mode if the FRZ bit is asserted.
                                      //0: No Freeze mode request

    FLD_CAN_RFEN       = BIT(5),      //1: Enable Legacy Rx FIFO.
                                     //0: Disable Legacy Rx FIFO.

    FLD_CAN_FRZ        = BIT(6),     //1: Enabled to enter Freeze mode.
                                     //0: Not enabled to enter Freeze mode.

    FLD_CAN_MDIS       = BIT(7),      //1: Disable the CAN module.
                                      //0: Enable the CAN module.

};

#define  reg_can_ctrl1(i)             REG_ADDR32(can_base_addr(i)+0x04)
#define  reg_can_ctrl1_0(i)           REG_ADDR8(can_base_addr(i)+0x04)
enum{
    FLD_CAN_PROPSEG      =  BIT_RNG(0,2),  //Propagation segment time = (PROPSEG + 1) × time-quanta.

    FLD_CAN_LOM          = BIT(3),         //In Listen-Only Mode mode, transmission is disabled

    FLD_CAN_LBUF         = BIT(4),         //1: Lowest number buffer is transmitted first.
                                           //0: Buffer with highest priority is transmitted first.
    FLD_CAN_TSYN         = BIT(5),         //Enables a mechanism that resets the free-running timer each time a message is received in message buffer 0

    FLD_CAN_BOFFREC      = BIT(6),         //0: Automatic recovering from Bus Off state enabled.
                                           //1: Automatic recovering from Bus Off state disabled.

    FLD_CAN_SMP          = BIT(7),         //0: One sample is used to determine the bit value.
                                           //1: Three samples are used to determine the value of the received bit: the regular sample (sample point) and two preceding samples; a majority rule is used.
};


#define reg_can_ctrl1_1(i)             REG_ADDR8(can_base_addr(i)+0x05)
enum{
    FLD_DMA_ADDR_CTRL            = BIT(1),
    FLD_CAN_RX_WARN_MASK         = BIT(2),   //Provides a mask for the Rx Warning interrupt

    FLD_CAN_TX_WARN_MASK         = BIT(3),   //Provides a mask for the Tx Warning interrupt

    FLD_CAN_LPB                  = BIT(4),   //1: Loop-Back enabled.
                                             //0: Loop-Back disabled.

    FLD_CAN_CLK_SRC              = BIT(5),   //CAN Engine Clock Source

    FLD_CAN_ERR_MASK             = BIT(6),   //0: Error interrupt disabled.
                                             //1: Error interrupt enabled.

    FLD_CAN_BUS_OFF_MASK         = BIT(7), //Provides a mask for the Bus Off interrupt
};


#define reg_can_ctrl1_2(i)            REG_ADDR8(can_base_addr(i)+0x06)
enum{
    FLD_CAN_PSEG2           = BIT_RNG(0,2), //Defines the length of phase segment 2 in the bit time.

    FLD_CAN_PSEG1           = BIT_RNG(3,5), //Defines the length of phase segment 1 in the bit time

    FLD_CAN_RJW             = BIT_RNG(6,7), // Defines the maximum number of time quanta that a bit time can be changed by one re-synchronization
};


#define reg_can_ctrl1_3(i)        REG_ADDR8(can_base_addr(i)+0x07)
enum{
    FLD_CAN_PRESDIV      = BIT_RNG(0,7),    //Sclock frequency = PE clock frequency / (PRESDIV + 1)
};



#define  reg_can_timer(i)          REG_ADDR16(can_base_addr(i)+0x08)  // Contains the free-running counter value

#define reg_can_rx_mail_mask(i)    REG_ADDR32(can_base_addr(i)+0x10)  //Rx Mailboxes Global Mask Bits

#define reg_can_rx_mail_mask_14(i)  REG_ADDR32(can_base_addr(i)+0x14)  //RX Mailboxes 14 MASK MATCH

#define reg_can_rx_mail_mask_15(i)  REG_ADDR32(can_base_addr(i)+0x18) //RX Mailboxes 15 MASK MATCH

#define reg_can_tx_err_cnt(i)       REG_ADDR8(can_base_addr(i)+0x1c)    //Transmit Error Counter - Transmit error counter for all errors detected in transmitted messages.

#define reg_can_rx_err_cnt(i)       REG_ADDR8(can_base_addr(i)+0x1d)   //Receive Error Counter - Receive error counter for all errors detected in received messages.

//Transmit Error Counter for Fast Bits - Transmit error counter for errors detected in the Data Phase of transmitted CAN FD messages with the BRS bit set.
#define reg_can_tx_err_cnt_fast(i)   REG_ADDR8(can_base_addr(i)+0x1e)

//Receive Error Counter for Fast Bits - Receive error counter for errors detected in the Data Phase of received CAN FD messages with the BRS bit set.
#define reg_can_rx_err_cnt_fast(i)   REG_ADDR8(can_base_addr(i)+0x1f)

#define reg_can_esr1(i)              REG_ADDR32(can_base_addr(i)+0x20)
#define reg_can_esr1_0(i)            REG_ADDR8(can_base_addr(i)+0x20)
enum{
    FLD_CAN_WAK_INT_STATUS      =  BIT(0), //Wake Up Interrupt - Applies when the CAN is in low-power mode under Self Wake Up mechanism, either Doze mode or Stop mode:

    FLD_CAN_ERR_INT_STATUS      =  BIT(1), //ERRINT indicates that at least one of the error bits (BIT1ERR, BIT0ERR, ACKERR, CRCERR, FRMERR, or STFERR) is set.

    FLD_CAN_BUS_OFF_INT_STATUS  = BIT(2),  //CAN module entered Bus Off state.

    FLD_CAN_IS_RX               = BIT(3),  //Indicates if the CAN is receiving a message

    FLD_CAN_FAULT_CONF          = BIT_RNG(4,5),  //00: Error Active 01: Error Passive 1x: Bus Off

    FLD_CAN_IS_TX               = BIT(6),  //Indicates if the CAN is transmitting a message

    FLD_CAN_IDLE                = BIT(7),   //IDLE - Indicates when the CAN bus is in the IDLE state
};

#define reg_can_esr1_1(i)        REG_ADDR8(can_base_addr(i)+0x21)
enum{
    FLD_CAN_RX_WRN              = BIT(0),   //1: RXERRCNT is greater than or equal to 96.
    FLD_CAN_TX_WRN              = BIT(1),   //1: TXERRCNT is greater than or equal to 96.
    FLD_CAN_STFERR              = BIT(2),   //Stuffing Error - Indicates that a stuffing error was detected by the receiver node in a non-FD message or in a CAN FD message arbitration or Data Phase:
    FLD_CAN_FRMERR              = BIT(3),   //Form Error - Indicates that a form error was detected by the receiver node in a non-FD message or in a CAN FD message arbitration or Data Phase
    FLD_CAN_CRCERR              = BIT(4),   //Indicates that a CRC error was detected by the receiver node either in a non-FD message or in the arbitration or Data Phase of a frame in CAN FD format
    FLD_CAN_ACKERR              = BIT(5),   //Indicates that an Acknowledge error was detected by the transmitter node
    FLD_CAN_BIT0ERR             = BIT(6),   // Indicates when an inconsistency occurs between the transmitted and the received bit in a non-CAN FD message or in the arbitration or Data Phase of a CAN FD message
                                            //1: At least one bit sent as dominant is received as recessive.
    FLD_CAN_BIT1ERR             = BIT(7),    //Indicates when an inconsistency occurs between the transmitted and the received bit in a non-CAN FD message or in the arbitration or Data Phase of a CAN FD message:
                                            //1: At least one bit sent as recessive is received as dominant.
};


#define reg_can_esr1_2(i)        REG_ADDR8(can_base_addr(i)+0x22)
enum{
    FLD_CAN_RX_WRN_INT_STATUS    = BIT(0),    //1: The Rx error counter transitioned from less than 96 to greater than or equal to 96
    FLD_CAN_TX_WRN_INT_STATUS    = BIT(1),    //1: The Tx error counter transitioned from less than 96 to greater than or equal to 96.
    FLD_CAN_SYNCH                = BIT(2),    //1: CAN is synchronized to the CAN bus.
                                              //0: CAN is not synchronized to the CAN bus.
    FLD_CAN_BUS_OFF_DONE_INT_STATUS  = BIT(3), //BOFFDONEINT is set when the Tx Error Counter (TXERRCNT) has finished counting 128 occurrences of 11 consecutive recessive bits on the CAN bus and is ready to leave Bus Off
    FLD_CAN_ERR_FAST_INT_STATUS     = BIT(4),  //ERRINT_FAST indicates that at least one of the error bits detected in the Data Phase of CAN FD frames with the BRS bit set (BIT1ERR_FAST, BIT0ERR_FAST, CRCERR_FAST, FRMERR_FAST, or STFERR_FAST) is set.
    FLD_CAN_ERR_OVERRUN_STATUS      = BIT(5), //Indicates that an error condition occurred when any error flag is already set:
};

#define reg_can_esr1_3(i)      REG_ADDR8(can_base_addr(i)+0x23)
enum{
    FLD_CAN_STF_ERR_FAST      = BIT(2),
    FLD_CAN_FRM_ERR_FAST      = BIT(3),
    FLD_CAN_CRC_ERR_FAST      = BIT(4),
    FLD_CAN_BIT0_ERR_FAST     = BIT(6),
    FLD_CAN_BIT1_ERR_FAST     = BIT(7),
};

//Each bit enables or disables the corresponding CAN Message Buffer Interrupt for MB63 to MB32
#define reg_can_imask2(i)      REG_ADDR32(can_base_addr(i)+0x24)


//Each bit enables or disables the corresponding CAN Message Buffer Interrupt for MB31 to MB0:
#define reg_can_imask1(i)     REG_ADDR32(can_base_addr(i)+0x28)

#define reg_can_imask1_0(i)   REG_ADDR8(can_base_addr(i)+0x28)

//Each bit flags the corresponding CAN Message Buffer Interrupt for MB63 to MB32
#define reg_can_iflag2(i)      REG_ADDR32(can_base_addr(i)+0x2c)

//Each bit flags the corresponding CAN Message Buffer Interrupt for MB31 to MB0
#define reg_can_iflag1(i)      REG_ADDR32(can_base_addr(i)+0x30)
#define reg_can_iflag1_0(i)    REG_ADDR8(can_base_addr(i)+0x30)

#define reg_can_ctrl2(i)       REG_ADDR32(can_base_addr(i)+0x34)
#define reg_can_ctrl2_0(i)     REG_ADDR8(can_base_addr(i)+0x34)
//00: The high-resolution time stamp capture is disabled.
//01: The high-resolution time stamp is captured at the end of the CAN frame.
//10: The high-resolution time stamp is captured at the start of the CAN frame.
//11: The high-resolution time stamp is captured at the start of frame for classical CAN frames and at the res bit for CAN FD frames.
enum{
    FLD_CAN_TIME_STAMP_CAP_POINT     = BIT_RNG(6,7)
};


#define reg_can_ctrl2_1(i)    REG_ADDR8(can_base_addr(i)+0x35)
enum{
    FLD_CAN_MB_TIME_STAMP_BASE     = BIT_RNG(0,1),  //MBTSBASE selects which time base is used for capturing the 16-bit TIME STAMP field of the message buffer
 
    FLD_CAN_EDGE_FILTER_DIS        = BIT(3), //1: Edge filter is disabled.0: Edge filter is enabled.
    //ISOCANFDEN enables ISO CAN FD compliant operation by enabling the following features, which are part of the ISO 11898 standard and not included in the original (Bosch) CAN FD protocol specification:
    FLD_CAN_ISO_CAN_FD_EM         = BIT(4),
    //Bit Timing Expansion Enable - Enables the use of the CAN_EPRS, CAN_EDCBT, and CAN_ENCBT registers to configure the CAN bit timing segments, instead of using the bit timing fields of the CAN_CBT, CAN_FDCBT, and CAN_CTRL1 registers:
    FLD_CAN_BIT_TIMING_EXPEN      = BIT(5),
   
    FLD_CAN_PROTOCOL_EXPEN        = BIT(6),//Protocol Exception Enable - Enables the protocol exception feature

    FLD_CAN_TIMER_SRC             = BIT(7), // Selects the time tick source used for incrementing the free-running timer counter
};

#define reg_can_ctrl2_2(i)        REG_ADDR8(can_base_addr(i)+0x36)
enum{
    //1: Enables the comparison of both the IDE and RTR bits of an Rx mailbox filter with the corresponding bits of the incoming frame. Mask bits do apply.
    //0: IDE bit of Rx mailbox filter is always compared and RTR is never compared, regardless of mask bits.
    FLD_CAN_EACEN                 = BIT(0),
    //If RRS is asserted, a Remote Request Frame is submitted to a matching process and stored in the corresponding message buffer in the same fashion as a Data Frame. No automatic Remote Response Frame will be generated.
    //If RRS is negated, the Remote Request Frame is submitted to a matching process and an automatic Remote Response Frame is generated if a message buffer with CODE=0b1010 is found with the same ID.
    FLD_CAN_RRS                   = BIT(1),
    //1: Matching starts from mailboxes and continues on Legacy Rx FIFO.
    //0: Matching starts from Legacy Rx FIFO and continues on mailboxes.
    FLD_CAN_MRP                   = BIT(2),
    //Tx Arbitration Start Delay - Indicates how many CAN bits the Tx arbitration process start point can be delayed from the first bit of CRC field on CAN bus.
    FLD_CAN_TASD                  = BIT_RNG(3,7),

};


#define reg_can_ctrl2_3(i)          REG_ADDR8(can_base_addr(i)+0x37)
enum{
    //Number of Legacy Rx FIFO Filters - Defines the number of Legacy Rx FIFO filters
    FLD_CAN_RFFN             = BIT_RNG(0,3),
    //Write-Access To Memory In Freeze Mode - Enable unrestricted write access to CAN memory in Freeze mode
    FLD_CAN_WR_MEM_FZ         = BIT(4),
    //Bus Off Done Interrupt Mask - Provides a mask for the Bus Off done interrupt in CAN_ESR1
    FLD_CAN_BUS_OFF_DONE_MASK  = BIT(6),
    //Error Interrupt Mask for errors detected in the Data Phase of fast CAN FD frames
    FLD_CAN_ERR_MASK_FAST    = BIT(7),
};

#define reg_can_esr2_1(i)     REG_ADDR8(can_base_addr(i)+0x39)
enum{
    FLD_CAN_IMB         = BIT(5),
    FLD_CAN_VPS         = BIT(6),
    FLD_CAN_TIMER_WRAP_MASK  = BIT(7),
};


#define reg_can_esr2_2(i)      REG_ADDR8(can_base_addr(i)+0x3a)
enum{
    FLD_CAN_LOWEST_PRI_TM  = BIT_RNG(0,6),
};
#define reg_can_esr2_3(i)      REG_ADDR8(can_base_addr(i)+0x3b)
enum{
    FLD_CAN_TWAINT  = BIT(7),
};
//Transmitted CRC Value - Indicates the CRC value of the last transmitted message for non-FD frames.
#define reg_can_crcr_0(i)     REG_ADDR16(can_base_addr(i)+0x44)

//CRC Mailbox - Indicates the number of the mailbox corresponding to the value in the CAN_CRCR.TXCRC field
#define reg_can_crcr_2(i)     REG_ADDR8(can_base_addr(i)+0x46)

//Legacy Rx FIFO Global Mask Bits
#define reg_can_legacy_rxfifo_gm(i)    REG_ADDR32(can_base_addr(i)+0x48)


//Identifier Acceptance Filter Hit Indicator - Indicates which Identifier Acceptance Filter was hit by the received message that is in the output of the Legacy Rx FIFO.
//If multiple filters match the incoming message ID, the first matching IDAF found (lowest number) by the matching process is indicated. IDHIT is valid only when CAN_IFLAG1.BUF5I is asserted.
#define reg_can_rxfir(i)        REG_ADDR16(can_base_addr(i)+0x4c)


#define reg_can_cbt_0(i)          REG_ADDR8(can_base_addr(i)+0x50)
enum{
    FLD_CAN_EPSEG2              = BIT_RNG(0,4),
    FLD_CAN_EPSEG1_1            = BIT_RNG(5,7),
};

#define reg_can_cbt_1(i)         REG_ADDR8(can_base_addr(i)+0x51)
enum{
    FLD_CAN_EPSEG1_2            = BIT_RNG(0,1),
    FLD_CAN_EPROPSEG            = BIT_RNG(2,7),
};

#define reg_can_cbt_2(i)        REG_ADDR8(can_base_addr(i)+0x52)
enum{
    FLD_CAN_ERJW               = BIT_RNG(0,4),
    FLD_CAN_EPRESDIV_1           = BIT_RNG(5,7),
};

#define reg_can_cbt_3(i)         REG_ADDR8(can_base_addr(i)+0x53)
enum{
    FLD_CAN_EPRESDIV_2          = BIT_RNG(0,6),
    FLD_CAN_BTF                 = BIT(7),
};

//Each bit enables or disables the corresponding CAN Message Buffer Interrupt for MB127 to MB96:
#define  reg_can_imask4(i)        REG_ADDR32(can_base_addr(i)+0x68)

//Each bit enables or disables the corresponding CAN Message Buffer Interrupt for MB95 to MB64:
#define reg_can_imask3(i)        REG_ADDR32(can_base_addr(i)+0x6c)

//Each bit flags the corresponding CAN Message Buffer Interrupt for MB127 to MB96
#define reg_can_iflag4(i)        REG_ADDR32(can_base_addr(i)+0x70)

//Each bit flags the corresponding CAN Message Buffer Interrupt for MB95 to MB64
#define reg_can_iflag3(i)        REG_ADDR32(can_base_addr(i)+0x74)


#define can_iflag(i,j)      (((j) < 64) ? ((j<32) ? (can_base_addr(i)+0x30) : (can_base_addr(i)+0x2c)) : ((j<96) ? (can_base_addr(i)+0x74) : ((can_base_addr(i)+0x70))))
#define can_imask(i,j)      (((j) < 64) ? ((j<32) ? (can_base_addr(i)+0x28) : (can_base_addr(i)+0x24)) : ((j<96) ? (can_base_addr(i)+0x6c) : ((can_base_addr(i)+0x68))))

#define reg_can_imask(i,j)        REG_ADDR32(can_imask(i,j))
#define reg_can_iflag(i,j)        REG_ADDR32(can_iflag(i,j))

#define reg_can_ctrl1_pn(i)       REG_ADDR32(can_base_addr(i)+0xb00)
#define reg_can_ctrl1_pn_0(i)     REG_ADDR8(can_base_addr(i)+0xb00)
enum{
    //00: Message ID filtering only.
    //01: Message ID filtering and payload filtering.
    //10: Message ID filtering occurring a specified number of times.
    //11: Message ID filtering and payload filtering a specified number of times.
    FLD_CAN_FCS         = BIT_RNG(0,1),
    //00: Match upon a ID contents against an exact target value.
    //01: Match upon a ID value greater than or equal to a specified target value.
    //10: Match upon a ID value smaller than or equal to a specified target value.
    //11: Match upon a ID value inside a range, greater than or equal to a specified lower limit, and smaller than or equal to a specified upper limit.
    FLD_CAN_IDFS        = BIT_RNG(2,3),

    //00: Match upon a payload contents against an exact target value.
    //01: Match upon a payload value greater than or equal to a specified target value.
    //10: Match upon a payload value smaller than or equal to a specified target value.
    //11: Match upon a payload value inside a range, greater than or equal to a specified lower limit, and smaller than or equal to a specified upper limit.
    FLD_CAN_PLFS         = BIT_RNG(4,5),
};

//Number of Messages Matching the Same Filtering Criteria
#define reg_can_ctrl1_pn_1(i)      REG_ADDR8(can_base_addr(i)+0xb01)


#define reg_can_ctrl1_pn_2(i)      REG_ADDR8(can_base_addr(i)+0xb02)
enum{
    FLD_CAN_WU_MATCH_MASK      = BIT(0),
    FLD_CAN_WU_TIMEOUT_MASK    = BIT(1),
};

//Timeout for No Message Matching the Filtering Criteria,
//MATCHTO defines a timeout value that generates a wakeup event if CAN_MCR.PNET_EN is asserted. If the timeout counter reaches the target value when the CAN is under Pretended Networking mode, a wakeup event is generated.
#define reg_can_ctrl2_pn_0(i)      REG_ADDR16(can_base_addr(i)+0xb04)


//Number of Matches while in Pretended Networking MCOUNTER reports the number of times a given message has matched the predefined filtering criteria for ID and/or PL before a wakeup event.
#define reg_can_wu_mtc_1(i)         REG_ADDR8(can_base_addr(i)+0xb09)

#define reg_can_wu_mtc_2(i)        REG_ADDR8(can_base_addr(i)+0xb0a)
enum{
    //Wake Up by Match Flag Bit - Indicates whether the CAN has detected a matching Rx incoming message that passed the filtering criteria specified in CAN_CTRL1_PN
    FLD_CAN_WU_MATCH_FLAG    = BIT(0),
    //Wake Up by Timeout Flag Bit - Indicates whether the CAN has detected a timeout event during a time interval defined by CAN_CTRL2_PN.MATCHTO:
    FLD_CAN_WU_TIMEOUT_FLAG  = BIT(1),
};

//ID Filter 1 for Pretended Networking Filtering - Defines either the 29 bits of an extended frame format, considering all bits, or the 11 bits of a standard frame format, considering just the 11 leftmost bits.
#define reg_can_flt_id1(i)        REG_ADDR32(can_base_addr(i)+0xb0c)

//Upper Limit for Length of Data Bytes Filter - Specifies the upper limit for the number of data bytes considered valid for payload comparison. FLT_DLC_HI is used as part of payload reception filter.
#define reg_can_flt_dlc_0(i)      REG_ADDR8(can_base_addr(i)+0xb10)
enum{
    FLD_CAN_FLT_DLC_HI       = BIT_RNG(0,3),
};

//Lower Limit for Length of Data Bytes Filter - Specifies the lower limit for the number of data bytes considered valid for payload comparison. FLT_DLC_LO is used as part of payload reception filter.
#define reg_can_flt_dlc_2(i)     REG_ADDR8(can_base_addr(i)+0xb12)

#define reg_can_pl1_lo(i)        REG_ADDR32(can_base_addr(i)+0xb14)
#define reg_can_pl1_lo_0(i)      REG_ADDR8(can_base_addr(i)+0xb14)
#define reg_can_pl1_lo_1(i)      REG_ADDR8(can_base_addr(i)+0xb15)
#define reg_can_pl1_lo_2(i)      REG_ADDR8(can_base_addr(i)+0xb16)
#define reg_can_pl1_lo_3(i)      REG_ADDR8(can_base_addr(i)+0xb17)

#define reg_can_pl1_hi(i)         REG_ADDR32(can_base_addr(i)+0xb18)
#define reg_can_pl1_hi_0(i)       REG_ADDR8(can_base_addr(i)+0xb18)
#define reg_can_pl1_hi_1(i)       REG_ADDR8(can_base_addr(i)+0xb19)
#define reg_can_pl1_hi_2(i)       REG_ADDR8(can_base_addr(i)+0xb1a)
#define reg_can_pl1_hi_3(i)       REG_ADDR8(can_base_addr(i)+0xb1b)

#define reg_can_flt_id2_idmask(i)      REG_ADDR32(can_base_addr(i)+0xb1c)
#define reg_can_flt_id2_idmask_0(i)    REG_ADDR8(can_base_addr(i)+0xb1c)
#define reg_can_flt_id2_idmask_1(i)    REG_ADDR8(can_base_addr(i)+0xb1d)
#define reg_can_flt_id2_idmask_2(i)    REG_ADDR8(can_base_addr(i)+0xb1e)
#define reg_can_flt_id2_idmask_3(i)    REG_ADDR8(can_base_addr(i)+0xb1f)
enum{
    FLD_CAN_FLT_ID2_IDMASK       =BIT_RNG(0,4),
    FLD_CAN_RTR_MASK             = BIT(5),
    FLD_CAN_IDE_MASK             = BIT(6),
};

#define reg_can_pl2_plmask_lo(i)    REG_ADDR32(can_base_addr(i)+0xb20)
#define reg_can_pl2_plmask_lo_0(i)  REG_ADDR8(can_base_addr(i)+0xb20)
#define reg_can_pl2_plmask_lo_1(i)  REG_ADDR8(can_base_addr(i)+0xb21)
#define reg_can_pl2_plmask_lo_2(i)  REG_ADDR8(can_base_addr(i)+0xb22)
#define reg_can_pl2_plmask_lo_3(i)  REG_ADDR8(can_base_addr(i)+0xb23)

#define reg_can_pl2_plmask_hi(i)     REG_ADDR32(can_base_addr(i)+0xb24)
#define reg_can_pl2_plmask_hi_0(i)   REG_ADDR8(can_base_addr(i)+0xb24)
#define reg_can_pl2_plmask_hi_1(i)   REG_ADDR8(can_base_addr(i)+0xb25)
#define reg_can_pl2_plmask_hi_2(i)   REG_ADDR8(can_base_addr(i)+0xb26)
#define reg_can_pl2_plmask_hi_3(i)   REG_ADDR8(can_base_addr(i)+0xb27)

//only read
#define  reg_can_wmb0_cs(i)       REG_ADDR8(can_base_addr(i)+0xb42)
#define  reg_can_wmb0_id(i)       REG_ADDR32(can_base_addr(i)+0xb44)
#define  reg_can_wmb0_data_0(i)   REG_ADDR32(can_base_addr(i)+0xb48)
#define  reg_can_wmb0_data_1(i)   REG_ADDR32(can_base_addr(i)+0xb4c)

#define  reg_can_wmb1_cs(i)       REG_ADDR8(can_base_addr(i)+0xb52)
#define  reg_can_wmb1_id(i)       REG_ADDR32(can_base_addr(i)+0xb54)
#define  reg_can_wmb1_data_0(i)   REG_ADDR32(can_base_addr(i)+0xb58)
#define  reg_can_wmb1_data_1(i)   REG_ADDR32(can_base_addr(i)+0xb5c)

#define  reg_can_wmb2_cs(i)       REG_ADDR8(can_base_addr(i)+0xb62)
#define  reg_can_wmb2_id(i)       REG_ADDR32(can_base_addr(i)+0xb64)
#define  reg_can_wmb2_data_0(i)   REG_ADDR32(can_base_addr(i)+0xb68)
#define  reg_can_wmb2_data_1(i)   REG_ADDR32(can_base_addr(i)+0xb6c)

#define  reg_can_wmb3_cs(i)       REG_ADDR8(can_base_addr(i)+0xb72)
#define  reg_can_wmb3_id(i)       REG_ADDR32(can_base_addr(i)+0xb74)
#define  reg_can_wmb3_data_0(i)   REG_ADDR32(can_base_addr(i)+0xb78)
#define  reg_can_wmb3_data_1(i)   REG_ADDR32(can_base_addr(i)+0xb7c)

//Extended Nominal Phase Prescaler Division Factor Sclock frequency = PE clock frequency / (ENPRESDIV + 1).
#define  reg_can_eprs_0(i)        REG_ADDR16(can_base_addr(i)+0xbf0)
enum{
    FLD_CAN_ENPRESDIV       = BIT_RNG(0,9),
};

#define  reg_can_eprs_2(i)        REG_ADDR16(can_base_addr(i)+0xbf2)
enum{
    FLD_CAN_EDPRESDIV       = BIT_RNG(0,9),
};

//NTSEG1
#define reg_can_encbt_0(i)     REG_ADDR8(can_base_addr(i)+0xbf4)

#define reg_can_encbt_1(i)      REG_ADDR8(can_base_addr(i)+0xbf5)
enum{
    FLD_CAN_NTSEG2_0      = BIT_RNG(4,7),
};

#define reg_can_encbt_2(i)       REG_ADDR8(can_base_addr(i)+0xbf6)
enum{
    FLD_CAN_NTSEG2_1     = BIT_RNG(0,2),
    FLD_CAN_NRJW_0       = BIT_RNG(6,7),
};

#define reg_can_encbt_3(i)       REG_ADDR8(can_base_addr(i)+0xbf7)
enum{
    FLD_CAN_NRJW_1       = BIT_RNG(0,4),
};

#define reg_can_edcbt_0(i)    REG_ADDR8(can_base_addr(i)+0xbf8)
enum{
    FLD_CAN_DTSEG1     = BIT_RNG(0,4),
};

#define reg_can_edcbt_1(i)   REG_ADDR8(can_base_addr(i)+0xbf9)
enum{
    FLD_CAN_DTSEG2    = BIT_RNG(4,7),
};

#define reg_can_edcbt_2(i)    REG_ADDR8(can_base_addr(i)+0xbfa)
enum{
    FLD_CAN_DRJW_0     = BIT_RNG(6,7),
};

#define reg_can_edcbt_3(i)     REG_ADDR8(can_base_addr(i)+0xbfb)
enum{
    FLD_CAN_DRJW_1    = BIT_RNG(0,1),
};

//Enhanced Transceiver Delay Compensation Value
#define reg_can_etdc_0(i)      REG_ADDR8(can_base_addr(i)+0xbfc)

#define reg_can_etdc_1(i)      REG_ADDR8(can_base_addr(i)+0xbfd)
enum{
    FLD_CAN_ETDC_FAIL      = BIT(7),//Transceiver Delay Compensation Fail
};

#define reg_can_etdc_2(i)      REG_ADDR8(can_base_addr(i)+0xbfe)
enum{
    //Enhanced Transceiver Delay Compensation Offset
    FLD_CAN_ETD_COFF    = BIT_RNG(0,6),
};

#define reg_can_etdc_3(i)    REG_ADDR8(can_base_addr(i)+0xbff)
enum{
    FLD_CAN_TDMDIS     = BIT(6),
    FLD_CAN_ETDCEN     = BIT(7),
};


#define reg_can_fdctrl_0(i)     REG_ADDR8(can_base_addr(i)+0xc00)
enum{
    //Transceiver Delay Compensation Value
    FLD_CAN_TDCVAL    = BIT_RNG(0,5),
};


#define reg_can_fdctrl_1(i)     REG_ADDR8(can_base_addr(i)+0xc01)
enum{
    FLD_CAN_TDCOFF       = BIT_RNG(0,4),
    FLD_CAN_TDCFAIL      = BIT(6),
    FLD_CAN_TDCEN        = BIT(7),
};


#define reg_can_fdctrl_2(i)      REG_ADDR8(can_base_addr(i)+0xc02)
enum{
    FLD_CAN_MBDSR0     = BIT_RNG(0,1),
    FLD_CAN_MBDSR1     = BIT_RNG(3,4),
    FLD_CAN_MBDSR2     = BIT_RNG(6,7),
};

//Bit Rate Switch Enable - Enables the effect of the Bit Rate Switch (BRS bit) during the data phase of Tx messages
#define reg_can_fdctrl_3(i)      REG_ADDR8(can_base_addr(i)+0xc03)
enum{
    FLD_CAN_MBDSR3     = BIT_RNG(1,2),
    //Bit Rate Switch Enable - Enables the effect of the Bit Rate Switch (BRS bit) during the data phase of Tx messages
    FLD_CAN_FDRATE     = BIT(7),
};

#define reg_can_fdcbt_0(i)     REG_ADDR8(can_base_addr(i)+0xc04)
enum{
    FLD_CAN_FPSEG2      = BIT_RNG(0,2),
    FLD_CAN_FPSEG1      = BIT_RNG(5,7),
};

#define reg_can_fdcbt_1(i)         REG_ADDR8(can_base_addr(i)+0xc05)
enum{
    FLD_CAN_FPROPSEG     = BIT_RNG(2,6),
};

#define reg_can_fdcbt_2(i)      REG_ADDR8(can_base_addr(i)+0xc06)
enum{
    FLD_CAN_FRJW     = BIT_RNG(0,2),
    FLD_CAN_FPRESDIV_0   = BIT_RNG(4,7),
};

#define reg_can_fdcbt_3(i)       REG_ADDR8(can_base_addr(i)+0xc07)
enum{
    FLD_CAN_FPRESDIV_1    = BIT_RNG(0,5),
};


#define reg_can_fdcrc(i)         REG_ADDR32(can_base_addr(i)+0xc08)


#define reg_can_erfcr_0(i)       REG_ADDR8(can_base_addr(i)+0xc0c)
enum{
    //Enhanced Rx FIFO Watermark - Defines the minimum number of CAN messages stored in the Enhanced Rx FIFO.
    FLD_CAN_ERFWM      = BIT_RNG(0,4),
};


#define reg_can_erfcr_1(i)       REG_ADDR8(can_base_addr(i)+0xc0d)
enum{
    //Number of Enhanced Rx FIFO Filter Elements - Defines the total number of filter elements used during the Enhanced Rx FIFO matching process.
    FLD_CAN_NFE      = BIT_RNG(0,5),
};

#define reg_can_erfcr_2(i)    REG_ADDR8(can_base_addr(i)+0xc0e)
enum{
    //Number of Extended ID Filter Elements - Defines the number of extended ID filter elements used during the Enhanced Rx FIFO matching process.
    FLD_CAN_NEXIF     = BIT_RNG(0,6),
};


#define reg_can_erfcr_3(i)    REG_ADDR8(can_base_addr(i)+0xc0f)
enum{
    FLD_CAN_DMALW     = BIT_RNG(2,6),
    FLD_CAN_ERFEN     = BIT(7),
};

#define reg_can_erfier(i)      REG_ADDR8(can_base_addr(i)+0xc13)
enum{
    FLD_CAN_ERFDAIE     = BIT(4),
    FLD_CAN_ERFWMIIE    = BIT(5),
    FLD_CAN_ERFOVFIE    = BIT(6),
    FLD_CAN_ERFUFWIE    = BIT(7),
};


#define reg_can_erfsr_0(i)     REG_ADDR8(can_base_addr(i)+0xc14)
enum{
    FLD_CAN_ERFEL    = BIT_RNG(0,5),
};


#define reg_can_erfsr_2(i)     REG_ADDR8(can_base_addr(i)+0xc16)
enum{
    FLD_CAN_ERFF    = BIT(0),
    FLD_CAN_ERFE    = BIT(1),
};


#define reg_can_erfsr_3(i)     REG_ADDR8(can_base_addr(i)+0xc17)
enum{
    FLD_CAN_ERFCLR      = BIT(3),
    FLD_CAN_ERFDA       = BIT(4),
    FLD_CAN_ERFWMI      = BIT(5),
    FLD_CAN_ERFOVF      = BIT(6),
    FLD_CAN_ERFUFW      = BIT(7),
};
#endif










