#ifndef PM_826x_H
#define PM_826x_H
/********************************************************************************************************
 * @file     pm_826x.h
 *
 * @brief    power management function API for tlsr826x
 *
 * @author
 * @date     Dec. 1, 2016
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

#include "register_826x.h"

enum {
	// WAKEUP_SRC_ANA 0 -- 2  not supported
	 PM_WAKEUP_CORE  = BIT(5),
	 PM_WAKEUP_TIMER = BIT(6),
	 PM_WAKEUP_COMP  = BIT(7),
	 PM_WAKEUP_PAD   = BIT(8),
};

enum{
	// WAKEUP_SRC_ANA 0 -- 2  not supported
	 WAKEUP_PC3_GRP0 = BIT(0),
	 WAKEUP_PC4_GRP0 = BIT(1),
	 WAKEUP_PC5_GRP0 = BIT(2),
	 WAKEUP_PD0_GRP0 = BIT(3),
	 WAKEUP_PD1_GRP1 = BIT(4),
	 WAKEUP_PD2_GRP1 = BIT(5),
	 WAKEUP_PD3_GRP1 = BIT(6),
	 WAKEUP_PD4_GRP1 = BIT(7),
	 WAKEUP_PD5_GRP2 = BIT(8),
	 WAKEUP_PD6_GRP2 = BIT(9),
	 WAKEUP_PD7_GRP2 = BIT(10),
	 WAKEUP_PA0_GRP2 = BIT(11),
	 WAKEUP_PA1_GRP3 = BIT(12),
	 WAKEUP_PA2_GRP3 = BIT(13),
	 WAKEUP_PA3_GRP3 = BIT(14),
	 WAKEUP_PA4_GRP3 = BIT(15),
	 WAKEUP_PA7_GRP4 = BIT(16),
	 WAKEUP_PC6_GRP4 = BIT(17),
	 WAKEUP_PC7_GRP4 = BIT(18),
	 WAKEUP_PE0_GRP4 = BIT(19),
	 WAKEUP_PE1_GRP5 = BIT(20),
	 WAKEUP_PE2_GRP5 = BIT(21),
	 WAKEUP_PA5_GRP5 = BIT(22),
	 WAKEUP_PA6_GRP5 = BIT(23),
};
/*wakeup-level*/
enum{
	WAKEUP_GRP0_POS_EDG = 0,
	WAKEUP_GRP1_POS_EDG = 0,
	WAKEUP_GRP2_POS_EDG = 0,
	WAKEUP_GRP3_POS_EDG = 0,
	WAKEUP_GRP4_POS_EDG = 0,
	WAKEUP_GRP5_POS_EDG = 0,

	WAKEUP_GRP0_NEG_EDG = BIT(0),
	WAKEUP_GRP1_NEG_EDG = BIT(1),
	WAKEUP_GRP2_NEG_EDG = BIT(2),
	WAKEUP_GRP3_NEG_EDG = BIT(3),
	WAKEUP_GRP4_NEG_EDG = BIT(4),
	WAKEUP_GRP5_NEG_EDG = BIT(5),

};

typedef enum {
	 WAKEUP_SRC_PAD        = BIT(4),
	 WAKEUP_SRC_DIG_GPIO   = BIT(5) | 0X0800,
     WAKEUP_SRC_DIG_USB    = BIT(5) | 0X0400,
	 WAKEUP_SRC_TIMER      = BIT(6),
	 WAKEUP_SRC_COMP       = BIT(7),
}WakeupSrc_TypeDef;

enum {
	LOWPWR_SUSPEND	 = 0,
	LOWPWR_DEEPSLEEP  =     1,
};

enum {
	SUSPEND_MODE	 = 0,
	DEEPSLEEP_MODE  =     1,
};

typedef enum{
    GROUPA_PIN0  = 0X000 | BIT(0),
    GROUPA_PIN1  = 0X000 | BIT(1),
    GROUPA_PIN2  = 0X000 | BIT(2),
    GROUPA_PIN3  = 0X000 | BIT(3),
    GROUPA_PIN4  = 0X000 | BIT(4),
    GROUPA_PIN5  = 0X000 | BIT(5),
    GROUPA_PIN6  = 0X000 | BIT(6),
    GROUPA_PIN7  = 0X000 | BIT(7),
    GROUPA_ALL   = 0X000 | 0X00ff,

    GROUPB_PIN0  = 0X100 | BIT(0),
    GROUPB_PIN1  = 0X100 | BIT(1),
    GROUPB_PIN2  = 0X100 | BIT(2),
    GROUPB_PIN3  = 0X100 | BIT(3),
    GROUPB_PIN4  = 0X100 | BIT(4),
    GROUPB_PIN5  = 0X100 | BIT(5),
    GROUPB_PIN6  = 0X100 | BIT(6),
    GROUPB_PIN7  = 0X100 | BIT(7),
    GROUPB_ALL   = 0X100 | 0x00ff,

    GROUPC_PIN0  = 0X200 | BIT(0),
    GROUPC_PIN1  = 0X200 | BIT(1),
    GROUPC_PIN2  = 0X200 | BIT(2),
    GROUPC_PIN3  = 0X200 | BIT(3),
    GROUPC_PIN4  = 0X200 | BIT(4),
    GROUPC_PIN5  = 0X200 | BIT(5),
    GROUPC_PIN6  = 0X200 | BIT(6),
    GROUPC_PIN7  = 0X200 | BIT(7),
    GROUPC_ALL   = 0X200 | 0x00ff,

    GROUPD_PIN0  = 0X300 | BIT(0),
    GROUPD_PIN1  = 0X300 | BIT(1),
    GROUPD_PIN2  = 0X300 | BIT(2),
    GROUPD_PIN3  = 0X300 | BIT(3),
    GROUPD_PIN4  = 0X300 | BIT(4),
    GROUPD_PIN5  = 0X300 | BIT(5),
    GROUPD_PIN6  = 0X300 | BIT(6),
    GROUPD_PIN7  = 0X300 | BIT(7),
    GROUPD_ALL   = 0X300 | 0x00ff,

    GROUPE_PIN0  = 0X400 | BIT(0),
    GROUPE_PIN1  = 0X400 | BIT(1),
    GROUPE_PIN2  = 0X400 | BIT(2),
    GROUPE_PIN3  = 0X400 | BIT(3),
    GROUPE_ALL   = 0X400 | 0x000f,
}Pin_TypeDef;

/* used to restore data during deep sleep mode or reset by software */
#define DATA_STORE_FLAG				0x55

#define	REG_DEEP_BACK_FLAG			0x3A//power on reset clean
#define	REG_DEEP_FLAG				0x34//watch dog reset clean
#define	REG_FRAMECOUNT				0x35//watch dog reset clean, 4Bytes from 0x35 to 0x38

extern void PM_PadSet(Pin_TypeDef pin, int pol, int en);
extern void PM_GPIOSet(Pin_TypeDef pin, int pol, int en);
extern void PM_WakeupInit(void);
extern int  PM_LowPwrEnter(int DeepSleep, int WakeupSrc, unsigned long WakeupTick);
extern int  PM_LowPwrEnter2(int DeepSleep, int WakeupSrc, unsigned long SleepDurationUs);
extern int  cpu_sleep_wakeup(int DeepSleep, int WakeupSrc, unsigned long WakeupTick);
extern void cpu_wakeup_init(void);
extern void sleep_start(void);
extern void suspend_start(void);

static inline void usb_dp_pullup_en (int en)
{
    unsigned char dat = ReadAnalogReg(0x00);
	if (en) {
		dat &= ~BIT(4);
	}else {
		dat |= BIT(4);
	}
	WriteAnalogReg (0x00, dat);
}



static inline void config_timer_interrupt (u32 tick) {
	reg_tmr1_tick = 0;
	reg_tmr1_capt = tick;
	reg_tmr_ctrl8 |= FLD_TMR1_EN;
	reg_irq_mask |= FLD_IRQ_TMR1_EN;
}



#endif //PM_H
