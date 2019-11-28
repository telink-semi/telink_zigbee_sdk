/********************************************************************************************************
 * @file     cfs-coffee-arch.h
 *
 * @brief
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

#ifndef CFS_COFFEE_ARCH_H_
#define CFS_COFFEE_ARCH_H_


#include "cfs-coffee.h"
#include "drv_flash.h"


/** Logical sector size */
#define COFFEE_SECTOR_SIZE     4096

/** Logical page size */
#define COFFEE_PAGE_SIZE       256

/** Start offset of the file system */
#define COFFEE_START           0x30000

/** Total size in bytes of the file system */
#define COFFEE_SIZE            (1024*64)

/** Maximal filename length */
#define COFFEE_NAME_LENGTH     40

/** Number of file cache entries */
#define COFFEE_MAX_OPEN_FILES  5

/** Number of file descriptor entries */
#define COFFEE_FD_SET_SIZE     5

/** Maximal amount of log table entries read in one batch */
#define COFFEE_LOG_TABLE_LIMIT 16

/** Default reserved file size */
#define COFFEE_DYN_SIZE        (COFFEE_SECTOR_SIZE - 50)

/** Default micro-log size */
#define COFFEE_LOG_SIZE        (4 * COFFEE_PAGE_SIZE)

/** Whether Coffee will use micro logs */
#define COFFEE_MICRO_LOGS      1

/** Whether files are expected to be appended to only */
#define COFFEE_APPEND_ONLY     0


/** Erase */
#define COFFEE_ERASE(sector) \
  flash_erase(COFFEE_START + (sector) * COFFEE_SECTOR_SIZE)
/** Write */
#define COFFEE_WRITE(buf, size, offset) \
  cfs_flash_write(COFFEE_START + (offset),(size),(buf))
/** Read */
#define COFFEE_READ(buf, size, offset) \
  cfs_flash_read(COFFEE_START + (offset),(size),(buf))


#endif
