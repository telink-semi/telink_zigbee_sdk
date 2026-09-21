/********************************************************************************************************
 * @file    smp_peripheral.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    12,2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef SMP_PERIPHERAL_H_
#define SMP_PERIPHERAL_H_




/**
 * @brief      This function is used to initialize each parameter configuration of SMP, including the initialization of the binding area FLASH.
 * @param[in]  none
 * @return     0: Initialization failed;
 *             1: initialization succeeded.
 */
int 		blc_smp_peripheral_init (void);


/**
 * @brief      This function is used to configure whether the slave sends a Security Request to the master immediately after the connection or after the connection is pending_ms milliseconds, or does not send the Security Request.
 * @param[in]  newConn_cfg - refer to "security request"
 * @param[in]  re_conn_cfg - refer to "security request"
 * @param[in]  pending_ms - Send a Security Request to the master after pending_ms milliseconds
 * @return     none.
 */
void 		blc_smp_configSecurityRequestSending( secReq_cfg newConn_cfg,  secReq_cfg re_conn_cfg, u16 pending_ms);


/**
 * @brief      This function is used to send a Security Request to the master.
 * @param[in]  none
 * @return     1: Push Security Request packet succeeded;
 *             0: Push Security Request packet failed.
 */
int 		blc_smp_sendSecurityRequest (void);



/**
 * @brief      This function is used to set the maximum number of devices that can be bound.
 * 			   attention: 1. this API is used on Slave device only; Master device can not be set !!!
 * 			   			  2. return an error code if device_num greater than the number stack can support, user need check !!!
 * @param[in]  device_num - Set the maximum number of devices that can be bound.
 * @return     error code: 0 - set success
 *                         others - error code for setting fail
 */
init_err_t 	blc_smp_param_setBondingDeviceMaxNumber ( int device_num);



#endif /* SMP_PERIPHERAL_H_ */
