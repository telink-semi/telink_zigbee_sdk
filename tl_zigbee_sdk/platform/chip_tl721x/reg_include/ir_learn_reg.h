/********************************************************************************************************
 * @file    ir_learn_reg.h
 *
 * @brief   This is the header file for TL721X
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
#ifndef IR_LEARN_REG_H
#define IR_LEARN_REG_H
#include "soc.h"
/*******************************      ir-learn registers: 0x1404C0      ******************************/
#define IR_LEARN_BASE_ADDR (0x1404C0)

#define reg_ir_learn_fifo_addr          (IR_LEARN_BASE_ADDR + 0x0c)
#define reg_ir_learn_data_hword_buff(i) REG_ADDR16(IR_LEARN_BASE_ADDR + 0x0c + i * 2)
#define reg_ir_learn_data_word_buff     REG_ADDR32(IR_LEARN_BASE_ADDR + 0x0c)

#define reg_ir_learn_ctrl0 REG_ADDR8(IR_LEARN_BASE_ADDR)
enum
{
    FLD_IR_LEARN_EN         = BIT(0), /**< 1: enable, 0: disable. */
    FLD_IR_LEARN_RXFIFO_CLR = BIT(1), /**< 1: clear rxfifo */
};

/**
 * @brief define ir_learn control struct.
 * -# BIT[1]  ir mode:
 *  -# 1 After the module is enabled, the counter should enter the LOW state to start counting at the first falling edge
 *  -# 0 After the module is enabled, the counter shall enter the HIGH state and start counting at the first rising edge
 * -# BIT[2]:
 *  -# 1 invert ir-learn input signal
 *  -# 0 not invert ir-learn input signal
 * -# BIT[3]:
 *  -# 1 write high count to rx-fifo
 *  -# 0 not write high count to rx-fifo
 * -# BIT[4]:
 *  -# 1 write cycle cnt to rxfifo
 *  -# 0 not write cycle cnt to rxfifo
 * -# BIT[5]:
 *  -# 1 16bits
 *  -# 0 24bits
 * -# BIT[6]
 *  -# 1 disable timeout,
 *  -# 0 enable timeout
 * -# BIT[7]:
 *  -# 1 select analog signal as ir-learn input signal
 *  -# 0 select GPIO level as ir-learn input signal
 */
#define reg_ir_learn_ctrl1 REG_ADDR8(IR_LEARN_BASE_ADDR + 0x01)
enum
{
    FLD_IR_LEARN_MODE        = BIT(1),
    FLD_IR_LEARN_INV         = BIT(2),
    FLD_IR_LEARN_HIGH_WR_EN  = BIT(3),
    FLD_IR_LEARN_CYCLE_WR_EN = BIT(4),
    FLD_IR_LEARN_MODE_2BYTE  = BIT(5),
    FLD_IR_LEARN_NO_TIMEOUT  = BIT(6),
    FLD_IR_LEARN_INPUT_SEL   = BIT(7),
};

#define reg_ir_learn_timeout_byte(i) REG_ADDR8(IR_LEARN_BASE_ADDR + 0x02 + (i))

/**
 * @brief ir_learn irq mask.
 * -# BIT[0]: An interrupt is generated when the ir learn module's level status changes from high to low.
 * -# BIT[1]: An interrupt is generated when the ir learn module's level status changes from low to high.
 * -# BIT[2]: An timeout interrupt is generated when the ir module is at a low level and the count value reaches the preset timeout threshold, storing the count value into the cycle register.
 * -# BIT[3]: An rx_fifo interrupt is generated when the threshold of the set fifo_level is reached.
 */
#define reg_ir_learn_irq_mask   REG_ADDR8(IR_LEARN_BASE_ADDR + 0x05)

/**
 * @brief ir_learn irq status.
 * BIT[0]: If this is set 1 Indicates that a high interrupt has been generated. You can manually write 1 to clear the interrupt flag.
 * BIT[1]: If this is set 1 Indicates that a cycle interrupt has been generated. You can manually write 1 to clear the interrupt flag.
 * BIT[2]: If this is set 1 Indicates that a timeout interrupt has been generated. You can manually write 1 to clear the interrupt flag.
 * BIT[3]: If this is set 1 Indicates that a rx buff interrupt has been generated. You can manually write 1 to clear the interrupt flag.
 */
#define reg_ir_learn_irq_status REG_ADDR8(IR_LEARN_BASE_ADDR + 0x06)
typedef enum
{
    FLD_IR_LEARN_HIGH_IRQ    = BIT(0), /**< W1C. */
    FLD_IR_LEARN_CYCLE_IRQ   = BIT(1), /**< W1C. */
    FLD_IR_LEARN_TIMEOUT_IRQ = BIT(2), /**< W1C. */
    FLD_IR_LEARN_RXFIFO_IRQ  = BIT(3), /**< W1C. */
} ir_learn_irq_e;

#define reg_ir_learn_fifo_ctrl REG_ADDR8(IR_LEARN_BASE_ADDR + 0x07)
typedef enum
{
    FLD_IR_LEARN_FIFO_THRES    = BIT_RNG(0, 2), /* unit is bytes */
    FLD_IR_LEARN_FIFO_AUTO_CLR = BIT(3),
    FLD_IR_LEARN_PEM_EVENT_EN  = BIT(4),
    FLD_IR_LEARN_PEM_TASK0_EN  = BIT(5),
    FLD_IR_LEARN_PEM_TASK1_EN  = BIT(6),
} ir_learn_fifo_ctrl_e;

#define reg_ir_learn_fifo_state REG_ADDR8(IR_LEARN_BASE_ADDR + 0x08)
typedef enum
{
    FLD_IR_LEARN_FIFO_RX_CNT   = BIT_RNG(0, 3), /* unit is bytes */
    FLD_IR_LEARN_FIFO_RX_FULL  = BIT(4),
    FLD_IR_LEARN_FIFO_RX_EMPTY = BIT(5),
    FLD_IR_LEARN_FIFO_RX_DONE  = BIT(6),
} ir_learn_fifo_sta_e;

#endif
