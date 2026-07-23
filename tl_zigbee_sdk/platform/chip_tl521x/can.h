/********************************************************************************************************
 * @file    can.h
 *
 * @brief   This is the header file for tl521x
 *
 * @author  Driver Group
 * @date    2026
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
#include "reg_include/register.h"
#include "string.h"
#include "dma.h"
#include "gpio.h"

/* TSEG1 corresponds to the sum of xPROPSEG and xPSEG1, TSEG2 corresponds to the xPSEG2 value. */
#define MIN_TIME_SEGMENT1 (2)
#define MIN_TIME_SEGMENT2 (2)
#define MAX_TIME_FD_SEGMENT1    (8)
#define MAX_TIME_FD_SEGMENT2     (8)

/* Define the max value of bit timing segments when use different timing register. */
//can
#define MAX_PROPSEG_MAX_PSEG1           (0x0e)
#define MAX_PSEG1                       (0x07)
#define MAX_PSEG2                       (0x07)
#define MAX_PRESDIV                     (0xff)
#define CAN_MIN_TIME_QUANTA (8)

//canfd FDCBT
#define MAX_EPROPSEG        (0x3f)
#define MAX_EPSEG1          (0x1f)
#define MAX_EPSEG2          (0x1f)
#define MAX_EPRESDIV        (0x3ff)
#define CBT_MAX_TIME_QUANTA (1 + MAX_EPROPSEG + 1 + MAX_EPSEG1 + 1 + MAX_EPSEG2 + 1)
#define CBT_MIN_TIME_QUANTA (8)

#define MAX_FPROPSEG          (0x1f)
#define MAX_FPSEG1            (0x07)
#define MAX_FPSEG2            (0x07)
#define MAX_FPRESDIV          (0x3ff)
#define FDCBT_MAX_TIME_QUANTA (1 + MAX_FPROPSEG + 0 + MAX_FPSEG1 + 1 + MAX_FPSEG2 + 1)
#define FDCBT_MIN_TIME_QUANTA (5)

#define MAX_TDCOFF            (0x1f)

//canfd EDCBT
#define MAX_NTSEG1            (0xff)
#define MAX_NTSEG2            (0x7f)
#define MAX_ENPRESDIV         (0x3ff)
#define ENCBT_MAX_TIME_QUANTA (1 + MAX_NTSEG1 + 1 + MAX_NTSEG2 + 1)
#define ENCBT_MIN_TIME_QUANTA (8)

#define MAX_DTSEG1            (0x1f)
#define MAX_DTSEG2            (0x0f)
#define MAX_EDPRESDIV         (0x3ff)
#define EDCBT_MAX_TIME_QUANTA (1 + MAX_DTSEG1 + 1 + MAX_DTSEG2 + 1)
#define EDCBT_MIN_TIME_QUANTA (5)

#define MAX_ETDCOFF (0x7f)


/**
 *  @brief  define can chn
 */
typedef enum
{
    CAN0 = 0,
}can_chn_e;

/**
 *  @brief  can fd timging protocol.
 */
typedef enum
{
    CANFD_EDCBT =0,
    CANFD_FDCBT,
}can_fd_protocol_timing_e;

/**
 *  @brief can protocol timing characteristic configuration structure.
 */
typedef struct
{
//can/canfd nominal phase
   volatile unsigned short presdiv;      /*!< classic can or can fd nominal phase bit rate prescaler. */
   volatile unsigned char  phase_seg1;   /*!< classic can or can fd nominal phase segment 1. */
   volatile unsigned char  phase_seg2;   /*!< classic can or can fd nominal phase segment 2. */
   volatile unsigned char  prop_seg;     /*!< classic can or can fd nominal phase propagation segment. */
//canfd data phase

   volatile unsigned short fd_presdiv;      /*!< can fd data phase bit rate prescaler. */
   volatile unsigned char  fd_phase_seg1;   /*!< can fd data phase phase segment 1. */
   volatile unsigned char  fd_phase_seg2;   /*!< can fd data phase phase segment 2. */
   volatile unsigned char  fd_prop_seg;     /*!< can fd data phase propagation segment. */
   volatile can_fd_protocol_timing_e canfd_timing_mode;
}can_timing_config_t;

/*
 * @brief can fd baud rate configuration
 */
typedef struct
{
    can_fd_protocol_timing_e canfd_timing_mode;
    unsigned int bit_rate;
    unsigned int bit_rate_fd;
}can_fd_baud_rate_t;

/*!
 * @brief can message buffer payload size.
 */
typedef enum
{
    CAN_8BYTE_PERMB  = 0x0, /*!< select 8  bytes per message buffer. */
    CAN_16BYTE_PERMB = 0x1, /*!< select 16 bytes per message buffer. */
    CAN_32BYTE_PERMB = 0x2, /*!< select 32 bytes per message buffer. */
    CAN_64BYTE_PERMB = 0x3, /*!< select 64 bytes per message buffer. */
}can_mb_data_size_e;


/*!
 * @brief can message buffer code for rx buffers.
 */
typedef enum
{
    CAN_RX_MB_INACTIVE = 0x0, /*!< mb is not active.*/
    CAN_RX_MB_FULL     = 0x2, /*!< mb is full.*/
    CAN_RX_MB_EMPTY    = 0x4, /*!< mb is active and empty.*/
    CAN_RX_MB_OVERRUN  = 0x6, /*!< mb is overwritten into a full buffer.*/
    CAN_RX_MB_BUSY     = 0x8, /*!< can is updating the contents of the mb, the cpu must not access the mb.*/
    CAN_RX_MB_RANSWER  = 0xa, /*!< a frame was configured to recognize a remote request frame and transmit a response frame in return.*/
    CAN_RX_MB_NOTUSED  = 0xf, /*!< not used.*/
}can_mb_code_rx_e;

/*!
 * @brief can message buffer code for tx buffers.
 */
typedef enum
{
    CAN_TX_MB_INACTIVE       = 0x8, /*!< mb is not active.*/
    CAN_TX_MB_ABORT          = 0x9, /*!< mb is aborted.*/
    CAN_TX_MB_DATA_OR_REMOTE = 0xc, /*!< mb is a tx data frame(when mb rtr = 0) or mb is a tx remote request frame (when mb rtr = 1).*/
    CAN_TX_MB_TANSWER        = 0xe, /*!< mb is a tx response request frame from an incoming remote request frame.*/
    CAN_TX_MB_NOTUSED        = 0xf, /*!< not used.*/
}can_mb_code_tx_e;

/*!
 * @brief can message buffer configuration.
 */
typedef struct{
    unsigned char mb_max;
    can_mb_data_size_e mb_data_size;
}can_mb_cfg_t;

/*!
 * @brief can fd frame length enum.
 */
typedef enum
{
    CAN_0B_PER_FRAME = 0x0,  /*!< Frame contains 0 valid data bytes. */
    CAN_1B_PER_FRAME,        /*!< Frame contains 1 valid data bytes. */
    CAN_2B_PER_FRAME,        /*!< Frame contains 2 valid data bytes. */
    CAN_3B_PER_FRAME,        /*!< Frame contains 3 valid data bytes. */
    CAN_4B_PER_FRAME,        /*!< Frame contains 4 valid data bytes. */
    CAN_5B_PER_FRAME,        /*!< Frame contains 5 valid data bytes. */
    CAN_6B_PER_FRAME,        /*!< Frame contains 6 valid data bytes. */
    CAN_7B_PER_FRAME,        /*!< Frame contains 7 valid data bytes. */
    CAN_8B_PER_FRAME,        /*!< Frame contains 8 valid data bytes. */
    CAN_12B_PER_FRAME,       /*!< Frame contains 12 valid data bytes. */
    CAN_16B_PER_FRAME,       /*!< Frame contains 16 valid data bytes. */
    CAN_20B_PER_FRAME,       /*!< Frame contains 20 valid data bytes. */
    CAN_24B_PER_FRAME,       /*!< Frame contains 24 valid data bytes. */
    CAN_32B_PER_FRAME,       /*!< Frame contains 32 valid data bytes. */
    CAN_48B_PER_FRAME,       /*!< Frame contains 48 valid data bytes. */
    CAN_64B_PER_FRAME,       /*!< Frame contains 64 valid data bytes. */
}can_fd_frame_length_e;

/*!
 * @brief can message buffer memory map.
 */
typedef struct {
union {                                               /* offset: 0x80 */
    struct {                                          /* offset: 0x80, array step: 0x10 */
      unsigned int cs;                                /**< message buffer 0 cs register..Message Buffer 127 CS Register, array offset: 0x80, array step: 0x10 */
      unsigned int id;                                /**< Message Buffer 0 ID Register..Message Buffer 127 ID Register, array offset: 0x84, array step: 0x10 */
      unsigned int word[2];                           /**< Message Buffer 0 WORD_8B Register..Message Buffer 127 WORD_8B Register, array offset: 0x88, array step: index*0x10, index2*0x4 */
    } mb_8byte[128];
    struct {                                         /* offset: 0x80 */
      struct {                                       /* offset: 0x80, array step: 0x18 */
        unsigned int cs;                             /**< Message Buffer 0 CS Register..Message Buffer 20 CS Register, array offset: 0x80, array step: 0x18 */
        unsigned int id;                             /**< Message Buffer 0 ID Register..Message Buffer 20 ID Register, array offset: 0x84, array step: 0x18 */
        unsigned int word[4];                        /**< Message Buffer 0 WORD_16B Register..Message Buffer 20 WORD_16B Register, array offset: 0x88, array step: index*0x18, index2*0x4 */
      } mb_16byte_0[21];
      unsigned char reserved_0[8];
      struct {                                       /* offset: 0x280, array step: 0x18 */
        unsigned int cs;                             /**< Message Buffer 0 CS Register..Message Buffer 20 CS Register, array offset: 0x80, array step: 0x18 */
        unsigned int id;                             /**< Message Buffer 0 ID Register..Message Buffer 20 ID Register, array offset: 0x84, array step: 0x18 */
        unsigned int word[4];                        /**< Message Buffer 0 WORD_16B Register..Message Buffer 20 WORD_16B Register, array offset: 0x88, array step: index*0x18, index2*0x4 */
     } mb_16byte_1[21];
     unsigned char reserved_1[8];
     struct {                                       /* offset: 0x480, array step: 0x18 */
        unsigned int cs;                            /**< Message Buffer 0 CS Register..Message Buffer 20 CS Register, array offset: 0x80, array step: 0x18 */
        unsigned int id;                            /**< Message Buffer 0 ID Register..Message Buffer 20 ID Register, array offset: 0x84, array step: 0x18 */
        unsigned int word[4];                       /**< Message Buffer 0 WORD_16B Register..Message Buffer 20 WORD_16B Register, array offset: 0x88, array step: index*0x18, index2*0x4 */
     } mb_16byte_2[21];
     unsigned char reserved_2[8];
     struct {                                       /* offset: 0x680, array step: 0x18 */
        unsigned int cs;                            /**< Message Buffer 0 CS Register..Message Buffer 20 CS Register, array offset: 0x80, array step: 0x18 */
        unsigned int id;                            /**< Message Buffer 0 ID Register..Message Buffer 20 ID Register, array offset: 0x84, array step: 0x18 */
        unsigned int word[4];                       /**< Message Buffer 0 WORD_16B Register..Message Buffer 20 WORD_16B Register, array offset: 0x88, array step: index*0x18, index2*0x4 */
     } mb_16byte_3[21];
    } mb_16byte;
    struct {                                         /* offset: 0x80 */
      struct {                                       /* offset: 0x80, array step: 0x28 */
        unsigned int cs;                             /**< Message Buffer 0 CS Register..Message Buffer 11 CS Register, array offset: 0x80, array step: 0x28 */
        unsigned int id;                             /**< Message Buffer 0 ID Register..Message Buffer 11 ID Register, array offset: 0x84, array step: 0x28 */
        unsigned int word[8];                        /**< Message Buffer 0 WORD_32B Register..Message Buffer 11 WORD_32B Register, array offset: 0x88, array step: index*0x28, index2*0x4 */
      } mb_32byte_0[12];
      unsigned char reserved_0[32];
      struct {                                         /* offset: 0x280, array step: 0x28 */
        unsigned int cs;                               /**< Message Buffer 0 CS Register..Message Buffer 11 CS Register, array offset: 0x280, array step: 0x28 */
        unsigned int id;                               /**< Message Buffer 0 ID Register..Message Buffer 11 ID Register, array offset: 0x284, array step: 0x28 */
        unsigned int word[8];                          /**< Message Buffer 0 WORD_32B Register..Message Buffer 11 WORD_32B Register, array offset: 0x288, array step: index*0x28, index2*0x4 */
      } mb_32byte_1[12];
      unsigned char reserved_1[32];
      struct {                                         /* offset: 0x480, array step: 0x28 */
        unsigned int cs;                               /**< Message Buffer 0 CS Register..Message Buffer 11 CS Register, array offset: 0x280, array step: 0x28 */
        unsigned int id;                               /**< Message Buffer 0 ID Register..Message Buffer 11 ID Register, array offset: 0x284, array step: 0x28 */
        unsigned int word[8];                          /**< Message Buffer 0 WORD_32B Register..Message Buffer 11 WORD_32B Register, array offset: 0x288, array step: index*0x28, index2*0x4 */
     } mb_32byte_2[12];
     unsigned char reserved_2[32];
     struct {                                         /* offset: 0x680, array step: 0x28 */
        unsigned int cs;                              /**< Message Buffer 0 CS Register..Message Buffer 11 CS Register, array offset: 0x280, array step: 0x28 */
        unsigned int id;                              /**< Message Buffer 0 ID Register..Message Buffer 11 ID Register, array offset: 0x284, array step: 0x28 */
        unsigned int word[8];                         /**< Message Buffer 0 WORD_32B Register..Message Buffer 11 WORD_32B Register, array offset: 0x288, array step: index*0x28, index2*0x4 */
     } mb_32byte_3[12];
    } mb_32byte;
    struct {                                         /* offset: 0x80 */
      struct {                                       /* offset: 0x80, array step: 0x48 */
        unsigned int cs;                             /**< Message Buffer 0 CS Register..Message Buffer 6 CS Register, array offset: 0x80, array step: 0x48 */
        unsigned int id;                             /**< Message Buffer 0 ID Register..Message Buffer 6 ID Register, array offset: 0x84, array step: 0x48 */
        unsigned int word[16];                       /**< Message Buffer 0 WORD_64B Register..Message Buffer 6 WORD_64B Register, array offset: 0x88, array step: index*0x48, index2*0x4 */
      } mb_64byte_0[7];
      unsigned char reserved_0[8];
      struct {                                         /* offset: 0x280, array step: 0x48 */
        unsigned int cs;                               /**< Message Buffer 0 CS Register..Message Buffer 6 CS Register, array offset: 0x280, array step: 0x48 */
        unsigned int id;                               /**< Message Buffer 0 ID Register..Message Buffer 6 ID Register, array offset: 0x284, array step: 0x48 */
        unsigned int word[16];                         /**< Message Buffer 0 WORD_64B Register..Message Buffer 6 WORD_64B Register, array offset: 0x288, array step: index*0x48, index2*0x4 */
      } mb_64byte_1[7];
      unsigned char reserved_1[8];
      struct {                                         /* offset: 0x480, array step: 0x48 */
        unsigned int cs;                               /**< Message Buffer 0 CS Register..Message Buffer 6 CS Register, array offset: 0x280, array step: 0x48 */
        unsigned int id;                               /**< Message Buffer 0 ID Register..Message Buffer 6 ID Register, array offset: 0x284, array step: 0x48 */
        unsigned int word[16];                         /**< Message Buffer 0 WORD_64B Register..Message Buffer 6 WORD_64B Register, array offset: 0x288, array step: index*0x48, index2*0x4 */
      } mb_64byte_2[7];
      unsigned char reserved_2[8];
      struct {                                         /* offset: 0x680, array step: 0x48 */
        unsigned int cs;                               /**< Message Buffer 0 CS Register..Message Buffer 6 CS Register, array offset: 0x280, array step: 0x48 */
        unsigned int id;                               /**< Message Buffer 0 ID Register..Message Buffer 6 ID Register, array offset: 0x284, array step: 0x48 */
        unsigned int word[16];                         /**< Message Buffer 0 WORD_64B Register..Message Buffer 6 WORD_64B Register, array offset: 0x288, array step: index*0x48, index2*0x4 */
      } mb_64byte_3[7];
    } mb_64byte;
    struct {                                          /* offset: 0x80, array step: 0x10 */
      unsigned int  cs;                               /**< Message Buffer 0 CS Register..Message Buffer 63 CS Register, array offset: 0x80, array step: 0x10 */
      unsigned int  id;                               /**< Message Buffer 0 ID Register..Message Buffer 63 ID Register, array offset: 0x84, array step: 0x10 */
      unsigned int  word0;                            /**< Message Buffer 0 WORD0 Register..Message Buffer 63 WORD0 Register, array offset: 0x88, array step: 0x10 */
      unsigned int  word1;                            /**< Message Buffer 0 WORD1 Register..Message Buffer 63 WORD1 Register, array offset: 0x8C, array step: 0x10 */
    } MB[127];
  };
}mailbox_allocate_t;

/*!
 * @brief can rx mb / legacy rxfifo id individual mask map.
 */
typedef struct {
   volatile unsigned int rx_mb_imr[128];
}can_rx_mb_individual_mask_t;

/*!
 * @brief can enhanced rxfifo id filter map.
 */
typedef struct {
    unsigned int filter[128];
}can_enhanced_rxfifo_filter_t;


/*!
 *  @brief can message frame structure.
 */
typedef struct
{
    struct
    {
        unsigned int timestamp : 16; /*!< CAN internal Free-Running Counter Time Stamp. */
        unsigned int length : 4;     /*!< CAN frame data length in bytes (Range: 0~8). */
        unsigned int type : 1;       /*!< CAN Frame Type(DATA or REMOTE). */
        unsigned int format : 1;     /*!< CAN Frame Identifier(STD or EXT format). */
        unsigned int : 1;            /*!< Reserved. */
        unsigned int idhit : 9;      /*!< CAN Rx FIFO filter hit id(This value is only used in Rx FIFO receive mode). */
    };
    struct
    {
        unsigned int id : 29; /*!< CAN Frame Identifier, should be set using CAN_ID_EXT() or CAN_ID_STD() macro. */
        unsigned int prio: 3; /*!< TX priority. */
    };
    union
   {
       struct
       {
           unsigned int data_word0; /*!< CAN Frame payload word0. */
           unsigned int data_word1; /*!< CAN Frame payload word1. */
       };
       struct
       {
           unsigned char data_byte0; /*!< CAN Frame payload byte3. */
           unsigned char data_byte1; /*!< CAN Frame payload byte2. */
           unsigned char data_byte2; /*!< CAN Frame payload byte1. */
           unsigned char data_byte3; /*!< CAN Frame payload byte0. */
           unsigned char data_byte4; /*!< CAN Frame payload byte7. */
           unsigned char data_byte5; /*!< CAN Frame payload byte6. */
           unsigned char data_byte6; /*!< CAN Frame payload byte5. */
           unsigned char data_byte7; /*!< CAN Frame payload byte4. */
       };
   };
}can_frame_t;

/*!
 *  @brief can message frame dma structure.
 */
typedef struct{
    unsigned int length;
    can_frame_t frame;
}can_frame_dma_t;


/*!
 * @brief can fd message frame structure.
 *
 * The CAN FD message supporting up to sixty four bytes can be used for a data frame, depending on the length
 * selected for the message buffers. The length should be a enumeration member, see @ref can_fd_frame_length_e.
 */
typedef struct
{
    struct
    {
        unsigned int timestamp : 16; /*!< CAN internal Free-Running Counter Time Stamp. */
        unsigned int length : 4; /*!< CAN FD frame data length code (DLC), range see @ref can_fd_frame_length_e, When the
                                length <= 8, it equal to the data length, otherwise the number of valid frame data is
                                not equal to the length value.  user can
                                use DLC_LENGTH_DECODE(length) macro to get the number of valid data bytes. */
        unsigned int type : 1;   /*!< CAN Frame Type(DATA or REMOTE). */
        unsigned int format : 1; /*!< CAN Frame Identifier(STD or EXT format). */
        unsigned int srr : 1;    /*!< Substitute Remote request. */
        unsigned int : 6;
        unsigned int esi : 1; /*!< Error State Indicator. */
        unsigned int brs : 1; /*!< Bit Rate Switch. */
        unsigned int edl : 1; /*!< Extended Data Length. */
    };
    struct
    {
        unsigned int id : 29; /*!< CAN Frame Identifier, should be set using CAN_ID_EXT() or CAN_ID_STD() macro. */
        unsigned int prio: 3; /*!< TX priority. */
    };
    union
    {
        struct
        {
            unsigned int data_word[16]; /*!< CAN FD Frame payload, 16 double word maximum. */
        };
        /* Note: the maximum databyte* below is actually 64, user can add them if needed,
           or just use dataWord[*] instead. */
        struct
        {
            unsigned char data_byte0; /*!< CAN Frame payload byte3. */
            unsigned char data_byte1; /*!< CAN Frame payload byte2. */
            unsigned char data_byte2; /*!< CAN Frame payload byte1. */
            unsigned char data_byte3; /*!< CAN Frame payload byte0. */
            unsigned char data_byte4; /*!< CAN Frame payload byte7. */
            unsigned char data_byte5; /*!< CAN Frame payload byte6. */
            unsigned char data_byte6; /*!< CAN Frame payload byte5. */
            unsigned char data_byte7; /*!< CAN Frame payload byte4. */
        };
    };

    /*! @note ID HIT offset is changed dynamically according to data length code (DLC), when DLC is 15, they will be
     * located below.*/
    unsigned int idhit; /*!< CAN Enhanced Rx FIFO filter hit id (This value is only used in Enhanced Rx FIFO receive
                           mode). */
    unsigned int hr_time_stamp;
}can_fd_frame_t;

/*!
 *  @brief CAN frame format.
 */
typedef enum
{
    CAN_FRAME_FORMAT_STANDARD = 0x0, /*!< Standard frame format attribute. */
    CAN_FRAME_FORMAT_EXTEND   = 0x1, /*!< Extend frame format attribute. */
} can_frame_format_t;

/*!
 * @brief CAN frame type.
 */
typedef enum
{
    CAN_FRAME_TYPE_DATA   = 0x0, /*!< Data frame type attribute. */
    CAN_FRAME_TYPE_REMOTE = 0x1, /*!< Remote frame type attribute. */
} can_frame_type_t;

/*!
 * @brief CAN remote frame type.
 *
 * when select CAN_FRAME_TYPE_DATA, select NONE.
 */
typedef enum{
     REMOTE_REQUSET_NO_STORING  = 0x00,
     REMOTE_REQUEST_STORING     = 0x01,
     NONE                       = 0x02,
}can_remote_received_type_e;

/*!
 * @brief the can rx mailbox configuration structure
 *
 * This structure is used as the parameter of can_set_rx_mailbox_cfg() function.
 * The can_set_rx_mailbox_cfg() function is used to configure can receive
 * message buffer. The function abort previous receiving process, clean the
 * message buffer and activate the rx message buffer using given message buffer
 * setting.
 */
typedef struct
{
    unsigned int id;                    /*!< CAN Message Buffer Frame Identifier, should be set using
                                        CAN_ID_EXT() or CAN_ID_STD() macro. */
    can_frame_format_t format;      /*!< CAN Frame Identifier format(Standard of Extend). */
    can_frame_type_t type;          /*!< CAN Frame Type(Data or Remote). */
    can_remote_received_type_e remote_type;
} can_rx_mb_config_t;

/*!
 * @brief the can fd rx mailbox configuration structure
 *
 * 1.This structure is used as the parameter of can_fd_set_rx_mailbox_cfg() function.
 * The can_fd_set_rx_mailbox_cfg() function is used to configure canfd receive
 * message buffer. The function abort previous receiving process, clean the
 * message buffer and activate the rx message buffer using given message buffer
 * setting.
 * 2. there is no remote frame in the can fd format.
 */
typedef struct
{
    unsigned int id;                    /*!< CAN Message Buffer Frame Identifier, should be set using
                                        CAN_ID_EXT() or CAN_ID_STD() macro. */
    can_frame_format_t format;      /*!< CAN Frame Identifier format(Standard of Extend). */
} can_fd_rx_mb_config_t;


/*! @name CS - Message Buffer 0 CS Register..Message Buffer 6 CS Register */
/*! @{ */

#define CAN_CS_TIME_STAMP_MASK                   (0xFFFF)
#define CAN_CS_TIME_STAMP_SHIFT                  (0)
/*! TIME_STAMP - Free-Running Counter Time stamp. This 16-bit field is a copy of the Free-Running
 *    Timer, captured for Tx and Rx frames at the time when the beginning of the Identifier field
 *    appears on the CAN bus.
 */
#define CAN_CS_TIME_STAMP(x)                     (((unsigned int)(((unsigned int)(x)) << CAN_CS_TIME_STAMP_SHIFT)) & CAN_CS_TIME_STAMP_MASK)

#define CAN_CS_DLC_MASK                          (0xF0000)
#define CAN_CS_DLC_SHIFT                         (16)
/*! DLC - Length of the data to be stored/transmitted. */
#define CAN_CS_DLC(x)                            (((unsigned int)(((unsigned int)(x)) << CAN_CS_DLC_SHIFT)) & CAN_CS_DLC_MASK)

#define CAN_CS_RTR_MASK                          (0x100000)
#define CAN_CS_RTR_SHIFT                         (20)
/*! RTR - Remote Transmission Request. One/zero for remote/data frame. */
#define CAN_CS_RTR(x)                            (((unsigned int)(((unsigned int)(x)) << CAN_CS_RTR_SHIFT)) & CAN_CS_RTR_MASK)

#define CAN_CS_IDE_MASK                          (0x200000)
#define CAN_CS_IDE_SHIFT                         (21)
/*! IDE - ID Extended. One/zero for extended/standard format frame. */
#define CAN_CS_IDE(x)                            (((unsigned int)(((unsigned int)(x)) << CAN_CS_IDE_SHIFT)) & CAN_CS_IDE_MASK)

#define CAN_CS_SRR_MASK                          (0x400000)
#define CAN_CS_SRR_SHIFT                         (22)
/*! SRR - Substitute Remote Request. Contains a fixed recessive bit. */
#define CAN_CS_SRR(x)                            (((unsigned int)(((unsigned int)(x)) << CAN_CS_SRR_SHIFT)) & CAN_CS_SRR_MASK)

#define CAN_CS_CODE_MASK                         (0xF000000)
#define CAN_CS_CODE_SHIFT                        (24)
/*! CODE - Message Buffer Code. This 4-bit field can be accessed (read or write) by the CPU and by
 *    the CAN module itself, as part of the message buffer matching and arbitration process.
 */
#define CAN_CS_CODE(x)                           (((unsigned int)(((unsigned int)(x)) << CAN_CS_CODE_SHIFT)) & CAN_CS_CODE_MASK)

#define CAN_CS_ESI_MASK                          (0x20000000)
#define CAN_CS_ESI_SHIFT                         (29)
/*! ESI - Error State Indicator. This bit indicates if the transmitting node is error active or error passive. */
#define CAN_CS_ESI(x)                            (((unsigned int)(((unsigned int)(x)) << CAN_CS_ESI_SHIFT)) & CAN_CS_ESI_MASK)

#define CAN_CS_BRS_MASK                          (0x40000000)
#define CAN_CS_BRS_SHIFT                         (30)
/*! BRS - Bit Rate Switch. This bit defines whether the bit rate is switched inside a CAN FD format frame. */
#define CAN_CS_BRS(x)                            (((unsigned int)(((unsigned int)(x)) << CAN_CS_BRS_SHIFT)) & CAN_CS_BRS_MASK)

#define CAN_CS_EDL_MASK                          (0x80000000)
#define CAN_CS_EDL_SHIFT                         (31)
/*! EDL - Extended Data Length. This bit distinguishes between CAN format and CAN FD format frames.
 *    The EDL bit must not be set for Message Buffers configured to RANSWER with code field 0b1010.
 */
#define CAN_CS_EDL(x)                            (((unsigned int)(((unsigned int)(x)) << CAN_CS_EDL_SHIFT)) & CAN_CS_EDL_MASK)
/*! @} */

/*!
 * @brief can rx mailbox filter id and mask.
 */
#define CAN_ID_STD_MASK                          (0x1FFC0000)
#define CAN_ID_STD_SHIFT                         (18)
#define CAN_ID_EXT_MASK                          (0x3FFFF)
#define CAN_ID_EXT_SHIFT                         (0)
//id
#define CAN_ID_STD(id)  \
        (((unsigned int)(((unsigned int)(id)) << CAN_ID_STD_SHIFT)) & CAN_ID_STD_MASK)
#define CAN_ID_EXT(id)  \
         (((unsigned int)(((unsigned int)(id)) << CAN_ID_EXT_SHIFT)) & (CAN_ID_EXT_MASK | CAN_ID_STD_MASK))
//mask
#define CAN_RX_MB_STD_MASK(id, rtr, ide)                                   \
    (((unsigned int)((unsigned int)(rtr) << 31) | (unsigned int)((unsigned int)(ide) << 30)) | \
     CAN_ID_STD(id))
#define CAN_RX_MB_EXT_MASK(id, rtr, ide)                                   \
    (((unsigned int)((unsigned int)(rtr) << 31) | (unsigned int)((unsigned int)(ide) << 30)) | \
     CAN_ID_EXT(id))

/*!
 * @brief can enhanced/legacy rxfifo priority.
 *
 * The matching process starts from the Rx MB(or Enhanced/Legacy Rx FIFO) with higher priority.
 * If no MB(or Enhanced/Legacy Rx FIFO filter) is satisfied, the matching process goes on with
 * the Enhanced/Legacy Rx FIFO(or Rx MB) with lower priority.
 */
typedef enum
{
    CAN_RXFIFO_PRIO_LOW   = 0x0, /*!< Matching process start from Rx Message Buffer first. */
    CAN_RXFIFO_PRIO_HIGH  = 0x1, /*!< Matching process start from Enhanced/Legacy Rx FIFO first. */
} can_rx_fifo_priority_t;

/*!
 *  @brief can legacy rxfio filter type.
 */
typedef enum
{
    CAN_RXFIFO_FILTER_TYPE_A = 0x0, /*!< One full ID (standard and extended) per ID Filter element. */
    CAN_RXFIFO_FILTER_TYPE_B = 0x1, /*!< Two full standard IDs or two partial 14-bit ID slices per ID Filter Table element. */
    CAN_RXFIFO_FILTER_TYPE_C = 0x2, /*!< Four partial 8-bit Standard or extended ID slices per ID Filter Table element. */
    CAN_RXFIFO_FILTER_TYPE_D = 0x3, /*!< All frames rejected. */
} can_rx_fifo_filter_type_t;

/*!
 *  @brief can legacy rxfifo mask macro.
 */
//std mask
#define CAN_RX_FIFO_STD_MASK_TYPE_A(id, rtr, ide)                          \
    (((unsigned int)((unsigned int)(rtr) << 31) | (unsigned int)((unsigned int)(ide) << 30)) | \
    (CAN_ID_STD(id) << 1)) /*!< Standard Rx FIFO Mask helper macro Type A helper macro. */
#define CAN_RX_FIFO_STD_MASK_TYPE_B_HIGH(id, rtr, ide)                     \
    (((unsigned int)((unsigned int)(rtr) << 31) | (unsigned int)((unsigned int)(ide) << 30)) | \
     (((unsigned int)(id)&0x7FF) << 19)) /*!< Standard Rx FIFO Mask helper macro Type B upper part helper macro. */
#define CAN_RX_FIFO_STD_MASK_TYPE_B_LOW(id, rtr, ide)                      \
    (((unsigned int)((unsigned int)(rtr) << 15) | (unsigned int)((unsigned int)(ide) << 14)) | \
     (((unsigned int)(id)&0x7FF) << 3)) /*!< Standard Rx FIFO Mask helper macro Type B lower part helper macro. */
#define CAN_RX_FIFO_STD_MASK_TYPE_C_HIGH(id) \
    (((unsigned int)(id)&0x7F8) << 21) /*!< Standard Rx FIFO Mask helper macro Type C upper part helper macro. */
#define CAN_RX_FIFO_STD_MASK_TYPE_C_MID_HIGH(id) \
    (((unsigned int)(id)&0x7F8) << 13) /*!< Standard Rx FIFO Mask helper macro Type C mid-upper part helper macro. */
#define CAN_RX_FIFO_STD_MASK_TYPE_C_MID_LOW(id) \
    (((unsigned int)(id)&0x7F8) << 5) /*!< Standard Rx FIFO Mask helper macro Type C mid-lower part helper macro. */
#define CAN_RX_FIFO_STD_MASK_TYPE_C_LOW(id) \
    (((unsigned int)(id)&0x7F8) >> 3) /*!< Standard Rx FIFO Mask helper macro Type C lower part helper macro. */

//ext mask
#define CAN_RX_FIFO_EXT_MASK_TYPE_A(id, rtr, ide)                          \
    (((unsigned int)((unsigned int)(rtr) << 31) | (unsigned int)((unsigned int)(ide) << 30)) | \
     (CAN_ID_EXT(id) << 1)) /*!< Extend Rx FIFO Mask helper macro Type A helper macro. */
#define CAN_RX_FIFO_EXT_MASK_TYPE_B_HIGH(id, rtr, ide)                        \
    (                                                                             \
        ((unsigned int)((unsigned int)(rtr) << 31) | (unsigned int)((unsigned int)(ide) << 30)) | \
        ((CAN_ID_EXT(id) & 0x1FFF800) << 1)) /*!< Extend Rx FIFO Mask helper macro Type B upper part helper macro. */
#define CAN_RX_FIFO_EXT_MASK_TYPE_B_LOW(id, rtr, ide)                      \
    (((unsigned int)((unsigned int)(rtr) << 15) | (unsigned int)((unsigned int)(ide) << 14)) | \
     ((CAN_ID_EXT(id) & 0x1FFF8000) >>                                     \
      15)) /*!< Extend Rx FIFO Mask helper macro Type B lower part helper macro. */
#define CAN_RX_FIFO_EXT_MASK_TYPE_C_HIGH(id) \
    (CAN_ID_EXT(id) & 0x1FE00000) << 3) /*!< Extend Rx FIFO Mask helper macro Type C upper part helper macro. */
#define CAN_RX_FIFO_EXT_MASK_TYPE_C_MID_HIGH(id) \
    ((CAN_ID_EXT(id) & 0x1FE00000) >>            \
     5) /*!< Extend Rx FIFO Mask helper macro Type C mid-upper part helper macro. */
#define CAN_RX_FIFO_EXT_MASK_TYPE_C_MID_LOW(id) \
    ((CAN_ID_EXT(id) & 0x1FE00000) >>           \
     13) /*!< Extend Rx FIFO Mask helper macro Type C mid-lower part helper macro. */
#define CAN_RX_FIFO_EXT_MASK_TYPE_C_LOW(id) \
    ((CAN_ID_EXT(id) & 0x1FE00000) >> 21) /*!< Extend Rx FIFO Mask helper macro Type C lower part helper macro. */

/*!
 * @brief can legacy rxfifo filter helper macro.
 */
//std
#define CAN_RX_FIFO_STD_FILTER_TYPE_A(id, rtr, ide) \
    CAN_RX_FIFO_STD_MASK_TYPE_A(id, rtr, ide) /*!< Standard Rx FIFO Filter helper macro Type A helper macro. */
#define CAN_RX_FIFO_STD_FILTER_TYPE_B_HIGH(id, rtr, ide) \
   CAN_RX_FIFO_STD_MASK_TYPE_B_HIGH(                    \
        id, rtr, ide) /*!< Standard Rx FIFO Filter helper macro Type B upper part helper macro. */
#define CAN_RX_FIFO_STD_FILTER_TYPE_B_LOW(id, rtr, ide) \
    CAN_RX_FIFO_STD_MASK_TYPE_B_LOW(                    \
        id, rtr, ide) /*!< Standard Rx FIFO Filter helper macro Type B lower part helper macro. */
#define CAN_RX_FIFO_STD_FILTER_TYPE_C_HIGH(id) \
    CAN_RX_FIFO_STD_MASK_TYPE_C_HIGH(          \
        id) /*!< Standard Rx FIFO Filter helper macro Type C upper part helper macro. */
#define CAN_RX_FIFO_STD_FILTER_TYPE_C_MID_HIGH(id) \
    CAN_RX_FIFO_STD_MASK_TYPE_C_MID_HIGH(          \
        id) /*!< Standard Rx FIFO Filter helper macro Type C mid-upper part helper macro. */
#define CAN_RX_FIFO_STD_FILTER_TYPE_C_MID_LOW(id) \
    CAN_RX_FIFO_STD_MASK_TYPE_C_MID_LOW(          \
        id) /*!< Standard Rx FIFO Filter helper macro Type C mid-lower part helper macro. */
#define CAN_RX_FIFO_STD_FILTER_TYPE_C_LOW(id) \
    CAN_RX_FIFO_STD_MASK_TYPE_C_LOW(          \
        id) /*!< Standard Rx FIFO Filter helper macro Type C lower part helper macro.  */

//ext
#define CAN_RX_FIFO_EXT_FILTER_TYPE_A(id, rtr, ide) \
    CAN_RX_FIFO_EXT_MASK_TYPE_A(id, rtr, ide) /*!< Extend Rx FIFO Filter helper macro Type A helper macro. */
#define CAN_RX_FIFO_EXT_FILTER_TYPE_B_HIGH(id, rtr, ide) \
    CAN_RX_FIFO_EXT_MASK_TYPE_B_HIGH(                    \
        id, rtr, ide) /*!< Extend Rx FIFO Filter helper macro Type B upper part helper macro. */
#define CAN_RX_FIFO_EXT_FILTER_TYPE_B_LOW(id, rtr, ide) \
    CAN_RX_FIFO_EXT_MASK_TYPE_B_LOW(                    \
        id, rtr, ide) /*!< Extend Rx FIFO Filter helper macro Type B lower part helper macro. */
#define CAN_RX_FIFO_EXT_FILTER_TYPE_C_HIGH(id) \
    CAN_RX_FIFO_EXT_MASK_TYPE_C_HIGH(          \
        id) /*!< Extend Rx FIFO Filter helper macro Type C upper part helper macro.           */
#define CAN_RX_FIFO_EXT_FILTER_TYPE_C_MID_HIGH(id) \
    CAN_RX_FIFO_EXT_MASK_TYPE_C_MID_HIGH(          \
        id) /*!< Extend Rx FIFO Filter helper macro Type C mid-upper part helper macro. */
#define CAN_RX_FIFO_EXT_FILTER_TYPE_C_MID_LOW(id) \
    CAN_RX_FIFO_EXT_MASK_TYPE_C_MID_LOW(          \
        id) /*!< Extend Rx FIFO Filter helper macro Type C mid-lower part helper macro. */
#define CAN_RX_FIFO_EXT_FILTER_TYPE_C_LOW(id) \
    CAN_RX_FIFO_EXT_MASK_TYPE_C_LOW(id) /*!< Extend Rx FIFO Filter helper macro Type C lower part helper macro. */

/*!
 * @brief can legacy rxfifo configuration structure.
 */
typedef struct
{
    unsigned int *id_filter_table;                    /*!< Pointer to the CAN Legacy Rx FIFO identifier filter table. */
    unsigned char id_filter_num;                      /*!< The CAN Legacy Rx FIFO Filter elements quantity. */
    can_rx_fifo_filter_type_t id_filter_type;     /*!< The CAN Legacy Rx FIFO Filter type. */
    can_rx_fifo_priority_t priority;              /*!< The CAN Legacy Rx FIFO receive priority. */
} can_rx_fifo_config_t;


/*!
 * @brief can enhanced rxfifo filter and mask helper macro.
 */
#define ENHANCED_RX_FIFO_FSCH(x) (((unsigned int)(((unsigned int)(x)) << 30)) & 0xC0000000)
#define RTR_STD_HIGH(x)          (((unsigned int)(((unsigned int)(x)) << 27)) & 0x08000000)
#define RTR_STD_LOW(x)           (((unsigned int)(((unsigned int)(x)) << 11)) & 0x00000800)
#define RTR_EXT(x)               (((unsigned int)(((unsigned int)(x)) << 29)) & 0x20000000)
#define ID_STD_LOW(id)           (((unsigned int)id) & 0x7FF)
#define ID_STD_HIGH(id)          (((unsigned int)(((unsigned int)(id)) << 16)) & 0x07FF0000)
#define ID_EXT(id)               (((unsigned int)id) & 0x1FFFFFFF)

/*! Standard ID filter element with filter + mask scheme. */
#define CAN_ENHANCED_RX_FIFO_STD_MASK_AND_FILTER(id, rtr, id_mask, rtr_mask) \
    (ENHANCED_RX_FIFO_FSCH(0x0) | RTR_STD_HIGH(rtr) | ID_STD_HIGH(id) | RTR_STD_LOW(rtr_mask) | ID_STD_LOW(id_mask))
/*! Standard ID filter element with filter range. */
#define CAN_ENHANCED_RX_FIFO_STD_FILTER_WITH_RANGE(id_upper, rtr, id_lower, rtr_mask)             \
    (ENHANCED_RX_FIFO_FSCH(0x1) | RTR_STD_HIGH(rtr) | ID_STD_HIGH(id_upper) | RTR_STD_LOW(rtr_mask) | \
     ID_STD_LOW(id_lower))
/*! Standard ID filter element with two filters without masks. */
#define CAN_ENHANCED_RX_FIFO_STD_TWO_FILTERS(id1, rtr1, id2, rtr2) \
    (ENHANCED_RX_FIFO_FSCH(0x2) | RTR_STD_HIGH(rtr1) | ID_STD_HIGH(id1) | RTR_STD_LOW(rtr2) | ID_STD_LOW(id2))
/*! Extended ID filter element with  filter + mask scheme low word. */
#define CAN_ENHANCED_RX_FIFO_EXT_MASK_AND_FILTER_LOW(id, rtr) \
    (ENHANCED_RX_FIFO_FSCH(0x0) | RTR_EXT(rtr) | ID_EXT(id))
/*! Extended ID filter element with  filter + mask scheme high word. */
#define CAN_ENHANCED_RX_FIFO_EXT_MASK_AND_FILTER_HIGH(id_mask, rtr_mask) \
    (ENHANCED_RX_FIFO_FSCH(0x0) | RTR_EXT(rtr_mask) | ID_EXT(id_mask))
/*! Extended ID filter element with range scheme low word. */
#define CAN_ENHANCED_RX_FIFO_EXT_FILTER_WITH_RANGE_LOW(id_upper, rtr) \
    (ENHANCED_RX_FIFO_FSCH(0x1) | RTR_EXT(rtr) | ID_EXT(id_upper))
/*! Extended ID filter element with range scheme high word. */
#define CAN_ENHANCED_RX_FIFO_EXT_FILTER_WITH_RANGE_HIGH(id_lower, rtr_mask) \
    (ENHANCED_RX_FIFO_FSCH(0x1) | RTR_EXT(rtr_mask) | ID_EXT(id_lower))
/*! Extended ID filter element with two filters without masks low word. */
#define CAN_ENHANCED_RX_FIFO_EXT_TWO_FILTERS_LOW(id2, rtr2) \
    (ENHANCED_RX_FIFO_FSCH(0x2) | RTR_EXT(rtr2) | ID_EXT(id2))
/*! Extended ID filter element with two filters without masks high word. */
#define CAN_ENHANCED_RX_FIFO_EXT_TWO_FILTERS_HIGH(id1, rtr1) \
    (ENHANCED_RX_FIFO_FSCH(0x2) | RTR_EXT(rtr1) | ID_EXT(id1))


/*!
 * @brief can enhanced rxfifo configuration structure.
 */
typedef struct
{
    unsigned int *id_filter_table;        /*!< Pointer to the CAN Enhanced Rx FIFO identifier filter table, each table member
                                          occupies 32 bit word, table size should be equal to idFilterNum. There are two types of
                                          Enhanced Rx FIFO filter elements that can be stored in table : extended-ID filter element
                                          (2 word, occupie 2 table members) and standard-ID filter element (1 words, occupies 1 table
                                          members), the extended-ID filter element needs to be placed in front of the table. */
    unsigned char id_filter_pairnum;     /*!< idFilterPairNum is the Enhanced Rx FIFO identifier filter element pair numbers,
                                         each pair of filter elements occupies 2 words and can consist of one extended ID filter
                                         element or two standard ID filter elements. */
    unsigned char extendid_filternum;    /*!< The number of extended ID filter element items in the CAN enhanced Rx FIFO
                                         identifier filter table, each extended-ID filter element occupies 2 words,
                                         extendIdFilterNum need less than or equal to idFilterPairNum. */
    can_rx_fifo_priority_t priority; /*!< The CAN Enhanced Rx FIFO receive priority. */
} can_enhanced_rx_fifo_config_t;

/*!
 * @brief can rxfifo mode type.
 */
typedef enum{
    CAN_LEGACY_RXFIFO  = 0x00,
    CAN_ENHANCED_RXFIFO = 0x01,
}can_rxfifo_mode_e;

/*!
 * @brief can error irq mask.
 */
typedef enum{
    CAN_RX_WARNING_MASK       = BIT(10),
    CAN_TX_WARNING_MASK       = BIT(11),
    CAN_ERR_MASK              = BIT(14),
    CAN_BUS_OFF_MASK          = BIT(15),

    CAN_BUS_OFF_DONE_MASK    = BIT(30),
    CAN_ERR_FAST_ERR_MASK    = BIT(31),
}can_err_irq_mask_e;

/*!
 * @brief can mb irq mask.
 */
/*
 * typedef enum{
 * }can_mb_irq_mask_e;
 */

/*!
 * @brief can wakeup irq mask.
 */
typedef enum{
    CAN_WAKEUP_MASK   = BIT(2),
}can_self_wakeup_irq_mask_e;

/*!
 * @brief can wake error irq status.
 */
typedef enum{
    CAN_ERR_STATUS          =  BIT(1),
    CAN_BUS_OFF_STATUS      =  BIT(2),
    CAN_RX_WARNING_STATUS   =  BIT(16),
    CAN_TX_WARNING_STATUS   =  BIT(17),
    CAN_BUS_OFF_DONE_STATUS = BIT(19),
    CAN_ERR_FAST_STATUS     = BIT(20),
    CAN_ERR_OVERRUN_STATUS  = BIT(21),
}can_err_irq_status_e;

/*!
 * @brief can mb irq status.
 */
/*
 * typedef enum{
 *
 * }can_mb_irq_status_e;
 */

/*!
 * @brief can legacy rxfifo irq mask and status.
 */
typedef enum{
    CAN_LEGACY_RXFIFO_AVAILD_IRQ = BIT(5),
    CAN_LEGACY_RXFIFO_WARNING_IRQ = BIT(6),
    CAN_LEGACY_RXFIFO_OVERFLOW_IRQ = BIT(7),
}can_legacy_rxfifo_irq_e;

/*!
 * @brief can enhanced rxfifo irq mask and status.
 */
typedef enum{
    CAN_ENHANCED_RXFIFO_AVAILD_IRQ    = BIT(4),
    CAN_ENHANCED_RXFIFO_WATERMARK_IRQ = BIT(5),
    CAN_ENHANCED_RXFIFO_OVERFLOW_IRQ  = BIT(6),
    CAN_ENHANCED_RXFIFO_UNDERFLOW_IRQ = BIT(7),
}can_enhanced_rxfifo_irq_e;

/*!
 * @brief can Pretended Networking irq mask and status.
 */
typedef enum{
    CAN_PN_MATCH_IRQ = BIT(0),
    CAN_PN_TIMEOUT_IRQ =BIT(1),
}can_pn_irq_e;

/*!
 * @brief can wakeup irq status.
 */
typedef enum{
    CAN_WAKEUP_STATUS     =  BIT(0),
}can_self_wakeup_irq_status_e;

/*!
 * @brief can error type
 */
typedef enum{
    CAN_RX_WARNING_ERROR = BIT(0),
    CAN_TX_WARNING_ERROR = BIT(1),
    CAN_STUFF_ERROR      = BIT(2),
    CAN_FORM_ERROR       = BIT(3),
    CAN_CRC_ERROR        = BIT(4),
    CAN_ACK_ERROR        = BIT(5),
    CAN_BIT0_ERROR       = BIT(6),
    CAN_BIT1_ERROR       = BIT(7),
}can_err_type_e;

/*!
 * @brief can work status.
 */
typedef enum{
    CAN_NOT_READY=0,
    CAN_RX,
    CAN_TX,
    CAN_IDLE,
}can_work_status_e;

/*!
 * @brief can fault confinement state.
 */
typedef enum{
    CAN_ERROR_ACTIVE =0,
    CAN_ERROR_PASSIVE,
    CAN_ERROR_BUS_OFF,     //0x02 or 0x03
}can_fault_confinement_state_e;



/*!
 * @brief can Pretended Networking match source selection.
 */
typedef enum
{
    CAN_PN_MATSRC_ID = 0,   /*!< Message match with ID filtering. */
    CAN_PN_MATSRC_ID_Data, /*!< Message match with ID filtering and payload filtering. */
}can_pn_match_source_t;

/*!
 * @brief can Pretended Networking mode match type.
 */
typedef enum
{
    CAN_PN_MATCH_EQUAL = 0x0, /*!< Match upon ID/Payload contents against an exact target value. */
    CAN_PN_MATCH_GREATER, /*!< Match upon an ID/Payload value greater than or equal to a specified target value.
                                */
    CAN_PN_MATCH_SMALLER, /*!< Match upon an ID/Payload value smaller than or equal to a specified target value.
                                */
    CAN_PN_MATCH_RANGE,   /*!< Match upon an ID/Payload value inside a range, greater than or equal to a specified
                                lower limit, and smaller than or equal to a specified upper limit */
} can_pn_match_mode_t;

/*!
 * @brief can Pretended Networking configuration structure
 */
typedef struct
{
    _Bool timeout_is_en;                     /*!< Enable or Disable timeout event trigger wakeup.*/
    unsigned short timeout_value;            /*!< The timeout value that generates a wakeup event, the counter timer is incremented based
                                             on 64 times the CAN Bit Time unit. the number of package * One pack of bytes(16 bytes)*8/64 */
    _Bool match_is_en;                       /*!< Enable or Disable match event trigger wakeup.*/
    can_pn_match_source_t match_src;     /*!< Selects the match source (ID and/or data match) to trigger wakeup. */
    unsigned char match_num;                 /*!< The number of times a given message must match the predefined ID and/or data before
                                             generating a wakeup event, range in 0x1 ~ 0xFF. */
    can_pn_match_mode_t id_match_mode;   /*!< The ID match type. */
    can_pn_match_mode_t data_match_mode; /*!< The data match type. */
    unsigned int id_lower;                    /*!< The ID target values 1 which used either for ID match "equal to", "smaller than",
                                             "greater than" comparisons, or as the lower limit value in ID match "range detection". */
    unsigned int id_upper;                   /*!< The ID target values 2 which used only as the upper limit value in ID match "range
                                              detection" or used to store the ID mask in "equal to". */
    unsigned char length_lower;               /*!< The lower limit for length of data bytes which used only in data match "range
                                              detection". Range in 0x0 ~ 0x8.*/
    unsigned char length_upper;              /*!< The upper limit for length of data bytes which used only in data match "range
                                             detection". Range in 0x0 ~ 0x8.*/
    union
    {
        /*!< The data target values 1 which used either for data match "equal to", "smaller than",
           "greater than" comparisons, or as the lower limit value in data match "range
           detection". */
        struct
        {
            unsigned int lower_word0; /*!< CAN Frame payload word0. */
            unsigned int lower_word1; /*!< CAN Frame payload word1. */
        };
        struct
        {
            unsigned char lower_byte3; /*!< CAN Frame payload byte3. */
            unsigned char lower_byte2; /*!< CAN Frame payload byte2. */
            unsigned char lower_byte1; /*!< CAN Frame payload byte1. */
            unsigned char lower_byte0; /*!< CAN Frame payload byte0. */
            unsigned char lower_byte7; /*!< CAN Frame payload byte7. */
            unsigned char lower_byte6; /*!< CAN Frame payload byte6. */
            unsigned char lower_byte5; /*!< CAN Frame payload byte5. */
            unsigned char lower_byte4; /*!< CAN Frame payload byte4. */
        };
    };
    union
    {
        /*!< The data target values 2 which used only as the upper limit value in data match "range
                                    detection" or used to store the data mask in "equal to". */
        struct
        {
            unsigned int upper_word0; /*!< CAN Frame payload word0. */
            unsigned int upper_word1; /*!< CAN Frame payload word1. */
        };
        struct
        {
            unsigned char upper_byte3; /*!< CAN Frame payload byte3. */
            unsigned char upper_byte2; /*!< CAN Frame payload byte2. */
            unsigned char upper_byte1; /*!< CAN Frame payload byte1. */
            unsigned char upper_byte0; /*!< CAN Frame payload byte0. */
            unsigned char upper_byte7; /*!< CAN Frame payload byte7. */
            unsigned char upper_byte6; /*!< CAN Frame payload byte6. */
            unsigned char upper_byte5; /*!< CAN Frame payload byte5. */
            unsigned char upper_byte4; /*!< CAN Frame payload byte4. */
        };
    };
} can_pn_config_t;

/*!
 * @brief can Pretended Networking ID Mask helper macro.
 */
#define CAN_FLT_ID1_FLT_RTR_SHIFT     29
#define CAN_FLT_ID1_FLT_IDE_SHIFT     30
#define CAN_PN_STD_MASK(id, rtr,ide)                            \
    (unsigned int)((unsigned int)(ide) << CAN_FLT_ID1_FLT_IDE_SHIFT)|((unsigned int)((unsigned int)(rtr) << CAN_FLT_ID1_FLT_RTR_SHIFT) | \
     CAN_ID_STD(id)) /*!< Standard Rx Message Buffer Mask helper macro. */
#define CAN_PN_EXT_MASK(id, rtr,ide)                                                                 \
    (unsigned int)((unsigned int)(ide) << CAN_FLT_ID1_FLT_IDE_SHIFT) | (unsigned int)((unsigned int)(rtr) << CAN_FLT_ID1_FLT_RTR_SHIFT) | \
     CAN_ID_EXT(id)) /*!< Extend Rx Message Buffer Mask helper macro. */


/*!
 * @brief can frame length helper macro.
 */
#define DLC_LENGTH_DECODE(dlc)   (((dlc) <= 8) ? (dlc) : (((dlc) <= 12) ? (((dlc)-6) * 4) : (((dlc)-11) * 16)))

/**
 * @brief  Message Buffer Time Stamp Base
 */
typedef enum {
    CAN_HR_TIME_STAMP_POINT_MODE0  = 0x00,   //The high-resolution time stamp capture is disabled
    CAN_HR_TIME_STAMP_POINT_MODE1  = 0x01,   //The high-resolution time stamp is captured at the end of the CAN frame.
    CAN_HR_TIME_STAMP_POINT_MODE2  = 0x02,   //The high-resolution time stamp is captured at the start of the CAN frame.
    CAN_HR_TIME_STAMP_POINT_MODE3  = 0x03,   //The high-resolution time stamp is captured at the start of frame for classical CAN frames and at the res bit for CAN FD frames
}can_timer_stamp_cap_point_e;

/**
 * @brief  Time Stamp Capture Point
 */
typedef enum {
    CAN_MB_TIME_STAMP_BASE0  = 0x00,   //Message buffer time stamp base is CAN_TIMER
    CAN_HR_TIME_STAMP_BASE1  = 0x01,   //Message buffer time stamp base is lower 16 bits of high- resolution timer.
    CAN_HR_TIME_STAMP_BASE2  = 0x02,   // Message buffer time stamp base is upper 16 bits of high-resolution timer.
}can_mb_timer_stamp_base_e;


/**
 *  @brief      This function serves to enable can module.
 *  @param[in]  chn - can_chn_e
 *  @return     none
 */
static inline  void can_module_en(can_chn_e chn){
    if(chn == CAN0){
        reg_rst7 |= FLD_RST7_CAN0;
        reg_clk_en7|=FLD_CLK7_CAN0_EN;
    }
    reg_can_mcr3(chn) &= ~FLD_CAN_MDIS;
    while(reg_can_mcr2(chn)&FLD_CAN_LPMACK);
}

/**
 *  @brief      This function serves to enter freeze mode.
 *  @param[in]  chn - can_chn_e
 *  @return     none
 */
static inline void can_enter_freeze_mode(can_chn_e chn){
/* Set Freeze, Halt bits. */
  reg_can_mcr3(chn) |= FLD_CAN_FRZ;
  reg_can_mcr3(chn) |= FLD_CAN_HALT;
  while(!(reg_can_mcr3(chn)&FLD_CAN_FRZACK));
}

/**
 *  @brief      This function serves to exit freeze mode.
 *  @param[in]  chn - can_chn_e
 *  @return     none
 */
static inline void can_exit_freeze_mode(can_chn_e chn){
     /* Clear Freeze, Halt bits. */
    reg_can_mcr3(chn) &= ~FLD_CAN_HALT;
    reg_can_mcr3(chn) &= ~FLD_CAN_FRZ;
    while(reg_can_mcr3(chn)&FLD_CAN_FRZACK);
}

/**
 *  @brief      This function serves to enable loop back.
 *  @param[in]  chn - can_chn_e
 *  @return     none
 */
static inline void can_loop_back_enable(can_chn_e chn){
    reg_can_etdc_3(chn) &= ~FLD_CAN_ETDCEN;
    reg_can_ctrl1_1(chn) |= FLD_CAN_LPB;
}

/**
 *  @brief      This function serves to disable loop back.
 *  @param[in]  chn - can_chn_e
 *  @return     none
 */
static inline void can_loop_back_disable(can_chn_e chn){
    reg_can_ctrl1_1(chn) &= ~FLD_CAN_LPB;
}

/**
 *  @brief      This function serves to enable self reception.
 *  @param[in]  chn - can_chn_e
 *  @return     none
 */
static inline void can_self_reception_enable(can_chn_e chn){
    reg_can_mcr2(chn) &=~FLD_CAN_SRXDIS;
}

/**
 *  @brief      This function serves to disable self reception.
 *  @param[in]  chn - can_chn_e
 *  @return     none
 */
static inline void can_self_reception_disable(can_chn_e chn){
    reg_can_mcr2(chn) |=FLD_CAN_SRXDIS;
}

/**
 *  @brief      This function serves to enable listen_only.
 *  @param[in]  chn - can_chn_e
 *  @return     none
 */
static inline void can_listen_only_enable(can_chn_e chn){
    reg_can_ctrl1_0(chn) |= FLD_CAN_LOM;
}

/**
 *  @brief      This function serves to disable listen_only.
 *  @param[in]  chn - can_chn_e
 *  @return     none
 */
static inline void can_listen_only_disable(can_chn_e chn){
    reg_can_ctrl1_0(chn) &= ~FLD_CAN_LOM;
}

/**
 *  @brief      This function serves to enable error irq mask.
 *  @param[in]  chn  - can_chn_e
 *  @param[in]  mask - can_err_irq_mask_e
 *  @return     none
 */
static inline void can_set_err_irq_mask(can_chn_e chn,can_err_irq_mask_e mask){
    if((mask ==CAN_BUS_OFF_DONE_MASK) ||(mask == CAN_ERR_FAST_ERR_MASK) ){
        reg_can_ctrl2(chn) |= mask;
    }else{
        reg_can_ctrl1(chn) |= mask;
    }
}

/**
 *  @brief      This function serves to clear error irq mask.
 *  @param[in]  chn  - can_chn_e
 *  @param[in]  mask - can_err_irq_mask_e
 *  @return     none
 */
static inline void can_clr_err_irq_mask(can_chn_e chn,can_err_irq_mask_e mask){
    if((mask ==CAN_BUS_OFF_DONE_MASK) ||(mask == CAN_ERR_FAST_ERR_MASK) ){
        reg_can_ctrl2(chn) &= ~mask;
    }else{
        reg_can_ctrl1(chn) &= ~mask;
    }
}

/**
 *  @brief      This function serves to enable mailbox irq mask.
 *  @param[in]  chn      - can_chn_e
 *  @param[in]  mb_index - the mailbox index
 *  @return     none
 */
static inline void can_set_mb_irq_mask(can_chn_e chn, unsigned char mb_index){
    reg_can_imask(chn,mb_index) |= BIT(mb_index%32);
}

/**
 *  @brief      This function serves to clear mailbox irq mask.
 *  @param[in]  chn      - can_chn_e
 *  @param[in]  mb_index - the mailbox index
 *  @return     none
 */
static inline void can_clr_mb_irq_mask(can_chn_e chn, unsigned char mb_index){
    reg_can_imask(chn,mb_index) &= ~BIT(mb_index%32);
}

/**
 *  @brief      This function serves to enable legacy rxfifo irq mask.
 *  @param[in]  chn      - can_chn_e
 *  @param[in]  mask     - can_legacy_rxfifo_irq_e
 *  @return     none
 */
static inline void can_set_legacy_rxfifo_irq_mask(can_chn_e chn,can_legacy_rxfifo_irq_e mask){
    reg_can_imask1_0(chn) |= mask;
    reg_can_mcr1(chn) &= ~FLD_CAN_DMA;
}

/**
 *  @brief      This function serves to clear mailbox irq mask.
 *  @param[in]  chn      - can_chn_e
 *  @param[in]  mask     - can_legacy_rxfifo_irq_e
 *  @return     none
 */
static inline void can_clr_legacy_rxfifo_irq_mask(can_chn_e chn,can_legacy_rxfifo_irq_e mask){
    reg_can_imask1_0(chn) &= ~mask;
}

/**
 *  @brief      This function serves to set enhanced rxfifo trigger cnt.
 *  @param[in]  chn           - can_chn_e
 *  @param[in]  watermark_cnt - the rxfifo trigger cnt
 *  @return     none
 */
static inline void can_set_enhanced_rxfifo_watermark(can_chn_e chn,unsigned char watermark_cnt){
    reg_can_erfcr_0(chn) =  (watermark_cnt-1);
}

/**
 *  @brief      This function serves to enable enhanced rxfifo irq mask.
 *  @param[in]  chn      - can_chn_e
 *  @param[in]  mask     - can_enhanced_rxfifo_irq_e
 *  @return     none
 */
static inline void can_set_enhanced_rxfifo_irq_mask(can_chn_e chn,can_enhanced_rxfifo_irq_e mask){
    reg_can_erfier(chn) |= mask;
    reg_can_mcr1(chn) &= ~FLD_CAN_DMA;
}

/**
 *  @brief      This function serves to clear mailbox irq mask.
 *  @param[in]  chn      - can_chn_e
 *  @param[in]  mask     - can_enhanced_rxfifo_irq_e
 *  @return     none
 */
static inline void can_clr_enhanced_rxfifo_irq_mask(can_chn_e chn,can_enhanced_rxfifo_irq_e mask){
    reg_can_erfier(chn) &= ~mask;
}

/**
 *  @brief      This function serves to enable self wakeup irq mask.
 *  @param[in]  chn      - can_chn_e
 *  @param[in]  mask     - can_self_wakeup_irq_mask_e
 *  @return     none
 */
static inline void can_set_self_wakeup_irq_mask(can_chn_e chn,can_self_wakeup_irq_mask_e mask){
    reg_can_mcr3(chn) |= mask;
}

/**
 *  @brief      This function serves to clear self wakeup irq mask.
 *  @param[in]  chn      - can_chn_e
 *  @param[in]  mask     - can_self_wakeup_irq_mask_e
 *  @return     none
 */
static inline void can_clr_self_wakeup_irq_mask(can_chn_e chn,can_self_wakeup_irq_mask_e mask){
    reg_can_mcr3(chn) &= ~mask;
}

/**
 *  @brief      This function serves to enable pretended networking irq mask.
 *  @param[in]  chn      - can_chn_e
 *  @param[in]  mask     - can_pn_irq_e
 *  @return     none
 */
static inline void can_set_pn_irq_mask(can_chn_e chn,can_pn_irq_e mask){
    reg_can_ctrl1_pn_2(chn) |=mask;
}

/**
 *  @brief      This function serves to clear pretended networking irq mask.
 *  @param[in]  chn      - can_chn_e
 *  @param[in]  mask     - can_pn_irq_e
 *  @return     none
 */
static inline void can_clr_pn_irq_mask(can_chn_e chn,can_pn_irq_e mask){
    reg_can_ctrl1_pn_2(chn) &=~mask;
}

/**
 *  @brief      This function serves to get error irq status.
 *  @param[in]  chn        - can_chn_e
 *  @param[in]  status     - can_err_irq_status_e
 *  @return     none
 */
static inline unsigned int can_get_err_irq_status(can_chn_e chn,can_err_irq_status_e status){
    return reg_can_esr1(chn) & status;
}

/**
 *  @brief      This function serves to clear error irq status.
 *  @param[in]  chn        - can_chn_e
 *  @param[in]  status     - can_err_irq_status_e
 *  @return     none
 */
static inline void can_clr_err_irq_status(can_chn_e chn,can_err_irq_status_e status){
    reg_can_esr1(chn) = status;
}

/**
 *  @brief      This function serves to get mb irq status.
 *  @param[in]  chn        - can_chn_e
 *  @param[in]  mb_index     - the mailbox index
 *  @return     none
 */
static inline unsigned int can_get_mb_irq_status(can_chn_e chn,unsigned char mb_index){
    return reg_can_iflag(chn,mb_index) &BIT(mb_index%32);
}

/**
 *  @brief      This function serves to clear mb irq status.
 *  @param[in]  chn        - can_chn_e
 *  @param[in]  mb_index     - the mailbox index
 *  @return     none
 */
static inline void can_clr_mb_irq_status(can_chn_e chn,unsigned char mb_index){
    reg_can_iflag(chn,mb_index) = BIT(mb_index%32);
}

/**
 *  @brief      This function serves to get legacy rxfifo irq status.
 *  @param[in]  chn        - can_chn_e
 *  @param[in]  status     - can_legacy_rxfifo_irq_e
 *  @return     none
 */
static inline unsigned char can_get_legacy_rxfifo_irq_status(can_chn_e chn,can_legacy_rxfifo_irq_e status){
    return reg_can_iflag1_0(chn) & status;
}

/**
 *  @brief      This function serves to clear legacy rxfifo irq status.
 *  @param[in]  chn        - can_chn_e
 *  @param[in]  status     - can_legacy_rxfifo_irq_e
 *  @return     none
 */
static inline void can_clr_legacy_rxfifo_irq_status(can_chn_e chn,can_legacy_rxfifo_irq_e status){
    reg_can_iflag1_0(chn) = status;
}

/**
 *  @brief      This function serves to get enhanced rxfifo irq status.
 *  @param[in]  chn        - can_chn_e
 *  @param[in]  status     - can_enhanced_rxfifo_irq_e
 *  @return     none
 */
static inline unsigned char can_get_enhanced_rxfifo_irq_status(can_chn_e chn,can_enhanced_rxfifo_irq_e status){
    return reg_can_erfsr_3(chn) & status;
}

/**
 *  @brief      This function serves to clear enhanced rxfifo irq status.
 *  @param[in]  chn        - can_chn_e
 *  @param[in]  status     - can_enhanced_rxfifo_irq_e
 *  @return     none
 */
static inline void can_clr_enhanced_rxfifo_irq_status(can_chn_e chn,can_enhanced_rxfifo_irq_e status){
    reg_can_erfsr_3(chn) = status;
}

/**
 *  @brief      This function serves to get self wakeup irq status.
 *  @param[in]  chn        - can_chn_e
 *  @param[in]  status     - can_self_wakeup_irq_status_e
 *  @return     none
 */
static inline unsigned char can_get_self_wakeup_irq_status(can_chn_e chn,can_self_wakeup_irq_status_e status){
    return reg_can_esr1_0(chn) & status;
}

/**
 *  @brief      This function serves to clear self wakeup irq status.
 *  @param[in]  chn        - can_chn_e
 *  @param[in]  status     - can_self_wakeup_irq_status_e
 *  @return     none
 */
static inline void can_clr_self_wakeup_irq_status(can_chn_e chn,can_self_wakeup_irq_status_e status){
    reg_can_esr1_0(chn) = status;
}

/**
 *  @brief      This function serves to get pretended networking irq status.
 *  @param[in]  chn        - can_chn_e
 *  @param[in]  status     - can_pn_irq_e
 *  @return     none
 */
static inline unsigned char can_get_pn_irq_status(can_chn_e chn,can_pn_irq_e status){
    return reg_can_wu_mtc_2(chn) & status;
}

/**
 *  @brief      This function serves to clear pretended networking irq status.
 *  @param[in]  chn        - can_chn_e
 *  @param[in]  status     - can_pn_irq_e
 *  @return     none
 */
static inline void can_clr_pn_irq_status(can_chn_e chn,can_pn_irq_e status){
    reg_can_wu_mtc_2(chn) = status;
}

/**
 *  @brief      This function serves to get error type.
 *  @param[in]  chn        - can_chn_e
 *  @return     none
 */
static inline can_err_type_e can_get_err_type(can_chn_e chn){
    return reg_can_esr1_1(chn);
}

/**
 *  @brief      This function serves to get error fast type.
 *  @param[in]  chn        - can_chn_e
 *  @return     none
 */
static inline can_err_type_e canfd_get_err_fast_type(can_chn_e chn){
    return reg_can_esr1_3(chn);
}

/**
 *  @brief      This function serves to get work type.
 *  @param[in]  chn        - can_chn_e
 *  @return     none
 */
static inline can_work_status_e can_get_work_status(can_chn_e chn){
    if(reg_can_mcr3(chn)&FLD_CAN_NOT_READY){
        return CAN_NOT_READY;
    }else if(reg_can_esr1_0(chn)&FLD_CAN_IS_RX){
        return CAN_RX;
    }else if(reg_can_esr1_0(chn)&FLD_CAN_IS_TX){
        return CAN_TX;
    }
    return CAN_IDLE;
}

/**
 *  @brief      This function serves to get bus fault status.
 *  @param[in]  chn        - can_chn_e
 *  @return     none
 */
static inline can_fault_confinement_state_e can_get_bus_fault_status(can_chn_e chn){
    return (reg_can_esr1_0(chn) & FLD_CAN_FAULT_CONF)>>4;
}

/**
 *  @brief      This function serves to get error count.
 *  @param[in]  chn               - can_chn_e
 *  @param[in]  tx_err_cnt        - tx err cnt
 *  @param[in]  rx_err_cnt        - rx err cnt
 *  @return     none
 */
static inline void can_get_bus_err_count(can_chn_e chn,unsigned char* tx_err_cnt,unsigned char* rx_err_cnt){
    if(tx_err_cnt != NULL){
        *tx_err_cnt = reg_can_tx_err_cnt(chn);
    }
    if(rx_err_cnt != NULL){
        *rx_err_cnt = reg_can_rx_err_cnt(chn);
    }
}

/**
 *  @brief      This function serves to get error fast count.
 *  @param[in]  chn                    - can_chn_e
 *  @param[in]  tx_err_fast_cnt        - tx err cnt
 *  @param[in]  tx_err_fast_cnt        - rx err cnt
 *  @return     none
 */
static inline void canfd_get_bus_err_fast_count(can_chn_e chn,unsigned char* tx_err_fast_cnt,unsigned char* rx_err_fast_cnt){
    if(tx_err_fast_cnt != NULL){
        *tx_err_fast_cnt = reg_can_tx_err_cnt_fast(chn);
    }
    if(rx_err_fast_cnt != NULL){
        *rx_err_fast_cnt = reg_can_rx_err_cnt_fast(chn);
    }
}

/**
 *  @brief      This function serves to enter doze mode.
 *  @param[in]  chn      - can_chn_e
 *  @return     none
 */
static inline void can_enter_doze_mode(can_chn_e chn){
    reg_can_mcr2(chn) |=FLD_CAN_SLF_WAK;
    reg_can_mcr2(chn) |= FLD_CAN_DOZE;
    reg_can_mcr0(chn) |= FLD_CAN_DOZE_REQ;
}

/**
 *  @brief      This function serves to enter stop mode.
 *  @param[in]  chn      - can_chn_e
 *  @return     none
 */
static inline void can_enter_stop_mode(can_chn_e chn){
    reg_can_mcr2(chn) |=FLD_CAN_SLF_WAK;
    reg_can_mcr1(chn) |=FLD_CAN_STOP_REQ;
}

/**
 *  @brief      This function serves to enter pretended networking doze mode.
 *  @param[in]  chn      - can_chn_e
 *  @return     none
 */
static inline void can_enter_pn_doze_mode(can_chn_e chn){
    reg_can_mcr2(chn) &= ~FLD_CAN_SLF_WAK;
    reg_can_mcr2(chn) |= FLD_CAN_DOZE;
    reg_can_mcr0(chn) |= FLD_CAN_DOZE_REQ;
    //while(!(reg_can_mcr2(CAN1)&FLD_CAN_LPMACK));
}

/**
 *  @brief      This function serves to enter pretended networking stop mode.
 *  @param[in]  chn      - can_chn_e
 *  @return     none
 */
static inline void can_enter_pn_stop_mode(can_chn_e chn){
    reg_can_mcr2(chn) &= ~FLD_CAN_SLF_WAK;
    reg_can_mcr1(chn) |=FLD_CAN_STOP_REQ;
    //while(!(reg_can_mcr2(CAN1)&FLD_CAN_LPMACK));
}

/**
 *  @brief      This function serves to exit doze mode.
 *  @param[in]  chn      - can_chn_e
 *  @return     none
 */
static inline void can_exit_doze_mode(can_chn_e chn){
     reg_can_mcr0(chn) &= ~FLD_CAN_DOZE_REQ;
     reg_can_mcr2(chn) &= ~FLD_CAN_DOZE;
}

/**
 *  @brief      This function serves to exit stop mode.
 *  @param[in]  chn      - can_chn_e
 *  @return     none
 */
static inline void can_exit_stop_mode(can_chn_e chn){
    reg_can_mcr1(chn) &=~FLD_CAN_STOP_REQ;
}

/**
 *  @brief      This function serves to set time stamp capture point.
 *  @param[in]  chn       - can_chn_e
 *  @param[in]  type      - can_timer_stamp_cap_point_e
 *  @return     none
 */
static inline void can_set_time_stamp_capture_point(can_chn_e chn,can_timer_stamp_cap_point_e type){
    reg_can_ctrl2_0(chn) = (reg_can_ctrl2_0(chn)&(~FLD_CAN_TIME_STAMP_CAP_POINT))|(type<<6);
}

/**
 *  @brief      This function serves to set message buffer time stamp base.
 *  @param[in]  chn      - can_chn_e
 *  @param[in]  type      - can_mb_timer_stamp_base_e
 *  @return     none
 */
static inline void can_set_mb_time_stamp_base(can_chn_e chn,can_mb_timer_stamp_base_e type){
    reg_can_ctrl2_1(chn) = (reg_can_ctrl2_1(chn)&(~FLD_CAN_MB_TIME_STAMP_BASE))|(type);
}
/**
 *  @brief      This function serves to set pin.
 *  @param[in]  chn         - can_chn_e
 *  @param[in]  tx_pin      - the tx pin
 *  @param[in]  rx_pin      - the rx pin
 *  @return     none
 */
void can_set_pin(can_chn_e chn, gpio_func_pin_e tx_pin, gpio_func_pin_e rx_pin);

/*!
 * @brief This function use to calculates the Classical CAN timing values according to the given bit rate.
 * @param[in]  chn           - can_chn_e
 * @param[in]  bit_rate      - The classical CAN speed in bps defined by user, should be less than or equal to 1Mbps.
 * @param[in]  pclk          - The Source clock frequency in Hz.
 * @param[in]  timing_config - TimingConfig Pointer to the CAN timing configuration structure.
 * @return     1- if timing configuration found, 0- if failed to find configuration.
 */
_Bool can_cal_timing_config(can_chn_e chn,unsigned int bit_rate, unsigned int pclk,can_timing_config_t* timing_config,float sampling_rate);

/*!
 * @brief This function use to calculates the CAN FD timing values according to the given nominal phase bit rate and data phase bit rate
 * @param[in]  chn           - can_chn_e
 * @param[in]  rate_cfg      - the bit rate cfg
 * @param[in]  pclk          - The Source clock frequency in Hz.
 * @param[in]  timing_config - TimingConfig Pointer to the CAN timing configuration structure.
 * @return     1- if timing configuration found, 0- if failed to find configuration.
 */
_Bool canfd_cal_timing_config(can_chn_e chn,can_fd_baud_rate_t* rate_cfg,unsigned int pclk,can_timing_config_t* timing_config,float sampling_rate);

/*!
 * @brief This function initializes the CAN module.
 * @param[in]  chn            - can_chn_e
 * @param[in]  timing_config  - TimingConfig Pointer to the CAN timing configuration structure.
 * @param[in]  mb_cfg         - can_mb_cfg_t. mailbox space allocation refer to mailbox_allocate_t.
 *                              mb_cfg.mb_max:set the maximum number based on application requirements.
 *                              mb_cfg.mb_data_size : CAN_8BYTE_PERMB
 * @param[in]  irmq_en        - 1:enable individual Rx masking and queue feature.
 *                              0:disable individual Rx masking and queue feature,the rx mailbox and legacy rxfifo select global mask.
 * @param[in]  tx_is_lprio_en - 1:the highest-priority mailbox first
 *                              0:the lowest-number mailbox first
 * @return     none
 */
void can_init(can_chn_e chn,can_timing_config_t* timing_config,can_mb_cfg_t* mb_cfg,unsigned char irmq_en,unsigned char tx_is_lprio_en);

/*!
 * @brief This function initializes the CAN FD module.
 * @param[in]  chn            - can_chn_e
 * @param[in]  timing_config  - TimingConfig Pointer to the CAN timing configuration structure.
 * @param[in]  mb_cfg         - can_mb_cfg_t. mailbox space allocation refer to mailbox_allocate_t.
 *                              mb_cfg.mb_max:set the maximum number based on application requirements.
 *                              mb_cfg.mb_data_size : CAN_64BYTE_PERMB
 * @param[in]  brs            - the nominal phase bit rate and data phase bit rate switch or not.
 * @param[in]  irmq_en        - 1:enable individual Rx masking and queue feature.
 *                              0:disable individual Rx masking and queue feature,the rx mailbox and legacy rxfifo select global mask.
 * @param[in]  tx_is_lprio_en - 1:the highest-priority mailbox first
 *                              0:the lowest-number mailbox first
 * @return     none
 */
void canfd_init(can_chn_e chn,can_timing_config_t* timing_config,can_mb_cfg_t* mb_cfg, unsigned char brs,unsigned char irmq_en,unsigned char tx_is_lprio_en);

/*!
 * @brief  This function aborts the previous transmission, cleans the Message Buffer, and configures it as a Transmit Message Buffer.
 * @param[in]  chn      - can_chn_e
 * @param[in]  mb_index - the Message Buffer index.
 * @param[in]  enable   - Enable/disable Tx Message Buffer.
 * @return     - 0: Enable Tx Message Buffer. - 1: the legacy rxfifo is occupied
 */
unsigned char can_set_tx_mailbox_cfg(can_chn_e chn,unsigned char mb_index, unsigned char enable);

/*!
 * @brief  This function aborts the previous transmission, cleans the Message Buffer, and configures it as a Transmit Message Buffer.
 * @param[in]  chn      - can_chn_e
 * @param[in]  mb_index - the Message Buffer index.
 * @param[in]  enable   - Enable/disable Tx Message Buffer.
 * @return     none
 */
void canfd_set_tx_mailbox_cfg(can_chn_e chn,unsigned char mb_index,unsigned char enable);

/*!
 * @brief this function cleans a build-in Message Buffer and configures it as a Receive Message Buffer.
 * @param[in]  chn      - can_chn_e
 * @param[in]  mb_index - the Message Buffer index.
 * @param[in]  rx_mb_cfg- RxMbConfig Pointer to the CAN Message Buffer configuration structure.
 * @param[in]  enable   - enable/disable Rx Message Buffer.
 * @return     - 0: Enable rx Message Buffer. - 1: the legacy rxfifo is occupied
 */
unsigned char can_set_rx_mailbox_cfg(can_chn_e chn,unsigned char mb_index, can_rx_mb_config_t *rx_mb_cfg, unsigned char enable);

/*!
 * @brief this function cleans a build-in Message Buffer and configures it as a Receive Message Buffer by can fd.
 * @param[in]  chn      - can_chn_e
 * @param[in]  mb_index - the Message Buffer index.
 * @param[in]  rx_mb_cfg- RxMbConfig Pointer to the CAN Message Buffer configuration structure.
 * @param[in]  enable   - enable/disable Rx Message Buffer.
 * @return     none
 */
void canfd_set_rx_mailbox_cfg(can_chn_e chn,unsigned char mb_index, can_fd_rx_mb_config_t *rx_mb_cfg, unsigned char enable);

/*!
 * @brief This function sets the individual mask for the CAN matching process.
 * The configuration is only effective when the Rx individual mask is enabled in the can_init().
 * If the legacy Rx FIFO is disabled, the individual mask is applied to the corresponding Message Buffer.
 * If the legacy Rx FIFO is enabled, the individual mask for Rx FIFO occupied Message Buffer is applied the Rx Filter with the same index.
 * Note that only the first 32 individual masks can be used as the Rx FIFO filter mask.
 *
 * @param[in]  chn      - can_chn_e
 * @param[in]  mb_index - the Message Buffer index.
 * @param[in]  mask     - the Rx Individual Mask value.
 */
void can_set_rx_individual_mask(can_chn_e chn,unsigned char mask_index,unsigned int* mask);

/*!
 * @brief This function sets the global mask for the CAN message buffer in a matching process.
 * The configuration is only effective when the Rx individual mask is disabled in the can_init().
 *
 * @param[in]  chn      - can_chn_e
 * @param[in]  mask     - Rx Message Buffer Global Mask value.
 */
void can_set_rxmb_global_mask(can_chn_e chn,unsigned int mask);

/*!
 * brief This function sets the global mask for CAN FIFO in a matching process.
 *
 * @param[in] chn  - can_chn_e
 * @param[in] mask - Rx Fifo Global Mask value.
 */
void can_set_legacy_rxfifo_global_mask(can_chn_e chn, unsigned int mask);

/*!
 * @brief This function configures the CAN Rx FIFO with given configuration.
 *        note 1.Legacy Rx FIFO only can receive classic CAN message.
 *             2.by (8* (n + 1)) >= rxfifo_config->rxfifo_config.id_filter_num, the number of mb occupied by rxfifo is (n+1)*8/4+6,
 *               can_set_tx_mailbox_cfg()/can_set_rx_mailbox_cfg()/can_write_tx_mb()/can_read_rx_mb(),
 *               When legacy rxfifo is enabled, these interfaces determine whether mb is occupied.
 *             3.when the rx individual mask is enabled in the can_init(), only the first 32 individual masks can be used as the Rx FIFO filter mask.
 *             4.rxfifo_config.rxfifo_config,decide whether to traverse the legacy rxfifo or the rx mailbox first.
 * @param[in] chn           - can_chn_e
 * @param[in] rxfifo_config - RxFifoConfig Pointer to the CAN Legacy Rx FIFO configuration structure. Can be NULL when enable parameter is false.
 * @param[in] enable        - Enable/disable Legacy Rx FIFO.
 * @return    - 1: Enable Legacy Rx FIFO. 0: set Legacy Rx FIFO fail.
 */
unsigned char can_set_legacy_rxfifo_cfg(can_chn_e chn,can_rx_fifo_config_t *rxfifo_config, unsigned char enable);

/*!
 * @brief This function configures the Enhanced Rx FIFO with given configuration.
 *        note:Enhanced Rx FIFO support receive classic CAN or CAN FD messages, Legacy Rx FIFO and Enhanced Rx FIFO
 *        cannot be enabled at the same time.
 * @param[in] chn   - can_chn_e
 * @param[in] cfg   - Config Pointer to the CAN Enhanced Rx FIFO configuration structure. Can be NULL when enable parameter is false.
 * @param[in] enable- Enable/disable Enhanced Rx FIFO.
 * @return    - none
 */
void can_set_enhanced_rxfifo_cfg(can_chn_e chn,can_enhanced_rx_fifo_config_t *cfg,unsigned char enable);

/**
  * @brief     Configures the legacy rxfifo or enhanced rxfifo dma channel control register.
  * @param[in] chn       - can_chn_e
  * @param[in] dma_chn   - dma channel.
  * @return    none
 */
void can_set_rx_dma_config(can_chn_e chn, dma_chn_e dma_chn);

/**
 * @brief       This function serves to receive data function by DMA, this function tell the DMA to get data from the rx fifo.
 *              regardless of the length of the DMA configuration, write_num is available,the dma automatically writes back the data receive length to the first four bytes of the rec_buff without manual calculation,
 *              so need addr = the receive buff addr +4.
 * @param[in]   chn       - can_chn_e
 * @param[in]   dma_chn   - dma channel.
 * @param[in]   rx_mode   - rx_mode.
 * @param[in]   addr     - pointer to the buffer receive data.
 * @param[in]   rev_size - the receive length of DMA,The maximum transmission length of DMA is 0xFFFFFC bytes, so dont'n over this length.
 * @return      none
 * @note        When it is CAN_ENHANCED_RXFIFO, for example, the number of triggers set by can_set_enhanced_rxfifo_watermark is n.
 *              When the number of packets in rxfifo reaches n, dma is triggered to carry n packets, and if the consecutively received packets are larger than 2n,
 *              and do not have the time to configure dma, then, after configuring the dma to carry n, and then configuring the next dma, the dma will not work.
 */
void can_receive_dma(can_chn_e chn,dma_chn_e dma_chn,can_rxfifo_mode_e rx_mode, unsigned char *addr, unsigned int rev_size);

/**
 * @brief      Configure DMA head node.
 * @param[in]  chn         - can_chn_e
 * @param[in]  dma_chn   - dma channel.
 * @param[in]  rx_mode   - rx_mode.
 * @param[in]  dst_addr    - Pointer to data buffer, which must be 4 bytes aligned.
 * @param[in]  data_len    - any length less than 0xfffffc(multiple of four).
 * @param[in]  head_of_list - the head address of DMA llp.
 * @return     none.
 * @note       there are no usage restrictions, there is no need to configure the dma length to a maximum length, and there is no limit to the send length:
 *              - The condition of linked list jump: one is when the length of the dma configuration is reached, the other rxfifo is avaid,
 *                no matter which condition has a write back length.
*/
void can_set_dma_chain_llp(can_chn_e chn, dma_chn_e dma_chn, can_rxfifo_mode_e rx_mode,unsigned char *dst_addr, unsigned int data_len, dma_chain_config_t *head_of_list);


/**
 * @brief      Configure DMA cycle chain node.
 * @param[in]  chn         - can_chn_e
 * @param[in]  dma_chn   - dma channel.
 * @param[in]  rx_mode   - rx_mode.
 * @param[in]  config_addr - to servers to configure the address of the current node.
 * @param[in]  llpointer   - to configure the address of the next node.
 * @param[in]  dst_addr    - Pointer to data buffer, which must be 4 bytes aligned.
 * @param[in]  data_len    - any length less than 0xfffffc(multiple of four).
 * @return     none.
 * @note       there are no usage restrictions, there is no need to configure the dma length to a maximum length, and there is no limit to the send length:
 *             - The condition of linked list jump: one is when the length of the dma configuration is reached, the other is rx_done,
 *               no matter which condition has a write back length.
 */
void can_rx_dma_add_list_element(can_chn_e chn, dma_chn_e dma_chn, can_rxfifo_mode_e rx_mode,dma_chain_config_t *config_addr, dma_chain_config_t *llpointer, unsigned char *dst_addr, unsigned int data_len);

/*!
 * @brief This function configures the Pretended Networking with given configuration.
 * @param[in] chn    - can_chn_e
 * @param[in] pn_cfg - Pretended Networking Configure Pointer to the Pretended Networking configuration structure.
 * @return    none
 */
void can_set_pn_config(can_chn_e chn, can_pn_config_t* pn_cfg);

/*!
 * @brief This function writes a CAN Message to the specified Transmit Message Buffer
 *        and changes the Message Buffer state to start CAN Message transmit. After
 *        that the function returns immediately.
 * @param[in] chn    - can_chn_e
 * @param[in] mb_index - The CAN Message Buffer index.
 * @param[in] frame -TxFrame Pointer to CAN message frame to be sent.
 * @param[in] is_rtr_response - can_remote_received_type_e
 * @return 0 - Write Tx Message Buffer Successfully.1 - Tx Message Buffer is currently in use.
 * @note  1. Sends data in big-end mode by default; 2. the length is sent according to the configured length, but the data is fetched in word units;
 */
unsigned char can_write_tx_mb(can_chn_e chn,unsigned char mb_index,can_frame_t* frame,unsigned char is_rtr_response);

/*!
 * @brief This function writes a CAN FD Message to the specified Transmit Message Buffer
 *        and changes the Message Buffer state to start CAN FD Message transmit. After
 *        that the function returns immediately.
 * @param[in] chn    - can_chn_e
 * @param[in] mb_index - The CAN Message Buffer index.
 * @param[in] frame -TxFrame Pointer to CAN FD message frame to be sent.
 * @return  - none
 * @note  1. Sends data in big-end mode by default; 2. the length is sent according to the configured length, but the data is fetched in word units;
 */
void canfd_write_tx_mb(can_chn_e chn,unsigned char mb_index,can_fd_frame_t* frame);

/*!
 * @brief This function reads a CAN message from a specified Receive Message Buffer.
 * The function fills a receive CAN message frame structure with
 * just received data and activates the Message Buffer again.
 * The function returns immediately.
 *
 * @param[in] chn    - can_chn_e
 * @param[in] mb_index - The CAN Message Buffer index.
 * @param[in] frame - pRxFrame Pointer to CAN message frame structure for reception.
 * @return    1           - Rx Message Buffer is full and has been read successfully.
 *            0           - Rx Message Buffer is already overflowed and has been read successfully.
 *            2           - Tx Message Buffer is currently in use.
 */
unsigned char can_read_rx_mb(can_chn_e chn,unsigned char mb_index,can_frame_t* frame);

/*!
 * @brief This function reads a CAN FD message from a specified Receive Message Buffer.
 * The function fills a receive CAN FD message frame structure with
 * just received data and activates the Message Buffer again.
 * The function returns immediately.
 *
 * @param[in] chn    - can_chn_e
 * @param[in] mb_index - The CAN Message Buffer index.
 * @param[in] frame - pRxFrame Pointer to CAN FD message frame structure for reception.
 * @return    1           - Rx Message Buffer is full and has been read successfully.
 *            0           - Rx Message Buffer is already overflowed and has been read successfully.
 */
unsigned char canfd_read_rx_mb(can_chn_e chn,unsigned char mb_index,can_fd_frame_t* frame);

/*!
 * @brief  This function reads a CAN message from the CAN Legacy Rx FIFO.
 *
 * @param[in] chn    - can_chn_e
 * @param[in] frame  - pRxFrame Pointer to CAN message frame structure for reception.
 * @return  none
 */
void can_read_legacy_rxfifo(can_chn_e chn,can_frame_t* frame);

/*!
 * @brief This function reads a CAN or CAN FD message from the CAN Enhanced Rx FIFO.
 *
 * @param[in] chn    - can_chn_e
 * @param[in] frame  - pRxFrame Pointer to CAN FD message frame structure for reception.
 * @return  none
 */
void can_read_enhanced_rxfifo(can_chn_e chn, can_fd_frame_t *frame);

/*!
 * @brief Since the ID HIT offset is changed dynamically according to data length code (DLC),
 *        needs to be reconfigured according to the frame structure to obtain the correct idhit
 * @param[in] frame  - pRxFrame Pointer to CAN FD message frame structure for reception.
 * @return  none
 */
void can_enhanced_rxfifo_dma_reset_idhit(can_chn_e chn,can_fd_frame_t* frame);

/*!
 * @brief when the remote frame is REMOTE_REQUEST_NO_STORING, the payload needs to be filled in advance to automatically respond to the returned data.
 * @param[in] chn    - can_chn_e
 * @param[in] mb_index - The CAN Message Buffer index.
 * @param[in] payload - payload data.
 * @return  none
 */
void can_set_mb_remote_frame_payload(can_chn_e chn,unsigned char mb_index,unsigned int*payload,can_fd_frame_length_e dlc_len);
