/********************************************************************************************************
 * @file     trace.h
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

#ifndef TRACE_H_
#define TRACE_H_


//	event: 0 for time stamp; 1 reserved; eid2 - eid31
#define			SLEV_timestamp				0
#define			SLEV_reserved				1

#define 		SLEV_irq_rf					2
#define 		SLEV_irq_sysTimer			3

#define 		SLEV_irq_rx					5
#define 		SLEV_irq_rxCrc				6
#define 		SLEV_irq_rxTimStamp			7
#define 		SLEV_irq_rxNew				8

#define			SLEV_irq_tx					10

#define 		SLEV_irq_cmddone			12
#define 		SLEV_irq_rxTmt				13
#define 		SLEV_irq_rxFirstTmt			14
#define 		SLEV_irq_fsmTmt				15

#define 		SLEV_slave_1stRx			16

#define			SLEV_txFifo_push			17
#define			SLEV_txFifo_empty			18




#define 		SLEV_test_event				31








// 1-bit data: 0/1/2 for hardware signal
#define			SL01_irq					1
#define			SL01_sysTimer				2
#define			SL01_adv     				3
#define			SL01_brx     				4
#define			SL01_btx     				5

// 8-bit data: cid0 - cid63
#define			SL08_test_1B				0



// 16-bit data: sid0 - sid63
#define			SL16_tf_hw_push				1
#define			SL16_tf_sw_push				2
#define			SL16_tf_hw_load1			3
#define			SL16_tf_sw_load1			4
#define			SL16_tf_hw_load2			5
#define			SL16_tf_sw_load2			6
#define			SL16_tf_hw_RX				7
#define			SL16_tf_sw_RX				8
#define			SL16_tf_hw_TX				9
#define			SL16_tf_sw_TX				10

#define			SL16_seq_notify				15
#define			SL16_seq_write				16

//#define			SL01_test_task				0
//#define			SL08_test_1B				0
//#define			SL16_test_2B				0


#endif
