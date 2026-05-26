/********************************************************************************************************
 * @file     gap_event.h
 *
 * @brief    This is the header file for b91 BLE SDK
 *
 * @author	 BLE GROUP
 * @date         12,2021
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
 *******************************************************************************************************/

#ifndef GAP_EVENT_H_
#define GAP_EVENT_H_




/*****************************************  SMP message sequence and event chart  *******************************************************

																							  GAP_SMP EVENT
Situation 1:   SMP Standard Pair																    |
	Phase 1: Paring Feature Exchange																|
	Phase 2: Encryption( STK/LTK Generation )														|
	Phase 3: Key Distribution																		|
																									|
  	     Master														  Slave							|
			|															|							|
			|														    |							|
    ________|___________________________________________________________|_______					|
	| 																			|					|
	|						   Establish LL connection							|					|
	|___________________________________________________________________________|					|
			|															|							|
			|															|							|
	        |              Phase 1: Paring Feature Exchange				|							|
	 _______|___________________________________________________________|_______					|
	|		|													        |	    |					|
	|		|			        (Optional)Security_Request		    	|	    |					|
	|		|<----------------------------------------------------------|	    |					|
	|		|						Paring_Req							|	    |
	| 		|---------------------------------------------------------->|=======|=====>>>  GAP_EVT_SMP_PAIRING_BEGIN
	|		|						Paring_Rsp					    	|	    |
	|  		|<----------------------------------------------------------|       |					|
	|		|						....... 							|	    |					|
	|_______|___________________________________________________________|_______|					|
			|															|							|
			|															|							|
	        |                  Phase 2: Encryption					    |							|
	 _______|___________________________________________________________|_______					|
	|		|															|	    |					|
	|		|					    LL_ENC_REQ				            |	    |					|
	|		|---------------------------------------------------------->|	    |					|
	|		|					    LL_ENC_RSP						    |	    |					|
	| 		|<----------------------------------------------------------|	    |					|
	|		|				     LL_START_ENC_REQ						|	    |					|
	|  		|<----------------------------------------------------------|       |					|
	|		|					 LL_START_ENC_RSP						|	    |					|
	|		|---------------------------------------------------------->|	    |					|
	|       |				     LL_START_ENC_RSP	 					|	    |
	|		|<----------------------------------------------------------|=======|=====>>>  GAP_EVT_SMP_CONN_ENCRYPTION_DONE(re_connect = SMP_STANDARD_PAIR)
	|_______|___________________________________________________________|_______|
			|															|							|
			|															|							|
	        |              Phase 3: Key Distribution				    |							|
	 _______|___________________________________________________________|_______					|
	|		|															|	    |					|
	|		|					  Key Distribution	 			        |	    |					|
	|		|<----------------------------------------------------------|	    |					|
	|		|					  Key Distribution						|	    |					|
	| 		|<----------------------------------------------------------|	    |					|
	|		|				           ....... 					        |	    |					|
	|  		|															|       |					|
	|		|					  Key Distribution				        |	    |					|
	|		|---------------------------------------------------------->|	    |					|
	|		|					  Key Distribution						|	    |					|
	| 		|---------------------------------------------------------->|	    |					|
	|		|				           ....... 					        |	    |					|
	|  		|															|       |
	|		|				 All Key Distribution Finish				|=======|=====>>>  GAP_EVT_SMP_PAIRING_SUCCESS
	|		|				 											|=======|=====>>>  GAP_EVT_SMP_SECURITY_PROCESS_DONE(re_connect = SMP_STANDARD_PAIR)
	|_______|___________________________________________________________|_______|
																									|
																									|
																									|
																									|
Situation 2:   SMP Fast Connect																		|
	Only 1 Phase: Encryption																	    |
																									|
  	     Master														  Slave							|
			|															|							|
			|														    |							|
    ________|___________________________________________________________|_______					|
	| 																			|					|
	|						  Establish LL connection							|					|
	|___________________________________________________________________________|					|
			|															|							|
			|															|							|
	        |                  Phase 2: Encryption					    |							|
	 _______|___________________________________________________________|_______					|
	|		|															|	    |					|
	|		|					    LL_ENC_REQ				            |	    |					|
	|		|---------------------------------------------------------->|	    |					|
	|		|					    LL_ENC_RSP						    |	    |					|
	| 		|<----------------------------------------------------------|	    |					|
	|		|				     LL_START_ENC_REQ						|	    |					|
	|  		|<----------------------------------------------------------|       |					|
	|		|					 LL_START_ENC_RSP						|	    |					|
	|		|---------------------------------------------------------->|	    |					|
	|       |				     LL_START_ENC_RSP	 					|	    |					|
	|		|<----------------------------------------------------------|=======|=====>>>  GAP_EVT_SMP_CONN_ENCRYPTION_DONE (re_connect = SMP_FAST_CONNECT)
	|		|				 											|=======|=====>>>  GAP_EVT_SMP_SECURITY_PROCESS_DONE(re_connect = SMP_FAST_CONNECT)
	|_______|___________________________________________________________|_______|


 *****************************************************************************************************************************/


/**
 * @brief      GAP event type
 */
#define GAP_EVT_SMP_PAIRING_BEGIN		                             0	// Refer to SMP message sequence and event chart above
#define GAP_EVT_SMP_PAIRING_SUCCESS			                         1	// Refer to SMP message sequence and event chart above
#define GAP_EVT_SMP_PAIRING_FAIL			                         2
#define GAP_EVT_SMP_CONN_ENCRYPTION_DONE							 3	// Refer to SMP message sequence and event chart above
#define GAP_EVT_SMP_SECURITY_PROCESS_DONE							 4	// Refer to SMP message sequence and event chart above

#define GAP_EVT_SMP_TK_DISPLAY			                             8
#define GAP_EVT_SMP_TK_REQUEST_PASSKEY								 9
#define GAP_EVT_SMP_TK_REQUEST_OOB									 10
#define GAP_EVT_SMP_TK_NUMERIC_COMPARE								 11


#define GAP_EVT_ATT_EXCHANGE_MTU									 16
#define GAP_EVT_GATT_HANDLE_VALUE_CONFIRM							 17

#if(MCU_CORE_TYPE == MCU_CORE_825x || MCU_CORE_TYPE == MCU_CORE_827x)
	///Correct spelling errors, but need to be compatible with the previous version
	#define GAP_EVT_SMP_PARING_BEAGIN		                             GAP_EVT_SMP_PAIRING_BEGIN	// Refer to SMP message sequence and event chart above
	#define GAP_EVT_SMP_PARING_SUCCESS			                         GAP_EVT_SMP_PAIRING_SUCCESS	// Refer to SMP message sequence and event chart above
	#define GAP_EVT_SMP_PARING_FAIL			                             GAP_EVT_SMP_PAIRING_FAIL
#endif

/**
 * @brief      GAP event mask
 */
#define GAP_EVT_MASK_NONE                                        	 0x00000000
#define GAP_EVT_MASK_SMP_PAIRING_BEGIN                          	 (1<<GAP_EVT_SMP_PAIRING_BEGIN)
#define GAP_EVT_MASK_SMP_PAIRING_SUCCESS                           	 (1<<GAP_EVT_SMP_PAIRING_SUCCESS)
#define GAP_EVT_MASK_SMP_PAIRING_FAIL                           	 (1<<GAP_EVT_SMP_PAIRING_FAIL)

#define GAP_EVT_MASK_SMP_CONN_ENCRYPTION_DONE                     	 (1<<GAP_EVT_SMP_CONN_ENCRYPTION_DONE)
#define GAP_EVT_MASK_SMP_SECURITY_PROCESS_DONE                     	 (1<<GAP_EVT_SMP_SECURITY_PROCESS_DONE)

#define GAP_EVT_MASK_SMP_TK_DISPLAY                  				 (1<<GAP_EVT_SMP_TK_DISPLAY)
#define GAP_EVT_MASK_SMP_TK_REQUEST_PASSKEY                  		 (1<<GAP_EVT_SMP_TK_REQUEST_PASSKEY)
#define GAP_EVT_MASK_SMP_TK_REQUEST_OOB	                     		 (1<<GAP_EVT_SMP_TK_REQUEST_OOB)
#define GAP_EVT_MASK_SMP_TK_NUMERIC_COMPARE                     	 (1<<GAP_EVT_SMP_TK_NUMERIC_COMPARE)

#define GAP_EVT_MASK_ATT_EXCHANGE_MTU                     	 		 (1<<GAP_EVT_ATT_EXCHANGE_MTU)
#define GAP_EVT_MASK_GATT_HANDLE_VALUE_CONFIRM                     	 (1<<GAP_EVT_GATT_HANDLE_VALUE_CONFIRM)


#define GAP_EVT_MASK_DEFAULT										( GAP_EVT_MASK_SMP_TK_DISPLAY 			| \
																	  GAP_EVT_MASK_SMP_TK_REQUEST_PASSKEY   | \
																	  GAP_EVT_MASK_SMP_TK_REQUEST_OOB		| \
																	  GAP_EVT_MASK_ATT_EXCHANGE_MTU )


#if(MCU_CORE_TYPE == MCU_CORE_825x || MCU_CORE_TYPE == MCU_CORE_827x)
	///Correct spelling errors, but need to be compatible with the previous version
	#define GAP_EVT_MASK_SMP_PARING_BEAGIN                               GAP_EVT_MASK_SMP_PAIRING_BEGIN
	#define GAP_EVT_MASK_SMP_PARING_SUCCESS                              GAP_EVT_MASK_SMP_PAIRING_SUCCESS
	#define GAP_EVT_MASK_SMP_PARING_FAIL                                 GAP_EVT_MASK_SMP_PAIRING_FAIL
#endif

/**
 * @brief      data structure of GAP event callback data
 */
typedef struct {
	u16	connHandle;
	u8	secure_conn;
	u8	tk_method;
} gap_smp_paringBeginEvt_t;


typedef struct {
	u16	connHandle;
	u8	bonding;
	u8	bonding_result;
} gap_smp_paringSuccessEvt_t;

typedef struct {
	u16	connHandle;
	u8  reason;
} gap_smp_paringFailEvt_t;

typedef struct {
	u16	connHandle;
	u8	re_connect;   //1: re_connect, encrypt with previous distributed LTK;   0: paring , encrypt with STK
} gap_smp_connEncDoneEvt_t;

typedef struct {
	u16	connHandle;
	u8	re_connect;   //1: re_connect, encrypt with previous distributed LTK;   0: paring , encrypt with STK
} gap_smp_securityProcessDoneEvt_t;



typedef struct {
	u16	connHandle;
	u32	tk_pincode;
} gap_smp_TkDisplayEvt_t;

typedef struct {
	u16	connHandle;
} gap_smp_TkReqPassKeyEvt_t;

typedef struct {
	u16	connHandle;
	u16	peer_MTU;
	u16	effective_MTU;
} gap_gatt_mtuSizeExchangeEvt_t;






/**
 * @brief     GAP event callback function declaration
 */
typedef int (*gap_event_handler_t) (u32 h, u8 *para, int n);






/**
 * @brief      set event mask for specific gap event
 * @param[in]  evtMask - event mask
 * @return     none
 */
void 	blc_gap_setEventMask(u32 evtMask);


/**
 * @brief      register public enter for all gap event callback
 * @param[in]  handler - public enter function
 * @return     none
 */
void 	blc_gap_registerHostEventHandler (gap_event_handler_t  handler);





#endif /* GAP_EVENT_H_ */
