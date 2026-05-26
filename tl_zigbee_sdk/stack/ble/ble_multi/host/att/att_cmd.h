/********************************************************************************************************
 * @file    att_cmd.h
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
#pragma once

#include "att.h"

/*
 *   return   buf_offset,  Copy the data to be packaged to txBuf
 */

/**
 * Format of the ATT_ERROR_RSP PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Request Opcode In Error            | 1                 |
 * | Attribute Handle In Error          | 2                 |
 * | Error Code                         | 1                 |
 */
u16 blt_att_packageErrorRsp(u8 errOpcode, u16 errHandle, u8 errReason, attr_pkt_t *txBuf);

/**
 * Format of ATT_EXCHANGE_MTU_REQ PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Server Rx MTU                      | 2                 |
 */
int blt_att_packageExchangeMtuReq(u16 mtuSize, attr_pkt_t *txBuf);

/**
 * Format of ATT_EXCHANGE_MTU_RSP PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Server Rx MTU                      | 2                 |
 */
u16 blt_att_packageExchangeMtuRsp(u16 mtu, attr_pkt_t *txBuf);

/**
 * Format of ATT_FIND_INFORMATION_REQ PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Starting Handle                    | 2                 |
 * | Ending Handle                      | 2                 |
 */
int blt_att_packageFindInfoReq(u16 startAttHandle, u16 endAttHandle, attr_pkt_t *txBuf);

/**
 * Format of ATT_FIND_INFORMATION_RSP PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Format                             | 1                 |
 * | Information Data                   | 4 to (ATT_MTU-2)  |
 */
int blt_att_packageFindInfoRsp(u8 format, u8 dataLen, u8 *pData, attr_pkt_t *txBuf);

/**
 * Format of ATT_FIND_BY_TYPE_VALUE_REQ PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Starting Handle                    | 2                 |
 * | Ending Handle                      | 2                 |
 * | Attribute Type                     | 2                 |
 * | Attribute Value                    | 0 to (ATT_MTU-7)  |
 */
int blt_att_packageFindByTypeReq(u16 startAttHandle, u16 endAttHandle, u8 *pUuid, u8 *pAttrValue, int valueLen, attr_pkt_t *txBuf);

/**
 * Format of ATT_FIND_BY_TYPE_VALUE_RSP PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Information Data                   | 4 to (ATT_MTU-1)  |
 *
 *      Information Data (Handles Information List):
 *      +------------------------------------+-------------------+
 *      |    Found Attribute Handle          |   Group End Handle|
 *      |         2 octets                   |        2 octets   |
 */
int blt_att_packageFindByTypeRsp(u8 dataLen, u8 *pData, attr_pkt_t *txBuf);


/**
 * Format of ATT_READ_BY_TYPE_REQ PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Starting Handle                    | 2                 |
 * | Ending Handle                      | 2                 |
 * | Attribute Type                     | 2 or 16           |
 */
int blt_att_packageReadByTypeReq(u16 startAttHandle, u16 endAttHandle, u8 *uuid, u8 uuidLen, attr_pkt_t *txBuf);

/**
 * Format of ATT_READ_BY_TYPE_RSP PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Length                             | 1                 |
 * | Attribute Data List                | 2 to (ATT_MTU-2)  |
 *
 *      Attribute Data List:
 *      +--------------------------+-------------------+
 *      |     Attribute Handle     |   Attribute Value |
 *      |             2            |    (Length C 2)   |
 */
int blt_att_packageReadByTypeRsp(u8 typeLen, u8 dataLen, u8 *pData, attr_pkt_t *txBuf);

/**
 * Format of ATT_READ_REQ PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Attribute Handle                   | 2                 |
 */
int blt_att_packageReadReq(u16 attHandle, attr_pkt_t *txBuf);

/**
 * Format of ATT_READ_RSP PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Attribute Value                    | 0 to (ATT_MTU-1)  |
 */
int blt_att_packageReadRsp(u8 dataLen, u8 *pData, attr_pkt_t *txBuf);

/**
 * Format of ATT_READ_BLOB_REQ PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Attribute Handle                   | 2                 |
 * | Value Offset                       | 2                 |
 */
int blt_att_packageReadBlobReq(u16 attHandle, u16 offset, attr_pkt_t *txBuf);

/**
 * Format of ATT_READ_BLOB_RSP PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Attribute Value                    | 0 to (ATT_MTU-1)  |
 */
int blt_att_packageReadBlobRsp(u8 dataLen, u8 *pData, attr_pkt_t *txBuf);


/**
 * Format of ATT_READ_MULTIPLE_REQ PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Set Of Handles                     | 4 to (ATT_MTU-1)  |
 */
int blt_att_packageReadMultipleReq(u8 numHandles, u16 *pHandle, attr_pkt_t *txBuf);

/**
 * Format of ATT_READ_MULTIPLE_RSP PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Set Of Values                      | 0 to (ATT_MTU-1)  |
 */
int blt_att_packageReadMultipleRsp(attr_pkt_t *txBuf);


/**
 * Format of ATT_READ_BY_GROUP_TYPE_REQ PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Starting Handle                    | 2                 |
 * | Ending Handle                      | 2                 |
 * | Attribute Group Type               | 2 or 16           |
 */
int blt_att_packageReadByGroupTypeReq(u16 startAttHandle, u16 endAttHandle, u8 *uuid, u8 uuidLen, attr_pkt_t *txBuf);

/**
 * Format of ATT_READ_BY_GROUP_TYPE_RSP PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Length                             | 1                 |
 * | Attribute Data List                | 2 to (ATT_MTU-2)  |
 *
 *      Attribute Data List:
 *      +--------------------+--------------------+------------------+
 *      |  Attribute Handle  |   End Group Handle |  Attribute Value |
 *      |          2         |          2         |   (Length C 4)   |
 */
int blt_att_packageReadByGroupTypeRsp(u8 typeLen, u8 *pData, u16 datalen, attr_pkt_t *txBuf);

/**
 * Format of ATT_READ_MULTIPLE_VARIABLE_REQ PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Set Of Handles                     | 4 to (ATT_MTU-1)  |
 */
int blt_att_packageReadMultVarReq(u8 numHandles, u16 *pHandle, attr_pkt_t *txBuf);

/**
 * Format of ATT_READ_MULTIPLE_VARIABLE_RSP PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Length Value Tuple List            | 4 to (ATT_MTU-1)  |
 *
 *      Length Value Tuple:
 *      +--------------------------+-------------------+
 *      |     Value Length         |   Attribute Value |
 *      |          2               |    Value Length   |
 */
int blt_att_packageReadMultVarRsp(u8 numVars, u16 *pVarLen, attr_pkt_t *txBuf);


/**
 * Format of ATT_WRITE_REQ PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Attribute Handle                   | 2                 |
 * | Attribute Value                    | 0 to (ATT_MTU-3)  |
 */
int blt_att_packageWriteReq(u16 attHandle, u16 dataLen, u8 *pData, attr_pkt_t *txBuf);


/**
 * Format of ATT_WRITE_RSP
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 */
int blt_att_packageWriteRsp(attr_pkt_t *txBuf);

/**
 * Format of ATT_WRITE_CMD PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Attribute Handle                   | 2                 |
 * | Attribute Value                    | 0 to (ATT_MTU-3)  |
 */
int blt_att_packageWriteCmd(u16 attHandle, u16 dataLen, u8 *pData, attr_pkt_t *txBuf);

/**
 * Format of ATT_PREPARE_WRITE_REQ PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Attribute Handle                   | 2                 |
 * | Value Offset                       | 2                 |
 * | Part Attribute Value               | 0 to (ATT_MTU-5)  |
 */
int blt_att_packagePrepareWriteReq(u16 attHandle, u16 offset, u16 dataLen, u8 *pData, attr_pkt_t *txBuf);

/**
 * Format of ATT_PREPARE_WRITE_RSP PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Attribute Handle                   | 2                 |
 * | Value Offset                       | 2                 |
 * | Part Attribute Value               | 0 to (ATT_MTU-5)  |
 */
int blt_att_packagePrepareWriteRsp(u16 attHandle, u16 offset, u16 dataLen, u8 *pData, attr_pkt_t *txBuf);

/**
 * Format of ATT_EXECUTE_WRITE_REQ PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Flags                              | 1                 |
 */
int blt_att_packageExecuteWriteReq(u8 flag, attr_pkt_t *txBuf);

/**
 * Format of ATT_EXECUTE_WRITE_RSP PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 */
int blt_att_packageExecuteWriteRsp(attr_pkt_t *txBuf);

/**
 * Format of ATT_HANDLE_VALUE_NTF PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Attribute Handle                   | 2                 |
 * | Attribute Value                    | 0 to (ATT_MTU-3)  |
 */
int blc_att_prepareNotify(u16 attHandle, u16 dataLen, u8 *pData, attr_pkt_t *txBuf);

/**
 * Format of ATT_HANDLE_VALUE_IND PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Attribute Handle                   | 2                 |
 * | Attribute Value                    | 0 to (ATT_MTU-3)  |
 */
int blc_att_prepareIndicate(u16 attHandle, u16 dataLen, u8 *pData, attr_pkt_t *txBuf);

/**
 * Format of ATT_HANDLE_VALUE_CFM PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 */
int blc_att_prepareConfirm(attr_pkt_t *txBuf);

/**
 * Format of ATT_READ_BY_GROUP_TYPE_RSP PDU
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | Attribute Opcode                   | 1                 |
 * | Length                             | 1                 |
 * | Handle Length Value Tuple List     | 8 to (ATT_MTU-1)  |
 *
 *      Handle Length Value Tuple:
 *      +--------------------+--------------------+------------------+
 *      |  Attribute Handle  |   End Group Handle |  Attribute Value |
 *      |          2         |          2         |   Value Length   |
 */
int blc_att_prepareMultNotify(attr_pkt_t *txBuf);
