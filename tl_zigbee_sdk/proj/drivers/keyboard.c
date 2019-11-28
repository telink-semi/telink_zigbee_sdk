/********************************************************************************************************
 * @file     keyboard.c
 *
 * @brief	 keyboard interface function file
 *
 * @author
 * @date     Oct. 8, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#include "keyboard.h"
#include "platform_includes.h"
#include "../common/utility.h"

#if (defined(KB_DRIVE_PINS) && defined(KB_SCAN_PINS))

u32 drive_pins[] = KB_DRIVE_PINS;	//8
u32 scan_pins[] = KB_SCAN_PINS;		//18

kb_data_t	kb_event;
kb_data_t	last_kb_event;

#ifndef		SCAN_PIN_50K_PULLUP_ENABLE
#define		SCAN_PIN_50K_PULLUP_ENABLE		0
#endif

#ifndef		KB_MAP_DEFAULT
#define		KB_MAP_DEFAULT		1
#endif

#ifndef		KB_LINE_MODE
#define		KB_LINE_MODE		1
#endif

#ifndef		KB_LINE_HIGH_VALID
#define		KB_LINE_HIGH_VALID		0
#endif

#ifndef		KB_KEY_FLASH_PIN_MULTI_USE
#define		KB_KEY_FLASH_PIN_MULTI_USE		0
#endif

#ifndef		KB_HAS_CTRL_KEYS
#define		KB_HAS_CTRL_KEYS		0
#endif

#ifndef		KB_ONLY_SINGLEKEY_SUPP
#define		KB_ONLY_SINGLEKEY_SUPP		0
#endif

#ifndef		KB_RM_GHOST_KEY_EN
#define		KB_RM_GHOST_KEY_EN		0
#endif

#ifndef		KB_HAS_FN_KEY
#define		KB_HAS_FN_KEY		0
#endif

#ifndef		KB_DRV_DELAY_TIME
#define		KB_DRV_DELAY_TIME		10
#endif

typedef u8 kb_k_mp_t[ARRAY_SIZE(drive_pins)]; //typedef u8 kb_k_mp_t[8]


static const u8 kb_map_normal[ARRAY_SIZE(scan_pins)][ARRAY_SIZE(drive_pins)] = KB_MAP_NORMAL;


kb_k_mp_t * kb_p_map[2] = {
		(kb_k_mp_t *)kb_map_normal,				(kb_k_mp_t *)kb_map_normal,
};


u32	scan_pin_need;

static u8 	kb_is_fn_pressed = 0;

//u8 (*kb_k_mp)[ARRAY_SIZE(drive_pins)];
kb_k_mp_t * kb_k_mp;

void kb_rmv_ghost_key(u32 * pressed_matrix){
	u32 mix_final = 0;
	foreach_arr(i, drive_pins){
		for(int j = (i+1); j < ARRAY_SIZE(drive_pins); ++j){
			u32 mix = (pressed_matrix[i] & pressed_matrix[j]);
			// >=2 根线重合,  那就是 ghost key
			//four or three key at "#" is pressed at the same time, should remove ghost key
			if( mix && (!BIT_IS_POW2(mix) || (pressed_matrix[i] ^ pressed_matrix[j])) ){
				// remove ghost keys
				//pressed_matrix[i] &= ~mix;
				//pressed_matrix[j] &= ~mix;
				mix_final |= mix;
			}
		}
		pressed_matrix[i] &= ~mix_final;
	}
}

extern u32     sys_host_status;

unsigned int key_debounce_filter( u32 mtrx_cur[], u32 filt_en ){
    u32 kc = 0;
    static u32 mtrx_pre[ARRAY_SIZE(drive_pins)];
    static u32 mtrx_last[ARRAY_SIZE(drive_pins)];
    foreach_arr(i, drive_pins){
        u32 mtrx_tmp = mtrx_cur[i];
        if( filt_en ){
            //mtrx_cur[i] = (mtrx_last[i] ^ mtrx_tmp) ^ (mtrx_last[i] | mtrx_tmp);  //key_matrix_pressed is valid when current and last value is the same
            mtrx_cur[i] = ( ~mtrx_last[i] & (mtrx_pre[i] & mtrx_tmp) ) | ( mtrx_last[i] & (mtrx_pre[i] | mtrx_tmp) );
        }
        if ( mtrx_cur[i] != mtrx_last[i] ) {
        	kc = 1;
        }
        mtrx_pre[i] = mtrx_tmp;
        mtrx_last[i] = mtrx_cur[i];
    }
    return kc;
}


// input:          pressed_matrix,
// key_code:   output keys array
// key_max:    max keys should be returned
static inline void kb_remap_key_row(int drv_ind, u32 m, int key_max, kb_data_t *kb_data){
	foreach_arr(i, scan_pins){
		if(m & 0x01){
			u8 kc = kb_k_mp[i][drv_ind];
#if(KB_HAS_CTRL_KEYS)

			if(kc >= VK_CTRL && kc <= VK_RWIN)
				kb_data->ctrl_key |= BIT(kc - VK_CTRL);
			//else if(kc == VK_MEDIA_END)
				//lock_button_pressed = 1;
			else if(VK_ZOOM_IN == kc || VK_ZOOM_OUT == kc){
				kb_data->ctrl_key |= VK_MSK_LCTRL;
				kb_data->keycode[kb_data->cnt++] = (VK_ZOOM_IN == kc)? VK_EQUAL : VK_MINUS;
			}
			else if(kc != VK_FN)//fix fn ghost bug
				kb_data->keycode[kb_data->cnt++] = kc;

#else
			kb_data->keycode[kb_data->cnt++] = kc;
#endif
			if(kb_data->cnt >= key_max){
				break;
			}
		}
		m = m >> 1;
		if(!m){
			break;
		}
	}
}

static inline void kb_remap_key_code(u32 * pressed_matrix, int key_max, kb_data_t *kb_data, int numlock_status){

	kb_k_mp = kb_p_map[0];

	foreach_arr(i, drive_pins){
		u32 m = pressed_matrix[i];
		if(!m) continue;
		kb_remap_key_row(i, m, key_max, kb_data);
		if(kb_data->cnt >= key_max){
			break;
		}
	}
}

static u32 kb_key_pressed(u32 * gpio)
{
#if(!KB_LINE_MODE)
	foreach_arr(i,drive_pins){
		gpio_write(drive_pins[i], KB_LINE_HIGH_VALID);
		gpio_set_output_en(drive_pins[i], 1);
	}
	sleep_us (20);
#endif
	gpio_read_all ((u8*)gpio);
	u32 ret = 0;
	static u8 release_cnt = 0;
	static u32 ret_last = 0;

	foreach_arr(i,scan_pins){
		if(KB_LINE_HIGH_VALID != !gpio_read_cache (scan_pins[i], (u8*)gpio)){
			ret |= (1 << i);
			release_cnt = 6;
			ret_last = ret;
		}
		//ret = ret && gpio_read(scan_pins[i]);
	}
	if(release_cnt){
		ret = ret_last;
		release_cnt--;
	}

#if(!KB_LINE_MODE)
	foreach_arr(i,drive_pins){
		gpio_write(drive_pins[i], 0);
		gpio_set_output_en(drive_pins[i], 0);
	}
#endif
	return ret;
}

u32 kb_scan_row(int drv_ind, u32 * gpio){
	/*
	 * set as gpio mode if using spi flash pin
	 * */
	//u8 sr = irq_disable();
#if	(KB_KEY_FLASH_PIN_MULTI_USE)
	MSPI_AS_GPIO;
#endif

#if(!KB_LINE_MODE)
	u32 drv_pin = drive_pins[drv_ind];
	gpio_write(drv_pin, KB_LINE_HIGH_VALID);
	gpio_set_output_en(drv_pin, 1);
#endif

	u32 matrix = 0;
	foreach_arr(j, scan_pins){
		if(scan_pin_need & BIT(j)){
			int key = !gpio_read_cache (scan_pins[j], (u8*)gpio);
			if(KB_LINE_HIGH_VALID != key) {
				matrix |= (1 << j);
			}
		}
	}
	//sleep_us(KB_DRV_DELAY_TIME);
	gpio_read_all ((u8*)gpio);
	/*
	 * set as spi mode  if using spi flash pin
	 * */
#if	(KB_KEY_FLASH_PIN_MULTI_USE)
	MSPI_AS_SPI;
#endif

#if(!KB_LINE_MODE)
	////////		float drive pin	////////////////////////////
	//sleep_us(KB_SCAN_DELAY_TIME);
	gpio_write(drv_pin, 0);
	gpio_set_output_en(drv_pin, 0);
#endif

	//irq_restore(sr);
	return matrix;
}

u32 	matrix_buff[4][ARRAY_SIZE(drive_pins)];
int		matrix_wptr, matrix_rptr;

u32 kb_scan_key (int numlock_status, int read_key) {
	u32 gpio[2];
	static u32 tick_kb_scan;
	u32 t = clock_time ();

	static u32 last_check_time = 0;

    scan_pin_need = kb_key_pressed (gpio);

	if(scan_pin_need){
		last_check_time = clock_time();

		kb_event.cnt = 0;
		kb_event.ctrl_key = 0;

		kb_is_fn_pressed = 0;

		u32 pressed_matrix[ARRAY_SIZE(drive_pins)] = {0};
		kb_k_mp = kb_p_map[0];

		kb_scan_row (0, gpio);
		for (int i=0; i<=ARRAY_SIZE(drive_pins); i++) {
			u32 r = kb_scan_row (i < ARRAY_SIZE(drive_pins) ? i : 0, gpio);
			if (i) {
				pressed_matrix[i - 1] = r;
			}
		}

#if(KB_RM_GHOST_KEY_EN)
		kb_rmv_ghost_key(&pressed_matrix[0]);
#endif

		 u32 key_changed = key_debounce_filter( pressed_matrix, 1);

		///////////////////////////////////////////////////////////////////
		//	insert buffer here
		//       key mapping requires NUMLOCK status
		///////////////////////////////////////////////////////////////////
		u32 *pd;
		if (key_changed) {

			/////////// push to matrix buffer /////////////////////////
			pd = matrix_buff[matrix_wptr&3];
			for (int k=0; k<ARRAY_SIZE(drive_pins); k++) {
				*pd++ = pressed_matrix[k];
			}
			matrix_wptr = (matrix_wptr + 1) & 7;
			if ( ((matrix_wptr - matrix_rptr) & 7) > 4 ) {	//overwrite older data
				matrix_rptr = (matrix_wptr - 4) & 7;
			}
		}

		if (numlock_status & KB_NUMLOCK_STATUS_INVALID) {
			return 1;		//return empty key
		}

		////////// read out //////////
		if (matrix_wptr == matrix_rptr || !read_key) {
			return 0;			//buffer empty, no data
		}
		pd = matrix_buff[matrix_rptr&3];
		matrix_rptr = (matrix_rptr + 1) & 7;

		///////////////////////////////////////////////////////////////////

#if(KB_ONLY_SINGLEKEY_SUPP)
		kb_remap_key_code(pd, 2, &kb_event, numlock_status);
		int multikey_pressed = 0;
		// multi-key is invalid, that is assuming not press, that is last status
		// do not set kb_event.cnt to zero, which may occur an bug, press A --> press AB --> Releae B
		if(kb_event.cnt > 1){
			kb_event.cnt = 0;
			multikey_pressed = 1;
		}
#else
		kb_remap_key_code(pd, KB_RETURN_KEY_MAX, &kb_event, numlock_status);
#endif
		return 1;
	}
	tick_kb_scan = clock_time () - t;
    return 0;
}
#endif


#if ( 1 || MODULE_KEYBOARD_ENABLE)
int kb_is_data_same(kb_data_t *a, kb_data_t *b){
	if(!a || !b){
		return 0;
	}
	if((a->cnt != b->cnt) || (a->ctrl_key != b->ctrl_key))
		return 0;
	foreach(i, a->cnt){
		if(a->keycode[i] != b->keycode[i]){
			return 0;
		}
	}
	return 1;
}
#endif
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
