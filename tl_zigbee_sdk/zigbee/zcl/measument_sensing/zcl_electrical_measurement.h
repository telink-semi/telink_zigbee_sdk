/********************************************************************************************************
 * @file     zcl_electrical_measurement.h
 *
 * @brief	 APIs statement for electrical measurement cluster
 *
 * @author
 * @date     June. 10, 2017
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#pragma once

#include "tl_common.h"
#include "../../zdo/zb_af.h"
#include "../zcl.h"

/*********************************************************************
 * CONSTANTS
 */
/**
 *  @brief	Attribute Sets
 */
#define BASIC_INFOMATION											0x00
#define DC_MEASUREMENT												0x01
#define DC_FORMATTING												0x02
#define AC_NON_PHASE_SPEC_MEASUREMENTS								0x03
#define AC_NON_PHASE_SPEC_FORMATTING								0x04
#define AC_SINGLE_PHASE_0R_PHASE_A_MEASUREMENTS						0x05
#define AC_FORMATTING												0x06
#define DC_MAUFACTURER_THRESHOLD_ALARMS								0x07
#define AC_MAUFACTURER_THRESHOLD_ALARMS								0x08
#define AC_PHASE_B_MEASUREMENTS										0x09
#define AC_PHASE_C_MEASUREMENTS										0x0A

/**
 *  @brief	electrical measurement cluster Attribute IDs
 */
//Basic Information
#define ZCL_ATTRID_MEASUREMENT_TYPE									0x0000
//DC Measurement
#define ZCL_ATTRID_DC_VOLTAGE			                    		0x0100
#define ZCL_ATTRID_DC_VOLTAGE_MIN                     				0x0101
#define ZCL_ATTRID_DC_VOLTAGE_MAX                           		0x0102
#define ZCL_ATTRID_DC_CURRENT		                        		0x0103
#define ZCL_ATTRID_DC_CURRENT_MIN	                        		0x0104
#define ZCL_ATTRID_DC_CURRENT_MAX	                        		0x0105
#define ZCL_ATTRID_DC_POWER			                        		0x0106
#define ZCL_ATTRID_DC_POWER_MIN		                        		0x0107
#define ZCL_ATTRID_DC_POWER_MAX		                        		0x0108
//DC Formatting
#define ZCL_ATTRID_DC_VOLTAGE_MULTIPLIER                    		0x0200
#define ZCL_ATTRID_DC_VOLTAGE_DIVISOR	                    		0x0201
#define ZCL_ATTRID_DC_CURRENT_MULTIPLIER                    		0x0202
#define ZCL_ATTRID_DC_CURRENT_DIVISOR	                    		0x0203
#define ZCL_ATTRID_DC_POWER_MULTIPLIER								0x0204
#define ZCL_ATTRID_DC_POWER_DIVISOR	                				0x0205
//AC (Non-phase Specific) Measurements
#define ZCL_ATTRID_AC_FREQUENCY			                    		0x0300
#define ZCL_ATTRID_AC_FREQUENCY_MIN		                    		0x0301
#define ZCL_ATTRID_AC_FREQUENCY_MAX		                    		0x0302
#define ZCL_ATTRID_NEUTRAL_CURRENT		                    		0x0303
#define ZCL_ATTRID_TOTAL_ACTIVE_POWER	                    		0x0304
#define ZCL_ATTRID_TOTAL_REACTIVE_POWER	                    		0x0305
#define ZCL_ATTRID_TOTAL_APPARENT_POWER	                    		0x0306
#define ZCL_ATTRID_MEASURED_1ST_HARMONIC_CURRENT            		0x0307
#define ZCL_ATTRID_MEASURED_3ST_HARMONIC_CURRENT            		0x0308
#define ZCL_ATTRID_MEASURED_5ST_HARMONIC_CURRENT            		0x0309
#define ZCL_ATTRID_MEASURED_7ST_HARMONIC_CURRENT            		0x030A
#define ZCL_ATTRID_MEASURED_9ST_HARMONIC_CURRENT            		0x030B
#define ZCL_ATTRID_MEASURED_11ST_HARMONIC_CURRENT           		0x030C
#define ZCL_ATTRID_MEASURED_PHASE_1ST_HARMONIC_CURRENT      		0x030D
#define ZCL_ATTRID_MEASURED_PHASE_3ST_HARMONIC_CURRENT      		0x030E
#define ZCL_ATTRID_MEASURED_PHASE_5ST_HARMONIC_CURRENT      		0x030F
#define ZCL_ATTRID_MEASURED_PHASE_7ST_HARMONIC_CURRENT      		0x0310
#define ZCL_ATTRID_MEASURED_PHASE_9ST_HARMONIC_CURRENT      		0x0311
#define ZCL_ATTRID_MEASURED_PHASE_11ST_HARMONIC_CURRENT     		0x0312
//AC (Non-phase Specific) Formatting
#define ZCL_ATTRID_AC_FREQUENCY_MULTIPLIER                  		0x0400
#define ZCL_ATTRID_AC_FREQUENCY_DIVISOR	                    		0x0401
#define ZCL_ATTRID_POWER_MULTIPLIER               		    		0x0402
#define ZCL_ATTRID_POWER_DIVISOR	               		    		0x0403
#define ZCL_ATTRID_HARMONIC_CURRENT_MULTIPLIER    		    		0x0404
#define ZCL_ATTRID_PHASE_HARMONIC_CURRENT_MULTIPLIER	    		0x0405
//AC (Single Phase or Phase A) Measurements
#define ZCL_ATTRID_LINE_CURRENT										0x0501
#define ZCL_ATTRID_ACTIVE_CURRENT									0x0502
#define ZCL_ATTRID_REACTIVE_CURRENT									0x0503
#define ZCL_ATTRID_RMS_VOLTAGE										0x0505
#define ZCL_ATTRID_RMS_VOLTAGE_MIN									0x0506
#define ZCL_ATTRID_RMS_VOLTAGE_MAX									0x0507
#define ZCL_ATTRID_RMS_CURRENT										0x0508
#define ZCL_ATTRID_RMS_CURRENT_MIN									0x0509
#define ZCL_ATTRID_RMS_CURRENT_MAX									0x050A
#define ZCL_ATTRID_ACTIVE_POWER										0x050B
#define ZCL_ATTRID_ACTIVE_POWER_MIN									0x050C
#define ZCL_ATTRID_ACTIVE_POWER_MAX									0x050D
#define ZCL_ATTRID_REACTIVE_POWER									0x050E
#define ZCL_ATTRID_APPARENT_POWER									0x050F
#define ZCL_ATTRID_POWER_FACTOR										0x0510
#define ZCL_ATTRID_AVERAGE_RMS_VOLTAGE_MEASUREMENT_PERIOD			0x0511
#define ZCL_ATTRID_AVERAGE_RMS_OVER_VOLTAGE_COUNTER					0x0512
#define ZCL_ATTRID_AVERAGE_RMS_UNDER_VOLTAGE_COUNTER				0x0513
#define ZCL_ATTRID_RMS_EXTREME_OVER_VOLTAGE_PERIOD					0x0514
#define ZCL_ATTRID_RMS_EXTREME_UNDER_VOLTAGE_PERIOD					0x0515
#define ZCL_ATTRID_RMS_VOLTAGE_SAG_PERIOD							0x0516
#define ZCL_ATTRID_RMS_VOLTAGE_SWELL_PERIOD							0x0517
//AC Formatting
#define ZCL_ATTRID_AC_VOLTAGE_MULTIPLIER							0x0600
#define ZCL_ATTRID_AC_VOLTAGE_DIVISOR								0x0601
#define ZCL_ATTRID_AC_CURRENT_MULTIPLIER							0x0602
#define ZCL_ATTRID_AC_CURRENT_DIVISOR								0x0603
#define ZCL_ATTRID_AC_POWER_MULTIPLIER								0x0604
#define ZCL_ATTRID_AC_POWER_DIVISOR									0x0605
//DC Manufacturer Threshold Alarms
#define ZCL_ATTRID_DC_OVERLOAD_ALARMS_MASK							0x0700
#define ZCL_ATTRID_DC_VOLTAGE_OVERLOAD								0x0701
#define ZCL_ATTRID_DC_CURRENT_OVERLOAD								0x0702
//AC Manufacturer Threshold Alarms
#define ZCL_ATTRID_AC_ALARMS_MASK									0x0800
#define ZCL_ATTRID_AC_VOLTAGE_OVERLOAD								0x0801
#define ZCL_ATTRID_AC_CURRENT_OVERLOAD								0x0802
#define ZCL_ATTRID_AC_ACTIVE_POWER_OVERLOAD							0x0803
#define ZCL_ATTRID_AC_REACTIVE_POWER_OVERLOAD						0x0804
#define ZCL_ATTRID_AVERAGE_RMS_OVER_VOLTAGE							0x0805
#define ZCL_ATTRID_AVERAGE_RMS_UNDER_VOLTAGE						0x0806
#define ZCL_ATTRID_RMS_EXTREME_OVER_VOLTAGE							0x0807
#define ZCL_ATTRID_RMS_EXTREME_UNDER_VOLTAGE						0x0808
#define ZCL_ATTRID_RMS_VOLTAGE_SAG									0x0809
#define ZCL_ATTRID_RMS_VOLTAGE_SWELL								0x080A
//AC Phase B Measurements
#define ZCL_ATTRID_LINE_CURRENT_PHB									0x0901
#define ZCL_ATTRID_ACTIVE_CURRENT_PHB								0x0902
#define ZCL_ATTRID_REACTIVE_CURRENT_PHB								0x0903
#define ZCL_ATTRID_RMS_VOLTAGE_PHB									0x0905
#define ZCL_ATTRID_RMS_VOLTAGE_MIN_PHB								0x0906
#define ZCL_ATTRID_RMS_VOLTAGE_MAX_PHB								0x0907
#define ZCL_ATTRID_RMS_CURRENT_PHB									0x0908
#define ZCL_ATTRID_RMS_CURRENT_MIN_PHB								0x0909
#define ZCL_ATTRID_RMS_CURRENT_MAX_PHB								0x090A
#define ZCL_ATTRID_ACTIVE_POWER_PHB									0x090B
#define ZCL_ATTRID_ACTIVE_POWER_MIN_PHB								0x090C
#define ZCL_ATTRID_ACTIVE_POWER_MAX_PHB								0x090D
#define ZCL_ATTRID_REACTIVE_POWER_PHB								0x090E
#define ZCL_ATTRID_APPARENT_POWER_PHB								0x090F
#define ZCL_ATTRID_POWER_FACTOR_PHB									0x0910
#define ZCL_ATTRID_AVERAGE_RMS_VOLTAGE_MEASUREMENT_PERIOD_PHB		0x0911
#define ZCL_ATTRID_AVERAGE_RMS_OVER_VOLTAGE_CURRENT_PHB				0x0912
#define ZCL_ATTRID_AVERAGE_RMS_UNDER_VOLTAGE_CURRENT_PHB			0x0913
#define ZCL_ATTRID_RMS_EXTREME_OVER_VOLTAHE_PERIOD_PHB				0x0914
#define ZCL_ATTRID_RMS_EXTREME_UNDER_VOLTAHE_PERIOD_PHB				0x0915
#define ZCL_ATTRID_RMS_VOLTAHE_SAG_PERIOD_PHB						0x0916
#define ZCL_ATTRID_RMS_VOLTAHE_SWELL_PERIOD_PHB						0x0917
//AC Phase C Measurements
#define ZCL_ATTRID_LINE_CURRENT_PHC									0x0A01
#define ZCL_ATTRID_ACTIVE_CURRENT_PHC								0x0A02
#define ZCL_ATTRID_REACTIVE_CURRENT_PHC								0x0A03
#define ZCL_ATTRID_RMS_VOLTAGE_PHC									0x0A05
#define ZCL_ATTRID_RMS_VOLTAGE_MIN_PHC								0x0A06
#define ZCL_ATTRID_RMS_VOLTAGE_MAX_PHC								0x0A07
#define ZCL_ATTRID_RMS_CURRENT_PHC									0x0A08
#define ZCL_ATTRID_RMS_CURRENT_MIN_PHC								0x0A09
#define ZCL_ATTRID_RMS_CURRENT_MAX_PHC								0x0A0A
#define ZCL_ATTRID_ACTIVE_POWER_PHC									0x0A0B
#define ZCL_ATTRID_ACTIVE_POWER_MIN_PHC								0x0A0C
#define ZCL_ATTRID_ACTIVE_POWER_MAX_PHC								0x0A0D
#define ZCL_ATTRID_REACTIVE_POWER_PHC								0x0A0E
#define ZCL_ATTRID_APPARENT_POWER_PHC								0x0A0F
#define ZCL_ATTRID_POWER_FACTOR_PHC									0x0A10
#define ZCL_ATTRID_AVERAGE_RMS_VOLTAGE_MEASUREMENT_PERIOD_PHC		0x0A11
#define ZCL_ATTRID_AVERAGE_RMS_OVER_VOLTAGE_CURRENT_PHC				0x0A12
#define ZCL_ATTRID_AVERAGE_RMS_UNDER_VOLTAGE_CURRENT_PHC			0x0A13
#define ZCL_ATTRID_RMS_EXTREME_OVER_VOLTAHE_PERIOD_PHC				0x0A14
#define ZCL_ATTRID_RMS_EXTREME_UNDER_VOLTAHE_PERIOD_PHC				0x0A15
#define ZCL_ATTRID_RMS_VOLTAHE_SAG_PERIOD_PHC						0x0A16
#define ZCL_ATTRID_RMS_VOLTAHE_SWELL_PERIOD_PHC						0x0A17

/**
 *  @brief	electrical measurement command IDs
 */
//Server commands
#define ZCL_CMD_GET_PROFILE_INFO_RESPONSE                           0x00
#define ZCL_CMD_GET_MEASUREMENT_PROFILE_RESPONSE					0x01
//client commands
#define ZCL_CMD_GET_PROFILE_INFO									0x00
#define ZCL_CMD_GET_MEASUREMENT_PROFILE								0x01



/**
 *  @brief	Format of the Get Profile Info command
 */
typedef struct
{
	u16	attrID;
	UTCTime	startTime;
	u8	numberOfIntervals;
}zcl_electricalMeasure_getMeasurementProfileCmd_t;

/**
 *  @brief	Format of the Get Profile Info Response command
 */
typedef struct
{
	u8	profileCnt;
	u8	profileIntervalPeriod;
	u8	maxNumberOfIntervals;
	u8	*listOfAttributes;
}zcl_electricalMeasure_getProfileInfoRspCmd_t;

/**
 *  @brief	Format of the Get Measurement Profile Response command
 */
typedef struct
{
	UTCTime	startTime;
	u8	status;
	u8	profileIntervalPeriod;
	u8	numberOfIntervals;
	u8	attributeId;
	u8	*intervals;
}zcl_electricalMeasure_getMeasurementProfileRspCmd_t;


#if 0
typedef status_t (*zcl_electricalMeasure_getProfileInfoCb_t) (apsdeDataInd_t *pInd);
typedef status_t (*zcl_electricalMeasure_getMeasurementProfileCb_t) (apsdeDataInd_t *pInd, zcl_electricalMeasure_getMeasurementProfileCmd_t *pCmd);
typedef status_t (*zcl_electricalMeasure_getProfileInfoRspCb_t) (apsdeDataInd_t *pInd, zcl_electricalMeasure_getProfileInfoRspCmd_t *pCmd);
typedef status_t (*zcl_electricalMeasure_getMeasurementProfileRspCb_t) (apsdeDataInd_t *pInd, zcl_electricalMeasure_getMeasurementProfileRspCmd_t *pCmd);

/**
 *  @brief  Structure definition for commands callBack functions in electrical measurement cluster
 */
typedef struct
{
	zcl_electricalMeasure_getProfileInfoCb_t                     	getProfileInfoCbFunc;
	zcl_electricalMeasure_getMeasurementProfileCb_t					getMeasurementProfileCbFunc;
	zcl_electricalMeasure_getProfileInfoRspCb_t                     getProfileInfoRspCbFunc;
	zcl_electricalMeasure_getMeasurementProfileRspCb_t				getMeasurementProfileRspCbFunc;
}zcl_electricalMeasure_AppCallbacks_t;


extern const zclAttrInfo_t electricalMeasure_attrTbl[];
extern const u8 zcl_electricalMeasure_attrNum;
#endif


status_t zcl_electricalMeasure_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

status_t zcl_electricalMeasure_getProfileInfo(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_electricalMeasure_getProfileInfoCmd(a,b,c)	(zcl_electricalMeasure_getProfileInfo((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_electricalMeasure_getMeasurementProfile(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_electricalMeasure_getMeasurementProfileCmd_t *pReq);
#define zcl_electricalMeasure_getMeasurementProfileCmd(a,b,c,d)	(zcl_electricalMeasure_getMeasurementProfile((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_electricalMeasure_getProfileInfoRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_electricalMeasure_getProfileInfoRspCmd_t *pRsp);
#define zcl_electricalMeasure_getProfileInfoRspCmd(a,b,c,d)	(zcl_electricalMeasure_getProfileInfoRsp((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_electricalMeasure_getMeasurementProfileRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_electricalMeasure_getMeasurementProfileRspCmd_t *pRsp);
#define zcl_electricalMeasure_getMeasurementProfileRspCmd(a,b,c,d)	(zcl_electricalMeasure_getMeasurementProfileRsp((a), (b), (c), ZCL_SEQ_NUM, (d)))

