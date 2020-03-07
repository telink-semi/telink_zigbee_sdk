
/**************************************************************************
 *                                                                          *
 * INSERT COPYRIGHT HERE!                                                   *
 *                                                                          *
 ****************************************************************************
PURPOSE: MAC layer main module
*/
#include "../include/zb_common.h"

//Telink ieee address range
const u8 startIEEEAddr[] = {0x38, 0xc1, 0xa4};

const mac_pibTbl_t g_zbMacPibTbl[] = {
	{OFFSETOF(tl_zb_mac_pib_t, ackWaitDuration), 	sizeof(u8), 54, 54},                         /* MAC_ACK_WAIT_DURATION */
	{OFFSETOF(tl_zb_mac_pib_t, associationPermit), 	sizeof(bool), FALSE, TRUE},                /* MAC_ASSOCIATION_PERMIT */
	{OFFSETOF(tl_zb_mac_pib_t, autoReq), 			sizeof(bool), FALSE, TRUE},                      /* MAC_AUTO_REQUEST */
	{OFFSETOF(tl_zb_mac_pib_t, battLifeExt), 		sizeof(bool), FALSE, TRUE},                      /* MAC_BATT_LIFE_EXT */
	{OFFSETOF(tl_zb_mac_pib_t, battLifeExtPeriods), sizeof(u8), 6, 6},                        /* MAC_BATT_LIFE_EXT_PERIODS */
	{OFFSETOF(tl_zb_mac_pib_t, beaconPayload), 		sizeof(zb_mac_beacon_payload_t), 0, 0},                          /* MAC_BEACON_PAYLOAD */
	{OFFSETOF(tl_zb_mac_pib_t, beaconPayloadLen), 	sizeof(u8), 0, 52},                      /* MAC_BEACON_PAYLOAD_LENGTH */
	{OFFSETOF(tl_zb_mac_pib_t, beaconOrder), 		sizeof(u8), 0, 15},                              /* MAC_BEACON_ORDER */
	{OFFSETOF(tl_zb_mac_pib_t, beaconTxTime), 		sizeof(u32), 1, 1},                             /* MAC_BEACON_TX_TIME */
	{OFFSETOF(tl_zb_mac_pib_t, beaconSeqNum), 		sizeof(u8), 0x00, 0xFF},                              /* MAC_BSN */
	{OFFSETOF(tl_zb_mac_pib_t, coordExtAddress), 	sizeof(addrExt_t), 0, 0},  /* MAC_COORD_EXTENDED_ADDRESS */
	{OFFSETOF(tl_zb_mac_pib_t, coordShortAddress), 	sizeof(u16), 0, 0},                        /* MAC_COORD_SHORT_ADDRESS */
	{OFFSETOF(tl_zb_mac_pib_t, seqNum), 			sizeof(u8), 0x00, 0xFF},                                 /* MAC_DSN */
	{OFFSETOF(tl_zb_mac_pib_t, gtsPermit), 			sizeof(bool), FALSE, TRUE},                        /* MAC_GTS_PERMIT */
	{OFFSETOF(tl_zb_mac_pib_t, maxCsmaBackoffs), 	sizeof(u8), 0, 254},                         /* MAC_MAX_CSMA_BACKOFFS */
	{OFFSETOF(tl_zb_mac_pib_t, minBe), 				sizeof(u8), 0, 8},                                     /* MAC_MIN_BE */
	{OFFSETOF(tl_zb_mac_pib_t, panId), 				sizeof(u16), 0, 0},                                    /* MAC_PAN_ID */
	{OFFSETOF(tl_zb_mac_pib_t, promiscuousMode), 	sizeof(bool), FALSE, TRUE},                  /* MAC_PROMISCUOUS_MODE */
	{OFFSETOF(tl_zb_mac_pib_t, rxOnWhenIdle), 		sizeof(bool), FALSE, TRUE},                     /* MAC_RX_ON_WHEN_IDLE */
	{OFFSETOF(tl_zb_mac_pib_t, shortAddress), 		sizeof(u16), 0, 0},                             /* MAC_SHORT_ADDRESS */
	{OFFSETOF(tl_zb_mac_pib_t, superframeOrder), 	sizeof(u8), 0, 15},                          /* MAC_SUPERFRAME_ORDER */
	{OFFSETOF(tl_zb_mac_pib_t, transactionPersistenceTime), sizeof(u16), 0, 0},               /* MAC_TRANSACTION_PERSISTENCE_TIME */
	{OFFSETOF(tl_zb_mac_pib_t, associatedPanCoord), sizeof(bool), FALSE, TRUE},               /* MAC_ASSOCIATED_PAN_COORD */
	{OFFSETOF(tl_zb_mac_pib_t, maxBe), 				sizeof(u8), 0, 8},                                     /* MAC_MAX_BE */
	{OFFSETOF(tl_zb_mac_pib_t, frameTotalWaitTime), sizeof(u16), 0x00, 0xFF},              /* MAC_MAX_FRAME_RESPONSE_TIME */
	{OFFSETOF(tl_zb_mac_pib_t, frameRetryNum), 		sizeof(u8), 0, 7},                           /* MAC_MAX_FRAME_RETRIES */
	{OFFSETOF(tl_zb_mac_pib_t, respWaitTime), 		sizeof(u8), 2, 64},                         /* MAC_RESPONSE_WAIT_TIME */
	{OFFSETOF(tl_zb_mac_pib_t, syncSymbolOffset), 	sizeof(u8), 0, 0},                          /* MAC_SYNC_SYMBOL_OFFSET */
	{OFFSETOF(tl_zb_mac_pib_t, timeStampSupported), sizeof(bool), FALSE, TRUE},               /* MAC_TIMESTAMP_SUPPORTED */
	{OFFSETOF(tl_zb_mac_pib_t, securityEnabled), 	sizeof(bool), FALSE, TRUE},                  /* MAC_SECURITY_ENABLED */
	{OFFSETOF(tl_zb_mac_pib_t, extAddress), 		sizeof(addrExt_t), FALSE, TRUE},                  /* MAC_SECURITY_ENABLED */
	{OFFSETOF(tl_zb_mac_pib_t, txPower), 			sizeof(u8), 0, 11},                  /* MAC_SECURITY_ENABLED */
};


/*
 * MLME-SET.request to set MAC pib attribute
 *
 * @param attribute MAC PIB attribut MAC PIB attributee id(Table 86 ¡ª MAC PIB attributes)
 *
 * @param value the pointer value of the attribute
 *
 * @param len the length of the attribute vlaue
 *
 * @return MAC_SUCCESS if it's successful, or MAC_INVALID_PARAMETER
 *
 * */
_CODE_MAC_ u8 tl_zbMacAttrSet(u8 attribute, u8 *value, u8 len){
	if(attribute == MAC_TX_POWER){
		if(*value > 11){
			*value = 11;
		}
		ZB_RADIO_TX_POWER_SET(*value);
		return MAC_SUCCESS;
	}else if(attribute == MAC_PHY_ATTR_CURRENT_CHANNEL){
		g_zbMacPib.phyChannelCur = *value;
		return MAC_SUCCESS;
	}


	if(attribute > MAC_PIB_ATTRIBUTE_STOP || attribute < MAC_PIB_ATTRIBUTE_START){
		return MAC_STA_INVALID_PARAMETER;
	}

	if(len > g_zbMacPibTbl[attribute-MAC_PIB_ATTRIBUTE_START].len){
		return MAC_STA_INVALID_PARAMETER;
	}
	memcpy((u8 *)&g_zbMacPib+g_zbMacPibTbl[attribute-MAC_PIB_ATTRIBUTE_START].offset, value, len);

	return MAC_SUCCESS;
}


/*
 * MLME-GET.request to get MAC pib attribute
 *
 * @param attribute MAC PIB attribut MAC PIB attributee id(Table 86 ¡ª MAC PIB attributes)
 *
 * @param value the pointer to the value of the attribute
 *
 * @param len the length of the attribute vlaue
 *
 * @return MAC_SUCCESS if it's successful, or MAC_INVALID_PARAMETER
 *
 * */
_CODE_MAC_ u8 tl_zbMacAttrGet(u8 attribute, u8* value, u8* len){
	if(attribute == MAC_PHY_ATTR_CURRENT_CHANNEL){
		*value = g_zbMacPib.phyChannelCur;
		return MAC_SUCCESS;
	}

	if(attribute > MAC_PIB_ATTRIBUTE_STOP || attribute < MAC_PIB_ATTRIBUTE_START){
		return MAC_STA_INVALID_PARAMETER;
	}
	*len = g_zbMacPibTbl[attribute-MAC_PIB_ATTRIBUTE_START].len;
	memcpy(value, (u8 *)&g_zbMacPib+g_zbMacPibTbl[attribute-MAC_PIB_ATTRIBUTE_START].offset, *len);

	return MAC_SUCCESS;
}

_CODE_MAC_ void generateIEEEAddr(void){
	u8 addr[8];

	flash_read(CFG_MAC_ADDRESS, 8, addr);

	if(ZB_IEEE_ADDR_IS_INVAILD(addr)){
		unsigned int t0 = clock_time();
		u32 jitter = 0;
		do{
			jitter = rand();
			jitter &= 0xfff;
		}while(jitter == 0);
		while(!clock_time_exceed(t0, jitter));

		generateRandomData(addr, 5);
		memcpy(addr+5, startIEEEAddr, 3);
		flash_write(CFG_MAC_ADDRESS, 6, addr + 2);
		flash_write(CFG_MAC_ADDRESS + 6, 2, addr);
	}else{
		/* MAC address format in TLSR serial chips:
		 * xx xx xx 38 C1 A4 xx xx
  	  	 * xx xx xx D1 19 C4 xx xx
  	  	 * xx xx xx CB 0B D8 xx xx
		 *
		 * so, it need to do shift
		 * */
		if((addr[3] == 0x38 && addr[4] == 0xC1 && addr[5] == 0xA4) ||
		   (addr[3] == 0xD1 && addr[4] == 0x19 && addr[5] == 0xC4) ||
		   (addr[3] == 0xCB && addr[4] == 0x0B && addr[5] == 0xD8)){
			flash_read(CFG_MAC_ADDRESS, 6, addr + 2);
			flash_read(CFG_MAC_ADDRESS + 6, 2, addr);
		}
	}
	ZB_IEEE_ADDR_COPY(ZB_PIB_EXTENDED_ADDRESS(), addr);
}

/*! @} */
