/********************************************************************************************************
 * @file     spi_i.h
 *
 * @brief    SPI flash interface for tlsr8258
 *
 * @author   jian.zhang@telink-semi.com
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
#pragma once

#include "compiler.h"
#include "register.h"


// use static inline, because, spi flash code must reside in memory..
// these code may be embedd in flash code


#if  0


 static inline void mspi_wait(void){
	while(reg_master_spi_ctrl & FLD_MASTER_SPI_BUSY)
		;
}

 static inline void mspi_high(void){
	reg_master_spi_ctrl = FLD_MASTER_SPI_CS;
}

 static inline void mspi_low(void){
	reg_master_spi_ctrl = 0;
}

 static inline unsigned char mspi_get(void){
	return reg_master_spi_data;
}

 static inline void mspi_write(unsigned char c){
	reg_master_spi_data = c;
}

 static inline void mspi_ctrl_write(unsigned char c){
	reg_master_spi_ctrl = c;
}

 static inline unsigned char mspi_read(void){
	mspi_write(0);		// dummy, issue clock
	mspi_wait();
	return mspi_get();
}



#else

_attribute_ram_code_ static inline void mspi_wait(void){
	while(reg_master_spi_ctrl & FLD_MASTER_SPI_BUSY)
		;
}

_attribute_ram_code_ static inline void mspi_high(void){
	reg_master_spi_ctrl = FLD_MASTER_SPI_CS;
}

_attribute_ram_code_ static inline void mspi_low(void){
	reg_master_spi_ctrl = 0;
}

_attribute_ram_code_ static inline unsigned char mspi_get(void){
	return reg_master_spi_data;
}

_attribute_ram_code_ static inline void mspi_write(unsigned char c){
	reg_master_spi_data = c;
}

_attribute_ram_code_ static inline void mspi_ctrl_write(unsigned char c){
	reg_master_spi_ctrl = c;
}

_attribute_ram_code_ static inline unsigned char mspi_read(void){
	mspi_write(0);		// dummy, issue clock
	mspi_wait();
	return mspi_get();
}

#endif


