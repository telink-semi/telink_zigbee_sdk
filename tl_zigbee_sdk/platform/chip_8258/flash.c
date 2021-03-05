/********************************************************************************************************
 * @file	flash.c
 *
 * @brief	This is the source file for B85
 *
 * @author	Driver & Zigbee Group
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
#include "flash.h"
#include "spi_i.h"
#include "irq.h"
#include "timer.h"
#include "string.h"

/*******************************************************************************************************************
 *												Primary interface
 ******************************************************************************************************************/

/**
 * @brief		This function to determine whether the flash is busy..
 * @return		1:Indicates that the flash is busy. 0:Indicates that the flash is free
 */
_attribute_ram_code_ static inline int flash_is_busy(void){
	return mspi_read() & 0x01;		//the busy bit, pls check flash spec
}

/**
 * @brief		This function serves to set flash write command.
 * @param[in]	cmd	- set command.
 * @return		none.
 */
_attribute_ram_code_ static void flash_send_cmd(unsigned char cmd){
	mspi_high();
	sleep_us(1);
	mspi_low();
	mspi_write(cmd);
	mspi_wait();
}

/**
 * @brief		This function serves to send flash address.
 * @param[in]	addr	- the flash address.
 * @return		none.
 */
_attribute_ram_code_ static void flash_send_addr(unsigned int addr){
	mspi_write((unsigned char)(addr>>16));
	mspi_wait();
	mspi_write((unsigned char)(addr>>8));
	mspi_wait();
	mspi_write((unsigned char)(addr));
	mspi_wait();
}

/**
 * @brief     This function serves to wait flash done.(make this a asynchorous version).
 * @return    none.
 */
_attribute_ram_code_ static void flash_wait_done(void)
{
	sleep_us(100);
	flash_send_cmd(FLASH_READ_STATUS_CMD_LOWBYTE);

	int i;
	for(i = 0; i < 10000000; ++i){
		if(!flash_is_busy()){
			break;
		}
	}
	mspi_high();
}

/**
 * @brief 		This function serves to erase a sector.
 * @param[in]   addr	- the start address of the sector needs to erase.
 * @return 		none.
 */
_attribute_ram_code_ void flash_erase_sector(unsigned long addr)
{
	unsigned char r = irq_disable();

	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_SECT_ERASE_CMD);
	flash_send_addr(addr);
	mspi_high();
	flash_wait_done();

	irq_restore(r);
}

/**
 * @brief 		This function writes the buffer's content to the flash.
 * @param[in]   addr	- the start address of the area.
 * @param[in]   len		- the length(in byte) of content needs to write into the flash.
 * @param[in]   buf		- the start address of the content needs to write into.
 * @return 		none.
 * @note        the funciton support cross-page writing,which means the len of buf can bigger than 256.
 */
_attribute_ram_code_ void flash_write_page(unsigned long addr, unsigned long len, unsigned char *buf)
{
	unsigned char r = irq_disable();

	// important:  buf must not reside at flash, such as constant string.  If that case, pls copy to memory first before write
	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_WRITE_CMD);
	flash_send_addr(addr);

	unsigned int i;
	for(i = 0; i < len; ++i){
		mspi_write(buf[i]);		/* write data */
		mspi_wait();
	}
	mspi_high();
	flash_wait_done();

	irq_restore(r);
}

/**
 * @brief 		This function reads the content from a page to the buf.
 * @param[in]   addr	- the start address of the page.
 * @param[in]   len		- the length(in byte) of content needs to read out from the page.
 * @param[out]  buf		- the start address of the buffer.
 * @return 		none.
 */
_attribute_ram_code_ void flash_read_page(unsigned long addr, unsigned long len, unsigned char *buf)
{
	unsigned char r = irq_disable();

	flash_send_cmd(FLASH_READ_CMD);
	flash_send_addr(addr);

	mspi_write(0x00);		/* dummy,  to issue clock */
	mspi_wait();
	mspi_ctrl_write(0x0a);	/* auto mode */
	mspi_wait();

	for(int i = 0; i < len; ++i){
		*buf++ = mspi_get();
		mspi_wait();
	}
	mspi_high();

	irq_restore(r);
}

/*
 * @brief		This function reads the status of flash.
 * @param[in] 	cmd	- the cmd of read status.
 * @return 		the value of status.
 */
_attribute_ram_code_ unsigned char flash_read_status(unsigned char cmd)
{
	unsigned char r = irq_disable();
	unsigned char status =0;
	flash_send_cmd(cmd);
	/* get 8 bit status */
	status = mspi_read();
	mspi_high();
	irq_restore(r);
	return status;
}

/**
 * @brief 		This function write the status of flash.
 * @param[in]  	type	- the type of status.8 bit or 16 bit.
 * @param[in]  	data	- the value of status.
 * @return 		none.
 */
_attribute_ram_code_ void flash_write_status(flash_status_typedef_e type , unsigned short data)
{
	unsigned char r = irq_disable();

	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_WRITE_STATUS_CMD_LOWBYTE);
	if ((type == FLASH_TYPE_8BIT_STATUS))
	{
		mspi_write((unsigned char)data);		//8 bit status
	}
	else if(type == FLASH_TYPE_16BIT_STATUS_ONE_CMD)
	{
		mspi_write((unsigned char)data);
		mspi_wait();
		mspi_write((unsigned char)(data>>8));	//16bit status
	}
	else if(type == FLASH_TYPE_16BIT_STATUS_TWO_CMD)
	{
		mspi_write((unsigned char)data);
		mspi_wait();
		flash_send_cmd(FLASH_WRITE_STATUS_CMD_HIGHBYTE);
		mspi_write((unsigned char)(data>>8));	//16bit status
	}
	mspi_wait();
	mspi_high();
	flash_wait_done();
	sleep_us(100);
	mspi_high();
	irq_restore(r);
}

#if 0

/**
 * @brief 		This function serves to read data from the Security Registers of the flash.
 * @param[in]   addr	- the start address of the Security Registers.
 * @param[in]   len		- the length of the content to be read.
 * @param[out]  buf		- the starting address of the content to be read.
 * @return 		none.
 */
_attribute_ram_code_ void flash_read_otp(unsigned long addr, unsigned long len, unsigned char* buf)
{
	unsigned char r = irq_disable();

	flash_send_cmd(FLASH_READ_SECURITY_REGISTERS_CMD);
	flash_send_addr(addr);

	mspi_write(0x00);		/* dummy,  to issue clock */
	mspi_wait();
	mspi_ctrl_write(0x0a);	/* auto mode */
	mspi_wait();

	mspi_get();
	mspi_wait();
	for (int i = 0; i < len; ++i)
	{
		buf[i] = mspi_get();
		mspi_wait();
	}
	mspi_high();
	flash_wait_done();
	irq_restore(r);
}

/**
 * @brief 		This function serves to write data to the Security Registers of the flash you choose.
 * @param[in]   addr	- the start address of the Security Registers.
 * @param[in]   len		- the length of content to be written.
 * @param[in]   buf		- the starting address of the content to be written.
 * @return 		none.
 */
_attribute_ram_code_ void flash_write_otp(unsigned long addr, unsigned long len, unsigned char *buf)
{
	unsigned char r = irq_disable();
	unsigned int ns = 256 - (addr&0xff);
	int nw = 0;

	do{
		nw = len > ns ? ns :len;
		// important:  buf must not reside at flash, such as constant string.  If that case, pls copy to memory first before write
		flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
		flash_send_cmd(FLASH_WRITE_SECURITY_REGISTERS_CMD);
		flash_send_addr(addr);

		for(int i = 0; i < nw; i++){
			mspi_write(buf[i]);
			mspi_wait();
		}
		mspi_high();
		flash_wait_done();
		ns = 256;
		addr+=nw;
		buf+=nw;
		len-=nw;
	}while(len>0);

	irq_restore(r);
}

/**
 * @brief 		This function serves to erase the data of the Security Registers that you choose.
 * @param[in]   addr	- the address that you want to erase.
 * @return 		none.
 * @Attention	Even you choose the middle area of the Security Registers,it will erase the whole area.
 */
_attribute_ram_code_ void flash_erase_otp(unsigned long addr)
{
	unsigned char r = irq_disable();

	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_ERASE_SECURITY_REGISTERS_CMD);
	flash_send_addr(addr);
	mspi_wait();

	mspi_high();
	flash_wait_done();

	irq_restore(r);
}
#endif


/**
 * @brief	  	This function serves to read MID of flash(MAC id). Before reading UID of flash,
 * 				you must read MID of flash. and then you can look up the related table to select
 * 				the idcmd and read UID of flash.
 * @return    	MID of the flash(4 bytes).
 */
_attribute_ram_code_ unsigned int flash_read_mid(void)
{
	unsigned char j = 0;
	unsigned int flash_mid = 0;
	unsigned char r = irq_disable();

	flash_send_cmd(FLASH_GET_JEDEC_ID);
	mspi_write(0x00);		/* dummy,  to issue clock */
	mspi_wait();
	mspi_ctrl_write(0x0a);	/* auto mode */
	mspi_wait();

	for(j = 0; j < 3; ++j){
		((unsigned char*)(&flash_mid))[j] = mspi_get();
		mspi_wait();
	}
	mspi_high();

	/*
	 * The mids of GD25LD80C and GD25LE80C are both 0x1460c8, but the status register of GD25LD80C is 8 bits,
	 * and the status register of GD25LE80C is 16 bits. The functions of the two chips are different.
	 * The software detection scheme is to write the 9th bit (0x0200) of the status register. If the write is successful,
	 * it is GD25LE80C, and the mid is changed to 0x011460c8. If it is unsuccessful, it is GD25LD80C.
	 */
	if(flash_mid == 0x001460c8)
	{
		unsigned short status = (flash_read_status(FLASH_READ_STATUS_CMD_LOWBYTE) | (flash_read_status(FLASH_READ_STATUS_CMD_HIGHBYTE) << 8));
		status |= 0x0200;
		flash_write_status(FLASH_TYPE_16BIT_STATUS_ONE_CMD, status);
		if(flash_read_status(FLASH_READ_STATUS_CMD_HIGHBYTE)&0x02)
		{
			flash_mid = 0x011460c8;
		}
	}

	irq_restore(r);
	return flash_mid;
}

/**
 * @brief	  	This function serves to read UID of flash.Before reading UID of flash, you must read MID of flash.
 * 				and then you can look up the related table to select the idcmd and read UID of flash.
 * @param[in] 	idcmd	- different flash vendor have different read-uid command. E.g: GD/PUYA:0x4B; XTX: 0x5A.
 * @param[in] 	buf		- store UID of flash.
 * @param[in] 	uidtype	- the number of uid bytes.
 * @return    	none.
 */
_attribute_ram_code_ void flash_read_uid(unsigned char idcmd,unsigned char *buf, flash_uid_typedef_e uidtype)
{
	unsigned char j = 0;
	unsigned char r = irq_disable();
	flash_send_cmd(idcmd);
	if(idcmd == FLASH_READ_UID_CMD_GD_PUYA_ZB_UT)	//< GD/PUYA/ZB/UT
	{
		flash_send_addr(0x00);
		mspi_write(0x00);		/* dummy,  to issue clock */
		mspi_wait();
	}
	else if (idcmd == FLASH_XTX_READ_UID_CMD)		//< XTX
	{
		flash_send_addr(0x80);
		mspi_write(0x00);		/* dummy,  to issue clock */
		mspi_wait();

	}
	mspi_write(0x00);			/* dummy,  to issue clock */
	mspi_wait();
	mspi_ctrl_write(0x0a);		/* auto mode */
	mspi_wait();

	for(j = 0; j < uidtype; ++j){
		*buf++ = mspi_get();
		mspi_wait();
	}
	mspi_high();
	irq_restore(r);
}

/*******************************************************************************************************************
 *												Secondary interface
 ******************************************************************************************************************/

/**
 * @brief		This function serves to read flash mid and uid,and check the correctness of mid and uid.
 * @param[out]	flash_mid	- Flash Manufacturer ID
 * @param[out]	flash_uid	- Flash Unique ID
 * @return		0: flash no uid or not a known flash model 	 1:the flash model is known and the uid is read.
 */
_attribute_ram_code_ int flash_read_mid_uid_with_check(unsigned int *flash_mid ,unsigned char *flash_uid)
{
	unsigned char no_uid[16]={0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01};
	int i,f_cnt=0;
	unsigned int mid;
	unsigned char uid_8byte = 0;
	mid = flash_read_mid();
	*flash_mid  = mid;

	/*
	 * If add flash type, need pay attention to the read uid cmd and the bir number of status register
	   Flash Type	CMD			MID		Company
	   ZB25WD40B	0x4b	0x13325E	ZB
	   ZB25WD80B	0x4b	0x14325E	ZB
	   GD25LD05C	----	0x1060C8	GD
	   GD25LD40C	0x4b	0x1360C8	GD
	   GD25LD80C	----	0x1460C8	GD
	   GD25LE80C	0x4b	0x1460C8	GD
	   GD25LQ80C	0x4b	0x1460C8	GD
	   MD25D40D		----	0x134051	GD
	   P25Q40L		0x4b	0x136085	PUYA
	   TH25D40LA	0x4b	0x1360EB	UT
	   TH25D40UA	0x4b	0x1360EB	UT
	 */
	if((mid == 0x001460C8)||(mid == 0x011460C8)||(mid == 0x1060C8)||(mid == 0x134051)||(mid == 0x136085)||(mid == 0x1360C8)||(mid == 0x1360EB)||(mid == 0x14325E)){
		flash_read_uid(FLASH_READ_UID_CMD_GD_PUYA_ZB_UT,(unsigned char *)flash_uid, FLASH_TYPE_16BYTE_UID);
	}else if(mid==0x13325e){
		flash_read_uid(FLASH_READ_UID_CMD_GD_PUYA_ZB_UT,(unsigned char *)flash_uid, FLASH_TYPE_8BYTE_UID);
		uid_8byte = 1;
	}else{
		return 0;
	}

	if(0 == uid_8byte){
		for(i=0;i<16;i++){
			if(flash_uid[i] == no_uid[i]){
				f_cnt++;
			}
		}
	}else{
		memset(flash_uid+8,0,8);	//Clear the last eight bytes of a 16 byte array when the length of uid is 8.
	 }

	if(f_cnt == 16){	//no uid flash
		return 0;
	}else{
		return 1;
	}
}

