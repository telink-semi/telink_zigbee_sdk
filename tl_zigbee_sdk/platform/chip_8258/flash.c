

#include "flash.h"
#include "spi_i.h"
#include "irq.h"
#include "clock.h"

_attribute_ram_code_ static inline int flash_is_busy(){
	return mspi_read() & 0x01;				//  the busy bit, pls check flash spec
}

_attribute_ram_code_ static void flash_send_cmd(unsigned char cmd){
	mspi_high();
	sleep_us(1);
	mspi_low();
	mspi_write(cmd);
	mspi_wait();
}

_attribute_ram_code_ static void flash_send_addr(unsigned int addr){
	mspi_write((unsigned char)(addr>>16));
	mspi_wait();
	mspi_write((unsigned char)(addr>>8));
	mspi_wait();
	mspi_write((unsigned char)(addr));
	mspi_wait();
}

//  make this a asynchorous version
_attribute_ram_code_ static void flash_wait_done()
{
	sleep_us(100);
	flash_send_cmd(FLASH_READ_STATUS_CMD);

	int i;
	for(i = 0; i < 10000000; ++i){
		if(!flash_is_busy()){
			break;
		}
	}
	mspi_high();
}
/**
 * @brief This function serves to erase a sector.
 * @param[in]   addr the start address of the sector needs to erase.
 * @return none
 */
_attribute_ram_code_ void flash_erase_sector(unsigned long addr){
	unsigned char r = irq_disable();

	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_SECT_ERASE_CMD);
	flash_send_addr(addr);
	mspi_high();
	flash_wait_done();

	irq_restore(r);
}
/**
 * @brief This function writes the buffer's content to a page.
 * @param[in]   addr the start address of the page
 * @param[in]   len the length(in byte) of content needs to write into the page
 * @param[in]   buf the start address of the content needs to write into
 * @return none
 */
_attribute_ram_code_ void flash_write_page(unsigned long addr, unsigned long len, unsigned char *buf){
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
 * @brief This function reads the content from a page to the buf.
 * @param[in]   addr the start address of the page
 * @param[in]   len the length(in byte) of content needs to read out from the page
 * @param[out]  buf the start address of the buffer
 * @return none
 */
_attribute_ram_code_ void flash_read_page(unsigned long addr, unsigned long len, unsigned char *buf){
	unsigned char r = irq_disable();


	flash_send_cmd(FLASH_READ_CMD);
	flash_send_addr(addr);

	mspi_write(0x00);		/* dummy,  to issue clock */
	mspi_wait();
	mspi_ctrl_write(0x0a);	/* auto mode */
	mspi_wait();
	/* get data */
	for(int i = 0; i < len; ++i){
		*buf++ = mspi_get();
		mspi_wait();
	}
	mspi_high();

	irq_restore(r);
}

#if 0
/**
 * @brief This function reads the status of flash.
 * @param[in]  none
 * @return none
 */
_attribute_ram_code_ unsigned char flash_read_status(void){
	unsigned char r = irq_disable();
	unsigned char status =0;
	flash_send_cmd(FLASH_READ_STATUS_CMD);
	/* get low 8 bit status */
	status = mspi_read();
	mspi_high();
	irq_restore(r);
	return status;
}
/**
 * @brief This function write the status of flash.
 * @param[in]  the value of status
 * @return status
 */
_attribute_ram_code_ unsigned char flash_write_status(unsigned char data)
{
	unsigned char r = irq_disable();
	unsigned char result;
	//int i;
	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_WRITE_STATUS_CMD);
	mspi_write(data);
	mspi_wait();
	mspi_high();
	flash_wait_done();

	sleep_us(100);
	flash_send_cmd(FLASH_READ_STATUS_CMD);

	result = mspi_read();
	mspi_high();

	irq_restore(r);
	return  result;
}

/**
 * @brief This function serves to erase a block(32k).
 * @param[in]   addr the start address of the block needs to erase.
 * @return none
 */
_attribute_ram_code_ void flash_erase_32kblock(unsigned int addr)
{
	unsigned char r = irq_disable();

	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_32KBLK_ERASE_CMD);
	flash_send_addr(addr);
	mspi_high();
	flash_wait_done();

    irq_restore(r);
}
/**
 * @brief This function serves to erase a block(64k).
 * @param[in]   addr the start address of the block needs to erase.
 * @return none
 */
_attribute_ram_code_ void flash_erase_64kblock(unsigned int addr)
{
	unsigned char r = irq_disable();

	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_64KBLK_ERASE_CMD);
	flash_send_addr(addr);
	mspi_high();
	flash_wait_done();

    irq_restore(r);
}
/***********************************
 * @brief  	Deep Power Down mode to put the device in the lowest consumption mode
 * 			it can be used as an extra software protection mechanism,while the device
 * 			is not in active use,since in the mode,  all write,Program and Erase commands
 * 			are ignored,except the Release from Deep Power-Down and Read Device ID(RDI)
 * 			command.This release the device from this mode
 * @param[in] none
 * @return none.
 */
_attribute_ram_code_ void flash_deep_powerdown(void)
{
	unsigned char r = irq_disable();

	flash_send_cmd(FLASH_POWER_DOWN);
	mspi_high();
	sleep_us(1);

    irq_restore(r);
}
/***********************************
 * @brief		The Release from Power-Down or High Performance Mode/Device ID command is a
 * 				Multi-purpose command.it can be used to release the device from the power-Down
 * 				State or High Performance Mode or obtain the devices electronic identification
 * 				(ID)number.Release from Power-Down will take the time duration of tRES1 before
 * 				the device will resume normal operation and other command are accepted.The CS#
 * 				pin must remain high during the tRES1(8us) time duration.
 * @param[in] none
 * @return none.
 */
_attribute_ram_code_ void flash_release_deep_powerdown(void)
{
	unsigned char r = irq_disable();

	flash_send_cmd(FLASH_POWER_DOWN_RELEASE);
	mspi_high();
	flash_wait_done();
	mspi_high();

    irq_restore(r);
}

/**
 * @brief This function serves to erase a page(256 bytes).
 * @param[in]   addr the start address of the page needs to erase.
 * @return none
 */
_attribute_ram_code_ void flash_erase_page(unsigned int addr)
{
	unsigned char r = irq_disable();

	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_PAGE_ERASE_CMD);
	flash_send_addr(addr);
	mspi_high();
	flash_wait_done();

    irq_restore(r);
}
/**
 * @brief This function serves to erase a page(256 bytes).
 * @param[in]   addr the start address of the page needs to erase.
 * @return none
 */
_attribute_ram_code_ void flash_erase_chip(void)
{
	unsigned char r = irq_disable();

	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_CHIP_ERASE_CMD);
	mspi_high();
	flash_wait_done();

    irq_restore(r);
}
#endif

/***********************************
 * @brief  read flash ID
 * @param[in] the buffer address to store the flash ID.
 * @return none.
 */
#if 0
_attribute_ram_code_ unsigned int flash_get_jedec_id(){
	unsigned char r = irq_disable();
	flash_send_cmd(FLASH_GET_JEDEC_ID);
	unsigned char manufacturer = mspi_read();
	unsigned char mem_type = mspi_read();
	unsigned char cap_id = mspi_read();
	mspi_high();
	irq_restore(r);
	return (unsigned int)((manufacturer << 24 | mem_type << 16 | cap_id));
}
#endif

_attribute_ram_code_ void flash_read_mid(unsigned char *buf){
	unsigned char j = 0;
	unsigned char r = irq_disable();
	flash_send_cmd(FLASH_GET_JEDEC_ID);
	mspi_write(0x00);		/* dummy,  to issue clock */
	mspi_wait();
	mspi_ctrl_write(0x0a);	/* auto mode */
	mspi_wait();

	for(j = 0; j < 3; ++j){
		*buf++ = mspi_get();
		mspi_wait();
	}
	mspi_high();
	irq_restore(r);
}

_attribute_ram_code_ void flash_read_uid(unsigned char idcmd,unsigned char *buf){

	unsigned char j = 0;
	unsigned char r = irq_disable();
	flash_send_cmd(idcmd);
	if(idcmd==FLASH_GD_PUYA_READ_UID_CMD)
	{
		flash_send_addr(0x00);
		mspi_write(0x00);		/* dummy,  to issue clock */
		mspi_wait();
	}
	else if (idcmd==FLASH_XTX_READ_UID_CMD)
	{
		flash_send_addr(0x80);
		mspi_write(0x00);		/* dummy,  to issue clock */
		mspi_wait();

	}
	mspi_write(0x00);		/* dummy,  to issue clock */
	mspi_wait();
	mspi_ctrl_write(0x0a);	/* auto mode */
	mspi_wait();

	for(j = 0; j < 16; ++j){
		*buf++ = mspi_get();
		mspi_wait();
	}
	mspi_high();
	irq_restore(r);
}
/**
 * @brief 		 This function serves to read flash mid and uid,and check the correctness of mid and uid.
 * @param[out]   flash_mid - Flash Manufacturer ID
 * @param[out]   flash_uid - Flash Unique ID
 * @return       0:error 1:ok

 */
_attribute_ram_code_ int flash_read_mid_uid_with_check( unsigned int *flash_mid ,unsigned char *flash_uid)
{
	  unsigned char no_uid[16]={0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01};
	  int i,f_cnt=0;
	  unsigned int mid;
	  flash_read_mid((unsigned char*)&mid);
	  mid = mid&0xffff;
	  *flash_mid  = mid;
	 /*
	   Flash Type    CMD        MID      Company
	   GD25LD40C 	 0x4b     0x60C8     GD
	   GD25LD05C  	 0x4b 	  0x60C8     GD
	   P25Q40L   	 0x4b     0x6085     PUYA
	   MD25D40DGIG	 0x4b     0x4051     GD
	   GD25D10C      0x4b     0x40C8     GD
	   PN25F04C      0x5a     0x311C     XTX
	 */
	  if( (mid == 0x60C8) || (mid == 0x6085) ||(mid == 0x4051)||(mid==0x40C8)){
		  flash_read_uid(FLASH_GD_PUYA_READ_UID_CMD,(unsigned char *)flash_uid);
	  }
	  else if(mid==0x311C){
		  flash_read_uid(FLASH_XTX_READ_UID_CMD	,(unsigned char *)flash_uid);
	  }
	  else{
		  return 0;
	  }
	  for(i=0;i<16;i++){
		if(flash_uid[i]==no_uid[i]){
			f_cnt++;
		}
	  }
	  if(f_cnt==16){//no uid flash
			  return 0;

	  }else{
		  return  1;
	  }
}
