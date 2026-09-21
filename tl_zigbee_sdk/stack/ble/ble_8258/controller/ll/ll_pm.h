/********************************************************************************************************
 * @file    ll_pm.h
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
#ifndef LL_PM_H_
#define LL_PM_H_




////////////////// Power Management ///////////////////////
/**
 * @brief	Telink defined Low power state Type
 */
#define			SUSPEND_DISABLE				0
#define			SUSPEND_ADV					BIT(0)
#define			SUSPEND_CONN				BIT(1)
#define			DEEPSLEEP_RETENTION_ADV		BIT(2)
#define			DEEPSLEEP_RETENTION_CONN	BIT(3)
#define			MCU_STALL					BIT(6)





/**
 * @brief	Telink defined application wake up low power mode process callback function
 */
typedef 	void (*pm_appWakeupLowPower_callback_t)(int);




/**
 * @brief	for user to initialize low power mode
 * @param	none
 * @return	none
 */
void 		blc_ll_initPowerManagement_module(void);

/**
 * @brief   LinkLayer initialization after deepSleep retention wake_up
 * @param	none
 * @return	none
 */
void 		blc_ll_recoverDeepRetention(void);

/**
 * @brief	for user to set low power mode mask
 * @param	mask - low power mode mask
 * @return	none
 */
void		bls_pm_setSuspendMask (u8 mask);

/**
 * @brief	for user to get low power mode mask
 * @param	none
 * @return	bltPm.suspend_mask
 */
u8 			bls_pm_getSuspendMask (void);

/**
 * @brief	for user to set low power mode wake up source
 * @param	source - low power mode wake up source
 * @return	none
 */
void 		bls_pm_setWakeupSource(u8 source);

/**
 * @brief	for user to get low power mode wake up time
 * @param	none
 * @return	bltPm.current_wakeup_tick
 */
u32 		bls_pm_getSystemWakeupTick(void);

/**
 * @brief	for user to get low power mode next connect event wake up time
 * @param	none
 * @return	blt_next_event_tick
 */
u32 		bls_pm_getNexteventWakeupTick(void);

/**
 * @brief	for user to set latency manually for save power
 * @param	latency - bltPm.user_latency
 * @return	none
 */
void 		bls_pm_setManualLatency(u16 latency);

/**
 * @brief	for user to set application wake up low power mode
 * @param	wakeup_tick - low power mode wake up time
 * @param	enable - low power mode application wake up enable
 * @return	none
 */
void 		bls_pm_setAppWakeupLowPower(u32 wakeup_tick, u8 enable);

/**
 * @brief	for user to register the callback for application wake up low power mode process
 * @param	cb - the pointer of callback function
 * @return  none.
 */
void 		bls_pm_registerAppWakeupLowPowerCb(pm_appWakeupLowPower_callback_t cb);

/**
 * @brief	for user to set the threshold of sleep tick for entering deep retention mode
 * @param	adv_thres_ms - the threshold of sleep tick for advertisement state
 * @param	conn_thres_ms - the threshold of sleep tick for connection state
 * @return  none.
 */
void 		blc_pm_setDeepsleepRetentionThreshold(u32 adv_thres_ms, u32 conn_thres_ms);

/**
 * @brief	for user to set early wake up tick for deep retention mode
 * @param	earlyWakeup_us - early wake up tick for deep retention mode
 * @return  none.
 */
void 		blc_pm_setDeepsleepRetentionEarlyWakeupTiming(u32 earlyWakeup_us);

/**
 * @brief	for user to set the type of deep retention mode
 * @param	sleep_type - the type of deep retention mode
 * @return  none.
 */
void 		blc_pm_setDeepsleepRetentionType(SleepMode_TypeDef sleep_type);


void bls_pm_conditionCbRegister(ble_lowpowerCond_t cb);
void bls_pm_conditionCbUnregister(void);
bool bls_pm_conditionCbIsValid(void);


#endif /* LL_PM_H_ */
