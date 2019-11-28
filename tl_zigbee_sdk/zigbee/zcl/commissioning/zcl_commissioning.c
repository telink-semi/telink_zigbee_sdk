/********************************************************************************************************
 * @file     zcl_commissioning.c
 *
 * @brief	 APIs for commissioning cluster
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


/**********************************************************************
 * INCLUDES
 */
#include "../zcl_include.h"

#if 0
#include "../../aps/aps_stackUse.h"
#define ZB_RANDOM_X(x) (x) ? (zb_random() % x) : 0

/**********************************************************************
 * LOCAL TYPES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */

#ifdef ZCL_COMMISSIONING

zcl_commissioning_AppCallbacks_t *zcl_commissioning_cb;

#endif    /* ZCL_COMMISSIONING */


/**********************************************************************
 * LOCAL FUNCTIONS
 */
u8 zcl_commissioning_cmdHandler(zclIncoming_t *pInMsg);


_CODE_ZCL_ static int zb_restart_device_func(void *arg)
{
	zcl_initialize_startup_attributes();
	/* TODO - */

	return -1;
}

/************************Client Command*************************************/
_CODE_ZCL_ void zcl_commissioning_restart_device(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo,
		u8 options, u8 delay, u8 jitter)
{
	u8 buf[3];
	buf[0] = options;
	buf[1] = delay;
	buf[2] = jitter;

	zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_COMMISSIONING, ZCL_CMD_RESTART_DEVICE, TRUE,
		ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 3, buf);
}

// Server side handler
_CODE_ZCL_ u8 zcl_commissioning_clientCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	u8 cmd = pInMsg->hdr.cmd;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
//	u8 endpoint = pApsdeInd->indInfo.dst_ep;

	epInfo_t dstEp;
	TL_SETSTRUCTCONTENT(dstEp, 0);

    dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
    dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
    dstEp.dstEp = pApsdeInd->indInfo.src_ep;
    dstEp.profileId = pApsdeInd->indInfo.profile_id;
	dstEp.radius = 0x0a;
	dstEp.txOptions = 0;

	switch (cmd) {
		case ZCL_CMD_RESTART_DEVICE:
		{
			zcl_restartDevice_t *restartDevice = (zcl_restartDevice_t *) pInMsg->pData;
			bool immediate = false;

			if(ZCL_RESTART_OPTIONS_GET_IMMEDIATE(restartDevice->options))
			{
				if(restartDevice->delay == 0)
				{
					/* Restart device immediately */
					immediate = true;
					zb_restart_device_func(&immediate);
				}
				else
				{
					/* Restart device immediately after delay(s) + jitter(ms) */
					u16 jitter_in_milliseconds = restartDevice->jitter ?
							ZB_RANDOM_X(restartDevice->jitter * 80) : 0;
					u32 max_delay = (restartDevice->delay +
							(jitter_in_milliseconds / 1000)) * 1000000;/* in microseconds */

					immediate = true;
					TL_ZB_TIMER_SCHEDULE(zb_restart_device_func,(void *)TRUE,max_delay);
				}
			}
			else
			{
				/* Restart device after (delay - jitter) + wait until pending frames have been transmitted */
				u16 jitter_in_milliseconds = restartDevice->jitter ?
						ZB_RANDOM_X(restartDevice->jitter * 80) : 0;
				u32 max_delay = 0;
				max_delay = (restartDevice->delay +
						(jitter_in_milliseconds / 1000)) * 1000000;/* in microseconds */

				immediate = false;
				if(max_delay)
				{
					TL_ZB_TIMER_SCHEDULE(zb_restart_device_func,(void *)TRUE,max_delay);
				}
				else
				{
					zb_restart_device_func(&immediate);
				}
			}
		}
		break;
		case ZCL_CMD_SAVE_STARTUP_PARAMETERS:
			//TODO - handle command
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
		break;
		case ZCL_CMD_RESTORE_STARTUP_PARAMETERS:
			//TODO - handle command
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
		break;
		case ZCL_CMD_RESET_STARTUP_PARAMETERS:
			//TODO - handle command
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
		break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
		break;
	}

	return status;
}


// Client side handler
_CODE_ZCL_ u8 zcl_commissioning_serverCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	u8 cmd = pInMsg->hdr.cmd;


	switch (cmd) {
		case ZCL_CMD_RESTART_DEVICE_RSP:
			//TODO - handle command
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
		break;
		case ZCL_CMD_SAVE_STARTUP_PARAMETERS_RSP:
			//TODO - handle command
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
		break;
		case ZCL_CMD_RESTORE_STARTUP_PARAMETERS_RSP:
			//TODO - handle command
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
		break;
		case ZCL_CMD_RESET_STARTUP_PARAMETERS_RSP:
			//TODO - handle command
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
		break;

		default:
			status = ZCL_STA_FAILURE;
		break;
	}

	return status;
}
#endif
