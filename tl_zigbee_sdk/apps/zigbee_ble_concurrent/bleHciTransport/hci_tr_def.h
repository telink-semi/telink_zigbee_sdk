/********************************************************************************************************
 * @file    hci_tr_def.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef HCI_TR_DEF_H_
#define HCI_TR_DEF_H_


/*!  HCI Transmit Type. */
#define HCI_TR_TYPE_NONE  0x00
#define HCI_TR_TYPE_CMD   0x01
#define HCI_TR_TYPE_ACL   0x02
#define HCI_TR_TYPE_SCO   0x03
#define HCI_TR_TYPE_EVENT 0x04
#define HCI_TR_TYPE_ISO   0x05

/*!  HCI Head length define. */
#define HCI_CMD_HEAD_LEN 3
#define HCI_ACL_HEAD_LEN 4
#define HCI_ISO_HEAD_LEN 4
#define HCI_EVT_HEAD_LEN 2
#define HCI_SCO_HEAD_LEN 3


/*! H5 packet length macro. */
#define HCI_H5_HEAD_LEN 4
#define HCI_H5_CRC_LEN  2


/*! Common macro. */
#define BYTES_TO_UINT8(n, p) \
    {                        \
        n = (u8)(p)[0];      \
    }

#define BYTES_TO_UINT16(n, p)                   \
    {                                           \
        n = ((u16)(p)[0] + ((u16)(p)[1] << 8)); \
    }

#define BYTES_TO_UINT32(n, p)                            \
    {                                                    \
        n = ((u32)(p)[0] + ((u32)(p)[1] << 8) +          \
             ((u32)(p)[2] << 16) + ((u32)(p)[3] << 24)); \
    }


#define BSTREAM_TO_UINT8(n, p) \
    {                          \
        BYTES_TO_UINT8(n, p);  \
        p += 1;                \
    }
#define BSTREAM_TO_UINT16(n, p) \
    {                           \
        BYTES_TO_UINT16(n, p);  \
        p += 2;                 \
    }
#define BSTREAM_TO_UINT32(n, p) \
    {                           \
        BYTES_TO_UINT32(n, p);  \
        p += 4;                 \
    }

#define UINT8_TO_BSTREAM(p, n) \
    {                          \
        *(p)++ = (u8)(n);      \
    }
#define UINT16_TO_BSTREAM(p, n)  \
    {                            \
        *(p)++ = (u8)(n);        \
        *(p)++ = (u8)((n) >> 8); \
    }
#define UINT32_TO_BSTREAM(p, n)   \
    {                             \
        *(p)++ = (u8)(n);         \
        *(p)++ = (u8)((n) >> 8);  \
        *(p)++ = (u8)((n) >> 16); \
        *(p)++ = (u8)((n) >> 24); \
    }

#define COUNTOF(x)          (sizeof((x)) / sizeof((x)[0]))

#define MEM_SIZE_ALIGN4(x)  (((x) + 3) & ~3)
#define MEM_SIZE_ALIGN16(x) (((x) + 15) & ~15)


/*! Debug system -- Author: GaoQiu. */
//#define HCI_TR_DEBUG

#ifdef HCI_TR_DEBUG
    #define ASSERT(exp, errCode)               \
        do {                                   \
            if (!(exp)) {                      \
                REG_ADDR32(0x40000) = errCode; \
                while (1)                      \
                    ;                          \
            }                                  \
        } while (0)

#else
    #define ASSERT(exp, errCode)
#endif

/*! Common error */
#define HCI_TR_ERR_INVALID_PARAM 0x00010000

/*! HCI H5 Protocol error */
#define HCI_TR_ERR_H5_HEAD_LEN        0x01010000
#define HCI_TR_ERR_H5_TX_BUF_OVFL     0x01020000
#define HCI_TR_ERR_H5_HCI_RX_BUF_OVFL 0x01030000

/*! HCI Slip Protocol error */
#define HCI_TR_ERR_SLIP_ESCAPE     0x02010000
#define HCI_TR_ERR_SLIP_ENCODE_BUF 0x02020000
#define HCI_TR_ERR_SLIP_DECODE_BUF 0x02030000

/*! HCI Transport error */
#define HCI_TR_ERR_TR_BACKUP_BUF 0x03010000
#define HCI_TR_ERR_TR_TX_BUF     0x03020000


#ifdef HCI_TR_DEBUG
    #define HCI_TRACK_DATA(pData, len)         \
        do {                                   \
            printf("{ ");                      \
            for (int i = 0; i < (len); i++) {  \
                printf("0x%02X ", (pData)[i]); \
            }                                  \
            printf("}\n");                     \
        } while (0);

    #define H5_TRACK(format, ...)        printf(format, ##__VA_ARGS__)
    #define H5_TRACK_ERR(format, ...)    printf("[ERR -H5  ] " format, ##__VA_ARGS__)
    #define H5_TRACK_INFO(format, ...)   printf("[INFO-H5  ] " format, ##__VA_ARGS__)
    #define H5_TRACK_WRN(format, ...)    printf("[WARN-H5  ] " format, ##__VA_ARGS__)

    #define SLIP_TRACK(format, ...)      printf(format, ##__VA_ARGS__)
    #define SLIP_TRACK_ERR(format, ...)  printf("[ERR -SLIP] " format, ##__VA_ARGS__)
    #define SLIP_TRACK_INFO(format, ...) printf("[INFO-SLIP] " format, ##__VA_ARGS__)
    #define SLIP_TRACK_WRN(format, ...)  printf("[WARN-SLIP] " format, ##__VA_ARGS__)

    #define TR_TRACK(format, ...)        printf(format, ##__VA_ARGS__)
    #define TR_TRACK_ERR(format, ...)    printf("[ERR -TR  ] " format, ##__VA_ARGS__)
    #define TR_TRACK_INFO(format, ...)   printf("[INFO-TR  ] " format, ##__VA_ARGS__)
    #define TR_TRACK_WRN(format, ...)    printf("[WARN-TR  ] " format, ##__VA_ARGS__)

#else
    #define HCI_TRACK_DATA(pData, len)

    #define H5_TRACK(format, ...)
    #define H5_TRACK_ERR(format, ...)
    #define H5_TRACK_INFO(format, ...)
    #define H5_TRACK_WRN(format, ...)

    #define SLIP_TRACK(format, ...)
    #define SLIP_TRACK_ERR(format, ...)
    #define SLIP_TRACK_INFO(format, ...)
    #define SLIP_TRACK_WRN(format, ...)

    #define TR_TRACK(format, ...)
    #define TR_TRACK_ERR(format, ...)
    #define TR_TRACK_INFO(format, ...)
    #define TR_TRACK_WRN(format, ...)
#endif

#endif /* HCI_TR_DEF_H_ */
