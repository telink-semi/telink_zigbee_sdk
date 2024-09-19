/********************************************************************************************************
 * @file    module_test.c
 *
 * @brief   This is the source file for module_test
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
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

#include "zb_common.h"


#define MODULE_TEST_UART		0

#if MODULE_TEST_UART

#if defined(MCU_CORE_826x)
	#define UART_TX_PIN		GPIO_PC2
	#define UART_RX_PIN		GPIO_PC3
#elif defined(MCU_CORE_8258)
	#define UART_TX_PIN		GPIO_PD7
	#define UART_RX_PIN		GPIO_PA0
#elif defined(MCU_CORE_8278)
	#define UART_TX_PIN		GPIO_PD7
	#define UART_RX_PIN		GPIO_PA0
#elif defined(MCU_CORE_B91)
	#define UART_TX_PIN		GPIO_PB2
	#define UART_RX_PIN		GPIO_PB3
#elif defined(MCU_CORE_B92)
	#define UART_TX_PIN		GPIO_PB1
	#define UART_RX_PIN		GPIO_PB2
#elif defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
	#define UART_TX_PIN		GPIO_PC4
	#define UART_RX_PIN		GPIO_PC5
#else
	#error	"undefined TEST_MODULE"
#endif

#if defined(MCU_CORE_826x)
	#define UART_PIN_INIT()		do{	\
									drv_uart_pin_set(UART_TX_PIN, UART_RX_PIN);	\
									UART_GPIO_CFG_PC2_PC3();					\
								}while(0)
#else
	#define UART_PIN_INIT()		do{	\
									drv_uart_pin_set(UART_TX_PIN, UART_RX_PIN);	\
								}while(0)
#endif


typedef struct{
	u32 dataLen;
	u8 dataPayload[1];
}uart_rxData_t;

__attribute__((aligned(4))) u8 moduleTest_uartTxBuf[4] = {0};
__attribute__((aligned(4))) u8 moduleTest_uartRxBuf[32] = {0};
volatile u8  T_uartPktSentSeqNo = 0;
volatile u32 T_uartPktRecvSeqNo = 0;
volatile u32 T_uartPktRecvLen = 0;
volatile u32 T_uartPktSentExcept = 0;

void module_test_uartRcvHandler(void){
	/*
	 * the format of the uart rx data: length(4 Bytes) + payload
	 *
	 * */
	uart_rxData_t *rxData = (uart_rxData_t *)moduleTest_uartRxBuf;
	T_uartPktRecvLen = rxData->dataLen;
	T_uartPktRecvSeqNo = rxData->dataPayload[0];

	if(T_uartPktRecvSeqNo == 0){
		drv_uart_tx_start(rxData->dataPayload, T_uartPktRecvLen);
	}

	if(T_uartPktRecvSeqNo == 0xBB){
		drv_adc_enable(1);
	}else{
		drv_adc_enable(0);
	}
}

void moduleTest_forUart(void){
	UART_PIN_INIT();

	drv_uart_init(115200, moduleTest_uartRxBuf, sizeof(moduleTest_uartRxBuf)/sizeof(u8), module_test_uartRcvHandler);

	drv_adc_init();

#if defined(MCU_CORE_826x)
	drv_adc_mode_pin_set(DRV_ADC_VBAT_MODE, NOINPUT);
#elif defined(MCU_CORE_8258)
	drv_adc_mode_pin_set(DRV_ADC_VBAT_MODE, GPIO_PC5);
#elif defined(MCU_CORE_8278)
	//drv_adc_mode_pin_set(DRV_ADC_BASE_MODE, GPIO_PB3);
	drv_adc_mode_pin_set(DRV_ADC_VBAT_MODE, GPIO_PB3);
#elif defined(MCU_CORE_B91)
	drv_adc_mode_pin_set(DRV_ADC_BASE_MODE, ADC_GPIO_PB0);
#elif defined(MCU_CORE_B92)
	drv_adc_mode_pin_set(DRV_ADC_BASE_MODE, ADC_GPIO_PB5);
#elif defined(MCU_CORE_TL721X)
	drv_adc_mode_pin_set(DRV_ADC_BASE_MODE, ADC_GPIO_PB6);
#elif defined(MCU_CORE_TL321X)
	drv_adc_mode_pin_set(DRV_ADC_BASE_MODE, ADC_GPIO_PB4);
#endif

	drv_enable_irq();

	for(int i = 0; i < sizeof(moduleTest_uartTxBuf)/sizeof(u8); i++){
		moduleTest_uartTxBuf[i] = i;
	}

	while(1)
	{
		if(T_uartPktRecvSeqNo == 0xAA){
			moduleTest_uartTxBuf[0] = T_uartPktSentSeqNo++;
			if(drv_uart_tx_start(moduleTest_uartTxBuf, sizeof(moduleTest_uartTxBuf)/sizeof(u8)) == 1){
				WaitMs(1000);
			}else{
				T_uartPktSentExcept++;
				while(1);
			}
		}else if(T_uartPktRecvSeqNo == 0xBB){
			u16 vBat = drv_get_adc_data();

			moduleTest_uartTxBuf[0] = HI_UINT16(vBat);
			moduleTest_uartTxBuf[1] = LO_UINT16(vBat);

			if(drv_uart_tx_start(moduleTest_uartTxBuf, 2) == 1){
				WaitMs(1000);
			}else{
				T_uartPktSentExcept++;
				while(1);
			}
		}else if(T_uartPktRecvSeqNo == 0xCC){
			u16 random = zb_random();

			moduleTest_uartTxBuf[0] = HI_UINT16(random);
			moduleTest_uartTxBuf[1] = LO_UINT16(random);

			if(drv_uart_tx_start(moduleTest_uartTxBuf, 2) == 1){
				WaitMs(1000);
			}else{
				T_uartPktSentExcept++;
				while(1);
			}
		}
	}
}

#endif

#define MODULE_TEST_NV		0

#if MODULE_TEST_NV

#if defined(MCU_CORE_826x)
	#define TEST_GPIO		GPIO_PB6
#elif defined(MCU_CORE_8258)
	#define TEST_GPIO		GPIO_PA3
#elif defined(MCU_CORE_8278)
	#define TEST_GPIO		GPIO_PA3
#elif defined(MCU_CORE_B91)
	#define TEST_GPIO		GPIO_PB7
#elif defined(MCU_CORE_B92)
	#define TEST_GPIO		GPIO_PE7
#elif defined(MCU_CORE_TL721X)
	#define TEST_GPIO		GPIO_PC0
#elif defined(MCU_CORE_TL321X)
	#define TEST_GPIO		GPIO_PD2
#else
	#error	"undefined TEST_MODULE"
#endif

enum{
	TEST_ITEM0 = 0x30,
	TEST_ITEM1,
	TEST_ITEM2,
};

typedef struct{
	u32 cnt;
	u8 data[16];
}nv_test_buf_t;

nv_test_buf_t nv_test_buf =
{
	.cnt = 0,
	.data = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff},
};

u8 nv_test_buf_0[32] = {
	0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff,
	0x01,0x12,0x23,0x34,0x45,0x56,0x67,0x78,0x89,0x9a,0xab,0xbc,0xcd,0xde,0xef,0xf0,
};
u8 nv_test_buf_1[200];
u8 test_chk_buf[256];

static void nv_dataStoreSet(void){
	u16 flag = 0x5a5a;
	flash_write(CFG_FACTORY_RST_CNT + 4, 2, (u8 *)&flag);
}

static bool nv_dataStoreCheck(void){
	u16 flag = 0xffff;
	flash_read(CFG_FACTORY_RST_CNT + 4, 2, (u8 *)&flag);
	if(flag != 0xffff){
		return TRUE;
	}else{
		return FALSE;
	}
}

void moduleTest_NV(void){
	drv_gpio_func_set(TEST_GPIO);
	drv_gpio_output_en(TEST_GPIO, 1); 		//enable output
	drv_gpio_input_en(TEST_GPIO, 0);		//disable input
	drv_gpio_write(TEST_GPIO, 1);           //LED On

	nv_sts_t ret = nv_flashReadNew(1, NV_MODULE_APP, TEST_ITEM0, sizeof(nv_test_buf_0), test_chk_buf);
	if(ret != NV_SUCC){
		if(nv_dataStoreCheck()){
			while(1);
		}
		if(nv_flashWriteNew(1, NV_MODULE_APP, TEST_ITEM0, sizeof(nv_test_buf_0), nv_test_buf_0) != NV_SUCC){
			while(1);
		}
		nv_dataStoreSet();
	}else{
		if(memcmp(nv_test_buf_0, test_chk_buf, sizeof(nv_test_buf_0))){
			while(1);
		}
	}

	while(1){
		nv_test_buf.cnt++;
		if(nv_flashWriteNew(1, NV_MODULE_APP, TEST_ITEM1, sizeof(nv_test_buf_t), (u8 *)&nv_test_buf) != NV_SUCC){
			while(1);
		}

		if(nv_flashReadNew(1, NV_MODULE_APP, TEST_ITEM1, sizeof(nv_test_buf_t), test_chk_buf) != NV_SUCC){
			while(1);
		}

		if(memcmp((u8 *)&nv_test_buf, test_chk_buf, sizeof(nv_test_buf_t))){
			while(1);
		}

		drv_generateRandomData(nv_test_buf_1, sizeof(nv_test_buf_1));
		if(nv_flashWriteNew(1, NV_MODULE_APP, TEST_ITEM2, sizeof(nv_test_buf_1), nv_test_buf_1) != NV_SUCC){
			while(1);
		}

		if(nv_flashReadNew(1, NV_MODULE_APP, TEST_ITEM2, sizeof(nv_test_buf_1), test_chk_buf) != NV_SUCC){
			while(1);
		}

		if(memcmp(nv_test_buf_1, test_chk_buf, sizeof(nv_test_buf_1))){
			while(1);
		}

		gpio_toggle(TEST_GPIO);
	}
}
#endif

#define MODULE_TEST_PM	0

#if MODULE_TEST_PM

#if (__PROJECT_TL_SWITCH__)
#define PAD_WAKUPUP_TEST		1
#define TIMER_WAKUPUP_TEST		1

extern drv_pm_pinCfg_t g_switchPmCfg[];
extern void light_on(void);
extern void light_off(void);

unsigned char txPktForPm[48] __attribute__ ((aligned (4))) = {0x09, 0x00, 0x00, 0x00, 0x0a, 0x03, 0x08, 0xd0, 0xff, 0xff, 0xff, 0xff, 0x07};//{0x14,0x00,0x00,0x00,0x15, 0x00, 0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff, 0xee, 0xdd, 0xcc, 0xbb};

void moduleTest_PM(void){
	drv_pm_wakeup_src_e wakeupSrc = 0;
	drv_pm_sleep_mode_e mode = 0;

	PM_CLOCK_INIT();

	ZB_RADIO_TX_POWER_SET(0);
	ZB_RADIO_TRX_CFG(144);

	ZB_RADIO_TRX_SWITCH(RF_MODE_TX, 50);	//2450, channel 20

	ZB_RADIO_DMA_HDR_BUILD(txPktForPm, 8);

#if PAD_WAKUPUP_TEST
	drv_pm_wakeupPinConfig(g_switchPmCfg, 2);

	wakeupSrc |= PM_WAKEUP_SRC_PAD;
#endif

	u32 interval = 0;
#if TIMER_WAKUPUP_TEST
	interval = 1000;
	wakeupSrc |= PM_WAKEUP_SRC_TIMER;
#endif

#if defined(MCU_CORE_826x)
	mode = PM_SLEEP_MODE_SUSPEND;
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278) || defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
	mode = PM_SLEEP_MODE_DEEP_WITH_RETENTION;
#endif

	if(wakeupSrc == (PM_WAKEUP_SRC_PAD | PM_WAKEUP_SRC_TIMER)){
		interval = 10000;
	}

	drv_enable_irq();

	WaitUs(1000*1000);

	while(1){
		ZB_RADIO_TX_START(txPktForPm);

		for(u32 i = 0; i < 2; i++){
			light_on();
			WaitUs(100*1000);
			light_off();
			WaitUs(100*1000);
		}

		drv_pm_sleep(mode, wakeupSrc, interval);
	}
}
#else
	#error please compile the project of "sampleSwitch"
#endif
#endif

#define MODULE_TEST_RF		0

#if MODULE_TEST_RF

#if defined(MCU_CORE_826x)
	#define TEST_GPIO		GPIO_PB6
#elif defined(MCU_CORE_8258)
	#define TEST_GPIO		GPIO_PA3
#elif defined(MCU_CORE_8278)
	#define TEST_GPIO		GPIO_PA3
#elif defined(MCU_CORE_B91)
	#define TEST_GPIO		GPIO_PB7
#elif defined(MCU_CORE_B92)
	#define TEST_GPIO		GPIO_PE7
#elif defined(MCU_CORE_TL721X)
	#define TEST_GPIO		GPIO_PC0
#elif defined(MCU_CORE_TL321X)
	#define TEST_GPIO		GPIO_PD0
#else
	#error	"undefined TEST_MODULE"
#endif

#define TX 	1
#define SRX	0

unsigned char rx_packet[192] __attribute__ ((aligned (4)));
unsigned char tx_packet[48] __attribute__ ((aligned (4))) = {
		0x09, 0x00, 0x00, 0x00, 0x0a, 0x03, 0x08, 0xd0, 0xff, 0xff, 0xff, 0xff, 0x07
};//{0x14,0x00,0x00,0x00,0x15, 0x00, 0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff, 0xee, 0xdd, 0xcc, 0xbb};

void moduleTest_RF(void){
	drv_gpio_func_set(TEST_GPIO);
	drv_gpio_output_en(TEST_GPIO, 1); 		//enable output
	drv_gpio_input_en(TEST_GPIO, 0);		//disable input
	drv_gpio_write(TEST_GPIO, 1);           //LED On

	ZB_RADIO_TX_POWER_SET(0);
	ZB_RADIO_TRX_CFG(144);

	ZB_RADIO_RX_BUF_SET(rx_packet);

	drv_enable_irq();

#if TX
	ZB_RADIO_TRX_SWITCH(RF_MODE_TX, 50);	//2450, channel 20

	ZB_RADIO_DMA_HDR_BUILD(tx_packet, 8);

	while(1){
		WaitMs(500);
		ZB_RADIO_TX_START(tx_packet);
		gpio_toggle(TEST_GPIO);
	}
#else

#if SRX
	ZB_RADIO_TRX_SWITCH(RF_MODE_AUTO, 50);
	ZB_RADIO_SRX_START(clock_time());
#else
	ZB_RADIO_TRX_SWITCH(RF_MODE_RX, 50);
#endif
	while(1);
#endif
}
#endif

#define MODULE_TEST_MMOHASH 	0

#if MODULE_TEST_MMOHASH
u8 T_DBG_installCode[18] = {0x83,0xfe,0xd3,0x40,0x7a,0x93,0x97,0x23,0xa5,0xc6,0x39,0xb2,0x69,0x16,0xd5,0x05};
volatile u16 T_DBG_insCodeCRC;
/*
 * expected: "66B6900981E1EE3CA4206B6B861C02BB"
 * crc: c3 b5
 */
u8 T_DBG_hashOut[16] = {0};
extern u16 tl_bdbInstallCodeCRC16(u8 *pInstallCode, u8 len);
void moduleTest_hash(void){
	T_DBG_insCodeCRC = tl_bdbInstallCodeCRC16(T_DBG_installCode, 16);
	T_DBG_installCode[16] = (u8)(T_DBG_insCodeCRC & 0xff);
	T_DBG_installCode[17] = (u8)(T_DBG_insCodeCRC >> 8);
	ss_mmoHash(T_DBG_installCode, 18, T_DBG_hashOut);
	while(1);
}
#endif

#define MODULE_TEST_CCM		0

#if MODULE_TEST_CCM

#define CCM_ENCRYPT		0
#define CCM_DECRYPT		1

#define TEST_MODE		CCM_ENCRYPT

u8 key[] = {0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF};
u8 nonce[] = {0x21, 0x43, 0x65, 0x87, 0x21, 0x43, 0x65, 0x87, 0x02, 0x00, 0x00, 0x00, 0x05};
u8 astring[] = {0x8c, 0x10, 0x21, 0x43, 0x65, 0x87, 0x02, 0x00, 0x00, 0x00, 0x20};
u8 astrLen = 11;
u8 mstrLen = 1;
#if (TEST_MODE == CCM_ENCRYPT)
u8 mstring[] = {0x20};//plain text
u8 mic[4];
#else
u8 mstring[] = {0x83};//cipher text
u8 mic[] = {0x92, 0x3c, 0x5d, 0x65};
volatile u8 T_ret = 0;
#endif

void moduleTest_ccm(void){
#if (TEST_MODE == CCM_ENCRYPT)
	aes_ccmAuthTran(4, key, nonce, mstring, mstrLen, astring, astrLen, mic);
	aes_ccmEncTran(4, key, nonce, mstring, mstrLen, astring, astrLen, mic);
#else
	aes_ccmDecTran(4, key, nonce, mstring, mstrLen, astring, astrLen, mic);
	u8 ret = aes_ccmDecAuthTran(4, key, nonce, mstring, mstrLen, astring, astrLen, mic);
	if(ret == RET_OK){
		T_ret++;//success
	}
#endif

	while(1);
}
#endif

#define MODULE_TEST_ADC			0

#if MODULE_TEST_ADC

volatile u16 Cvoltage_value = 0;

void moduleTest_adc(void){
	drv_adc_init();

#if defined(MCU_CORE_826x)
	drv_adc_mode_pin_set(DRV_ADC_VBAT_MODE, NOINPUT);
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	drv_adc_mode_pin_set(DRV_ADC_BASE_MODE, GPIO_PB3);
#elif defined(MCU_CORE_B91)
	drv_adc_mode_pin_set(DRV_ADC_BASE_MODE, ADC_GPIO_PB0);
#elif defined(MCU_CORE_B92)
	drv_adc_mode_pin_set(DRV_ADC_BASE_MODE, ADC_GPIO_PB5);
#elif defined(MCU_CORE_TL721X)
	drv_adc_mode_pin_set(DRV_ADC_BASE_MODE, ADC_GPIO_PB6);
#elif defined(MCU_CORE_TL321X)
	drv_adc_mode_pin_set(DRV_ADC_BASE_MODE, ADC_GPIO_PB4);
#endif

	drv_adc_enable(1);

	while(1)
	{
		Cvoltage_value = drv_get_adc_data();
		WaitMs(2000);
	}
}
#endif

#define MODULE_TEST_TIMER		0

#if MODULE_TEST_TIMER

#if defined(MCU_CORE_826x)
	#define TEST_GPIO_0		GPIO_PB6
	#define TEST_GPIO_1		GPIO_PB4
#elif defined(MCU_CORE_8258)
	#define TEST_GPIO_0		GPIO_PA3
	#define TEST_GPIO_1		GPIO_PA2
#elif defined(MCU_CORE_8278)
	#define TEST_GPIO_0		GPIO_PA3
	#define TEST_GPIO_1		GPIO_PA2
#elif defined(MCU_CORE_B91)
	#define TEST_GPIO_0		GPIO_PB7
	#define TEST_GPIO_1		GPIO_PB6
#elif defined(MCU_CORE_B92)
	#define TEST_GPIO_0		GPIO_PD1
	#define TEST_GPIO_1		GPIO_PE7
#elif defined(MCU_CORE_TL721X)
	#define TEST_GPIO_0		GPIO_PC0
	#define TEST_GPIO_1		GPIO_PC1
#elif defined(MCU_CORE_TL321X)
	#define TEST_GPIO_0		GPIO_PB0
	#define TEST_GPIO_1		GPIO_PB1
#else
	#error	"undefined TEST_MODULE"
#endif

volatile u8 moduleTestTimerFlg = 0;
s32 moduleTestTimer0Cb(void *arg){
	static u8 cnt = 0;

	gpio_toggle(TEST_GPIO_0);

	if(++cnt >= 10){
		cnt = 0;
		moduleTestTimerFlg = 1;

		return -1;
	}

	return 0;
}

s32 moduleTestTimer1Cb(void *arg){
	gpio_toggle(TEST_GPIO_1);

	if(moduleTestTimerFlg){
		return 500*1000;
	}

	return 0;
}

s32 moduleTestTimer3Cb(void *arg){
	if(moduleTestTimerFlg){
		gpio_toggle(TEST_GPIO_0);
		return 2000*1000;
	}

	return 0;
}

void moduleTest_timer(void){
	drv_gpio_func_set(TEST_GPIO_0);
	drv_gpio_output_en(TEST_GPIO_0, 1); 		//enable output
	drv_gpio_input_en(TEST_GPIO_0, 0);			//disable input
	drv_gpio_write(TEST_GPIO_0, 1);             //LED On

	drv_gpio_func_set(TEST_GPIO_1);
	drv_gpio_output_en(TEST_GPIO_1, 1); 		//enable output
	drv_gpio_input_en(TEST_GPIO_1, 0);			//disable input
	drv_gpio_write(TEST_GPIO_1, 1);             //LED On

	drv_hwTmr_init(TIMER_IDX_0, TIMER_MODE_SCLK);
	drv_hwTmr_set(TIMER_IDX_0, 500*1000, moduleTestTimer0Cb, NULL);

	drv_hwTmr_init(TIMER_IDX_1, TIMER_MODE_SCLK);
	drv_hwTmr_set(TIMER_IDX_1, 1000*1000, moduleTestTimer1Cb, NULL);

	drv_hwTmr_init(TIMER_IDX_3, TIMER_MODE_SCLK);
	drv_hwTmr_set(TIMER_IDX_3, 2000*1000, moduleTestTimer3Cb, NULL);

	drv_enable_irq();

	while(1){

	}
}
#endif


#define MODULE_TEST_PWM		0
#if MODULE_TEST_PWM

#if (__PROJECT_TL_DIMMABLE_LIGHT__)
void moduleTest_pwm(void){
	PWM_R_CHANNEL_SET();
	drv_pwm_init();

	u8 duty = 5;
	u8 fullDuty = 100;
	u32 max_tick = PWM_CLOCK_SOURCE / 1000;
	u32 cmp_tick = (duty * max_tick) / fullDuty;

	drv_pwm_cfg(PWM_R_CHANNEL, (u16)cmp_tick, (u16)max_tick);
	drv_pwm_start(PWM_R_CHANNEL);

	while(1){
		if(cmp_tick <= max_tick){
			drv_pwm_cfg(PWM_R_CHANNEL, (u16)cmp_tick, (u16)max_tick);
			cmp_tick += 2400;
			WaitMs(500);
		}else{
			cmp_tick = 0;
		}
	}
}
#else
	#error please compile the project of "sampleLight"
#endif
#endif

#define MODULE_TEST_SPI 	0

#if MODULE_TEST_SPI
/*********************************************
 *  @brief  SPI interface
 *  -----------------------------------
 *   	 |	SDO  SDI  SCL  CS |
 *  826x |	A2   A3   A4   A5 |
 *  	 |	B5   B6   B7   B4 |
 *  -----------------------------------
 *  	 |	SDO  SDI  SCL  CS |
 *  8258 |	A2   A3   A4   D6 |
 *  	 |	B7   B6   D7   D2 |
 *  -----------------------------------
 *  	 |	SDO  SDI  SCL  CS |
 *  8278 |	A2   A3   A4   D6 |
 *  	 |	B7   B6   D7   D2 |
 *  -----------------------------------
 *  	 |	MOSI MISO SCL  CS |
 *  B91  |	C7   C6   C5   C4 |
 *  	 |	B7   B6   B5   C0 |
 *  -----------------------------------
 *  	 |	MOSI MISO SCL  CS |
 *  B92  |	B3   B2   A3   A0 |
 *  	 |	B7   B6   B5   C0 |
 *  -----------------------------------
 *    	 |	MOSI MISO SCL  CS |
 * TL321X|	B3   B2   A3   A0 |
 *  	 |	A3   A2   A4   D6 |		//connect to 8258
 *  -----------------------------------
 */

#if defined(MCU_CORE_826x)
	#define TEST_SPI_PIN_GROUP		SPI_PIN_GPIOA
	#define TEST_SPI_PIN_CS			GPIO_PA5
#elif defined(MCU_CORE_8258)
	#define TEST_SPI_PIN_GROUP		SPI_GPIO_GROUP_A2A3A4D6
	#define TEST_SPI_PIN_CS			GPIO_PD6
#elif defined(MCU_CORE_8278)
	#define TEST_SPI_PIN_SDO		GPIO_PA2
	#define TEST_SPI_PIN_SDI		GPIO_PA3
	#define TEST_SPI_PIN_SCL		GPIO_PA4
	#define TEST_SPI_PIN_CS			GPIO_PD6
#elif defined(MCU_CORE_B91)
	#define TEST_SPI_PIN_MOSI		GPIO_PC7
	#define TEST_SPI_PIN_MISO		GPIO_PC6
	#define TEST_SPI_PIN_SCL		GPIO_PC5
	#define TEST_SPI_PIN_CS			GPIO_PC4
#elif defined(MCU_CORE_B92)
	#define TEST_SPI_PIN_MOSI		GPIO_PB3
	#define TEST_SPI_PIN_MISO		GPIO_PB2
	#define TEST_SPI_PIN_SCL		GPIO_PA3
	#define TEST_SPI_PIN_CS			GPIO_PA0
#elif defined(MCU_CORE_TL321X)
	#define TEST_SPI_PIN_MOSI		GPIO_PB3
	#define TEST_SPI_PIN_MISO		GPIO_PB2
	#define TEST_SPI_PIN_SCL		GPIO_PA3
	#define TEST_SPI_PIN_CS			GPIO_PA0
#else
	#error	"undefined"
#endif

#define SPI_MASTER_MODE				0
#define SPI_SLAVE_MODE				1

/* Select test mode. */
#define SPI_MODE					SPI_MASTER_MODE


/* SPI Clock */
#define SPI_CLOCK					400000//400K

/* SPI slave address, for kite or vulture as slave*/
#define SPI_SLAVE_ADDR				0x48000//0x8800
#define SPI_SLAVE_ADDR_LEN			3//2

/* SPI read/write command */
#define SPI_READ_CMD  				0x80
#define SPI_WRITE_CMD 				0x00

#define CMD_BUF_LEN					6
#define DBG_DATA_LEN    			16


u8 cmd_buf[CMD_BUF_LEN] = {0};
//write buff
u8 spi_master_tx_buff[DBG_DATA_LEN] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
//read buff
u8 spi_master_rx_buff[DBG_DATA_LEN] = {0x00};




void spi_master_test_init(void){
	drv_spi_master_init(SPI_CLOCK, SPI_MODE_0);

#if	defined(MCU_CORE_826x) || defined(MCU_CORE_8258)
	drv_spi_master_pin_select(TEST_SPI_PIN_GROUP);
#elif defined(MCU_CORE_8278)
	drv_spi_master_pin_select(TEST_SPI_PIN_SCL, TEST_SPI_PIN_CS, TEST_SPI_PIN_SDO, TEST_SPI_PIN_SDI);
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL321X)
	drv_spi_master_pin_select(TEST_SPI_PIN_SCL, TEST_SPI_PIN_CS, TEST_SPI_PIN_MOSI, TEST_SPI_PIN_MISO);
#endif
}

void spi_slave_test_init(void){
	drv_spi_slave_init(SPI_MODE_0);

#if	defined(MCU_CORE_826x) || defined(MCU_CORE_8258)
	drv_spi_slave_pin_select(TEST_SPI_PIN_GROUP);
#elif defined(MCU_CORE_8278)
	drv_spi_slave_pin_select(TEST_SPI_PIN_SCL, TEST_SPI_PIN_CS, TEST_SPI_PIN_SDO, TEST_SPI_PIN_SDI);
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL321X)
	drv_spi_slave_pin_select(TEST_SPI_PIN_SCL, TEST_SPI_PIN_CS, TEST_SPI_PIN_MOSI, TEST_SPI_PIN_MISO);
#endif
}

void spi_master_test(void){
	WaitMs(1000);   //1S

	spi_master_tx_buff[0] += 1;
	spi_master_tx_buff[0] &= 0xff;

	u8 idx = 0;
	for(u8 i = 0; i < SPI_SLAVE_ADDR_LEN; i++){
		cmd_buf[idx++] = (SPI_SLAVE_ADDR >> ((SPI_SLAVE_ADDR_LEN - 1 - i) << 3));
	}

	cmd_buf[idx] = SPI_WRITE_CMD;
	drv_spi_write(cmd_buf, idx + 1, spi_master_tx_buff, DBG_DATA_LEN, TEST_SPI_PIN_CS);

	WaitMs(1000);   //1S

	cmd_buf[idx] = SPI_READ_CMD;
	drv_spi_read(cmd_buf, idx + 1, spi_master_rx_buff, DBG_DATA_LEN, TEST_SPI_PIN_CS);
}

void main_loop(void){
#if(SPI_MODE == SPI_MASTER_MODE)
	spi_master_test();
#else
	WaitMs(50);
#endif
}

void moduleTest_spi(void){
	WaitMs(2000);//leave enough time for SWS_reset when power on

#if(SPI_MODE == SPI_MASTER_MODE)
	 spi_master_test_init();
#else
	 spi_slave_test_init();
#endif

	while(1){
		main_loop();
	}
}
#endif


#define MODULE_TEST_I2C		0

#if MODULE_TEST_I2C

#if	defined(MCU_CORE_826x)
	#define TEST_I2C_PIN_GROUP		I2C_PIN_GPIOA
#elif defined(MCU_CORE_8258)
	#define TEST_I2C_PIN_GROUP		I2C_GPIO_GROUP_A3A4
#elif defined(MCU_CORE_8278)
	#define TEST_I2C_PIN_SDA		I2C_GPIO_SDA_A3
	#define TEST_I2C_PIN_SCL		I2C_GPIO_SCL_A4
#elif defined(MCU_CORE_B91)
	#define TEST_I2C_PIN_SDA		I2C_GPIO_SDA_B3
	#define TEST_I2C_PIN_SCL		I2C_GPIO_SCL_B2
#elif defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X)
	#define TEST_I2C_PIN_SDA		GPIO_FC_PB3
	#define TEST_I2C_PIN_SCL		GPIO_FC_PB2
#elif defined(MCU_CORE_TL321X)
	#define TEST_I2C_PIN_SDA  		GPIO_FC_PB5
	#define TEST_I2C_PIN_SCL  		GPIO_FC_PB6
#endif

/* I2C Clock */
#define I2C_CLOCK					200000//200K

/* I2C slave ID, use B85 dma mode as slave */
#define I2C_SLAVE_ID				0x5A
#define I2C_SLAVE_ADDR				0x48000//0x8800
#define I2C_SLAVE_ADDR_LEN			3//2

#define DBG_DATA_LEN				16
//write buff
u8 i2c_master_tx_buff[DBG_DATA_LEN] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
//read buff
u8 i2c_master_rx_buff[DBG_DATA_LEN] = {0x00};

void i2c_master_test_init(void){
	drv_i2c_master_init(I2C_CLOCK);

#if	defined(MCU_CORE_826x) || defined(MCU_CORE_8258)
	drv_i2c_gpio_set(TEST_I2C_PIN_GROUP);
#elif defined(MCU_CORE_8278) || defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
	drv_i2c_gpio_set(TEST_I2C_PIN_SDA, TEST_I2C_PIN_SCL);
#endif
}

void i2c_master_test(void){
	WaitMs(1000);   //1S

	gpio_toggle(GPIO_PB7);

	i2c_master_tx_buff[0] += 1;
	i2c_master_tx_buff[0] &= 0xff;

	drv_i2c_write_series(I2C_SLAVE_ID, I2C_SLAVE_ADDR, I2C_SLAVE_ADDR_LEN, i2c_master_tx_buff, DBG_DATA_LEN);

	WaitMs(1000);   //1S

	drv_i2c_read_series(I2C_SLAVE_ID, I2C_SLAVE_ADDR, I2C_SLAVE_ADDR_LEN, i2c_master_rx_buff, DBG_DATA_LEN);
}

void moduleTest_i2c(void){
	WaitMs(2000);//leave enough time for SWS_reset when power on

	drv_gpio_func_set(GPIO_PB7);
	drv_gpio_output_en(GPIO_PB7, 1); 		//enable output
	drv_gpio_input_en(GPIO_PB7, 0);		//disable input
	drv_gpio_write(GPIO_PB7, 1);              	//LED On

	i2c_master_test_init();

	while(1){
		i2c_master_test();
	}
}
#endif


#define MODULE_TEST_GPIO_IRQ	0

#if MODULE_TEST_GPIO_IRQ

#if	defined(MCU_CORE_826x)
	#define TEST_LED1		GPIO_PC2
	#define TEST_LED2		GPIO_PC3
	#define TEST_SW1		GPIO_PD2
	#define TEST_SW2		GPIO_PC5
	#define TEST_GPIO		GPIO_PD3
#elif defined(MCU_CORE_8258)
	#define TEST_LED1		GPIO_PA4
	#define TEST_LED2		GPIO_PB1
	#define TEST_SW1		GPIO_PD5
	#define TEST_SW2		GPIO_PD6
	#define TEST_GPIO		GPIO_PB2
#elif defined(MCU_CORE_8278)
	#define TEST_LED1		GPIO_PA2
	#define TEST_LED2		GPIO_PA3
	#define TEST_SW1		GPIO_PD6
	#define TEST_SW2		GPIO_PD5
	#define TEST_GPIO		GPIO_PB2
#elif defined(MCU_CORE_B91)	//board_9128_dongle
	#define TEST_LED1		GPIO_PB1
	#define TEST_LED2		GPIO_PB4
	#define TEST_SW1		GPIO_PB3
	#define TEST_SW2		GPIO_PB2
	#define TEST_GPIO		GPIO_PC1
#elif defined(MCU_CORE_B92)		//B92 EVK
	#define TEST_LED1		GPIO_PE7	//LED_R
	#define TEST_LED2		GPIO_PD0	//LED_B
	#define TEST_SW1		GPIO_PD2
	#define TEST_SW2		GPIO_PD7
	#define TEST_GPIO		GPIO_PB3
#elif defined(MCU_CORE_TL721X)		//TL721X EVK
	#define TEST_LED1		GPIO_PC0
	#define TEST_LED2		GPIO_PC1
	#define TEST_SW1		GPIO_PD4
	#define TEST_SW2		GPIO_PD6
	#define TEST_GPIO		GPIO_PB6
#elif defined(MCU_CORE_TL321X)		//TL321X EVK
	#define TEST_LED1		GPIO_PB0
	#define TEST_LED2		GPIO_PB1
	#define TEST_SW1		GPIO_PB3
	#define TEST_SW2		GPIO_PB5
	#define TEST_GPIO		GPIO_PC6
#else
	#error	"undefined TEST_MODULE"
#endif


volatile u8 T_DBG_gpioIrqCb1 = 0;
void moduleTest_gpioIrqCb1(void){
	T_DBG_gpioIrqCb1++;
	gpio_toggle(TEST_LED1);
}

volatile u8 T_DBG_gpioIrqCb2 = 0;
void moduleTest_gpioIrqCb2(void){
	T_DBG_gpioIrqCb2++;
	gpio_toggle(TEST_LED2);
}

volatile u8 T_DBG_gpioIrqCb3 = 0;
void moduleTest_gpioIrqCb3(void){
	T_DBG_gpioIrqCb3++;
	gpio_toggle(TEST_LED1);

	if(drv_gpio_read(TEST_GPIO)){
#if defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
		drv_gpio_up_down_resistor(TEST_GPIO, GPIO_PIN_PULLUP_10K);
#else
		drv_gpio_up_down_resistor(TEST_GPIO, PM_PIN_PULLUP_10K);
#endif

#if defined(MCU_CORE_TL321X)
		drv_gpio_irq_config(GPIO_IRQ0, TEST_GPIO, FALLING_EDGE, moduleTest_gpioIrqCb3);
#else
		drv_gpio_irq_config(GPIO_IRQ_MODE, TEST_GPIO, FALLING_EDGE, moduleTest_gpioIrqCb3);
#endif
	}else{
#if defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
		drv_gpio_up_down_resistor(TEST_GPIO, GPIO_PIN_PULLDOWN_100K);
#else
		drv_gpio_up_down_resistor(TEST_GPIO, PM_PIN_PULLDOWN_100K);
#endif

#if defined(MCU_CORE_TL321X)
		drv_gpio_irq_config(GPIO_IRQ0, TEST_GPIO, RISING_EDGE, moduleTest_gpioIrqCb3);
#else
		drv_gpio_irq_config(GPIO_IRQ_MODE, TEST_GPIO, RISING_EDGE, moduleTest_gpioIrqCb3);
#endif
	}
}

volatile u8 T_DBG_mainCnt = 0;
void moduleTest_gpioIrq(void)
{
	//1.init the LED pin,for indication
	drv_gpio_func_set(TEST_LED1);
	drv_gpio_output_en(TEST_LED1, 1); 		//enable output
	drv_gpio_input_en(TEST_LED1, 0);		//disable input
	//drv_gpio_write(TEST_LED1, 1);         //LED On

	drv_gpio_func_set(TEST_LED2);
	drv_gpio_output_en(TEST_LED2, 1); 		//enable output
	drv_gpio_input_en(TEST_LED2, 0);		//disable input
	drv_gpio_write(TEST_LED2, 1);           //LED On

#if 0
	drv_gpio_func_set(TEST_SW1);
	drv_gpio_output_en(TEST_SW1, 0);
	drv_gpio_input_en(TEST_SW1, 1);
#if defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
	drv_gpio_up_down_resistor(TEST_SW1, GPIO_PIN_PULLUP_10K);
#else
	drv_gpio_up_down_resistor(TEST_SW1, PM_PIN_PULLUP_10K);
#endif

#if defined(MCU_CORE_TL321X)
	drv_gpio_irq_config(GPIO_IRQ1, TEST_SW1, FALLING_EDGE, moduleTest_gpioIrqCb1);
	drv_gpio_irq_en(TEST_SW1);
#else
	drv_gpio_irq_config(GPIO_IRQ_RISC0_MODE, TEST_SW1, FALLING_EDGE, moduleTest_gpioIrqCb1);
	drv_gpio_irq_risc0_en(TEST_SW1);
#endif

#endif

#if 0
	drv_gpio_func_set(TEST_SW2);
	drv_gpio_output_en(TEST_SW2, 0);
	drv_gpio_input_en(TEST_SW2, 1);
#if defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
	drv_gpio_up_down_resistor(TEST_SW2, GPIO_PIN_PULLUP_10K);
#else
	drv_gpio_up_down_resistor(TEST_SW2, PM_PIN_PULLUP_10K);
#endif

#if defined(MCU_CORE_TL321X)
	drv_gpio_irq_config(GPIO_IRQ2, TEST_SW2, FALLING_EDGE, moduleTest_gpioIrqCb2);
	drv_gpio_irq_en(TEST_SW2);
#else
	drv_gpio_irq_config(GPIO_IRQ_RISC1_MODE, TEST_SW2, FALLING_EDGE, moduleTest_gpioIrqCb2);
	drv_gpio_irq_risc1_en(TEST_SW2);
#endif

#endif

	drv_gpio_func_set(TEST_GPIO);
	drv_gpio_output_en(TEST_GPIO, 0);
	drv_gpio_input_en(TEST_GPIO, 1);
	if(drv_gpio_read(TEST_GPIO)){
#if defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
		drv_gpio_up_down_resistor(TEST_GPIO, GPIO_PIN_PULLUP_10K);
#else
		drv_gpio_up_down_resistor(TEST_GPIO, PM_PIN_PULLUP_10K);
#endif

#if defined(MCU_CORE_TL321X)
		drv_gpio_irq_config(GPIO_IRQ0, TEST_GPIO, FALLING_EDGE, moduleTest_gpioIrqCb3);
#else
		drv_gpio_irq_config(GPIO_IRQ_MODE, TEST_GPIO, FALLING_EDGE, moduleTest_gpioIrqCb3);
#endif
		drv_gpio_write(TEST_LED1, 1);
	}else{
#if defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
		drv_gpio_up_down_resistor(TEST_GPIO, GPIO_PIN_PULLDOWN_100K);
#else
		drv_gpio_up_down_resistor(TEST_GPIO, PM_PIN_PULLDOWN_100K);
#endif

#if defined(MCU_CORE_TL321X)
		drv_gpio_irq_config(GPIO_IRQ0, TEST_GPIO, RISING_EDGE, moduleTest_gpioIrqCb3);
#else
		drv_gpio_irq_config(GPIO_IRQ_MODE, TEST_GPIO, RISING_EDGE, moduleTest_gpioIrqCb3);
#endif
		drv_gpio_write(TEST_LED1, 0);
	}

	drv_gpio_irq_en(TEST_GPIO);

	drv_enable_irq();

	while(1){
		T_DBG_mainCnt++;
		gpio_toggle(TEST_LED2);
		WaitMs(500);
	};
}
#endif


void moduleTest_start(void){
#if MODULE_TEST_MMOHASH
	moduleTest_hash();
#endif

#if MODULE_TEST_CCM
	moduleTest_ccm();
#endif

#if MODULE_TEST_PM
	moduleTest_PM();
#endif

#if MODULE_TEST_UART
	moduleTest_forUart();
#endif

#if MODULE_TEST_RF
	moduleTest_RF();
#endif

#if MODULE_TEST_ADC
	moduleTest_adc();
#endif

#if MODULE_TEST_TIMER
	moduleTest_timer();
#endif

#if MODULE_TEST_PWM
	moduleTest_pwm();
#endif

#if MODULE_TEST_SPI
	moduleTest_spi();
#endif

#if MODULE_TEST_I2C
	moduleTest_i2c();
#endif

#if MODULE_TEST_NV
	moduleTest_NV();
#endif

#if MODULE_TEST_GPIO_IRQ
	moduleTest_gpioIrq();
#endif
}
