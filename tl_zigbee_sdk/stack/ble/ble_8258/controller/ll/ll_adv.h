/********************************************************************************************************
 * @file    ll_adv.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef LL_ADV_H_
#define LL_ADV_H_

#include "stack/ble/ble_8258/ble_format.h"




/**
 * @brief      This function is used to initialize advertising module.
 * @param[in]  public_adr -  public address pointer
 * @return     none
 */
void 		blc_ll_initAdvertising_module(u8 *public_adr);


/**
 * @brief	   This function is used to set the data used in advertising packets that have a data field.
 *  		   Please refer to BLE Core Specification: Vol 4, Part E, 7.8.7 for more information to understand the meaning of each parameters and
 * 			   the return values.
 * @param[in]  data -  advertising data buffer
 * @param[in]  len - The number of significant octets in the Advertising_Data.
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t	bls_ll_setAdvData(u8 *data, u8 len);


/**
 * @brief	   This function is used to provide data used in Scanning Packets that have a data field.
 * 			   Please refer to BLE Core Specification: Vol 4, Part E, 7.8.8 for more information to understand the meaning of each parameters and
 * 			   the return values.
 * @param[in]  data -  Scan_Response_Data buffer
 * @param[in]  len - The number of significant octets in the Scan_Response_Data.
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t 	bls_ll_setScanRspData(u8 *data, u8 len);



/**
* @brief	   This function is used to set whether to check the adv_interval.
* 			   ADV interval is checked for undirected ADV by default according to BLE Core Specification.
* 			   User can use this API to bypass the rule if they want some none standard ADV interval being used for undirected ADV.
* @param[in]   enable -  1: check ADV interval; 0: not check ADV interval
* @return      none
*/
void 		blc_ll_setAdvIntervalCheckEnable(u8 enable);



/**
 * @brief      This function is used to set the advertising parameters.
 * 			   attention: this API is as same as LE controller commands "LE Set Advertising Parameters command".
 * 			   Please refer to BLE Core Specification: Vol 4, Part E, 7.8.5 for more information to understand the meaning of each
 * 			   parameters and the return value.
 * @param[in]  intervalMin - Minimum advertising interval(Time = N * 0.625 ms, Range: 0x0020 to 0x4000)
 * @param[in]  intervalMin - Maximum advertising interval(Time = N * 0.625 ms, Range: 0x0020 to 0x4000)
 * @param[in]  advType - Advertising_Type
 * @param[in]  ownAddrType - Own_Address_Type
 * @param[in]  peerAddrType - Peer_Address_Type
 * @param[in]  peerAddr - Peer_Address
 * @param[in]  adv_channelMap - Advertising_Channel_Map
 * @param[in]  advFilterPolicy - Advertising_Filter_Policy
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t   bls_ll_setAdvParam( u16 intervalMin,  u16 intervalMax,  adv_type_t advType,  		 	  own_addr_type_t ownAddrType,  \
							     u8 peerAddrType, u8  *peerAddr,    adv_chn_map_t 	adv_channelMap,   adv_fp_type_t   advFilterPolicy);




/**
 * @brief      This function is used to request the Controller to start or stop advertising.
 *             Please refer to BLE Core Specification: Vol 4, Part E, 7.8.9 for more information to understand the meaning of each parameters and
 * 			   the return values.
 * @param[in]  adv_enable - Advertising_Enable
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t   bls_ll_setAdvEnable(int adv_enable);






/**
 * @brief      This function is used to set advertise duration time.
 * @param[in]  duration_us - the length of duration, the unit is us.
 * @param[in]  duration_en - Duration_Enable
 * @return     Status - 0x00: BLE success; 0x01-0xFF: fail
 */
ble_sts_t   bls_ll_setAdvDuration (u32 duration_us, u8 duration_en);





/**
 * @brief      This function is used to set some other channel to replace advertising chn37/38/39.
 * @param[in]  chn0 - channel to replace channel 37
 * @param[in]  chn1 - channel to replace channel 38
 * @param[in]  chn2 - channel to replace channel 39
 * @return     none
 */
void 		blc_ll_setAdvCustomedChannel (u8 chn0, u8 chn1, u8 chn2);

/**
 * @brief      This function is used to set whether to continue sending broadcast packets when receiving scan request in the current adv interval.
 * @param[in]  enable - enable:continue sending broadcast packets when receiving scan request.
 * @return     none.
 */
void 		bls_ll_continue_adv_after_scan_req(u8 enable);

/**
 * @brief      This function is used to set direct advertising initial address type.
 * @param[in]  cmdPara - command parameter
 * @return     Status - 0x00:  success;
 * 						other: fail
 */
u8 			blt_set_adv_direct_init_addrtype(u8* cmdPara);


/**
 * @brief      This function is used to set advertising type.
 * @param[in]  advType - adv type
 * @return     Status - 0x00:  success;
 * 						other: fail
 */
ble_sts_t   bls_ll_setAdvType(u8 advType);


/**
 * @brief      This function is used to set address type.
 * @param[in]  cmdPara - command parameter
 * @return     Status - 0x00:  success;
 * 						other: fail
 */
ble_sts_t 	blt_set_adv_addrtype(u8* cmdPara);


extern u32  blc_rcvd_connReq_tick;


/**
 * @brief      This function is used to get connection time.
 * @param	   none
 * @return     connection time
 */
static inline u32 	bls_ll_getConnectionCreateTime(void)
{
	return blc_rcvd_connReq_tick;
}


/**
 * @brief      This function is used to add adv in connection slave role.
 * @param      none
 * @return     Status - 0x00:  success;
 * 						other: fail
 */
ble_sts_t   blc_ll_addAdvertisingInConnSlaveRole(void);


/**
 * @brief      This function is used to remove adv in connection slave role.
 * @param      none
 * @return     Status - 0x00:  success;
 * 						other: fail
 */
ble_sts_t   blc_ll_removeAdvertisingFromConnSLaveRole(void);


/**
 * @brief      This function is used to set ADV parameter in slave role.
 * @param[in]  adv_data -advertising data
 * @param[in]  advData_len - length of the advertising data.
 * @param[in]  scanRsp_data - scan response data
 * @param[in]  scanRspData_len -  length of the scan response data.
 * @param[in]  advType - advertising type
 * @param[in]  ownAddrType - address type of the local device, which can be public or random
 * @param[in]  adv_channelMap - channel map
 * @param[in]  advFilterPolicy - advertising filter policy
 * @return     Status - 0x00:  success;
 * 						other: fail
 */
ble_sts_t 	blc_ll_setAdvParamInConnSlaveRole( u8 		  *adv_data,  u8              advData_len, u8 *scanRsp_data,  u8 scanRspData_len,
											   adv_type_t  advType,   own_addr_type_t ownAddrType, u8 adv_channelMap, adv_fp_type_t advFilterPolicy);


/**
 * @brief      This function is used to set ADV interval in slave role.
 * @param[in]  intervalMin - minimum adv interval
 * @param[in]  intervalMin - maximum adv interval
 * @return     Status - 0x00:  success;
 * 						other: fail
 */
ble_sts_t 	bls_ll_setAdvInterval(u16 intervalMin, u16 intervalMax);


/**
 * @brief      This function is used to set ADV channel used in slave role.
 * @param[in]  adv_channelMap - channel map
 * @return     Status - 0x00:  success;
 * 						other: fail
 */
ble_sts_t 	bls_ll_setAdvChannelMap(adv_chn_map_t adv_channelMap);


/**
 * @brief      This function is used to set ADV filter policy used in slave role.
 * @param[in]  advFilterPolicy - advertising filter policy
 * @return     Status - 0x00:  success;
 * 						other: fail
 */
ble_sts_t 	bls_ll_setAdvFilterPolicy(adv_fp_type_t advFilterPolicy);


typedef int (*advertise_prepare_handler_t) (rf_packet_adv_t * p);


/**
 * @brief      This function is used to set advertising prepare_handler.
 * @param[in]  p - data pointer
 * @return     none
 */
void 		bls_set_advertise_prepare (void *p);


#endif /* LL_ADV_H_ */
