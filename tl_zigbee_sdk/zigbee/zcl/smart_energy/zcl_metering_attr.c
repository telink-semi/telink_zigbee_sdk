/********************************************************************************************************
 * @file    zcl_metering_attr.c
 *
 * @brief   This is the source file for zcl_metering_attr
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
#if 0
#include "tl_common.h"

/**********************************************************************
 * INCLUDES
 */
#include "../zcl_include.h"


/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * LOCAL TYPES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */
#ifdef ZCL_METERING

//Reading Information Set
#ifdef ZCL_ATTRID_CURRENT_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentSummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_SUMMATION_RECEIVED_ENABLE
u8 zcl_attr_currentSummationReceived[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_MAX_DEMAND_DELIVERD_ENABLE
u8 zcl_attr_currentMaxDemandDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_MAX_DEMAND_RECEIVED_ENABLE
u8 zcl_attr_currentMaxDemandReceived[6] = {0};
#endif
#ifdef ZCL_ATTRID_DFT_SUMMATION_ENABLE
u8 zcl_attr_DFTSummation[6] = {0};
#endif
#ifdef ZCL_ATTRID_DAILY_FREEZE_TIME_ENABLE
u16 zcl_attr_dailyFreezeTime = 0x0000;
#endif
#ifdef ZCL_ATTRID_METERING_POWER_FACTOR_ENABLE
s8 zcl_attr_metering_powerFactor = 0x00;
#endif
#ifdef ZCL_ATTRID_READING_SNAP_SHORT_TIME_ENABLE
UTCTime zcl_attr_readingSnapShotTime = 0x0;
#endif
#ifdef ZCL_ATTRID_CURRENT_MAX_DEMAND_DELIVERD_TIME_ENABLE
UTCTime zcl_attr_currentMaxDemandDeliveredTime = 0x0;
#endif
#ifdef ZCL_ATTRID_CURRENT_MAX_DEMAND_RECEIVED_TIME_ENABLE
UTCTime zcl_attr_currentMaxDemandReceivedTime  = 0x0;
#endif
#ifdef ZCL_ATTRID_DEFAULT_UPDATE_PERIOD_ENABLE
u8 zcl_attr_defaultUpdatePeriod = 0x1E;
#endif
#ifdef ZCL_ATTRID_FAST_POLL_UPDATE_PERIOD_ENABLE
u8 zcl_attr_fastPollUpdatePeriod = 0x05;
#endif
#ifdef ZCL_ATTRID_CURRENT_BLOCK_PERIOD_CONSUMPTION_DELIVERD_ENABLE
u8 zcl_attr_currentBlockPeriodConsumptionDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_DAILY_CONSUMPTION_TARGET_ENABLE
u24 zcl_attr_dailyConsumptionTarget = 0x000000;
#endif
#ifdef ZCL_ATTRID_CURRENT_BLOCK_ENABLE
u8 zcl_attr_currentBlock = 0x00;
#endif
#ifdef ZCL_ATTRID_PROFILE_INTERVAL_PERIOD_ENABLE
u8 zcl_attr_profileIntervalPeriod = 0x00;
#endif
#ifdef ZCL_ATTRID_INTERVAL_READ_REPORTING_PERIOD_ENABLE
u16 zcl_attr_intervalReadReportingPeriod = 0x0000;
#endif
#ifdef ZCL_ATTRID_PRESENT_READING_TIME_ENABLE
u16 zcl_attr_presetReadingTime = 0x0000;
#endif
#ifdef ZCL_ATTRID_VOLUME_PER_REPORT_ENABLE
u16 zcl_attr_volumePerReport = 0x0000;
#endif
#ifdef ZCL_ATTRID_FLOW_RESTRICTION_ENABLE
u8 zcl_attr_flowRestriction = 0x05;
#endif
#ifdef ZCL_ATTRID_SUPPLY_STATUS_ENABLE
u8 zcl_attr_supplyStatus = 0x00;
#endif
#ifdef ZCL_ATTRID_CURRENT_INLET_ENERGY_CARRIER_SUMMATION_ENABLE
u8 zcl_attr_currentInletEnergyCarrierSummation[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_OUTLET_ENERGY_CARRIER_SUMMATION_ENABLE
u8 zcl_attr_currentOutletEnergyCarrierSummation[6] = {0};
#endif
#ifdef ZCL_ATTRID_INLET_TEMPERATURE_ENABLE
s24 zcl_attr_inletTemperature = 0x000000;
#endif
#ifdef ZCL_ATTRID_OUTLET_TEMPERATURE_ENABLE
s24 zcl_attr_outletTemperature = 0x000000;
#endif
#ifdef ZCL_ATTRID_CONTROL_TEMPERATURE_ENABLE
s24 zcl_attr_controlTemperature = 0x000000;
#endif
#ifdef ZCL_ATTRID_CURRENT_INLET_ENERGY_CARRIER_DEMAND_ENABLE
s24 zcl_attr_currentInletEnergyCarrierDemand = 0x000000;
#endif
#ifdef ZCL_ATTRID_CURRENT_OUTLET_ENERGY_CARRIER_DEMAND_ENABLE
s24 zcl_attr_currentOutletEnergyCarrierDemand = 0x000000;
#endif
#ifdef ZCL_ATTRID_PREVIOUS_BLOCK_PERIOD_CONSUMPTION_DELIVERD_ENABLE
u8 zcl_attr_previousBlockPeriodConsumptionDelivered[6] = {0};
#endif

//Summation TOU Information Set
#ifdef ZCL_ATTRID_CURRENT_TIER_1_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier1SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_1_SUMMATION_RECEIVED_ENABLE
u8 zcl_attr_currentTier1SummationReceived[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_2_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier2SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_2_SUMMATION_RECEIVED_ENABLE
u8 zcl_attr_currentTier2SummationReceived[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_3_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier3SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_3_SUMMATION_RECEIVED_ENABLE
u8 zcl_attr_currentTier3SummationReceived[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_4_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier4SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_4_SUMMATION_RECEIVED_ENABLE
u8 zcl_attr_currentTier4SummationReceived[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_5_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier5SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_5_SUMMATION_RECEIVED_ENABLE
u8 zcl_attr_currentTier5SummationReceived[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_6_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier6SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_6_SUMMATION_RECEIVED_ENABLE
u8 zcl_attr_currentTier6SummationReceived[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_7_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier7SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_7_SUMMATION_RECEIVED_ENABLE
u8 zcl_attr_currentTier7SummationReceived[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_8_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier8SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_8_SUMMATION_RECEIVED_ENABLE
u8 zcl_attr_currentTier8SummationReceived[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_9_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier9SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_9_SUMMATION_RECEIVED_ENABLE
u8 zcl_attr_currentTier9SummationReceived[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_10_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier10SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_10_SUMMATION_RECEIVED_ENABLE
u8 zcl_attr_currentTier10SummationReceived[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_11_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier11SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_11_SUMMATION_RECEIVED_ENABLE
u8 zcl_attr_currentTier11SummationReceived[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_12_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier12SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_12_SUMMATION_RECEIVED_ENABLE
u8 zcl_attr_currentTier12SummationReceived[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_13_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier13SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_13_SUMMATION_RECEIVED_ENABLE
u8 zcl_attr_currentTier13SummationReceived[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_14_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier14SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_14_SUMMATION_RECEIVED_ENABLE
u8 zcl_attr_currentTier14SummationReceived[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_15_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier15SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_15_SUMMATION_RECEIVED_ENABLE
u8 zcl_attr_currentTier15SummationReceived[6] = {0};
#endif

//Meter Status Attribute Set
#ifdef ZCL_ATTRID_STATUS_ENABLE
u8 zcl_attr_status = 0x00;
#endif
#ifdef ZCL_ATTRID_REMAINING_BATTERY_LIFE_ENABLE
u8 zcl_attr_remainingBatteryLife = 0x00;
#endif
#ifdef ZCL_ATTRID_HOURS_IN_OPERATION_ENABLE
u24 zcl_attr_hoursInOperation = 0x00;
#endif
#ifdef ZCL_ATTRID_HOURS_IN_FAULT_ENABLE
u24 zcl_attr_hoursInFault = 0x00;
#endif

//Formatting
#ifdef ZCL_ATTRID_UNIT_OF_MEASURE_ENABLE
u8 zcl_attr_unitOfMeasure = 0x00;
#endif
#ifdef ZCL_ATTRID_MULTIPLIER_ENABLE
u24 zcl_attr_multiplier = 0x0;
#endif
#ifdef ZCL_ATTRID_DIVISOR_ENABLE
u24 zcl_attr_divisor = 0x0;
#endif
#ifdef ZCL_ATTRID_SUMMATION_FORMATTING_ENABLE
u8 zcl_attr_summationFormatting = 0x00;
#endif
#ifdef ZCL_ATTRID_DEMAND_FORMATTING_ENABLE
u8 zcl_attr_demandFormatting = 0x00;
#endif
#ifdef ZCL_ATTRID_HISTORICAL_CONSUMPTION_FORMATTING_ENABLE
u8 zcl_attr_historicalConsumptionFormatting = 0x00;
#endif
#ifdef ZCL_ATTRID_METERING_DEVICE_TYPE_ENABLE
u8 zcl_attr_meteringDeviceType = 0x00;
#endif
#ifdef ZCL_ATTRID_SITE_ID_ENABLE
u8 zcl_attr_siteID[33] = {0};
#endif
#ifdef ZCL_ATTRID_METER_SERIAL_NUMBER_ENABLE
u8 zcl_attr_meterSerialNumber[25] = {0};
#endif
#ifdef ZCL_ATTRID_ENERGY_CARRIER_UNIT_OF_MEASURE_ENABLE
u8 zcl_attr_energyCarrierUnitOfMeasure = 0x00;
#endif
#ifdef ZCL_ATTRID_ENERGY_CARRIER_SUMMATION_FORMATTING_ENABLE
u8 zcl_attr_energyCarrierSummationFormatting = 0x00;
#endif
#ifdef ZCL_ATTRID_ENERGY_CARRIER_DEMAND_FORMATTING_ENABLE
u8 zcl_attr_energyCarrierDemandFormatting = 0x00;
#endif
#ifdef ZCL_ATTRID_TEMPERATURE_UNIT_OF_MEASURE_ENABLE
u8 zcl_attr_temperatureUnitOfMeasure = 0x00;
#endif
#ifdef ZCL_ATTRID_TEMPERATURE_FORMATTING_ENABLE
u8 zcl_attr_temperatureFormatting = 0x00;
#endif

//Historical Consumption Attribute
#ifdef ZCL_ATTRID_INSTANTANEOUS_DEMAND_ENABLE
s24 zcl_attr_instantaneousDemand = 0x0;
#endif
#ifdef ZCL_ATTRID_CURRENT_DAY_CONSUMPTION_DELIVERD_ENABLE
u24 zcl_attr_currentDayConsumptionDelivered = 0x0;
#endif
#ifdef ZCL_ATTRID_CURRENT_DAY_CONSUMPTION_RECEIVED_ENABLE
u24 zcl_attr_currentDayConsumptionReceived = 0x0;
#endif
#ifdef ZCL_ATTRID_PREVIOUS_DAY_CONSUMPTION_DELIVERD_ENABLE
u24 zcl_attr_previousDayConsumptionDelivered = 0x0;
#endif
#ifdef ZCL_ATTRID_PREVIOUS_DAY_CONSUMPTION_RECEIVED_ENABLE
u24 zcl_attr_previousDayConsumptionReceived = 0x0;
#endif
#ifdef ZCL_ATTRID_CUR_PARTITAL_PROFILE_INT_START_TIME_DELIVERD_ENABLE
UTCTime zcl_attr_currentPartialProfileIntervalStartTimeDelivered = 0x0;
#endif
#ifdef ZCL_ATTRID_CUR_PARTITAL_PROFILE_INT_START_TIME_REVEIVED_ENABLE
UTCTime zcl_attr_currentPartialProfileIntervalStartTimeReceived = 0x0;
#endif
#ifdef ZCL_ATTRID_CUR_PARTITAL_PROFILE_INT_VALUE_DELIVERD_ENABLE
u24 zcl_attr_currentPartialProfileIntervalValueDelivered = 0x0;
#endif
#ifdef ZCL_ATTRID_CUR_PARTITAL_PROFILE_INT_VALUE_REVEIVED_ENABLE
u24 zcl_attr_currentPartialProfileIntervalValueReceived = 0x0;
#endif
#ifdef ZCL_ATTRID_CURRENT_DAY_MAX_PRESSURE_ENABLE
u8 zcl_attr_currentDayMaxPressure[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_DAY_MIN_PRESSURE_ENABLE
u8 zcl_attr_currentDayMinPressure[6] = {0};
#endif
#ifdef ZCL_ATTRID_PREVIOUS_DAY_MAX_PRESSURE_ENABLE
u8 zcl_attr_previousDayMaxPressure[6] = {0};
#endif
#ifdef ZCL_ATTRID_PREVIOUS_DAY_MIN_PRESSURE_ENABLE
u8 zcl_attr_previousDayMinPressure[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_DAY_MAX_DEMAND_ENABLE
s24 zcl_attr_currentDayMaxDemand = 0x0;
#endif
#ifdef ZCL_ATTRID_PREVIOUS_DAY_MAX_DEMAND_ENABLE
s24 zcl_attr_previousDayMaxDemand = 0x0;
#endif
#ifdef ZCL_ATTRID_CURRENT_MONTH_MAX_DEMAND_ENABLE
s24 zcl_attr_currentMonthMaxDemand = 0x0;
#endif
#ifdef ZCL_ATTRID_CURRENT_YEAR_MAX_DEMAND_ENABLE
s24 zcl_attr_currentYearMaxDemand = 0x0;
#endif
#ifdef ZCL_ATTRID_CURRENT_DAY_MAX_ENERGY_CARRIER_DEMAND_ENABLE
s24 zcl_attr_currentDayMaxEnergyCarrierDemand = 0x0;
#endif
#ifdef ZCL_ATTRID_PREVIOUS_DAY_MAX_ENERGY_CARRIER_DEMAND_ENABLE
s24 zcl_attr_previousDayMaxEnergyCarrierDemand = 0x0;
#endif
#ifdef ZCL_ATTRID_CURRENT_MONTH_MAX_ENERGY_CARRIER_DEMAND_ENABLE
s24 zcl_attr_currentMonthMaxEnergyCarrierDemand = 0x0;
#endif
#ifdef ZCL_ATTRID_CURRENT_MONTH_MIN_ENERGY_CARRIER_DEMAND_ENABLE
s24 zcl_attr_currentMonthMinEnergyCarrierDemand = 0x0;
#endif
#ifdef ZCL_ATTRID_CURRENT_YEAR_MAX_ENERGY_CARRIER_DEMAND_ENABLE
s24 zcl_attr_currentYearMaxEnergyCarrierDemand = 0x0;
#endif
#ifdef ZCL_ATTRID_CURRENT_YEAR_MIN_ENERGY_CARRIER_DEMAND_ENABLE
s24 zcl_attr_currentYearMinEnergyCarrierDemand = 0x0;
#endif

//Load Profile Configuration
#ifdef ZCL_ATTRID_MAX_NUMBER_OF_PERIODS_DELIVERD_ENABLE
u8 zcl_attr_maxNumberOfPeriodsDelivered = 0x18;
#endif

//Supply Limit Attributes
#ifdef ZCL_ATTRID_CURRENT_DEMAND_DELIVERD_ENABLE
u24 zcl_attr_currentDemandDelivered = 0x0;
#endif
#ifdef ZCL_ATTRID_DEMAND_LIMIT_ENABLE
u24 zcl_attr_demandLimit = 0x0;
#endif
#ifdef ZCL_ATTRID_DEMAND_INTEGRATION_PERIOD_ENABLE
u8 zcl_attr_demandIntegrationPeriod = 0x0;
#endif
#ifdef ZCL_ATTRID_NUMBER_OF_DEMAND_SUB_INTERVALS_ENABLE
u8 zcl_attr_numberOfDemandSubintervals = 0x0;
#endif

//Block Information Set
#ifdef ZCL_ATTRID_CURRENT_NO_TIER_BLOCK_1_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentNoTierBlock1SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_1_BLOCK_1_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier1Block1SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_2_BLOCK_1_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier2Block1SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_3_BLOCK_1_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier3Block1SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_4_BLOCK_1_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier4Block1SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_5_BLOCK_1_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier5Block1SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_6_BLOCK_1_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier6Block1SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_7_BLOCK_1_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier7Block1SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_8_BLOCK_1_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier8Block1SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_9_BLOCK_1_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier9Block1SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_10_BLOCK_1_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier10Block1SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_11_BLOCK_1_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier11Block1SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_12_BLOCK_1_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier12Block1SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_13_BLOCK_1_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier13Block1SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_14_BLOCK_1_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier14Block1SummationDelivered[6] = {0};
#endif
#ifdef ZCL_ATTRID_CURRENT_TIER_15_BLOCK_1_SUMMATION_DELIVERD_ENABLE
u8 zcl_attr_currentTier15Block1SummationDelivered[6] = {0};
#endif

//Alarms Set
#ifdef ZCL_ATTRID_GENERIC_ALARM_MASK_ENABLE
u16 zcl_attr_genericAlarmMask = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_ELECTRICITY_ALARM_MASK_ENABLE
u32 zcl_attr_electricityAlarmMask = 0xFFFFFFFF;
#endif
#ifdef ZCL_ATTRID_GENERIC_FLOW_PRESSURE_ALARM_MASK_ENABLE
u16 zcl_attr_genericFlowPressureAlarmMask = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_WATER_SPEC_ALARM_MASK_ENABLE
u16 zcl_attr_waterSpecificAlarmMask = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_HEAT_AND_COOLING_SPEC_ALARM_MASK_ENABLE
u16 zcl_attr_heatAndCoolingSpecificAlarmMask = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_GAS_SPEC_ALARM_MASK_ENABLE
u16 zcl_attr_gasSpecificAlarmMask = 0xFFFF;
#endif

/* Attribute record list */
const zclAttrInfo_t metering_attrTbl[] = {
    //Reading Information Set
#ifdef ZCL_ATTRID_CURRENT_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_SUMMATION_DELIVERD,                    ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentSummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_SUMMATION_RECEIVED_ENABLE
    { ZCL_ATTRID_CURRENT_SUMMATION_RECEIVED,                    ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentSummationReceived },
#endif /* ZCL_ATTRID_CURRENT_SUMMATION_RECEIVED_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_MAX_DEMAND_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_MAX_DEMAND_DELIVERD,                   ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentMaxDemandDelivered },
#endif /* ZCL_ATTRID_CURRENT_MAX_DEMAND_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_MAX_DEMAND_RECEIVED_ENABLE
    { ZCL_ATTRID_CURRENT_MAX_DEMAND_RECEIVED,                   ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentMaxDemandReceived },
#endif /* ZCL_ATTRID_CURRENT_MAX_DEMAND_RECEIVED_ENABLE */
#ifdef ZCL_ATTRID_DFT_SUMMATION_ENABLE
    { ZCL_ATTRID_DFT_SUMMATION,                                 ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_DFTSummation },
#endif /* ZCL_ATTRID_DFT_SUMMATION_ENABLE */
#ifdef ZCL_ATTRID_DAILY_FREEZE_TIME_ENABLE
    { ZCL_ATTRID_DAILY_FREEZE_TIME,                             ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,    (u8*)&zcl_attr_dailyFreezeTime },
#endif /* ZCL_ATTRID_DAILY_FREEZE_TIME_ENABLE */
#ifdef ZCL_ATTRID_METERING_POWER_FACTOR_ENABLE
    { ZCL_ATTRID_METERING_POWER_FACTOR,                         ZCL_DATA_TYPE_INT8,     ACCESS_CONTROL_READ,    (u8*)&zcl_attr_metering_powerFactor },
#endif /* ZCL_ATTRID__METERINGPOWER_FACTOR_ENABLE */
#ifdef ZCL_ATTRID_READING_SNAP_SHORT_TIME_ENABLE
    { ZCL_ATTRID_READING_SNAP_SHORT_TIME,                       ZCL_DATA_TYPE_UTC,      ACCESS_CONTROL_READ,    (u8*)&zcl_attr_readingSnapShotTime },
#endif /* ZCL_ATTRID_READING_SNAP_SHORT_TIME_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_MAX_DEMAND_DELIVERD_TIME_ENABLE
    { ZCL_ATTRID_CURRENT_MAX_DEMAND_DELIVERD_TIME,              ZCL_DATA_TYPE_UTC,      ACCESS_CONTROL_READ,    (u8*)&zcl_attr_currentMaxDemandDeliveredTime },
#endif /* ZCL_ATTRID_CURRENT_MAX_DEMAND_DELIVERD_TIME_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_MAX_DEMAND_RECEIVED_TIME_ENABLE
    { ZCL_ATTRID_CURRENT_MAX_DEMAND_RECEIVED_TIME,              ZCL_DATA_TYPE_UTC,      ACCESS_CONTROL_READ,    (u8*)&zcl_attr_currentMaxDemandReceivedTime },
#endif /* ZCL_ATTRID_CURRENT_MAX_DEMAND_RECEIVED_TIME_ENABLE */
#ifdef ZCL_ATTRID_DEFAULT_UPDATE_PERIOD_ENABLE
    { ZCL_ATTRID_DEFAULT_UPDATE_PERIOD,                         ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_defaultUpdatePeriod },
#endif /* ZCL_ATTRID_DEFAULT_UPDATE_PERIOD_ENABLE */
#ifdef ZCL_ATTRID_FAST_POLL_UPDATE_PERIOD_ENABLE
    { ZCL_ATTRID_FAST_POLL_UPDATE_PERIOD,                       ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_fastPollUpdatePeriod },
#endif /* ZCL_ATTRID_FAST_POLL_UPDATE_PERIOD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_BLOCK_PERIOD_CONSUMPTION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_BLOCK_PERIOD_CONSUMPTION_DELIVERD,     ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentBlockPeriodConsumptionDelivered },
#endif /* ZCL_ATTRID_CURRENT_BLOCK_PERIOD_CONSUMPTION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_DAILY_CONSUMPTION_TARGET_ENABLE
    { ZCL_ATTRID_DAILY_CONSUMPTION_TARGET,                      ZCL_DATA_TYPE_UINT24,   ACCESS_CONTROL_READ,    (u8*)&zcl_attr_dailyConsumptionTarget },
#endif /* ZCL_ATTRID_DAILY_CONSUMPTION_TARGET_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_BLOCK_ENABLE
    { ZCL_ATTRID_CURRENT_BLOCK,                                 ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_currentBlock },
#endif /* ZCL_ATTRID_CURRENT_BLOCK_ENABLE */
#ifdef ZCL_ATTRID_PROFILE_INTERVAL_PERIOD_ENABLE
    { ZCL_ATTRID_PROFILE_INTERVAL_PERIOD,                       ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_profileIntervalPeriod },
#endif /* ZCL_ATTRID_PROFILE_INTERVAL_PERIOD_ENABLE */
#ifdef ZCL_ATTRID_INTERVAL_READ_REPORTING_PERIOD_ENABLE
    { ZCL_ATTRID_INTERVAL_READ_REPORTING_PERIOD,                ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,    (u8*)&zcl_attr_intervalReadReportingPeriod },
#endif /* ZCL_ATTRID_INTERVAL_READ_REPORTING_PERIOD_ENABLE */
#ifdef ZCL_ATTRID_PRESENT_READING_TIME_ENABLE
    { ZCL_ATTRID_PRESENT_READING_TIME,                          ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,    (u8*)&zcl_attr_presetReadingTime },
#endif /* ZCL_ATTRID_PRESENT_READING_TIME_ENABLE */
#ifdef ZCL_ATTRID_VOLUME_PER_REPORT_ENABLE
    { ZCL_ATTRID_VOLUME_PER_REPORT,                             ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,    (u8*)&zcl_attr_volumePerReport },
#endif /* ZCL_ATTRID_VOLUME_PER_REPORT_ENABLE */
#ifdef ZCL_ATTRID_FLOW_RESTRICTION_ENABLE
    { ZCL_ATTRID_FLOW_RESTRICTION,                              ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_flowRestriction },
#endif /* ZCL_ATTRID_FLOW_RESTRICTION_ENABLE */
#ifdef ZCL_ATTRID_SUPPLY_STATUS_ENABLE
    { ZCL_ATTRID_SUPPLY_STATUS,                                 ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_supplyStatus },
#endif /* ZCL_ATTRID_SUPPLY_STATUS_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_INLET_ENERGY_CARRIER_SUMMATION_ENABLE
    { ZCL_ATTRID_CURRENT_INLET_ENERGY_CARRIER_SUMMATION,        ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentInletEnergyCarrierSummation },
#endif /* ZCL_ATTRID_CURRENT_INLET_ENERGY_CARRIER_SUMMATION_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_OUTLET_ENERGY_CARRIER_SUMMATION_ENABLE
    { ZCL_ATTRID_CURRENT_OUTLET_ENERGY_CARRIER_SUMMATION,       ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentOutletEnergyCarrierSummation },
#endif /* ZCL_ATTRID_CURRENT_OUTLET_ENERGY_CARRIER_SUMMATION_ENABLE */
#ifdef ZCL_ATTRID_INLET_TEMPERATURE_ENABLE
    { ZCL_ATTRID_INLET_TEMPERATURE,                             ZCL_DATA_TYPE_INT24,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_inletTemperature },
#endif /* ZCL_ATTRID_INLET_TEMPERATURE_ENABLE */
#ifdef ZCL_ATTRID_OUTLET_TEMPERATURE_ENABLE
    { ZCL_ATTRID_OUTLET_TEMPERATURE,                            ZCL_DATA_TYPE_INT24,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_outletTemperature },
#endif /* ZCL_ATTRID_OUTLET_TEMPERATURE_ENABLE */
#ifdef ZCL_ATTRID_CONTROL_TEMPERATURE_ENABLE
    { ZCL_ATTRID_CONTROL_TEMPERATURE,                           ZCL_DATA_TYPE_INT24,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_controlTemperature },
#endif /* ZCL_ATTRID_CONTROL_TEMPERATURE_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_INLET_ENERGY_CARRIER_DEMAND_ENABLE
    { ZCL_ATTRID_CURRENT_INLET_ENERGY_CARRIER_DEMAND,           ZCL_DATA_TYPE_INT24,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_currentInletEnergyCarrierDemand },
#endif /* ZCL_ATTRID_CURRENT_INLET_ENERGY_CARRIER_DEMAND_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_OUTLET_ENERGY_CARRIER_DEMAND_ENABLE
    { ZCL_ATTRID_CURRENT_OUTLET_ENERGY_CARRIER_DEMAND,          ZCL_DATA_TYPE_INT24,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_currentOutletEnergyCarrierDemand },
#endif /* ZCL_ATTRID_CURRENT_OUTLET_ENERGY_CARRIER_DEMAND_ENABLE */
#ifdef ZCL_ATTRID_PREVIOUS_BLOCK_PERIOD_CONSUMPTION_DELIVERD_ENABLE
    { ZCL_ATTRID_PREVIOUS_BLOCK_PERIOD_CONSUMPTION_DELIVERD,    ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_previousBlockPeriodConsumptionDelivered },
#endif /* ZCL_ATTRID_PREVIOUS_BLOCK_PERIOD_CONSUMPTION_DELIVERD_ENABLE */

    //Summation TOU Information Set
#ifdef ZCL_ATTRID_CURRENT_TIER_1_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_1_SUMMATION_DELIVERD,             ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier1SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_1_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_1_SUMMATION_RECEIVED_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_1_SUMMATION_RECEIVED,             ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier1SummationReceived },
#endif /* ZCL_ATTRID_CURRENT_TIER_1_SUMMATION_RECEIVED_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_2_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_2_SUMMATION_DELIVERD,             ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier2SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_2_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_2_SUMMATION_RECEIVED_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_2_SUMMATION_RECEIVED,             ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier2SummationReceived },
#endif /* ZCL_ATTRID_CURRENT_TIER_2_SUMMATION_RECEIVED_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_3_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_3_SUMMATION_DELIVERD,             ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier3SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_3_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_3_SUMMATION_RECEIVED_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_3_SUMMATION_RECEIVED,             ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier3SummationReceived },
#endif /* ZCL_ATTRID_CURRENT_TIER_3_SUMMATION_RECEIVED_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_4_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_4_SUMMATION_DELIVERD,             ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier4SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_4_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_4_SUMMATION_RECEIVED_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_4_SUMMATION_RECEIVED,             ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier4SummationReceived },
#endif /* ZCL_ATTRID_CURRENT_TIER_4_SUMMATION_RECEIVED_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_5_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_5_SUMMATION_DELIVERD,             ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier5SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_5_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_5_SUMMATION_RECEIVED_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_5_SUMMATION_RECEIVED,             ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier5SummationReceived },
#endif /* ZCL_ATTRID_CURRENT_TIER_5_SUMMATION_RECEIVED_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_6_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_6_SUMMATION_DELIVERD,             ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier6SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_6_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_6_SUMMATION_RECEIVED_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_6_SUMMATION_RECEIVED,             ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier6SummationReceived },
#endif /* ZCL_ATTRID_CURRENT_TIER_6_SUMMATION_RECEIVED_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_7_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_7_SUMMATION_DELIVERD,             ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier7SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_7_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_7_SUMMATION_RECEIVED_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_7_SUMMATION_RECEIVED,             ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier7SummationReceived },
#endif /* ZCL_ATTRID_CURRENT_TIER_7_SUMMATION_RECEIVED_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_8_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_8_SUMMATION_DELIVERD,             ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier8SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_8_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_8_SUMMATION_RECEIVED_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_8_SUMMATION_RECEIVED,             ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier8SummationReceived },
#endif /* ZCL_ATTRID_CURRENT_TIER_8_SUMMATION_RECEIVED_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_9_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_9_SUMMATION_DELIVERD,             ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier9SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_9_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_9_SUMMATION_RECEIVED_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_9_SUMMATION_RECEIVED,             ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier9SummationReceived },
#endif /* ZCL_ATTRID_CURRENT_TIER_9_SUMMATION_RECEIVED_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_10_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_10_SUMMATION_DELIVERD,            ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier10SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_10_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_10_SUMMATION_RECEIVED_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_10_SUMMATION_RECEIVED,            ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier10SummationReceived },
#endif /* ZCL_ATTRID_CURRENT_TIER_10_SUMMATION_RECEIVED_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_11_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_11_SUMMATION_DELIVERD,            ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier11SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_11_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_11_SUMMATION_RECEIVED_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_11_SUMMATION_RECEIVED,            ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier11SummationReceived },
#endif /* ZCL_ATTRID_CURRENT_TIER_11_SUMMATION_RECEIVED_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_12_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_12_SUMMATION_DELIVERD,            ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier12SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_12_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_12_SUMMATION_RECEIVED_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_12_SUMMATION_RECEIVED,            ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier12SummationReceived },
#endif /* ZCL_ATTRID_CURRENT_TIER_12_SUMMATION_RECEIVED_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_13_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_13_SUMMATION_DELIVERD,            ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier13SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_13_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_13_SUMMATION_RECEIVED_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_13_SUMMATION_RECEIVED,            ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier13SummationReceived },
#endif /* ZCL_ATTRID_CURRENT_TIER_13_SUMMATION_RECEIVED_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_14_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_14_SUMMATION_DELIVERD,            ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier14SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_14_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_14_SUMMATION_RECEIVED_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_14_SUMMATION_RECEIVED,            ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier14SummationReceived },
#endif /* ZCL_ATTRID_CURRENT_TIER_14_SUMMATION_RECEIVED_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_15_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_15_SUMMATION_DELIVERD,            ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier15SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_15_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_15_SUMMATION_RECEIVED_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_15_SUMMATION_RECEIVED,            ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier15SummationReceived },
#endif /* ZCL_ATTRID_CURRENT_TIER_15_SUMMATION_RECEIVED_ENABLE */

    //Meter Status Attribute Set
#ifdef ZCL_ATTRID_STATUS_ENABLE
    { ZCL_ATTRID_STATUS,                                ZCL_DATA_TYPE_BITMAP8,  ACCESS_CONTROL_READ,    (u8*)&zcl_attr_status },
#endif /* ZCL_ATTRID_STATUS_ENABLE */
#ifdef ZCL_ATTRID_REMAINING_BATTERY_LIFE_ENABLE
    { ZCL_ATTRID_REMAINING_BATTERY_LIFE,                ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_remainingBatteryLife },
#endif /* ZCL_ATTRID_REMAINING_BATTERY_LIFE_ENABLE */
#ifdef ZCL_ATTRID_HOURS_IN_OPERATION_ENABLE
    { ZCL_ATTRID_HOURS_IN_OPERATION,                    ZCL_DATA_TYPE_UINT24,   ACCESS_CONTROL_READ,    (u8*)&zcl_attr_hoursInOperation },
#endif /* ZCL_ATTRID_HOURS_IN_OPERATION_ENABLE */
#ifdef ZCL_ATTRID_HOURS_IN_FAULT_ENABLE
    { ZCL_ATTRID_HOURS_IN_FAULT,                        ZCL_DATA_TYPE_UINT24,   ACCESS_CONTROL_READ,    (u8*)&zcl_attr_hoursInFault },
#endif /* ZCL_ATTRID_HOURS_IN_FAULT_ENABLE */

    //Formatting
#ifdef ZCL_ATTRID_UNIT_OF_MEASURE_ENABLE
    { ZCL_ATTRID_UNIT_OF_MEASURE,                       ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_unitOfMeasure },
#endif /* ZCL_ATTRID_UNIT_OF_MEASURE_ENABLE */
#ifdef ZCL_ATTRID_MULTIPLIER_ENABLE
    { ZCL_ATTRID_MULTIPLIER,                            ZCL_DATA_TYPE_UINT24,   ACCESS_CONTROL_READ,    (u8*)&zcl_attr_multiplier },
#endif /* ZCL_ATTRID_MULTIPLIER_ENABLE */
#ifdef ZCL_ATTRID_DIVISOR_ENABLE
    { ZCL_ATTRID_DIVISOR,                               ZCL_DATA_TYPE_UINT24,   ACCESS_CONTROL_READ,    (u8*)&zcl_attr_divisor },
#endif /* ZCL_ATTRID_DIVISOR_ENABLE */
#ifdef ZCL_ATTRID_SUMMATION_FORMATTING_ENABLE
    { ZCL_ATTRID_SUMMATION_FORMATTING,                  ZCL_DATA_TYPE_BITMAP8,  ACCESS_CONTROL_READ,    (u8*)&zcl_attr_summationFormatting },
#endif /* ZCL_ATTRID_SUMMATION_FORMATTING_ENABLE */
#ifdef ZCL_ATTRID_DEMAND_FORMATTING_ENABLE
    { ZCL_ATTRID_DEMAND_FORMATTING,                     ZCL_DATA_TYPE_BITMAP8,  ACCESS_CONTROL_READ,    (u8*)&zcl_attr_demandFormatting },
#endif /* ZCL_ATTRID_DEMAND_FORMATTING_ENABLE */
#ifdef ZCL_ATTRID_HISTORICAL_CONSUMPTION_FORMATTING_ENABLE
    { ZCL_ATTRID_HISTORICAL_CONSUMPTION_FORMATTING,     ZCL_DATA_TYPE_BITMAP8,  ACCESS_CONTROL_READ,    (u8*)&zcl_attr_historicalConsumptionFormatting },
#endif /* ZCL_ATTRID_HISTORICAL_CONSUMPTION_FORMATTING_ENABLE */
#ifdef ZCL_ATTRID_METERING_DEVICE_TYPE_ENABLE
    { ZCL_ATTRID_METERING_DEVICE_TYPE,                  ZCL_DATA_TYPE_BITMAP8,  ACCESS_CONTROL_READ,    (u8*)&zcl_attr_meteringDeviceType },
#endif /* ZCL_ATTRID_METERING_DEVICE_TYPE_ENABLE */
#ifdef ZCL_ATTRID_SITE_ID_ENABLE
    { ZCL_ATTRID_SITE_ID,                               ZCL_DATA_TYPE_OCTET_STR,ACCESS_CONTROL_READ,    (u8*)zcl_attr_siteID },
#endif /* ZCL_ATTRID_SITE_ID_ENABLE */
#ifdef ZCL_ATTRID_METER_SERIAL_NUMBER_ENABLE
    { ZCL_ATTRID_METER_SERIAL_NUMBER,                   ZCL_DATA_TYPE_OCTET_STR,ACCESS_CONTROL_READ,    (u8*)zcl_attr_meterSerialNumber },
#endif /* ZCL_ATTRID_METER_SERIAL_NUMBER_ENABLE */
#ifdef ZCL_ATTRID_ENERGY_CARRIER_UNIT_OF_MEASURE_ENABLE
    { ZCL_ATTRID_ENERGY_CARRIER_UNIT_OF_MEASURE,        ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_energyCarrierUnitOfMeasure },
#endif /* ZCL_ATTRID_ENERGY_CARRIER_UNIT_OF_MEASURE_ENABLE */
#ifdef ZCL_ATTRID_ENERGY_CARRIER_SUMMATION_FORMATTING_ENABLE
    { ZCL_ATTRID_ENERGY_CARRIER_SUMMATION_FORMATTING,   ZCL_DATA_TYPE_BITMAP8,  ACCESS_CONTROL_READ,    (u8*)&zcl_attr_energyCarrierSummationFormatting },
#endif /* ZCL_ATTRID_ENERGY_CARRIER_SUMMATION_FORMATTING_ENABLE */
#ifdef ZCL_ATTRID_ENERGY_CARRIER_DEMAND_FORMATTING_ENABLE
    { ZCL_ATTRID_ENERGY_CARRIER_DEMAND_FORMATTING,      ZCL_DATA_TYPE_BITMAP8,  ACCESS_CONTROL_READ,    (u8*)&zcl_attr_energyCarrierDemandFormatting },
#endif /* ZCL_ATTRID_ENERGY_CARRIER_DEMAND_FORMATTING_ENABLE */
#ifdef ZCL_ATTRID_TEMPERATURE_UNIT_OF_MEASURE_ENABLE
    { ZCL_ATTRID_TEMPERATURE_UNIT_OF_MEASURE,           ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_temperatureUnitOfMeasure },
#endif /* ZCL_ATTRID_TEMPERATURE_UNIT_OF_MEASURE_ENABLE */
#ifdef ZCL_ATTRID_TEMPERATURE_FORMATTING_ENABLE
    { ZCL_ATTRID_TEMPERATURE_FORMATTING,                ZCL_DATA_TYPE_BITMAP8,  ACCESS_CONTROL_READ,    (u8*)&zcl_attr_temperatureFormatting },
#endif /* ZCL_ATTRID_TEMPERATURE_FORMATTING_ENABLE */

    //Historical Consumption Attribute
#ifdef ZCL_ATTRID_INSTANTANEOUS_DEMAND_ENABLE
    { ZCL_ATTRID_INSTANTANEOUS_DEMAND,                          ZCL_DATA_TYPE_INT24,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_instantaneousDemand },
#endif /* ZCL_ATTRID_INSTANTANEOUS_DEMAND_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_DAY_CONSUMPTION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_DAY_CONSUMPTION_DELIVERD,              ZCL_DATA_TYPE_UINT24,   ACCESS_CONTROL_READ,    (u8*)&zcl_attr_currentDayConsumptionDelivered },
#endif /* ZCL_ATTRID_CURRENT_DAY_CONSUMPTION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_DAY_CONSUMPTION_RECEIVED_ENABLE
    { ZCL_ATTRID_CURRENT_DAY_CONSUMPTION_RECEIVED,              ZCL_DATA_TYPE_UINT24,   ACCESS_CONTROL_READ,    (u8*)&zcl_attr_currentDayConsumptionReceived },
#endif /* ZCL_ATTRID_CURRENT_DAY_CONSUMPTION_RECEIVED_ENABLE */
#ifdef ZCL_ATTRID_PREVIOUS_DAY_CONSUMPTION_DELIVERD_ENABLE
    { ZCL_ATTRID_PREVIOUS_DAY_CONSUMPTION_DELIVERD,             ZCL_DATA_TYPE_UINT24,   ACCESS_CONTROL_READ,    (u8*)&zcl_attr_previousDayConsumptionDelivered },
#endif /* ZCL_ATTRID_PREVIOUS_DAY_CONSUMPTION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_PREVIOUS_DAY_CONSUMPTION_RECEIVED_ENABLE
    { ZCL_ATTRID_PREVIOUS_DAY_CONSUMPTION_RECEIVED,             ZCL_DATA_TYPE_UINT24,   ACCESS_CONTROL_READ,    (u8*)&zcl_attr_previousDayConsumptionReceived },
#endif /* ZCL_ATTRID_PREVIOUS_DAY_CONSUMPTION_RECEIVED_ENABLE */
#ifdef ZCL_ATTRID_CUR_PARTITAL_PROFILE_INT_START_TIME_DELIVERD_ENABLE
    { ZCL_ATTRID_CUR_PARTITAL_PROFILE_INT_START_TIME_DELIVERD,  ZCL_DATA_TYPE_UTC,      ACCESS_CONTROL_READ,    (u8*)&zcl_attr_currentPartialProfileIntervalStartTimeDelivered },
#endif /* ZCL_ATTRID_CUR_PARTITAL_PROFILE_INT_START_TIME_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CUR_PARTITAL_PROFILE_INT_START_TIME_REVEIVED_ENABLE
    { ZCL_ATTRID_CUR_PARTITAL_PROFILE_INT_START_TIME_REVEIVED,  ZCL_DATA_TYPE_UTC,      ACCESS_CONTROL_READ,    (u8*)&zcl_attr_currentPartialProfileIntervalStartTimeReceived },
#endif /* ZCL_ATTRID_CUR_PARTITAL_PROFILE_INT_START_TIME_REVEIVED_ENABLE */
#ifdef ZCL_ATTRID_CUR_PARTITAL_PROFILE_INT_VALUE_DELIVERD_ENABLE
    { ZCL_ATTRID_CUR_PARTITAL_PROFILE_INT_VALUE_DELIVERD,       ZCL_DATA_TYPE_UINT24,   ACCESS_CONTROL_READ,    (u8*)&zcl_attr_currentPartialProfileIntervalValueDelivered },
#endif /* ZCL_ATTRID_CUR_PARTITAL_PROFILE_INT_VALUE_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CUR_PARTITAL_PROFILE_INT_VALUE_REVEIVED_ENABLE
    { ZCL_ATTRID_CUR_PARTITAL_PROFILE_INT_VALUE_REVEIVED,       ZCL_DATA_TYPE_UINT24,   ACCESS_CONTROL_READ,    (u8*)&zcl_attr_currentPartialProfileIntervalValueReceived },
#endif /* ZCL_ATTRID_CUR_PARTITAL_PROFILE_INT_VALUE_REVEIVED_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_DAY_MAX_PRESSURE_ENABLE
    { ZCL_ATTRID_CURRENT_DAY_MAX_PRESSURE,                      ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentDayMaxPressure },
#endif /* ZCL_ATTRID_CURRENT_DAY_MAX_PRESSURE_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_DAY_MIN_PRESSURE_ENABLE
    { ZCL_ATTRID_CURRENT_DAY_MIN_PRESSURE,                      ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentDayMinPressure },
#endif /* ZCL_ATTRID_CURRENT_DAY_MIN_PRESSURE_ENABLE */
#ifdef ZCL_ATTRID_PREVIOUS_DAY_MAX_PRESSURE_ENABLE
    { ZCL_ATTRID_PREVIOUS_DAY_MAX_PRESSURE,                     ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_previousDayMaxPressure },
#endif /* ZCL_ATTRID_PREVIOUS_DAY_MAX_PRESSURE_ENABLE */
#ifdef ZCL_ATTRID_PREVIOUS_DAY_MIN_PRESSURE_ENABLE
    { ZCL_ATTRID_PREVIOUS_DAY_MIN_PRESSURE,                     ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_previousDayMinPressure },
#endif /* ZCL_ATTRID_PREVIOUS_DAY_MIN_PRESSURE_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_DAY_MAX_DEMAND_ENABLE
    { ZCL_ATTRID_CURRENT_DAY_MAX_DEMAND,                        ZCL_DATA_TYPE_INT24,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_currentDayMaxDemand },
#endif /* ZCL_ATTRID_CURRENT_DAY_MAX_DEMAND_ENABLE */
#ifdef ZCL_ATTRID_PREVIOUS_DAY_MAX_DEMAND_ENABLE
    { ZCL_ATTRID_PREVIOUS_DAY_MAX_DEMAND,                       ZCL_DATA_TYPE_INT24,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_previousDayMaxDemand },
#endif /* ZCL_ATTRID_PREVIOUS_DAY_MAX_DEMAND_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_MONTH_MAX_DEMAND_ENABLE
    { ZCL_ATTRID_CURRENT_MONTH_MAX_DEMAND,                      ZCL_DATA_TYPE_INT24,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_currentMonthMaxDemand },
#endif /* ZCL_ATTRID_CURRENT_MONTH_MAX_DEMAND_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_YEAR_MAX_DEMAND_ENABLE
    { ZCL_ATTRID_CURRENT_YEAR_MAX_DEMAND,                       ZCL_DATA_TYPE_INT24,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_currentYearMaxDemand },
#endif /* ZCL_ATTRID_CURRENT_YEAR_MAX_DEMAND_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_DAY_MAX_ENERGY_CARRIER_DEMAND_ENABLE
    { ZCL_ATTRID_CURRENT_DAY_MAX_ENERGY_CARRIER_DEMAND,	        ZCL_DATA_TYPE_INT24,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_currentDayMaxEnergyCarrierDemand },
#endif /* ZCL_ATTRID_CURRENT_DAY_MAX_ENERGY_CARRIER_DEMAND_ENABLE */
#ifdef ZCL_ATTRID_PREVIOUS_DAY_MAX_ENERGY_CARRIER_DEMAND_ENABLE
    { ZCL_ATTRID_PREVIOUS_DAY_MAX_ENERGY_CARRIER_DEMAND,        ZCL_DATA_TYPE_INT24,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_previousDayMaxEnergyCarrierDemand },
#endif /* ZCL_ATTRID_PREVIOUS_DAY_MAX_ENERGY_CARRIER_DEMAND_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_MONTH_MAX_ENERGY_CARRIER_DEMAND_ENABLE
    { ZCL_ATTRID_CURRENT_MONTH_MAX_ENERGY_CARRIER_DEMAND,       ZCL_DATA_TYPE_INT24,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_currentMonthMaxEnergyCarrierDemand },
#endif /* ZCL_ATTRID_CURRENT_MONTH_MAX_ENERGY_CARRIER_DEMAND_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_MONTH_MIN_ENERGY_CARRIER_DEMAND_ENABLE
    { ZCL_ATTRID_CURRENT_MONTH_MIN_ENERGY_CARRIER_DEMAND,       ZCL_DATA_TYPE_INT24,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_currentMonthMinEnergyCarrierDemand },
#endif /* ZCL_ATTRID_CURRENT_MONTH_MIN_ENERGY_CARRIER_DEMAND_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_YEAR_MAX_ENERGY_CARRIER_DEMAND_ENABLE
    { ZCL_ATTRID_CURRENT_YEAR_MAX_ENERGY_CARRIER_DEMAND,        ZCL_DATA_TYPE_INT24,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_currentYearMaxEnergyCarrierDemand },
#endif /* ZCL_ATTRID_CURRENT_YEAR_MAX_ENERGY_CARRIER_DEMAND_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_YEAR_MIN_ENERGY_CARRIER_DEMAND_ENABLE
    { ZCL_ATTRID_CURRENT_YEAR_MIN_ENERGY_CARRIER_DEMAND,        ZCL_DATA_TYPE_INT24,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_currentYearMinEnergyCarrierDemand },
#endif /* ZCL_ATTRID_CURRENT_YEAR_MIN_ENERGY_CARRIER_DEMAND_ENABLE */

    //Load Profile Configuration
#ifdef ZCL_ATTRID_MAX_NUMBER_OF_PERIODS_DELIVERD_ENABLE
    { ZCL_ATTRID_MAX_NUMBER_OF_PERIODS_DELIVERD,                ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_maxNumberOfPeriodsDelivered },
#endif /* ZCL_ATTRID_MAX_NUMBER_OF_PERIODS_DELIVERD_ENABLE */

    //Supply Limit Attributes
#ifdef ZCL_ATTRID_CURRENT_DEMAND_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_DEMAND_DELIVERD,                       ZCL_DATA_TYPE_UINT24,   ACCESS_CONTROL_READ,    (u8*)&zcl_attr_currentDemandDelivered },
#endif /* ZCL_ATTRID_CURRENT_DEMAND_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_DEMAND_LIMIT_ENABLE
    { ZCL_ATTRID_DEMAND_LIMIT,                                  ZCL_DATA_TYPE_UINT24,   ACCESS_CONTROL_READ,    (u8*)&zcl_attr_demandLimit },
#endif /* ZCL_ATTRID_DEMAND_LIMIT_ENABLE */
#ifdef ZCL_ATTRID_DEMAND_INTEGRATION_PERIOD_ENABLE
    { ZCL_ATTRID_DEMAND_INTEGRATION_PERIOD,                     ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_demandIntegrationPeriod },
#endif /* ZCL_ATTRID_DEMAND_INTEGRATION_PERIOD_ENABLE */
#ifdef ZCL_ATTRID_NUMBER_OF_DEMAND_SUB_INTERVALS_ENABLE
    { ZCL_ATTRID_NUMBER_OF_DEMAND_SUB_INTERVALS,                ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,    (u8*)&zcl_attr_numberOfDemandSubintervals },
#endif /* ZCL_ATTRID_NUMBER_OF_DEMAND_SUB_INTERVALS_ENABLE */

    //Block Information Set
#ifdef ZCL_ATTRID_CURRENT_NO_TIER_BLOCK_1_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_NO_TIER_BLOCK_1_SUMMATION_DELIVERD,    ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentNoTierBlock1SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_NO_TIER_BLOCK_1_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_1_BLOCK_1_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_1_BLOCK_1_SUMMATION_DELIVERD,     ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier1Block1SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_1_BLOCK_1_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_2_BLOCK_1_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_2_BLOCK_1_SUMMATION_DELIVERD,     ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier2Block1SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_2_BLOCK_1_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_3_BLOCK_1_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_3_BLOCK_1_SUMMATION_DELIVERD,     ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier3Block1SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_3_BLOCK_1_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_4_BLOCK_1_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_4_BLOCK_1_SUMMATION_DELIVERD,     ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier4Block1SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_4_BLOCK_1_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_5_BLOCK_1_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_5_BLOCK_1_SUMMATION_DELIVERD,     ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier5Block1SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_5_BLOCK_1_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_6_BLOCK_1_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_6_BLOCK_1_SUMMATION_DELIVERD,     ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier6Block1SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_6_BLOCK_1_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_7_BLOCK_1_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_7_BLOCK_1_SUMMATION_DELIVERD,     ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier7Block1SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_7_BLOCK_1_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_8_BLOCK_1_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_8_BLOCK_1_SUMMATION_DELIVERD,     ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier8Block1SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_8_BLOCK_1_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_9_BLOCK_1_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_9_BLOCK_1_SUMMATION_DELIVERD,     ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier9Block1SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_9_BLOCK_1_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_10_BLOCK_1_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_10_BLOCK_1_SUMMATION_DELIVERD,    ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier10Block1SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_10_BLOCK_1_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_11_BLOCK_1_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_11_BLOCK_1_SUMMATION_DELIVERD,    ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier11Block1SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_11_BLOCK_1_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_12_BLOCK_1_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_12_BLOCK_1_SUMMATION_DELIVERD,    ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier12Block1SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_12_BLOCK_1_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_13_BLOCK_1_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_13_BLOCK_1_SUMMATION_DELIVERD,    ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier13Block1SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_13_BLOCK_1_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_14_BLOCK_1_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_14_BLOCK_1_SUMMATION_DELIVERD,    ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier14Block1SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_14_BLOCK_1_SUMMATION_DELIVERD_ENABLE */
#ifdef ZCL_ATTRID_CURRENT_TIER_15_BLOCK_1_SUMMATION_DELIVERD_ENABLE
    { ZCL_ATTRID_CURRENT_TIER_15_BLOCK_1_SUMMATION_DELIVERD,    ZCL_DATA_TYPE_UINT48,   ACCESS_CONTROL_READ,    (u8*)zcl_attr_currentTier15Block1SummationDelivered },
#endif /* ZCL_ATTRID_CURRENT_TIER_15_BLOCK_1_SUMMATION_DELIVERD_ENABLE */

    //Alarms Set
#ifdef ZCL_ATTRID_GENERIC_ALARM_MASK_ENABLE
    { ZCL_ATTRID_GENERIC_ALARM_MASK,                    ZCL_DATA_TYPE_BITMAP16, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,     (u8*)&zcl_attr_genericAlarmMask },
#endif /* ZCL_ATTRID_GENERIC_ALARM_MASK_ENABLE */
#ifdef ZCL_ATTRID_ELECTRICITY_ALARM_MASK_ENABLE
    { ZCL_ATTRID_ELECTRICITY_ALARM_MASK,                ZCL_DATA_TYPE_BITMAP32, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,     (u8*)&zcl_attr_electricityAlarmMask },
#endif /* ZCL_ATTRID_ELECTRICITY_ALARM_MASK_ENABLE */
#ifdef ZCL_ATTRID_GENERIC_FLOW_PRESSURE_ALARM_MASK_ENABLE
    { ZCL_ATTRID_GENERIC_FLOW_PRESSURE_ALARM_MASK,      ZCL_DATA_TYPE_BITMAP16, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,     (u8*)&zcl_attr_genericFlowPressureAlarmMask },
#endif /* ZCL_ATTRID_GENERIC_FLOW_PRESSURE_ALARM_MASK_ENABLE */
#ifdef ZCL_ATTRID_WATER_SPEC_ALARM_MASK_ENABLE
    { ZCL_ATTRID_WATER_SPEC_ALARM_MASK,                 ZCL_DATA_TYPE_BITMAP16, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,     (u8*)&zcl_attr_waterSpecificAlarmMask },
#endif /* ZCL_ATTRID_WATER_SPEC_ALARM_MASK_ENABLE */
#ifdef ZCL_ATTRID_HEAT_AND_COOLING_SPEC_ALARM_MASK_ENABLE
    { ZCL_ATTRID_HEAT_AND_COOLING_SPEC_ALARM_MASK,      ZCL_DATA_TYPE_BITMAP16, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,     (u8*)&zcl_attr_heatAndCoolingSpecificAlarmMask },
#endif /* ZCL_ATTRID_HEAT_AND_COOLING_SPEC_ALARM_MASK_ENABLE */
#ifdef ZCL_ATTRID_GAS_SPEC_ALARM_MASK_ENABLE
    { ZCL_ATTRID_GAS_SPEC_ALARM_MASK,                   ZCL_DATA_TYPE_BITMAP16, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,     (u8*)&zcl_attr_gasSpecificAlarmMask },
#endif /* ZCL_ATTRID_GAS_SPEC_ALARM_MASK_ENABLE */
};

const u8 zcl_metering_attrNum = (sizeof(metering_attrTbl) / sizeof(zclAttrInfo_t));

#endif    /* ZCL_METERING */
#endif
