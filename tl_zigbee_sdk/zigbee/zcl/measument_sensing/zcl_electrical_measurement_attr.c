/********************************************************************************************************
 * @file    zcl_electrical_measurement_attr.c
 *
 * @brief   This is the source file for zcl_electrical_measurement_attr
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
#ifdef ZCL_ELECTRICAL_MEASUREMENT
//Basic Information
u32 zclElectricalMeasure_measurementType = 0x00000000;

//DC Measurement
#ifdef ZCL_ATTRID_DC_VOLTAGE_ENABLE
s16 zclElectricalMeasure_DCVoltage = 0x8000;
#endif
#ifdef ZCL_ATTRID_DC_VOLTAGE_MIN_ENABLE
s16 zclElectricalMeasure_DCVoltageMin = 0x8000;
#endif
#ifdef ZCL_ATTRID_DC_VOLTAGE_MAX_ENABLE
s16 zclElectricalMeasure_DCVoltageMax = 0x8000;
#endif
#ifdef ZCL_ATTRID_DC_CURRENT_ENABLE
s16 zclElectricalMeasure_DCCurrent = 0x8000;
#endif
#ifdef ZCL_ATTRID_DC_CURRENT_MIN_ENABLE
s16 zclElectricalMeasure_DCCurrentMin = 0x8000;
#endif
#ifdef ZCL_ATTRID_DC_CURRENT_MAX_ENABLE
s16 zclElectricalMeasure_DCCurrentMax = 0x8000;
#endif
#ifdef ZCL_ATTRID_DC_POWER_ENABLE
s16 zclElectricalMeasure_DCPower = 0x8000;
#endif
#ifdef ZCL_ATTRID_DC_POWER_MIN_ENABLE
s16 zclElectricalMeasure_DCPowerMin = 0x8000;
#endif
#ifdef ZCL_ATTRID_DC_POWER_MAX_ENABLE
s16 zclElectricalMeasure_DCPowerMax = 0x8000;
#endif

//DC Formatting
#ifdef ZCL_ATTRID_DC_VOLTAGE_MULTIPLIER_ENABLE
u16 zclElectricalMeasure_DCVoltageMultiplier = 0x0001;
#endif
#ifdef ZCL_ATTRID_DC_VOLTAGE_DIVISOR_ENABLE
u16 zclElectricalMeasure_DCVoltageDivisor = 0x0001;
#endif
#ifdef ZCL_ATTRID_DC_CURRENT_MULTIPLIER_ENABLE
u16 zclElectricalMeasure_DCCurrentMultiplier = 0x0001;
#endif
#ifdef ZCL_ATTRID_DC_CURRENT_DIVISOR_ENABLE
u16 zclElectricalMeasure_DCCurrentDivisor = 0x0001;
#endif
#ifdef ZCL_ATTRID_DC_POWER_MULTIPLIER_ENABLE
u16 zclElectricalMeasure_DCPowerMultiplier = 0x0001;
#endif
#ifdef ZCL_ATTRID_DC_POWER_DIVISOR_ENABLE
u16 zclElectricalMeasure_DCPowerDivisor = 0x0001;
#endif

//AC (Non-phase Specific) Measurements
#ifdef ZCL_ATTRID_AC_FREQUENCY_ENABLE
u16 zclElectricalMeasure_ACFrequency = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_AC_FREQUENCY_MIN_ENABLE
u16 zclElectricalMeasure_ACFrequencyMin = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_AC_FREQUENCY_MAX_ENABLE
u16 zclElectricalMeasure_ACFrequencyMax = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_NEUTRAL_CURRENT_ENABLE
u16 zclElectricalMeasure_neutralCurrent = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_TOTAL_ACTIVE_POWER_ENABLE
s32 zclElectricalMeasure_totalActivePower = 0x0;
#endif
#ifdef ZCL_ATTRID_TOTAL_REACTIVE_POWER_ENABLE
s32 zclElectricalMeasure_totalReactivePower = 0x0;
#endif
#ifdef ZCL_ATTRID_TOTAL_APPARENT_POWER_ENABLE
u32 zclElectricalMeasure_totalApparentPower = 0x0;
#endif
#ifdef ZCL_ATTRID_MEASURED_1ST_HARMONIC_CURRENT_ENABLE
s16 zclElectricalMeasure_measured1stHarmonicCurrent = 0x8000;
#endif
#ifdef ZCL_ATTRID_MEASURED_3ST_HARMONIC_CURRENT_ENABLE
s16 zclElectricalMeasure_measured3stHarmonicCurrent = 0x8000;
#endif
#ifdef ZCL_ATTRID_MEASURED_5ST_HARMONIC_CURRENT_ENABLE
s16 zclElectricalMeasure_measured5stHarmonicCurrent = 0x8000;
#endif
#ifdef ZCL_ATTRID_MEASURED_7ST_HARMONIC_CURRENT_ENABLE
s16 zclElectricalMeasure_measured7stHarmonicCurrent = 0x8000;
#endif
#ifdef ZCL_ATTRID_MEASURED_9ST_HARMONIC_CURRENT_ENABLE
s16 zclElectricalMeasure_measured9stHarmonicCurrent = 0x8000;
#endif
#ifdef ZCL_ATTRID_MEASURED_11ST_HARMONIC_CURRENT_ENABLE
s16 zclElectricalMeasure_measured11stHarmonicCurrent = 0x8000;
#endif
#ifdef ZCL_ATTRID_MEASURED_PHASE_1ST_HARMONIC_CURRENT_ENABLE
s16 zclElectricalMeasure_measuredPhase1stHarmonicCurrent = 0x8000;
#endif
#ifdef ZCL_ATTRID_MEASURED_PHASE_3ST_HARMONIC_CURRENT_ENABLE
s16 zclElectricalMeasure_measuredPhase3stHarmonicCurrent = 0x8000;
#endif
#ifdef ZCL_ATTRID_MEASURED_PHASE_5ST_HARMONIC_CURRENT_ENABLE
s16 zclElectricalMeasure_measuredPhase5stHarmonicCurrent = 0x8000;
#endif
#ifdef ZCL_ATTRID_MEASURED_PHASE_7ST_HARMONIC_CURRENT_ENABLE
s16 zclElectricalMeasure_measuredPhase7stHarmonicCurrent = 0x8000;
#endif
#ifdef ZCL_ATTRID_MEASURED_PHASE_9ST_HARMONIC_CURRENT_ENABLE
s16 zclElectricalMeasure_measuredPhase9stHarmonicCurrent = 0x8000;
#endif
#ifdef ZCL_ATTRID_MEASURED_PHASE_11ST_HARMONIC_CURRENT_ENABLE
s16 zclElectricalMeasure_measuredPhase11stHarmonicCurrent = 0x8000;
#endif

//AC (Non-phase Specific) Formatting
#ifdef ZCL_ATTRID_AC_FREQUENCY_MULTIPLIER_ENABLE
u16 zclElectricalMeasure_ACFrequencyMultiplier = 0x0001;
#endif
#ifdef ZCL_ATTRID_AC_FREQUENCY_DIVISOR_ENABLE
u16 zclElectricalMeasure_ACFrequencyDivisor = 0x0001;
#endif
#ifdef ZCL_ATTRID_POWER_MULTIPLIER_ENABLE
u32 zclElectricalMeasure_powerMultiplier = 0x00000001;
#endif
#ifdef ZCL_ATTRID_POWER_DIVISOR_ENABLE
u32 zclElectricalMeasure_powerDivisor = 0x00000001;
#endif
#ifdef ZCL_ATTRID_HARMONIC_CURRENT_MULTIPLIER_ENABLE
s8 zclElectricalMeasure_harmonicCurrentMultiplier = 0x00;
#endif
#ifdef ZCL_ATTRID_PHASE_HARMONIC_CURRENT_MULTIPLIER_ENABLE
s8 zclElectricalMeasure_phaseHarmonicCurrentMultiplier = 0x00;
#endif

//AC (Single Phase or Phase A) Measurements
#ifdef ZCL_ATTRID_LINE_CURRENT_ENABLE
u16 zclElectricalMeasure_lineCurrent = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_ACTIVE_CURRENT_ENABLE
s16 zclElectricalMeasure_activeCurrent = 0x8000;
#endif
#ifdef ZCL_ATTRID_REACTIVE_CURRENT_ENABLE
s16 zclElectricalMeasure_reactiveCurrent = 0x8000;
#endif
#ifdef ZCL_ATTRID_RMS_VOLTAGE_ENABLE
u16 zclElectricalMeasure_RMSVoltage = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_RMS_VOLTAGE_MIN_ENABLE
u16 zclElectricalMeasure_RMSVoltageMin = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_RMS_VOLTAGE_MAX_ENABLE
u16 zclElectricalMeasure_RMSVoltageMax = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_RMS_CURRENT_ENABLE
u16 zclElectricalMeasure_RMSCurrent = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_RMS_CURRENT_MIN_ENABLE
u16 zclElectricalMeasure_RMSCurrentMin = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_RMS_CURRENT_MAX_ENABLE
u16 zclElectricalMeasure_RMSCurrentMax = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_ACTIVE_POWER_ENABLE
s16 zclElectricalMeasure_activePower = 0x8000;
#endif
#ifdef ZCL_ATTRID_ACTIVE_POWER_MIN_ENABLE
s16 zclElectricalMeasure_activePowerMin = 0x8000;
#endif
#ifdef ZCL_ATTRID_ACTIVE_POWER_MAX_ENABLE
s16 zclElectricalMeasure_activePowerMax = 0x8000;
#endif
#ifdef ZCL_ATTRID_REACTIVE_POWER_ENABLE
s16 zclElectricalMeasure_reactivePower = 0x8000;
#endif
#ifdef ZCL_ATTRID_APPARENT_POWER_ENABLE
u16 zclElectricalMeasure_apparentPower = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_POWER_FACTOR_ENABLE
s8 zclElectricalMeasure_powerFactor = 0x00;
#endif
#ifdef ZCL_ATTRID_AVERAGE_RMS_VOLTAGE_MEASUREMENT_PERIOD_ENABLE
u16 zclElectricalMeasure_averageRMSVoltageMeasurementPeriod = 0x0000;
#endif
#ifdef ZCL_ATTRID_AVERAGE_RMS_OVER_VOLTAGE_COUNTER_ENABLE
u16 zclElectricalMeasure_averageRMSOverVoltageCounter = 0x0000;
#endif
#ifdef ZCL_ATTRID_AVERAGE_RMS_UNDER_VOLTAGE_COUNTER_ENABLE
u16 zclElectricalMeasure_averageRMSUnderVoltageCounter = 0x0000;
#endif
#ifdef ZCL_ATTRID_RMS_EXTREME_OVER_VOLTAGE_PERIOD_ENABLE
u16 zclElectricalMeasure_RMSExtremeOverVoltagePeriod = 0x0000;
#endif
#ifdef ZCL_ATTRID_RMS_EXTREME_UNDER_VOLTAGE_PERIOD_ENABLE
u16 zclElectricalMeasure_RMSExtremeUnderVoltagePeriod = 0x0000;
#endif
#ifdef ZCL_ATTRID_RMS_VOLTAGE_SAG_PERIOD_ENABLE
u16 zclElectricalMeasure_RMSVoltageSagPeriod = 0x0000;
#endif
#ifdef ZCL_ATTRID_RMS_VOLTAGE_SWELL_PERIOD_ENABLE
u16 zclElectricalMeasure_RMSVoltageSwellPeriod = 0x0000;
#endif

//AC Formatting
#ifdef ZCL_ATTRID_AC_VOLTAGE_MULTIPLIER_ENABLE
u16 zclElectricalMeasure_ACVoltageMultiplier = 0x0001;
#endif
#ifdef ZCL_ATTRID_AC_VOLTAGE_DIVISOR_ENABLE
u16 zclElectricalMeasure_ACVoltageDivisor = 0x0001;
#endif
#ifdef ZCL_ATTRID_AC_CURRENT_MULTIPLIER_ENABLE
u16 zclElectricalMeasure_ACCurrentMultiplier = 0x0001;
#endif
#ifdef ZCL_ATTRID_AC_CURRENT_DIVISOR_ENABLE
u16 zclElectricalMeasure_ACCurrentDivisor = 0x0001;
#endif
#ifdef ZCL_ATTRID_AC_POWER_MULTIPLIER_ENABLE
u16 zclElectricalMeasure_ACPowerMultiplier = 0x0001;
#endif
#ifdef ZCL_ATTRID_AC_POWER_DIVISOR_ENABLE
u16 zclElectricalMeasure_ACPowerDivisor = 0x0001;
#endif

//DC Manufacturer Threshold Alarms
#ifdef ZCL_ATTRID_DC_OVERLOAD_ALARMS_MASK_ENABLE
u8 zclElectricalMeasure_DCOverloadAlarmsMask = 0;
#endif
#ifdef ZCL_ATTRID_DC_VOLTAGE_OVERLOAD_ENABLE
s16 zclElectricalMeasure_DCVoltageOverload = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_DC_CURRENT_OVERLOAD_ENABLE
s16 zclElectricalMeasure_DCCurrentOverload = 0xFFFF;
#endif

//AC Manufacturer Threshold Alarms
#ifdef ZCL_ATTRID_AC_ALARMS_MASK_ENABLE
u16 zclElectricalMeasure_ACAlarmsMask = 0;
#endif
#ifdef ZCL_ATTRID_AC_VOLTAGE_OVERLOAD_ENABLE
s16 zclElectricalMeasure_ACVoltageOverload = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_AC_CURRENT_OVERLOAD_ENABLE
s16 zclElectricalMeasure_ACCurrentOverload = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_AC_ACTIVE_POWER_OVERLOAD_ENABLE
s16 zclElectricalMeasure_ACActivePowerOverload = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_AC_REACTIVE_POWER_OVERLOAD_ENABLE
s16 zclElectricalMeasure_ACReactivePowerOverload = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_AVERAGE_RMS_OVER_VOLTAGE_ENABLE
s16 zclElectricalMeasure_averageRMSOverVoltage = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_AVERAGE_RMS_UNDER_VOLTAGE_ENABLE
s16 zclElectricalMeasure_averageRMSUnderVoltage = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_RMS_EXTREME_OVER_VOLTAGE_ENABLE
s16 zclElectricalMeasure_RMSExtremeOverVoltage = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_RMS_EXTREME_UNDER_VOLTAGE_ENABLE
s16 zclElectricalMeasure_RMSExtremeUnderVoltage = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_RMS_VOLTAGE_SAG_ENABLE
s16 zclElectricalMeasure_RMSVoltageSag = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_RMS_VOLTAGE_SWELL_ENABLE
s16 zclElectricalMeasure_RMSVoltageSwell = 0xFFFF;
#endif

//AC Phase B Measurements
#ifdef ZCL_ATTRID_LINE_CURRENT_PHB_ENABLE
u16 zclElectricalMeasure_lineCurrentPhB = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_ACTIVE_CURRENT_PHB_ENABLE
s16 zclElectricalMeasure_activeCurrentPhB = 0x8000;
#endif
#ifdef ZCL_ATTRID_REACTIVE_CURRENT_PHB_ENABLE
s16 zclElectricalMeasure_reactiveCurrentPhB = 0x8000;
#endif
#ifdef ZCL_ATTRID_RMS_VOLTAGE_PHB_ENABLE
u16 zclElectricalMeasure_RMSVoltagePhB = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_RMS_VOLTAGE_MIN_PHB_ENABLE
u16 zclElectricalMeasure_RMSVoltageMinPhB = 0x8000;
#endif
#ifdef ZCL_ATTRID_RMS_VOLTAGE_MAX_PHB_ENABLE
u16 zclElectricalMeasure_RMSVoltageMaxPhB = 0x8000;
#endif
#ifdef ZCL_ATTRID_RMS_CURRENT_PHB_ENABLE
u16 zclElectricalMeasure_RMSCurrentPhB = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_RMS_CURRENT_MIN_PHB_ENABLE
u16 zclElectricalMeasure_RMSCurrentMinPhB = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_RMS_CURRENT_MAX_PHB_ENABLE
u16 zclElectricalMeasure_RMSCurrentMaxPhB = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_ACTIVE_POWER_PHB_ENABLE
s16 zclElectricalMeasure_activePowerPhB = 0x8000;
#endif
#ifdef ZCL_ATTRID_ACTIVE_POWER_MIN_PHB_ENABLE
s16 zclElectricalMeasure_activePowerMinPhB = 0x8000;
#endif
#ifdef ZCL_ATTRID_ACTIVE_POWER_MAX_PHB_ENABLE
s16 zclElectricalMeasure_activePowerMaxPhB = 0x8000;
#endif
#ifdef ZCL_ATTRID_REACTIVE_POWER_PHB_ENABLE
s16 zclElectricalMeasure_reactivePowerPhB = 0x8000;
#endif
#ifdef ZCL_ATTRID_APPARENT_POWER_PHB_ENABLE
u16 zclElectricalMeasure_apparentPowerPhB = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_POWER_FACTOR_PHB_ENABLE
s8 zclElectricalMeasure_powerFactorPhB = 0x00;
#endif
#ifdef ZCL_ATTRID_AVERAGE_RMS_VOLTAGE_MEASUREMENT_PERIOD_PHB_ENABLE
u16 zclElectricalMeasure_averageRMSVoltageMeasurementPeriodPhB = 0x0000;
#endif
#ifdef ZCL_ATTRID_AVERAGE_RMS_OVER_VOLTAGE_CURRENT_PHB_ENABLE
u16 zclElectricalMeasure_averageRMSOverVoltageCounterPhB = 0x0000;
#endif
#ifdef ZCL_ATTRID_AVERAGE_RMS_UNDER_VOLTAGE_CURRENT_PHB_ENABLE
u16 zclElectricalMeasure_averageRMSUnderVoltageCounterPhB = 0x0000;
#endif
#ifdef ZCL_ATTRID_RMS_EXTREME_OVER_VOLTAHE_PERIOD_PHB_ENABLE
u16 zclElectricalMeasure_RMSExtremeOverVoltagePeriodPhB = 0x0000;
#endif
#ifdef ZCL_ATTRID_RMS_EXTREME_UNDER_VOLTAHE_PERIOD_PHB_ENABLE
u16 zclElectricalMeasure_RMSExtremeUnderVoltagePeriodPhB = 0x0000;
#endif
#ifdef ZCL_ATTRID_RMS_VOLTAHE_SAG_PERIOD_PHB_ENABLE
u16 zclElectricalMeasure_RMSVoltageSagPeriodPhB = 0x0000;
#endif
#ifdef ZCL_ATTRID_RMS_VOLTAHE_SWELL_PERIOD_PHB_ENABLE
u16 zclElectricalMeasure_RMSVoltageSwellPeriodPhB = 0x0000;
#endif

//AC Phase C Measurements
#ifdef ZCL_ATTRID_LINE_CURRENT_PHC_ENABLE
u16 zclElectricalMeasure_lineCurrentPhC = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_ACTIVE_CURRENT_PHC_ENABLE
s16 zclElectricalMeasure_activeCurrentPhC = 0x8000;
#endif
#ifdef ZCL_ATTRID_REACTIVE_CURRENT_PHC_ENABLE
s16 zclElectricalMeasure_reactiveCurrentPhC = 0x8000;
#endif
#ifdef ZCL_ATTRID_RMS_VOLTAGE_PHC_ENABLE
u16 zclElectricalMeasure_RMSVoltagePhC = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_RMS_VOLTAGE_MIN_PHC_ENABLE
u16 zclElectricalMeasure_RMSVoltageMinPhC = 0x8000;
#endif
#ifdef ZCL_ATTRID_RMS_VOLTAGE_MAX_PHC_ENABLE
u16 zclElectricalMeasure_RMSVoltageMaxPhC = 0x8000;
#endif
#ifdef ZCL_ATTRID_RMS_CURRENT_PHC_ENABLE
u16 zclElectricalMeasure_RMSCurrentPhC = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_RMS_CURRENT_MIN_PHC_ENABLE
u16 zclElectricalMeasure_RMSCurrentMinPhC = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_RMS_CURRENT_MAX_PHC_ENABLE
u16 zclElectricalMeasure_RMSCurrentMaxPhC = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_ACTIVE_POWER_PHC_ENABLE
s16 zclElectricalMeasure_activePowerPhC = 0x8000;
#endif
#ifdef ZCL_ATTRID_ACTIVE_POWER_MIN_PHC_ENABLE
s16 zclElectricalMeasure_activePowerMinPhC = 0x8000;
#endif
#ifdef ZCL_ATTRID_ACTIVE_POWER_MAX_PHC_ENABLE
s16 zclElectricalMeasure_activePowerMaxPhC = 0x8000;
#endif
#ifdef ZCL_ATTRID_REACTIVE_POWER_PHC_ENABLE
s16 zclElectricalMeasure_reactivePowerPhC = 0x8000;
#endif
#ifdef ZCL_ATTRID_APPARENT_POWER_PHC_ENABLE
u16 zclElectricalMeasure_apparentPowerPhC = 0xFFFF;
#endif
#ifdef ZCL_ATTRID_POWER_FACTOR_PHC_ENABLE
s8 zclElectricalMeasure_powerFactorPhC = 0x00;
#endif
#ifdef ZCL_ATTRID_AVERAGE_RMS_VOLTAGE_MEASUREMENT_PERIOD_PHC_ENABLE
u16 zclElectricalMeasure_averageRMSVoltageMeasurementPeriodPhC = 0x0000;
#endif
#ifdef ZCL_ATTRID_AVERAGE_RMS_OVER_VOLTAGE_CURRENT_PHC_ENABLE
u16 zclElectricalMeasure_averageRMSOverVoltageCounterPhC = 0x0000;
#endif
#ifdef ZCL_ATTRID_AVERAGE_RMS_UNDER_VOLTAGE_CURRENT_PHC_ENABLE
u16 zclElectricalMeasure_averageRMSUnderVoltageCounterPhC = 0x0000;
#endif
#ifdef ZCL_ATTRID_RMS_EXTREME_OVER_VOLTAHE_PERIOD_PHC_ENABLE
u16 zclElectricalMeasure_RMSExtremeOverVoltagePeriodPhC = 0x0000;
#endif
#ifdef ZCL_ATTRID_RMS_EXTREME_UNDER_VOLTAHE_PERIOD_PHC_ENABLE
u16 zclElectricalMeasure_RMSExtremeUnderVoltagePeriodPhC = 0x0000;
#endif
#ifdef ZCL_ATTRID_RMS_VOLTAHE_SAG_PERIOD_PHC_ENABLE
u16 zclElectricalMeasure_RMSVoltageSagPeriodPhC = 0x0000;
#endif
#ifdef ZCL_ATTRID_RMS_VOLTAHE_SWELL_PERIOD_PHC_ENABLE
u16 zclElectricalMeasure_RMSVoltageSwellPeriodPhC = 0x0000;
#endif


/* Attribute record list */
const zclAttrInfo_t electricalMeasure_attrTbl[] = {
    { ZCL_ATTRID_MEASUREMENT_TYPE,                              ZCL_DATA_TYPE_BITMAP32, ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_measurementType },
#ifdef ZCL_ATTRID_DC_VOLTAGE_ENABLE
    { ZCL_ATTRID_DC_VOLTAGE,                                    ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_DCVoltage },
#endif /* ZCL_ATTRID_DC_VOLTAGE_ENABLE */
#ifdef ZCL_ATTRID_DC_VOLTAGE_MIN_ENABLE
    { ZCL_ATTRID_DC_VOLTAGE_MIN,                                ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_DCVoltageMin },
#endif /* ZCL_ATTRID_DC_VOLTAGE_MIN_ENABLE */
#ifdef ZCL_ATTRID_DC_VOLTAGE_MAX_ENABLE
    { ZCL_ATTRID_DC_VOLTAGE_MAX,                                ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_DCVoltageMax },
#endif /* ZCL_ATTRID_DC_VOLTAGE_MAX_ENABLE */
#ifdef ZCL_ATTRID_DC_CURRENT_ENABLE
    { ZCL_ATTRID_DC_CURRENT,                                    ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_DCCurrent },
#endif /* ZCL_ATTRID_DC_CURRENT_ENABLE */
#ifdef ZCL_ATTRID_DC_CURRENT_MIN_ENABLE
    { ZCL_ATTRID_DC_CURRENT_MIN,                                ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_DCCurrentMin },
#endif /* ZCL_ATTRID_DC_CURRENT_MIN_ENABLE */
#ifdef ZCL_ATTRID_DC_CURRENT_MAX_ENABLE
    { ZCL_ATTRID_DC_CURRENT_MAX,                                ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_DCCurrentMax },
#endif /* ZCL_ATTRID_DC_CURRENT_MAX_ENABLE */
#ifdef ZCL_ATTRID_DC_POWER_ENABLE
    { ZCL_ATTRID_DC_POWER,                                      ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_DCPower },
#endif /* ZCL_ATTRID_DC_POWER_ENABLE */
#ifdef ZCL_ATTRID_DC_POWER_MIN_ENABLE
    { ZCL_ATTRID_DC_POWER_MIN,                                  ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_DCPowerMin },
#endif /* ZCL_ATTRID_DC_POWER_MIN_ENABLE */
#ifdef ZCL_ATTRID_DC_POWER_MAX_ENABLE
    { ZCL_ATTRID_DC_POWER_MAX,                                  ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_DCPowerMax },
#endif /* ZCL_ATTRID_DC_POWER_MAX_ENABLE */

#ifdef ZCL_ATTRID_DC_VOLTAGE_MULTIPLIER_ENABLE
    { ZCL_ATTRID_DC_VOLTAGE_MULTIPLIER,                         ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_DCVoltageMultiplier },
#endif /* ZCL_ATTRID_DC_VOLTAGE_MULTIPLIER_ENABLE */
#ifdef ZCL_ATTRID_DC_VOLTAGE_DIVISOR_ENABLE
    { ZCL_ATTRID_DC_VOLTAGE_DIVISOR,                            ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_DCVoltageDivisor },
#endif /* ZCL_ATTRID_DC_VOLTAGE_DIVISOR_ENABLE */
#ifdef ZCL_ATTRID_DC_CURRENT_MULTIPLIER_ENABLE
    { ZCL_ATTRID_DC_CURRENT_MULTIPLIER,                         ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_DCCurrentMultiplier },
#endif /* ZCL_ATTRID_DC_CURRENT_MULTIPLIER_ENABLE */
#ifdef ZCL_ATTRID_DC_CURRENT_DIVISOR_ENABLE
    { ZCL_ATTRID_DC_CURRENT_DIVISOR,                            ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_DCCurrentDivisor },
#endif /* ZCL_ATTRID_DC_CURRENT_DIVISOR_ENABLE */
#ifdef ZCL_ATTRID_DC_POWER_MULTIPLIER_ENABLE
    { ZCL_ATTRID_DC_POWER_MULTIPLIER,                           ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_DCPowerMultiplier },
#endif /* ZCL_ATTRID_DC_POWER_MULTIPLIER_ENABLE */
#ifdef ZCL_ATTRID_DC_POWER_DIVISOR_ENABLE
    { ZCL_ATTRID_DC_POWER_DIVISOR,                              ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_DCPowerDivisor },
#endif /* ZCL_ATTRID_DC_POWER_DIVISOR_ENABLE */

#ifdef ZCL_ATTRID_AC_FREQUENCY_ENABLE
    { ZCL_ATTRID_AC_FREQUENCY,                                  ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_ACFrequency },
#endif /* ZCL_ATTRID_AC_FREQUENCY_ENABLE */
#ifdef ZCL_ATTRID_AC_FREQUENCY_MIN_ENABLE
    { ZCL_ATTRID_AC_FREQUENCY_MIN,                              ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_ACFrequencyMin },
#endif /* ZCL_ATTRID_AC_FREQUENCY_MIN_ENABLE */
#ifdef ZCL_ATTRID_AC_FREQUENCY_MAX_ENABLE
    { ZCL_ATTRID_AC_FREQUENCY_MAX,                              ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_ACFrequencyMax },
#endif /* ZCL_ATTRID_AC_FREQUENCY_MAX_ENABLE */
#ifdef ZCL_ATTRID_NEUTRAL_CURRENT_ENABLE
    { ZCL_ATTRID_NEUTRAL_CURRENT,                               ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_neutralCurrent },
#endif /* ZCL_ATTRID_NEUTRAL_CURRENT_ENABLE */
#ifdef ZCL_ATTRID_TOTAL_ACTIVE_POWER_ENABLE
    { ZCL_ATTRID_TOTAL_ACTIVE_POWER,                            ZCL_DATA_TYPE_INT32,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_totalActivePower },
#endif /* ZCL_ATTRID_TOTAL_ACTIVE_POWER_ENABLE */
#ifdef ZCL_ATTRID_TOTAL_REACTIVE_POWER_ENABLE
    { ZCL_ATTRID_TOTAL_REACTIVE_POWER,                          ZCL_DATA_TYPE_INT32,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_totalReactivePower },
#endif /* ZCL_ATTRID_TOTAL_REACTIVE_POWER_ENABLE */
#ifdef ZCL_ATTRID_TOTAL_APPARENT_POWER_ENABLE
    { ZCL_ATTRID_TOTAL_APPARENT_POWER,                          ZCL_DATA_TYPE_UINT32,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_totalApparentPower },
#endif /* ZCL_ATTRID_TOTAL_APPARENT_POWER_ENABLE */
#ifdef ZCL_ATTRID_MEASURED_1ST_HARMONIC_CURRENT_ENABLE
    { ZCL_ATTRID_MEASURED_1ST_HARMONIC_CURRENT,                 ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_measured1stHarmonicCurrent },
#endif /* ZCL_ATTRID_MEASURED_1ST_HARMONIC_CURRENT_ENABLE */
#ifdef ZCL_ATTRID_MEASURED_3ST_HARMONIC_CURRENT_ENABLE
    { ZCL_ATTRID_MEASURED_3ST_HARMONIC_CURRENT,                 ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_measured3stHarmonicCurrent },
#endif /* ZCL_ATTRID_MEASURED_3ST_HARMONIC_CURRENT_ENABLE */
#ifdef ZCL_ATTRID_MEASURED_5ST_HARMONIC_CURRENT_ENABLE
    { ZCL_ATTRID_MEASURED_5ST_HARMONIC_CURRENT,                 ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_measured5stHarmonicCurrent },
#endif /* ZCL_ATTRID_MEASURED_5ST_HARMONIC_CURRENT_ENABLE */
#ifdef ZCL_ATTRID_MEASURED_7ST_HARMONIC_CURRENT_ENABLE
    { ZCL_ATTRID_MEASURED_7ST_HARMONIC_CURRENT,                 ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_measured7stHarmonicCurrent },
#endif /* ZCL_ATTRID_MEASURED_7ST_HARMONIC_CURRENT_ENABLE */
#ifdef ZCL_ATTRID_MEASURED_9ST_HARMONIC_CURRENT_ENABLE
    { ZCL_ATTRID_MEASURED_9ST_HARMONIC_CURRENT,                 ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_measured9stHarmonicCurrent },
#endif /* ZCL_ATTRID_MEASURED_9ST_HARMONIC_CURRENT_ENABLE */
#ifdef ZCL_ATTRID_MEASURED_11ST_HARMONIC_CURRENT_ENABLE
    { ZCL_ATTRID_MEASURED_11ST_HARMONIC_CURRENT,                ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_measured11stHarmonicCurrent },
#endif /* ZCL_ATTRID_MEASURED_11ST_HARMONIC_CURRENT_ENABLE */
#ifdef ZCL_ATTRID_MEASURED_PHASE_1ST_HARMONIC_CURRENT_ENABLE
    { ZCL_ATTRID_MEASURED_PHASE_1ST_HARMONIC_CURRENT,           ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_measuredPhase1stHarmonicCurrent },
#endif /* ZCL_ATTRID_MEASURED_PHASE_1ST_HARMONIC_CURRENT_ENABLE */
#ifdef ZCL_ATTRID_MEASURED_PHASE_3ST_HARMONIC_CURRENT_ENABLE
    { ZCL_ATTRID_MEASURED_PHASE_3ST_HARMONIC_CURRENT,           ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_measuredPhase3stHarmonicCurrent },
#endif /* ZCL_ATTRID_MEASURED_PHASE_3ST_HARMONIC_CURRENT_ENABLE */
#ifdef ZCL_ATTRID_MEASURED_PHASE_5ST_HARMONIC_CURRENT_ENABLE
    { ZCL_ATTRID_MEASURED_PHASE_5ST_HARMONIC_CURRENT,           ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_measuredPhase5stHarmonicCurrent },
#endif /* ZCL_ATTRID_MEASURED_PHASE_5ST_HARMONIC_CURRENT_ENABLE */
#ifdef ZCL_ATTRID_MEASURED_PHASE_7ST_HARMONIC_CURRENT_ENABLE
    { ZCL_ATTRID_MEASURED_PHASE_7ST_HARMONIC_CURRENT,           ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_measuredPhase7stHarmonicCurrent },
#endif /* ZCL_ATTRID_MEASURED_PHASE_7ST_HARMONIC_CURRENT_ENABLE */
#ifdef ZCL_ATTRID_MEASURED_PHASE_9ST_HARMONIC_CURRENT_ENABLE
    { ZCL_ATTRID_MEASURED_PHASE_9ST_HARMONIC_CURRENT,           ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_measuredPhase9stHarmonicCurrent },
#endif /* ZCL_ATTRID_MEASURED_PHASE_9ST_HARMONIC_CURRENT_ENABLE */
#ifdef ZCL_ATTRID_MEASURED_PHASE_11ST_HARMONIC_CURRENT_ENABLE
    { ZCL_ATTRID_MEASURED_PHASE_11ST_HARMONIC_CURRENT,          ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_measuredPhase11stHarmonicCurrent },
#endif /* ZCL_ATTRID_MEASURED_PHASE_11ST_HARMONIC_CURRENT_ENABLE */

#ifdef ZCL_ATTRID_AC_FREQUENCY_MULTIPLIER_ENABLE
    { ZCL_ATTRID_AC_FREQUENCY_MULTIPLIER,                       ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_ACFrequencyMultiplier },
#endif /* ZCL_ATTRID_AC_FREQUENCY_MULTIPLIER_ENABLE */
#ifdef ZCL_ATTRID_AC_FREQUENCY_DIVISOR_ENABLE
    { ZCL_ATTRID_AC_FREQUENCY_DIVISOR,                          ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_ACFrequencyDivisor },
#endif /* ZCL_ATTRID_AC_FREQUENCY_DIVISOR_ENABLE */
#ifdef ZCL_ATTRID_POWER_MULTIPLIER_ENABLE
    { ZCL_ATTRID_POWER_MULTIPLIER,                              ZCL_DATA_TYPE_UINT32,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_powerMultiplier },
#endif /* ZCL_ATTRID_POWER_MULTIPLIER_ENABLE */
#ifdef ZCL_ATTRID_POWER_DIVISOR_ENABLE
    { ZCL_ATTRID_POWER_DIVISOR,                                 ZCL_DATA_TYPE_UINT32,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_powerDivisor },
#endif /* ZCL_ATTRID_POWER_DIVISOR_ENABLE */
#ifdef ZCL_ATTRID_HARMONIC_CURRENT_MULTIPLIER_ENABLE
    { ZCL_ATTRID_HARMONIC_CURRENT_MULTIPLIER,                   ZCL_DATA_TYPE_INT8,     ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_harmonicCurrentMultiplier },
#endif /* ZCL_ATTRID_HARMONIC_CURRENT_MULTIPLIER_ENABLE */
#ifdef ZCL_ATTRID_PHASE_HARMONIC_CURRENT_MULTIPLIER_ENABLE
    { ZCL_ATTRID_PHASE_HARMONIC_CURRENT_MULTIPLIER,             ZCL_DATA_TYPE_INT8,     ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_phaseHarmonicCurrentMultiplier },
#endif /* ZCL_ATTRID_PHASE_HARMONIC_CURRENT_MULTIPLIER_ENABLE */

#ifdef ZCL_ATTRID_LINE_CURRENT_ENABLE
    { ZCL_ATTRID_LINE_CURRENT,                                  ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_lineCurrent },
#endif /* ZCL_ATTRID_LINE_CURRENT_ENABLE */
#ifdef ZCL_ATTRID_ACTIVE_CURRENT_ENABLE
    { ZCL_ATTRID_ACTIVE_CURRENT,                                ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_activeCurrent },
#endif /* ZCL_ATTRID_ACTIVE_CURRENT_ENABLE */
#ifdef ZCL_ATTRID_REACTIVE_CURRENT_ENABLE
    { ZCL_ATTRID_REACTIVE_CURRENT,                              ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_reactiveCurrent },
#endif /* ZCL_ATTRID_REACTIVE_CURRENT_ENABLE */
#ifdef ZCL_ATTRID_RMS_VOLTAGE_ENABLE
    { ZCL_ATTRID_RMS_VOLTAGE,                                   ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_RMSVoltage },
#endif /* ZCL_ATTRID_RMS_VOLTAGE_ENABLE */
#ifdef ZCL_ATTRID_RMS_VOLTAGE_MIN_ENABLE
    { ZCL_ATTRID_RMS_VOLTAGE_MIN,                               ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_RMSVoltageMin },
#endif /* ZCL_ATTRID_RMS_VOLTAGE_MIN_ENABLE */
#ifdef ZCL_ATTRID_RMS_VOLTAGE_MAX_ENABLE
    { ZCL_ATTRID_RMS_VOLTAGE_MAX,                               ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_RMSVoltageMax },
#endif /* ZCL_ATTRID_RMS_VOLTAGE_MAX_ENABLE */
#ifdef ZCL_ATTRID_RMS_CURRENT_ENABLE
    { ZCL_ATTRID_RMS_CURRENT,                                   ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_RMSCurrent },
#endif /* ZCL_ATTRID_RMS_CURRENT_ENABLE */
#ifdef ZCL_ATTRID_RMS_CURRENT_MIN_ENABLE
    { ZCL_ATTRID_RMS_CURRENT_MIN,                               ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_RMSCurrentMin },
#endif /* ZCL_ATTRID_RMS_CURRENT_MIN_ENABLE */
#ifdef ZCL_ATTRID_RMS_CURRENT_MAX_ENABLE
    { ZCL_ATTRID_RMS_CURRENT_MAX,                               ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_RMSCurrentMax },
#endif /* ZCL_ATTRID_RMS_CURRENT_MAX_ENABLE */
#ifdef ZCL_ATTRID_ACTIVE_POWER_ENABLE
    { ZCL_ATTRID_ACTIVE_POWER,                                  ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_activePower },
#endif /* ZCL_ATTRID_ACTIVE_POWER_ENABLE */
#ifdef ZCL_ATTRID_ACTIVE_POWER_MIN_ENABLE
    { ZCL_ATTRID_ACTIVE_POWER_MIN,                              ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_activePowerMin },
#endif /* ZCL_ATTRID_ACTIVE_POWER_MIN_ENABLE */
#ifdef ZCL_ATTRID_ACTIVE_POWER_MAX_ENABLE
    { ZCL_ATTRID_ACTIVE_POWER_MAX,                              ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_activePowerMax },
#endif /* ZCL_ATTRID_ACTIVE_POWER_MAX_ENABLE */
#ifdef 	ZCL_ATTRID_REACTIVE_POWER_ENABLE
    { ZCL_ATTRID_REACTIVE_POWER,                                ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_reactivePower },
#endif /* ZCL_ATTRID_REACTIVE_POWER_ENABLE */
#ifdef ZCL_ATTRID_APPARENT_POWER_ENABLE
    { ZCL_ATTRID_APPARENT_POWER,                                ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_apparentPower },
#endif /* ZCL_ATTRID_APPARENT_POWER_ENABLE */
#ifdef ZCL_ATTRID_POWER_FACTOR_ENABLE
    { ZCL_ATTRID_POWER_FACTOR,                                  ZCL_DATA_TYPE_INT8,     ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_powerFactor },
#endif /* ZCL_ATTRID_POWER_FACTOR_ENABLE */
#ifdef ZCL_ATTRID_AVERAGE_RMS_VOLTAGE_MEASUREMENT_PERIOD_ENABLE
    { ZCL_ATTRID_AVERAGE_RMS_VOLTAGE_MEASUREMENT_PERIOD,        ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_averageRMSVoltageMeasurementPeriod },
#endif /* ZCL_ATTRID_AVERAGE_RMS_VOLTAGE_MEASUREMENT_PERIOD_ENABLE */
#ifdef ZCL_ATTRID_AVERAGE_RMS_OVER_VOLTAGE_COUNTER_ENABLE
    { ZCL_ATTRID_AVERAGE_RMS_OVER_VOLTAGE_COUNTER,              ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_averageRMSOverVoltageCounter },
#endif /* ZCL_ATTRID_AVERAGE_RMS_OVER_VOLTAGE_COUNTER_ENABLE */
#ifdef ZCL_ATTRID_AVERAGE_RMS_UNDER_VOLTAGE_COUNTER_ENABLE
    { ZCL_ATTRID_AVERAGE_RMS_UNDER_VOLTAGE_COUNTER,             ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_averageRMSUnderVoltageCounter },
#endif /* ZCL_ATTRID_AVERAGE_RMS_UNDER_VOLTAGE_COUNTER_ENABLE */
#ifdef ZCL_ATTRID_RMS_EXTREME_OVER_VOLTAGE_PERIOD_ENABLE
    { ZCL_ATTRID_RMS_EXTREME_OVER_VOLTAGE_PERIOD,               ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_RMSExtremeOverVoltagePeriod },
#endif /* ZCL_ATTRID_RMS_EXTREME_OVER_VOLTAGE_PERIOD_ENABLE */
#ifdef ZCL_ATTRID_RMS_EXTREME_UNDER_VOLTAGE_PERIOD_ENABLE
    { ZCL_ATTRID_RMS_EXTREME_UNDER_VOLTAGE_PERIOD,              ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_RMSExtremeUnderVoltagePeriod },
#endif /* ZCL_ATTRID_RMS_EXTREME_UNDER_VOLTAGE_PERIOD_ENABLE */
#ifdef ZCL_ATTRID_RMS_VOLTAGE_SAG_PERIOD_ENABLE
    { ZCL_ATTRID_RMS_VOLTAGE_SAG_PERIOD,                        ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_RMSVoltageSagPeriod },
#endif /* ZCL_ATTRID_RMS_VOLTAGE_SAG_PERIOD_ENABLE */
#ifdef ZCL_ATTRID_RMS_VOLTAGE_SWELL_PERIOD_ENABLE
    { ZCL_ATTRID_RMS_VOLTAGE_SWELL_PERIOD,                      ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_RMSVoltageSwellPeriod },
#endif /* ZCL_ATTRID_RMS_VOLTAGE_SWELL_PERIOD_ENABLE */

#ifdef ZCL_ATTRID_AC_VOLTAGE_MULTIPLIER_ENABLE
    { ZCL_ATTRID_AC_VOLTAGE_MULTIPLIER,                         ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_ACVoltageMultiplier },
#endif /* ZCL_ATTRID_AC_VOLTAGE_MULTIPLIER_ENABLE */
#ifdef ZCL_ATTRID_AC_VOLTAGE_DIVISOR_ENABLE
    { ZCL_ATTRID_AC_VOLTAGE_DIVISOR,                            ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_ACVoltageDivisor },
#endif /* ZCL_ATTRID_AC_VOLTAGE_DIVISOR_ENABLE */
#ifdef ZCL_ATTRID_AC_CURRENT_MULTIPLIER_ENABLE
    { ZCL_ATTRID_AC_CURRENT_MULTIPLIER,                         ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_ACCurrentMultiplier },
#endif /* ZCL_ATTRID_AC_CURRENT_MULTIPLIER_ENABLE */
#ifdef ZCL_ATTRID_AC_CURRENT_DIVISOR_ENABLE
    { ZCL_ATTRID_AC_CURRENT_DIVISOR,                            ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_ACCurrentDivisor },
#endif /* ZCL_ATTRID_AC_CURRENT_DIVISOR_ENABLE */
#ifdef ZCL_ATTRID_AC_POWER_MULTIPLIER_ENABLE
    { ZCL_ATTRID_AC_POWER_MULTIPLIER,                           ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_ACPowerMultiplier },
#endif /* ZCL_ATTRID_AC_POWER_MULTIPLIER_ENABLE */
#ifdef ZCL_ATTRID_AC_POWER_DIVISOR_ENABLE
    { ZCL_ATTRID_AC_POWER_DIVISOR,                              ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_ACPowerDivisor },
#endif /* ZCL_ATTRID_AC_POWER_DIVISOR_ENABLE */

#ifdef ZCL_ATTRID_DC_OVERLOAD_ALARMS_MASK_ENABLE
    { ZCL_ATTRID_DC_OVERLOAD_ALARMS_MASK,                       ZCL_DATA_TYPE_BITMAP8,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_DCOverloadAlarmsMask },
#endif /* ZCL_ATTRID_DC_OVERLOAD_ALARMS_MASK_ENABLE */
#ifdef ZCL_ATTRID_DC_VOLTAGE_OVERLOAD_ENABLE
    { ZCL_ATTRID_DC_VOLTAGE_OVERLOAD,                           ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_DCVoltageOverload },
#endif /* ZCL_ATTRID_DC_VOLTAGE_OVERLOAD_ENABLE */
#ifdef ZCL_ATTRID_DC_CURRENT_OVERLOAD_ENABLE
    { ZCL_ATTRID_DC_CURRENT_OVERLOAD,                           ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_DCCurrentOverload },
#endif /* ZCL_ATTRID_DC_CURRENT_OVERLOAD_ENABLE */

#ifdef ZCL_ATTRID_AC_ALARMS_MASK_ENABLE
    { ZCL_ATTRID_AC_ALARMS_MASK,                                ZCL_DATA_TYPE_BITMAP16, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_ACAlarmsMask },
#endif /* ZCL_ATTRID_AC_ALARMS_MASK_ENABLE */
#ifdef ZCL_ATTRID_AC_VOLTAGE_OVERLOAD_ENABLE
    { ZCL_ATTRID_AC_VOLTAGE_OVERLOAD,                           ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_ACVoltageOverload },
#endif /* ZCL_ATTRID_AC_VOLTAGE_OVERLOAD_ENABLE */
#ifdef ZCL_ATTRID_AC_CURRENT_OVERLOAD_ENABLE
    { ZCL_ATTRID_AC_CURRENT_OVERLOAD,                           ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_ACCurrentOverload },
#endif /* ZCL_ATTRID_AC_CURRENT_OVERLOAD_ENABLE */
#ifdef ZCL_ATTRID_AC_ACTIVE_POWER_OVERLOAD_ENABLE
    { ZCL_ATTRID_AC_ACTIVE_POWER_OVERLOAD,                      ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_ACActivePowerOverload },
#endif /* ZCL_ATTRID_AC_ACTIVE_POWER_OVERLOAD_ENABLE */
#ifdef ZCL_ATTRID_AC_REACTIVE_POWER_OVERLOAD_ENABLE
    { ZCL_ATTRID_AC_REACTIVE_POWER_OVERLOAD,                    ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_ACReactivePowerOverload },
#endif /* ZCL_ATTRID_AC_REACTIVE_POWER_OVERLOAD_ENABLE */
#ifdef ZCL_ATTRID_AVERAGE_RMS_OVER_VOLTAGE_ENABLE
    { ZCL_ATTRID_AVERAGE_RMS_OVER_VOLTAGE,                      ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_averageRMSOverVoltage },
#endif /* ZCL_ATTRID_AVERAGE_RMS_OVER_VOLTAGE_ENABLE */
#ifdef ZCL_ATTRID_AVERAGE_RMS_UNDER_VOLTAGE_ENABLE
    { ZCL_ATTRID_AVERAGE_RMS_UNDER_VOLTAGE,                     ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_averageRMSUnderVoltage },
#endif /* ZCL_ATTRID_AVERAGE_RMS_UNDER_VOLTAGE_ENABLE */
#ifdef ZCL_ATTRID_RMS_EXTREME_OVER_VOLTAGE_ENABLE
    { ZCL_ATTRID_RMS_EXTREME_OVER_VOLTAGE,                      ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_RMSExtremeOverVoltage },
#endif /* ZCL_ATTRID_RMS_EXTREME_OVER_VOLTAGE_ENABLE */
#ifdef ZCL_ATTRID_RMS_EXTREME_UNDER_VOLTAGE_ENABLE
    { ZCL_ATTRID_RMS_EXTREME_UNDER_VOLTAGE,                     ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_RMSExtremeUnderVoltage },
#endif /* ZCL_ATTRID_RMS_EXTREME_UNDER_VOLTAGE_ENABLE */
#ifdef ZCL_ATTRID_RMS_VOLTAGE_SAG_ENABLE
    { ZCL_ATTRID_RMS_VOLTAGE_SAG,                               ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_RMSVoltageSag },
#endif /* ZCL_ATTRID_RMS_VOLTAGE_SAG_ENABLE */
#ifdef ZCL_ATTRID_RMS_VOLTAGE_SWELL_ENABLE
    { ZCL_ATTRID_RMS_VOLTAGE_SWELL,                             ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_RMSVoltageSwell },
#endif /* ZCL_ATTRID_RMS_VOLTAGE_SWELL_ENABLE */

#ifdef ZCL_ATTRID_LINE_CURRENT_PHB_ENABLE
    { ZCL_ATTRID_LINE_CURRENT_PHB,                              ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_lineCurrentPhB },
#endif /* ZCL_ATTRID_LINE_CURRENT_PHB_ENABLE */
#ifdef ZCL_ATTRID_ACTIVE_CURRENT_PHB_ENABLE
    { ZCL_ATTRID_ACTIVE_CURRENT_PHB,                            ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_activeCurrentPhB },
#endif /* ZCL_ATTRID_ACTIVE_CURRENT_PHB_ENABLE */
#ifdef ZCL_ATTRID_REACTIVE_CURRENT_PHB_ENABLE
    { ZCL_ATTRID_REACTIVE_CURRENT_PHB,                          ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_reactiveCurrentPhB },
#endif /* ZCL_ATTRID_REACTIVE_CURRENT_PHB_ENABLE */
#ifdef ZCL_ATTRID_RMS_VOLTAGE_PHB_ENABLE
    { ZCL_ATTRID_RMS_VOLTAGE_PHB,                               ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_RMSVoltagePhB },
#endif /* ZCL_ATTRID_RMS_VOLTAGE_PHB_ENABLE */
#ifdef ZCL_ATTRID_RMS_VOLTAGE_MIN_PHB_ENABLE
    { ZCL_ATTRID_RMS_VOLTAGE_MIN_PHB,                           ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_RMSVoltageMinPhB },
#endif /* ZCL_ATTRID_RMS_VOLTAGE_MIN_PHB_ENABLE */
#ifdef ZCL_ATTRID_RMS_VOLTAGE_MAX_PHB_ENABLE
    { ZCL_ATTRID_RMS_VOLTAGE_MAX_PHB,                           ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_RMSVoltageMaxPhB },
#endif /* ZCL_ATTRID_RMS_VOLTAGE_MAX_PHB_ENABLE */
#ifdef ZCL_ATTRID_RMS_CURRENT_PHB_ENABLE
    { ZCL_ATTRID_RMS_CURRENT_PHB,                               ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_RMSCurrentPhB },
#endif /* ZCL_ATTRID_RMS_CURRENT_PHB_ENABLE */
#ifdef ZCL_ATTRID_RMS_CURRENT_MIN_PHB_ENABLE
    { ZCL_ATTRID_RMS_CURRENT_MIN_PHB,                           ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_RMSCurrentMinPhB },
#endif /* ZCL_ATTRID_RMS_CURRENT_MIN_PHB_ENABLE */
#ifdef ZCL_ATTRID_RMS_CURRENT_MAX_PHB_ENABLE
    { ZCL_ATTRID_RMS_CURRENT_MAX_PHB,                           ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_RMSCurrentMaxPhB },
#endif /* ZCL_ATTRID_RMS_CURRENT_MAX_PHB_ENABLE */
#ifdef ZCL_ATTRID_ACTIVE_POWER_PHB_ENABLE
    { ZCL_ATTRID_ACTIVE_POWER_PHB,                              ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_activePowerPhB },
#endif /* ZCL_ATTRID_ACTIVE_POWER_PHB_ENABLE */
#ifdef ZCL_ATTRID_ACTIVE_POWER_MIN_PHB_ENABLE
    { ZCL_ATTRID_ACTIVE_POWER_MIN_PHB,                          ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_activePowerMinPhB },
#endif /* ZCL_ATTRID_ACTIVE_POWER_MIN_PHB_ENABLE */
#ifdef ZCL_ATTRID_ACTIVE_POWER_MAX_PHB_ENABLE
    { ZCL_ATTRID_ACTIVE_POWER_MAX_PHB,                          ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_activePowerMaxPhB },
#endif /* ZCL_ATTRID_ACTIVE_POWER_MAX_PHB_ENABLE */
#ifdef ZCL_ATTRID_REACTIVE_POWER_PHB_ENABLE
    { ZCL_ATTRID_REACTIVE_POWER_PHB,                            ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_reactivePowerPhB },
#endif /* ZCL_ATTRID_REACTIVE_POWER_PHB_ENABLE */
#ifdef ZCL_ATTRID_APPARENT_POWER_PHB_ENABLE
    { ZCL_ATTRID_APPARENT_POWER_PHB,                            ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_apparentPowerPhB },
#endif /* ZCL_ATTRID_APPARENT_POWER_PHB_ENABLE */
#ifdef ZCL_ATTRID_POWER_FACTOR_PHB_ENABLE
    { ZCL_ATTRID_POWER_FACTOR_PHB,                              ZCL_DATA_TYPE_INT8,     ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_powerFactorPhB },
#endif /* ZCL_ATTRID_POWER_FACTOR_PHB_ENABLE */
#ifdef ZCL_ATTRID_AVERAGE_RMS_VOLTAGE_MEASUREMENT_PERIOD_PHB_ENABLE
    { ZCL_ATTRID_AVERAGE_RMS_VOLTAGE_MEASUREMENT_PERIOD_PHB,    ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_averageRMSVoltageMeasurementPeriodPhB },
#endif /* ZCL_ATTRID_AVERAGE_RMS_VOLTAGE_MEASUREMENT_PERIOD_PHB_ENABLE */
#ifdef ZCL_ATTRID_AVERAGE_RMS_OVER_VOLTAGE_CURRENT_PHB_ENABLE
    { ZCL_ATTRID_AVERAGE_RMS_OVER_VOLTAGE_CURRENT_PHB,          ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_averageRMSOverVoltageCounterPhB },
#endif /* ZCL_ATTRID_AVERAGE_RMS_OVER_VOLTAGE_CURRENT_PHB_ENABLE */
#ifdef ZCL_ATTRID_AVERAGE_RMS_UNDER_VOLTAGE_CURRENT_PHB_ENABLE
    { ZCL_ATTRID_AVERAGE_RMS_UNDER_VOLTAGE_CURRENT_PHB,         ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_averageRMSUnderVoltageCounterPhB },
#endif /* ZCL_ATTRID_AVERAGE_RMS_UNDER_VOLTAGE_CURRENT_PHB_ENABLE */
#ifdef ZCL_ATTRID_RMS_EXTREME_OVER_VOLTAHE_PERIOD_PHB_ENABLE
    { ZCL_ATTRID_RMS_EXTREME_OVER_VOLTAHE_PERIOD_PHB,           ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_RMSExtremeOverVoltagePeriodPhB },
#endif /* ZCL_ATTRID_RMS_EXTREME_OVER_VOLTAHE_PERIOD_PHB_ENABLE */
#ifdef ZCL_ATTRID_RMS_EXTREME_UNDER_VOLTAHE_PERIOD_PHB_ENABLE
    { ZCL_ATTRID_RMS_EXTREME_UNDER_VOLTAHE_PERIOD_PHB,          ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_RMSExtremeUnderVoltagePeriodPhB },
#endif /* ZCL_ATTRID_RMS_EXTREME_UNDER_VOLTAHE_PERIOD_PHB_ENABLE */
#ifdef ZCL_ATTRID_RMS_VOLTAHE_SAG_PERIOD_PHB_ENABLE
    { ZCL_ATTRID_RMS_VOLTAHE_SAG_PERIOD_PHB,                    ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_RMSVoltageSagPeriodPhB },
#endif /* ZCL_ATTRID_RMS_VOLTAHE_SAG_PERIOD_PHB_ENABLE */
#ifdef ZCL_ATTRID_RMS_VOLTAHE_SWELL_PERIOD_PHB_ENABLE
    { ZCL_ATTRID_RMS_VOLTAHE_SWELL_PERIOD_PHB,                  ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_RMSVoltageSwellPeriodPhB },
#endif /* ZCL_ATTRID_RMS_VOLTAHE_SWELL_PERIOD_PHB_ENABLE */

#ifdef ZCL_ATTRID_LINE_CURRENT_PHC_ENABLE
    { ZCL_ATTRID_LINE_CURRENT_PHC,                              ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_lineCurrentPhC },
#endif /* ZCL_ATTRID_LINE_CURRENT_PHC_ENABLE */
#ifdef ZCL_ATTRID_ACTIVE_CURRENT_PHC_ENABLE
    { ZCL_ATTRID_ACTIVE_CURRENT_PHC,                            ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_activeCurrentPhC },
#endif /* ZCL_ATTRID_ACTIVE_CURRENT_PHC_ENABLE */
#ifdef ZCL_ATTRID_REACTIVE_CURRENT_PHC_ENABLE
    { ZCL_ATTRID_REACTIVE_CURRENT_PHC,                          ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_reactiveCurrentPhC },
#endif /* ZCL_ATTRID_REACTIVE_CURRENT_PHC_ENABLE */
#ifdef ZCL_ATTRID_RMS_VOLTAGE_PHC_ENABLE
    { ZCL_ATTRID_RMS_VOLTAGE_PHC,                               ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_RMSVoltagePhC },
#endif /* ZCL_ATTRID_RMS_VOLTAGE_PHC_ENABLE */
#ifdef ZCL_ATTRID_RMS_VOLTAGE_MIN_PHC_ENABLE
    { ZCL_ATTRID_RMS_VOLTAGE_MIN_PHC,                           ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_RMSVoltageMinPhC },
#endif /* ZCL_ATTRID_RMS_VOLTAGE_MIN_PHC_ENABLE */
#ifdef ZCL_ATTRID_RMS_VOLTAGE_MAX_PHC_ENABLE
    { ZCL_ATTRID_RMS_VOLTAGE_MAX_PHC,                           ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_RMSVoltageMaxPhC },
#endif /* ZCL_ATTRID_RMS_VOLTAGE_MAX_PHC_ENABLE */
#ifdef ZCL_ATTRID_RMS_CURRENT_PHC_ENABLE
    { ZCL_ATTRID_RMS_CURRENT_PHC,                               ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_RMSCurrentPhC },
#endif /* ZCL_ATTRID_RMS_CURRENT_PHC_ENABLE */
#ifdef ZCL_ATTRID_RMS_CURRENT_MIN_PHC_ENABLE
    { ZCL_ATTRID_RMS_CURRENT_MIN_PHC,                           ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_RMSCurrentMinPhC },
#endif /* ZCL_ATTRID_RMS_CURRENT_MIN_PHC_ENABLE */
#ifdef ZCL_ATTRID_RMS_CURRENT_MAX_PHC_ENABLE
    { ZCL_ATTRID_RMS_CURRENT_MAX_PHC,                           ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_RMSCurrentMaxPhC },
#endif /* ZCL_ATTRID_RMS_CURRENT_MAX_PHC_ENABLE */
#ifdef ZCL_ATTRID_ACTIVE_POWER_PHC_ENABLE
    { ZCL_ATTRID_ACTIVE_POWER_PHC,                              ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_activePowerPhC },
#endif /* ZCL_ATTRID_ACTIVE_POWER_PHC_ENABLE */
#ifdef ZCL_ATTRID_ACTIVE_POWER_MIN_PHC_ENABLE
    { ZCL_ATTRID_ACTIVE_POWER_MIN_PHC,                          ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_activePowerMinPhC },
#endif /* ZCL_ATTRID_ACTIVE_POWER_MIN_PHC_ENABLE */
#ifdef ZCL_ATTRID_ACTIVE_POWER_MAX_PHC_ENABLE
    { ZCL_ATTRID_ACTIVE_POWER_MAX_PHC,                          ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_activePowerMaxPhC },
#endif /* ZCL_ATTRID_ACTIVE_POWER_MAX_PHC_ENABLE */
#ifdef ZCL_ATTRID_REACTIVE_POWER_PHC_ENABLE
    { ZCL_ATTRID_REACTIVE_POWER_PHC,                            ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_reactivePowerPhC },
#endif /* ZCL_ATTRID_REACTIVE_POWER_PHC_ENABLE */
#ifdef ZCL_ATTRID_APPARENT_POWER_PHC_ENABLE
    { ZCL_ATTRID_APPARENT_POWER_PHC,                            ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zclElectricalMeasure_apparentPowerPhC },
#endif /* ZCL_ATTRID_APPARENT_POWER_PHC_ENABLE */
#ifdef ZCL_ATTRID_POWER_FACTOR_PHC_ENABLE
    { ZCL_ATTRID_POWER_FACTOR_PHC,                              ZCL_DATA_TYPE_INT8,     ACCESS_CONTROL_READ,                             (u8*)&zclElectricalMeasure_powerFactorPhC },
#endif /* ZCL_ATTRID_POWER_FACTOR_PHC_ENABLE */
#ifdef ZCL_ATTRID_AVERAGE_RMS_VOLTAGE_MEASUREMENT_PERIOD_PHC_ENABLE
    { ZCL_ATTRID_AVERAGE_RMS_VOLTAGE_MEASUREMENT_PERIOD_PHC,    ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_averageRMSVoltageMeasurementPeriodPhC },
#endif /* ZCL_ATTRID_AVERAGE_RMS_VOLTAGE_MEASUREMENT_PERIOD_PHC_ENABLE */
#ifdef ZCL_ATTRID_AVERAGE_RMS_OVER_VOLTAGE_CURRENT_PHC_ENABLE
    { ZCL_ATTRID_AVERAGE_RMS_OVER_VOLTAGE_CURRENT_PHC,          ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_averageRMSOverVoltageCounterPhC },
#endif /* ZCL_ATTRID_AVERAGE_RMS_OVER_VOLTAGE_CURRENT_PHC_ENABLE */
#ifdef ZCL_ATTRID_AVERAGE_RMS_UNDER_VOLTAGE_CURRENT_PHC_ENABLE
    { ZCL_ATTRID_AVERAGE_RMS_UNDER_VOLTAGE_CURRENT_PHC,         ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_averageRMSUnderVoltageCounterPhC },
#endif /* ZCL_ATTRID_AVERAGE_RMS_UNDER_VOLTAGE_CURRENT_PHC_ENABLE */
#ifdef ZCL_ATTRID_RMS_EXTREME_OVER_VOLTAHE_PERIOD_PHC_ENABLE
    { ZCL_ATTRID_RMS_EXTREME_OVER_VOLTAHE_PERIOD_PHC,           ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_RMSExtremeOverVoltagePeriodPhC },
#endif /* ZCL_ATTRID_RMS_EXTREME_OVER_VOLTAHE_PERIOD_PHC_ENABLE */
#ifdef ZCL_ATTRID_RMS_EXTREME_UNDER_VOLTAHE_PERIOD_PHC_ENABLE
    { ZCL_ATTRID_RMS_EXTREME_UNDER_VOLTAHE_PERIOD_PHC,          ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_RMSExtremeUnderVoltagePeriodPhC },
#endif /* ZCL_ATTRID_RMS_EXTREME_UNDER_VOLTAHE_PERIOD_PHC_ENABLE */
#ifdef ZCL_ATTRID_RMS_VOLTAHE_SAG_PERIOD_PHC_ENABLE
    { ZCL_ATTRID_RMS_VOLTAHE_SAG_PERIOD_PHC,                    ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_RMSVoltageSagPeriodPhC },
#endif /* ZCL_ATTRID_RMS_VOLTAHE_SAG_PERIOD_PHC_ENABLE */
#ifdef ZCL_ATTRID_RMS_VOLTAHE_SWELL_PERIOD_PHC_ENABLE
    { ZCL_ATTRID_RMS_VOLTAHE_SWELL_PERIOD_PHC,                  ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zclElectricalMeasure_RMSVoltageSwellPeriodPhC },
#endif /* ZCL_ATTRID_RMS_VOLTAHE_SWELL_PERIOD_PHC_ENABLE */
};

const u8 zcl_electricalMeasure_attrNum = (sizeof(electricalMeasure_attrTbl) / sizeof(zclAttrInfo_t));

#endif    /* ZCL_ELECTRICAL_MEASUREMENT */
#endif
