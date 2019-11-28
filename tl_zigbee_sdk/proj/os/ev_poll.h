/********************************************************************************************************
 * @file     ev_poll.h
 *
 * @brief    Interface of Telink EV poll Module
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

#include "../common/types.h"
#include "user_config.h"


/** @addtogroup  TELINK_COMMON_MODULE TELINK Common Module
 *  @{
 */

/** @addtogroup  EV_SCHEDULE EV Schedule
 *  @{
 */

/** @addtogroup  EV_POLL EV Poll
 *  @{
 */
 
/** @defgroup EV_POLL_CONSTANT EV Poll Constants
 *  @{
 */



/** @} end of group EV_POLL_CONSTANT */


/** @defgroup EV_POLL_TYPE EV Poll Types
 *  @{
 */
 
/**
 *  @brief Definition for poll callback function 
 */
typedef void (*ev_poll_callback_t)(void);
 
/**
 *  @brief Definition for each poll item 
 */
typedef struct ev_poll_s {
    int                 valid;              //!< Indicate the poll task is used or not
    ev_poll_callback_t  cb;                 //!< Callback of the poll task
} ev_poll_t; 

/** @} end of group EV_POLL_TYPE */


/** @defgroup EV_POLL_FUNCTION EV Poll API
 *  @{
 */

 /**
  * @brief       Schedule a new priority task
  *
  * @param[in]   e   - Specified poll event ID
  * @param[in]   cb  - Pointer for the poll callback function
  *
  * @return      None
  */
void ev_on_poll(ev_poll_e e, ev_poll_callback_t cb);

 /**
  * @brief       Enable the specified poll event
  *
  * @param[in]   e - Specified poll event ID
  *
  * @return      Status
  */
void ev_enable_poll(ev_poll_e e);

 /**
  * @brief       Schedule a LOW Medium task
  *
  * @param[in]   e - Specified poll event ID
  *
  * @return      Status
  */
void ev_disable_poll(ev_poll_e e);

 /**
  * @brief       Schedule a Poll Task
  *
  * @param[in]   e    - Specified poll event ID
  * @param[in]   task - Pointer for the task callback function
  *
  * @return      Status
  */
#define EV_SCHEDULE_POLL(e, task) (ev_on_poll(e, task))

/**  @} end of group EV_POLL_FUNCTION */

/**  @} end of group EV_POLL */

/**  @} end of group EV_SCHEDULE */

/**  @} end of group TELINK_COMMON_MODULE */
