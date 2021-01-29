/********************************************************************************************************
 * @file	zcl_commissioning.c
 *
 * @brief	This is the source file for zcl_commissioning
 *
 * @author	Zigbee Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
/**********************************************************************
 * INCLUDES
 */
#include "../zcl_include.h"

#if 0
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
							(jitter_in_milliseconds / 1000)) * 1000;/* in microseconds */

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
						(jitter_in_milliseconds / 1000)) * 1000;/* in microseconds */

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
