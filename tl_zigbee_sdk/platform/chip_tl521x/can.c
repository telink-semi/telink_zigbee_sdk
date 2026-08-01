/********************************************************************************************************
 * @file    can.c
 *
 * @brief   This is the source file for tl521x
 *
 * @author  Driver Group
 * @date    2026
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "can.h"
static volatile mailbox_allocate_t* can[1]={(mailbox_allocate_t*)0x80144080};
static volatile can_rx_mb_individual_mask_t* can_rx_mb_im[1] = {(can_rx_mb_individual_mask_t*)0x80144880};
static volatile can_enhanced_rxfifo_filter_t* can_enhanced_rxfifo_filter[1] = {(can_enhanced_rxfifo_filter_t*)(unsigned int*)(0x80144000+0x3000)};
dma_config_t can_rx_dma_config[1] = {
    {
        .dst_req_sel = 0, // tx req
        .src_req_sel = DMA_REQ_CAN0_RX,
        .dst_addr_ctrl = DMA_ADDR_INCREMENT,
        .src_addr_ctrl = DMA_ADDR_FIX,
        .dstmode = DMA_NORMAL_MODE,
        .srcmode = DMA_HANDSHAKE_MODE,
        .dstwidth = DMA_CTR_WORD_WIDTH, // must be word
        .srcwidth = DMA_CTR_WORD_WIDTH, ////must be word
        .src_burst_size = 0,
        .read_num_en = 0,
        .priority = 0,
        .write_num_en = 1,
        .auto_en = 0, // must be 0
    },
};

/**
 *  @brief      This function serves to set pin.
 *  @param[in]  chn         - can_chn_e
 *  @param[in]  tx_pin      - the tx pin
 *  @param[in]  rx_pin      - the rx pin
 *  @return     none
 */
void can_set_pin(can_chn_e chn, gpio_func_pin_e tx_pin, gpio_func_pin_e rx_pin)
{
    // When the pad is configured with mux input and a pull-up resistor is required, gpio_input_en needs to be placed before gpio_function_dis,
    // otherwise first set gpio_input_disable and then call the mux function interface,the mux pad will misread the short low-level timing.confirmed by minghai.20210709.
    if (tx_pin != GPIO_NONE_PIN)
    {
        switch (chn)
        {
        case CAN0:
            gpio_set_mux_function(tx_pin, CAN0_TX);
            break;
        default:
            break;
        }
        gpio_function_dis((gpio_pin_e)tx_pin);
    }
    if (rx_pin != GPIO_NONE_PIN)
    {
        gpio_input_en((gpio_pin_e)rx_pin);
        switch (chn)
        {
        case CAN0:
            gpio_set_mux_function(rx_pin, CAN0_RX_I);
            break;
        default:
            break;
        }
        gpio_function_dis((gpio_pin_e)rx_pin);
    }
}

/*!
 * @brief This function use to calculates the Classical CAN timing values according to the given bit rate.
 * @param[in]  chn           - can_chn_e
 * @param[in]  bit_rate      - The classical CAN speed in bps defined by user, should be less than or equal to 1Mbps.
 * @param[in]  pclk          - The Source clock frequency in Hz.
 * @param[in]  timing_config - TimingConfig Pointer to the CAN timing configuration structure.
 * @return     1- if timing configuration found, 0- if failed to find configuration.
 */
_Bool can_cal_timing_config(can_chn_e chn,unsigned int bit_rate, unsigned int pclk,can_timing_config_t* timing_config,float sampling_rate){
     (void)chn;
    unsigned int  best_prescaler = 0;
    unsigned char  best_ts1 = 0;
    unsigned char  best_ts2 = 0;

    for (unsigned char ts1_val = MAX_PROPSEG_MAX_PSEG1; ts1_val >= MIN_TIME_SEGMENT1; ts1_val--) {
        for (unsigned char ts2_val = MAX_PSEG2; ts2_val >= MIN_TIME_SEGMENT2; ts2_val--) {
            unsigned char time_segments_total = ts1_val + ts2_val + 4;
            // Prescaler
            unsigned int calculated_prescaler = pclk / (bit_rate * time_segments_total) -1;
            // actual rate
            unsigned int actual_baudrate = pclk / ((calculated_prescaler+1) * time_segments_total);
            // err
            unsigned int error;
            if(bit_rate>actual_baudrate){
                error = bit_rate - actual_baudrate;
            }else{
                error = actual_baudrate -bit_rate ;
            }
            float timing_total= time_segments_total;
            float timing_seg1 = ts1_val+3;
            float caiyang_lv = timing_seg1/timing_total;
            if ((error ==0)&&(calculated_prescaler<MAX_PRESDIV)&&(caiyang_lv>=sampling_rate)&&(time_segments_total>=CAN_MIN_TIME_QUANTA)) {
                best_prescaler = calculated_prescaler;
                best_ts1 = ts1_val;
                best_ts2 = ts2_val;
                timing_config->phase_seg2 = best_ts2;
                timing_config->presdiv = best_prescaler;
                if(best_ts1>(MAX_PSEG1+MIN_TIME_SEGMENT2-1)){
                    timing_config->prop_seg = best_ts1-7;
                    timing_config->phase_seg1 =7;
                }else{
                    timing_config->prop_seg= 1;
                    timing_config->phase_seg1 =best_ts1-1;
                }
                return 0;
            }
        }
    }

  return 1;

}

static _Bool canfd_cal_fdcbt_header_timing_config(can_fd_baud_rate_t* rate_cfg,unsigned int pclk,unsigned int pclk_div,can_timing_config_t* timing_config,float Sampling_rate){
   unsigned char  best_ts1 = 0;
   unsigned char  best_ts2 = 0;
   unsigned int   min_error = 0;
   for (unsigned char ts2_val = MAX_EPSEG2; ts2_val >= MIN_TIME_SEGMENT2; ts2_val--) {
         for (unsigned char ts1_val = (MAX_EPROPSEG+MAX_EPSEG1); ts1_val >= MIN_TIME_SEGMENT1; ts1_val--) {

           unsigned char time_segments_total = ts1_val + ts2_val + 4;
           // actual rate
           unsigned int actual_baudrate = pclk / ((pclk_div+1) * time_segments_total);
           // err
           unsigned int error;
           if(rate_cfg->bit_rate>actual_baudrate){
            error = rate_cfg->bit_rate - actual_baudrate;
           }else{
            error = actual_baudrate -rate_cfg->bit_rate ;
           }
           float timing_total= time_segments_total;
           float timing_seg1 = ts1_val+3;
           float caiyang_lv = timing_seg1/timing_total;
           if ((error ==min_error)&&(caiyang_lv>=Sampling_rate)&&(time_segments_total>=CBT_MIN_TIME_QUANTA)) {
               best_ts1 = ts1_val;
               best_ts2 = ts2_val;
               timing_config->phase_seg2 = best_ts2;
               timing_config->presdiv = pclk_div;
               if((best_ts1/2)>MAX_EPSEG1){
                    timing_config->prop_seg = best_ts1-MAX_EPSEG1;
                    timing_config->phase_seg1 =MAX_EPSEG1;
               }else{
                    timing_config->prop_seg= best_ts1/2;
                    timing_config->phase_seg1 =best_ts1-best_ts1/2;
               }
               return 0;
           }
       }
   }
   return 1;

}


static _Bool canfd_cal_fdcbt_data_timing_config(can_fd_baud_rate_t* rate_cfg,unsigned int pclk,can_timing_config_t* timing_config,float Sampling_rate){
    static unsigned int   best_prescaler = 0;
    static unsigned char  best_ts1 = 0;
    static unsigned char  best_ts2 = 0;
    for (unsigned char ts2_val = MAX_FPSEG2; ts2_val >= MIN_TIME_SEGMENT2; ts2_val--) {
         for (unsigned char ts1_val = (MAX_FPROPSEG+MAX_FPSEG1); ts1_val >= MIN_TIME_SEGMENT1; ts1_val--) {
            unsigned int time_segments_total = ts1_val + ts2_val + 3;

            // Prescaler
            unsigned int calculated_prescaler = pclk / (rate_cfg->bit_rate_fd * time_segments_total) -1;
            // actual_baudrate
            unsigned int actual_baudrate = pclk / ((calculated_prescaler+1) * time_segments_total);
            // err
            unsigned int error;
            if(rate_cfg->bit_rate_fd>actual_baudrate){
                error = rate_cfg->bit_rate_fd - actual_baudrate;
            }else{
                error = actual_baudrate -rate_cfg->bit_rate_fd ;
            }

            float timing_total= time_segments_total;
            float timing_seg1 = ts1_val+2;
            float caiyang_lv = timing_seg1/timing_total;

            if ((error ==0)&&(calculated_prescaler<MAX_FPRESDIV)&&(caiyang_lv>=Sampling_rate)&&(time_segments_total>=FDCBT_MIN_TIME_QUANTA)&&(ts1_val<=(MAX_TIME_FD_SEGMENT1*2)&&(ts2_val<=MAX_TIME_FD_SEGMENT2))) {
                best_prescaler = calculated_prescaler;
                best_ts1 = ts1_val;
                best_ts2 = ts2_val;

                if(!canfd_cal_fdcbt_header_timing_config(rate_cfg,pclk,best_prescaler,timing_config,Sampling_rate)){

                    timing_config->fd_phase_seg2 = best_ts2;
                    timing_config->fd_presdiv = best_prescaler;

                    if((best_ts1/2)>MAX_FPSEG1){
                        timing_config->fd_prop_seg = best_ts1-MAX_FPSEG1;
                        timing_config->fd_phase_seg1 =MAX_FPSEG1;
                    }else{
                        timing_config->fd_prop_seg= best_ts1/2;
                        timing_config->fd_phase_seg1 =best_ts1-best_ts1/2;
                    }

                    return 0;
                }
            }
        }
    }

  return 1;
}

static _Bool canfd_cal_edcbt_header_timing_config(can_fd_baud_rate_t* rate_cfg,unsigned int pclk,unsigned int pclk_div,can_timing_config_t* timing_config,float Sampling_rate){

   unsigned char  best_ts1 = 0;
   unsigned char  best_ts2 = 0;
   unsigned int   min_error = 0;

   for (volatile unsigned char ts1_val = (MAX_NTSEG1); ts1_val >= MIN_TIME_SEGMENT1; ts1_val--) {
       for (volatile unsigned char ts2_val = MAX_NTSEG2; ts2_val >= MIN_TIME_SEGMENT2; ts2_val--) {
           unsigned int time_segments_total = ts1_val + ts2_val + 3;

           // actual_baudrate
           unsigned int actual_baudrate = pclk / ((pclk_div+1) * time_segments_total);
           //err
           unsigned int error;
           if(rate_cfg->bit_rate>actual_baudrate){
            error = rate_cfg->bit_rate - actual_baudrate;
           }else{
            error = actual_baudrate -rate_cfg->bit_rate ;
           }
           float timing_total= time_segments_total;
           float timing_seg1 = ts1_val+2;
           float caiyang_lv = timing_seg1/timing_total;
           if ((error ==min_error)&&(caiyang_lv>=Sampling_rate)&&(time_segments_total>=ENCBT_MIN_TIME_QUANTA)) {
               best_ts1 = ts1_val;
               best_ts2 = ts2_val;
               timing_config->phase_seg2 = best_ts2;
               timing_config->presdiv = pclk_div;
               timing_config->phase_seg1 = best_ts1;
               return 0;
           }
       }
   }
   return 1;

}

static _Bool canfd_cal_edcbt_data_timing_config(can_fd_baud_rate_t* rate_cfg,unsigned int pclk,can_timing_config_t* timing_config,float Sampling_rate){
    unsigned int   best_prescaler = 0;
    unsigned char  best_ts1 = 0;
    unsigned char  best_ts2 = 0;

    for (unsigned char ts1_val = (MAX_DTSEG1); ts1_val >= MIN_TIME_SEGMENT1; ts1_val--) {
        for (unsigned char ts2_val = MAX_DTSEG2; ts2_val >= MIN_TIME_SEGMENT2; ts2_val--) {
            unsigned int time_segments_total = ts1_val + ts2_val + 3;
            // Prescaler
            unsigned int calculated_prescaler = pclk / (rate_cfg->bit_rate_fd * time_segments_total) -1;
            // actual_baudrate
            unsigned int actual_baudrate = pclk / ((calculated_prescaler+1) * time_segments_total);
            // error
            unsigned int error;
            if(rate_cfg->bit_rate_fd>actual_baudrate){
                error = rate_cfg->bit_rate_fd - actual_baudrate;
            }else{
                error = actual_baudrate -rate_cfg->bit_rate_fd ;
            }
            float timing_total= time_segments_total;
            float timing_seg1 = ts1_val+2;
            float caiyang_lv = timing_seg1/timing_total;
            if ((error ==0)&&(calculated_prescaler<MAX_EDPRESDIV)&&(caiyang_lv>=Sampling_rate)&&(time_segments_total>=EDCBT_MIN_TIME_QUANTA)) {
                best_prescaler = calculated_prescaler;
                best_ts1 = ts1_val;
                best_ts2 = ts2_val;
                if(!canfd_cal_edcbt_header_timing_config(rate_cfg,pclk,best_prescaler,timing_config,Sampling_rate)){
                    timing_config->fd_phase_seg2 = best_ts2;
                    timing_config->fd_presdiv = best_prescaler;
                    timing_config->fd_phase_seg1 = best_ts1;
                    return 0;
                }
            }
        }
    }

  return 1;
}

/*!
 * @brief This function use to calculates the CAN FD timing values according to the given nominal phase bit rate and data phase bit rate
 * @param[in]  chn           - can_chn_e
 * @param[in]  rate_cfg      - the bit rate cfg
 * @param[in]  pclk          - The Source clock frequency in Hz.
 * @param[in]  timing_config - TimingConfig Pointer to the CAN timing configuration structure.
 * @return     1- if timing configuration found, 0- if failed to find configuration.
 */
_Bool canfd_cal_timing_config(can_chn_e chn,can_fd_baud_rate_t* rate_cfg,unsigned int pclk,can_timing_config_t* timing_config,float sampling_rate){
    (void)chn;
    if(rate_cfg->canfd_timing_mode == CANFD_EDCBT){
        timing_config->canfd_timing_mode = CANFD_EDCBT;
        if(canfd_cal_edcbt_data_timing_config(rate_cfg,pclk,timing_config,sampling_rate)){
            return 1;
        }


   }else if(rate_cfg->canfd_timing_mode == CANFD_FDCBT){
       timing_config->canfd_timing_mode = CANFD_FDCBT;
      if( canfd_cal_fdcbt_data_timing_config(rate_cfg,pclk,timing_config,sampling_rate)){
          return 1;
      }

   }
    return 0;
}



/*!
 * @brief This function initializes the CAN module.
 * @param[in]  chn            - can_chn_e
 * @param[in]  timing_config  - TimingConfig Pointer to the CAN timing configuration structure.
 * @param[in]  mb_cfg         - can_mb_cfg_t. mailbox space allocation refer to mailbox_allocate_t.
 *                              mb_cfg.mb_max:set the maximum number based on application requirements.
 *                              mb_cfg.mb_data_size : CAN_8BYTE_PERMB
 * @param[in]  irmq_en        - 1:enable individual Rx masking and queue feature.
 *                              0:disable individual Rx masking and queue feature,the rx mailbox and legacy rxfifo select global mask.
 * @param[in]  tx_is_lprio_en - 1:the highest-priority mailbox first
 *                              0:the lowest-number mailbox first
 * @return     none
 */
void can_init(can_chn_e chn,can_timing_config_t* timing_config,can_mb_cfg_t* mb_cfg,unsigned char irmq_en,unsigned char tx_is_lprio_en){
    //bit rate
    reg_can_ctrl1_0(chn)  =  (reg_can_ctrl1_0(chn)&(~FLD_CAN_PROPSEG))|(timing_config->prop_seg);
    reg_can_ctrl1_2(chn)  =  (reg_can_ctrl1_2(chn)&(~FLD_CAN_PSEG1))|((timing_config->phase_seg1)<<3);
    reg_can_ctrl1_2(chn)  =  (reg_can_ctrl1_2(chn)&(~FLD_CAN_PSEG2))|(timing_config->phase_seg2);
    reg_can_ctrl1_3(chn) = (timing_config->presdiv);
    reg_can_ctrl1_2(chn)  =  (reg_can_ctrl1_2(chn)&(~FLD_CAN_RJW))|((0x01)<<6);
    //mb_cfg
    reg_can_mcr0(chn) = (reg_can_mcr0(chn) &(~FLD_CAN_MAXMB))|(mb_cfg->mb_max - 1);
    reg_can_fdctrl_2(chn) = (reg_can_fdctrl_2(chn) &(~ FLD_CAN_MBDSR0))|(mb_cfg->mb_data_size);
    reg_can_fdctrl_2(chn) = (reg_can_fdctrl_2(chn) &(~ FLD_CAN_MBDSR1))|((mb_cfg->mb_data_size)<<3);
    reg_can_fdctrl_2(chn) = (reg_can_fdctrl_2(chn) &(~ FLD_CAN_MBDSR2))|((mb_cfg->mb_data_size)<<6);
    reg_can_fdctrl_3(chn) = (reg_can_fdctrl_2(chn) &(~ FLD_CAN_MBDSR3))|((mb_cfg->mb_data_size)<<1);

    for(unsigned char i=0;i<mb_cfg->mb_max;i++){
        can[chn]->MB[i].cs =0;
    }
    //irmq
    reg_can_mcr2(chn) = (irmq_en) ? (reg_can_mcr2(chn) | FLD_CAN_IRMQ) : (reg_can_mcr2(chn) & ~FLD_CAN_IRMQ);
    //tx priority
    if(tx_is_lprio_en == 1){
       reg_can_ctrl1_0(chn) &= ~FLD_CAN_LBUF;
       reg_can_mcr1(chn) |= FLD_CAN_LPRIO_EN;
    }else{
       reg_can_ctrl1_0(chn) |= FLD_CAN_LBUF;
       reg_can_mcr1(chn) &= ~FLD_CAN_LPRIO_EN;
    }
    //the default enable configuration
    // CAN_CTRL2_EACEN_MASK
    reg_can_ctrl2_2(chn) |= FLD_CAN_EACEN;
    //CAN_CTRL2_WRMFRZ_MASK
     reg_can_ctrl2_3(chn)|= FLD_CAN_WR_MEM_FZ;
    //  /* Reset CTRL1 and CTRL2 register, default to enable SMP feature which enable three sample point to determine the
    //  * received bit's value of the. */
    // base->CTRL1 = CAN_CTRL1_SMP_MASK;
     reg_can_ctrl1_0(chn)|=FLD_CAN_SMP;

    ///* Enable Timer Sync? */
    //ctrl1Temp = (pConfig->enableTimerSync) ? (ctrl1Temp | CAN_CTRL1_TSYN_MASK) : (ctrl1Temp & ~CAN_CTRL1_TSYN_MASK);
     reg_can_ctrl1_0(chn) |= FLD_CAN_TSYN;

     reg_can_mcr2(chn) |= FLD_CAN_WRN_EN;
//   reg_can_mcr1(chn) |= FLD_CAN_DMA;

     //the default disable configuration
     can_self_reception_disable(chn);
     can_listen_only_disable(chn);
     can_loop_back_disable(chn);

}

static unsigned int canfd_get_mailbox_offset(can_mb_data_size_e data_size,unsigned char mb_index){
    unsigned int offset=0;
    if(data_size == CAN_8BYTE_PERMB){
        offset = (((unsigned int)mb_index / 32) * 512 + ((unsigned int)mb_index % 32) * 16);
    }else if(data_size == CAN_16BYTE_PERMB){
        offset = (((unsigned int)mb_index / 21) * 512 + ((unsigned int)mb_index % 21) * 24);
    }else if(data_size == CAN_32BYTE_PERMB){
        offset = (((unsigned int)mb_index / 12) * 512 + ((unsigned int)mb_index % 12) * 40);
    }else if(data_size == CAN_64BYTE_PERMB){
        offset = (((unsigned int)mb_index / 7) * 512 + ((unsigned int)mb_index % 7) * 72);
    }
    /* To get the dword aligned offset, need to divide by 4. */
   offset = offset / 4;
   return offset;
}

/*!
 * @brief This function initializes the CAN FD module.
 * @param[in]  chn            - can_chn_e
 * @param[in]  timing_config  - TimingConfig Pointer to the CAN timing configuration structure.
 * @param[in]  mb_cfg         - can_mb_cfg_t. mailbox space allocation refer to mailbox_allocate_t.
 *                              mb_cfg.mb_max:set the maximum number based on application requirements.
 *                              mb_cfg.mb_data_size : CAN_64BYTE_PERMB
 * @param[in]  brs            - the nominal phase bit rate and data phase bit rate switch or not.
 * @param[in]  irmq_en        - 1:enable individual Rx masking and queue feature.
 *                              0:disable individual Rx masking and queue feature,the rx mailbox and legacy rxfifo select global mask.
 * @param[in]  tx_is_lprio_en - 1:the highest-priority mailbox first
 *                              0:the lowest-number mailbox first
 * @return     none
 */
void canfd_init(can_chn_e chn,can_timing_config_t* timing_config,can_mb_cfg_t* mb_cfg, unsigned char brs,unsigned char irmq_en,unsigned char tx_is_lprio_en){

    //bit rate
    if(timing_config->canfd_timing_mode == CANFD_EDCBT){
        reg_can_ctrl2_1(chn) |=FLD_CAN_BIT_TIMING_EXPEN;
        //header
        reg_can_encbt_0(chn)  = timing_config->phase_seg1;
        reg_can_encbt_1(chn)  =  (reg_can_encbt_1(chn)&(~FLD_CAN_NTSEG2_0))|(((timing_config->phase_seg2)&0xf)<<4);
        reg_can_encbt_2(chn)  =  (reg_can_encbt_2(chn)&(~FLD_CAN_NTSEG2_1))|(((timing_config->phase_seg2)>>4));
        reg_can_eprs_0(chn) = (timing_config->presdiv);
        reg_can_encbt_2(chn)  =  (reg_can_encbt_2(chn)&(~FLD_CAN_NRJW_0))|(((0x01)&0x03)<<6);
        reg_can_encbt_3(chn)  =  (reg_can_encbt_3(chn)&(~FLD_CAN_NRJW_1))|(((0x01)>>2&0x1f));

        //data
        reg_can_edcbt_0(chn)  =  (reg_can_edcbt_0(chn)&(~FLD_CAN_DTSEG1))|((timing_config->fd_phase_seg1));
        reg_can_edcbt_1(chn)  =  (reg_can_edcbt_1(chn)&(~FLD_CAN_DTSEG2))|((timing_config->fd_phase_seg2)<<4);
        reg_can_eprs_2(chn) = (timing_config->fd_presdiv);

        reg_can_edcbt_2(chn)  =  (reg_can_edcbt_2(chn)&(~FLD_CAN_DRJW_0))|(((0x01)&0x03)<<6);
        reg_can_edcbt_3(chn)  =  (reg_can_edcbt_3(chn)&(~FLD_CAN_DRJW_1))|(((0x01)>>2&0x3));



    }else if(timing_config->canfd_timing_mode == CANFD_FDCBT){
        //header
        reg_can_cbt_1(chn)  =  (reg_can_cbt_1(chn)&(~FLD_CAN_EPROPSEG))|((timing_config->prop_seg)<<2);
        reg_can_cbt_0(chn)  =  (reg_can_cbt_0(chn)&(~FLD_CAN_EPSEG1_1))|(((timing_config->phase_seg1)&0x7)<<5);
        reg_can_cbt_1(chn)  =  (reg_can_cbt_1(chn)&(~FLD_CAN_EPSEG1_2))|((((timing_config->phase_seg1)>>3)&0x3));
        reg_can_cbt_0(chn)  =  (reg_can_cbt_0(chn)&(~FLD_CAN_EPSEG2))|(timing_config->phase_seg2);

        reg_can_cbt_2(chn)  =  (reg_can_cbt_2(chn)&(~FLD_CAN_EPRESDIV_1))|(((timing_config->presdiv)&0x7)<<5);
        reg_can_cbt_3(chn)  =  (reg_can_cbt_3(chn)&(~FLD_CAN_EPRESDIV_2))|((((timing_config->presdiv)>>3)));

        reg_can_cbt_2(chn)  =  (reg_can_cbt_2(chn)&(~FLD_CAN_ERJW))|(0x01);


        //data
        reg_can_fdcbt_1(chn)  =  (reg_can_fdcbt_1(chn)&(~FLD_CAN_FPROPSEG))|((timing_config->fd_prop_seg)<<2);
        reg_can_fdcbt_0(chn)  =  (reg_can_fdcbt_0(chn)&(~FLD_CAN_FPSEG1))|((timing_config->fd_phase_seg1)<<5);
        reg_can_fdcbt_0(chn)  =  (reg_can_fdcbt_0(chn)&(~FLD_CAN_FPSEG2))|(timing_config->fd_phase_seg2);

        reg_can_fdcbt_2(chn)  =  (reg_can_fdcbt_2(chn)&(~FLD_CAN_FPRESDIV_0))|(((timing_config->fd_presdiv) &0xf)<<4);
        reg_can_fdcbt_3(chn)  =  (reg_can_fdcbt_3(chn)&(~FLD_CAN_FPRESDIV_1))|((((timing_config->fd_presdiv)>>4 )&0x1f));

        reg_can_fdcbt_2(chn)  =  (reg_can_fdcbt_2(chn)&(~FLD_CAN_FRJW))|(0x01);

        reg_can_cbt_3(chn) |=FLD_CAN_BTF;
    }

    //brs
    if(brs){
        reg_can_fdctrl_3(chn) |=FLD_CAN_FDRATE;
    }else{
        reg_can_fdctrl_3(chn) &= ~FLD_CAN_FDRATE;
    }

    if(brs){
        //transceiver delay compensation enable
        if(timing_config->canfd_timing_mode == CANFD_EDCBT){
            reg_can_etdc_3(chn) |=FLD_CAN_ETDCEN;
            reg_can_etdc_3(chn) &= ~FLD_CAN_TDMDIS;
            if((( timing_config->fd_phase_seg1+2)*(timing_config->fd_presdiv+1))< 0x7f){
                reg_can_etdc_2(chn) = (( timing_config->fd_phase_seg1+2)*(timing_config->fd_presdiv+1));
            }else{
                reg_can_etdc_2(chn) = 0x7f;
            }
        }else if(timing_config->canfd_timing_mode == CANFD_FDCBT){
            reg_can_fdctrl_1(chn) |=FLD_CAN_TDCEN;
            reg_can_etdc_3(chn) |= FLD_CAN_TDMDIS;
            if(((timing_config->fd_prop_seg + timing_config->fd_phase_seg1+2)*(timing_config->fd_presdiv+1))< 0x1f){
                reg_can_fdctrl_1(chn) = (reg_can_fdctrl_1(chn)&(~FLD_CAN_TDCOFF))|((timing_config->fd_prop_seg + timing_config->fd_phase_seg1+2)*(timing_config->fd_presdiv+1));
            }else{
                reg_can_fdctrl_1(chn) = (reg_can_fdctrl_1(chn)&(~FLD_CAN_TDCOFF))|(0x1f);
            }
        }
    }

    //enable canfd
    reg_can_mcr1(chn) |= FLD_CAN_FDEN;

    reg_can_ctrl2_1(chn)|=FLD_CAN_ISO_CAN_FD_EM;
    //mb cfg
    reg_can_mcr0(chn) = (reg_can_mcr0(chn) &(~FLD_CAN_MAXMB))|(mb_cfg->mb_max - 1);
    reg_can_fdctrl_2(chn) = (reg_can_fdctrl_2(chn) &(~ FLD_CAN_MBDSR0))|(mb_cfg->mb_data_size);
    reg_can_fdctrl_2(chn) = (reg_can_fdctrl_2(chn) &(~ FLD_CAN_MBDSR1))|((mb_cfg->mb_data_size)<<3);
    reg_can_fdctrl_2(chn) = (reg_can_fdctrl_2(chn) &(~ FLD_CAN_MBDSR2))|((mb_cfg->mb_data_size)<<6);
    reg_can_fdctrl_3(chn) = (reg_can_fdctrl_2(chn) &(~ FLD_CAN_MBDSR3))|((mb_cfg->mb_data_size)<<1);

    //clear all mb cs to zero

    volatile unsigned int *mb_addr = &(can[chn]->MB[0].cs);
    for(unsigned char i=0;i<mb_cfg->mb_max;i++){
        unsigned int offset = canfd_get_mailbox_offset(mb_cfg->mb_data_size,i);
        volatile unsigned int *mb_addr_i = (volatile unsigned int *)(mb_addr + offset);
        *mb_addr_i = 0;
    }
    //irmq
    reg_can_mcr2(chn) = (irmq_en) ? (reg_can_mcr2(chn) | FLD_CAN_IRMQ) : (reg_can_mcr2(chn) & ~FLD_CAN_IRMQ);

    //tx priority
    if(tx_is_lprio_en == 1){
       reg_can_ctrl1_0(chn) &= ~FLD_CAN_LBUF;
       reg_can_mcr1(chn) |= FLD_CAN_LPRIO_EN;
    }else{
       reg_can_ctrl1_0(chn) |= FLD_CAN_LBUF;
       reg_can_mcr1(chn) &= ~FLD_CAN_LPRIO_EN;
    }

    //the default enable configuration
    // CAN_CTRL2_EACEN_MASK
    reg_can_ctrl2_2(chn) |= FLD_CAN_EACEN;
    //CAN_CTRL2_WRMFRZ_MASK
     reg_can_ctrl2_3(chn)|= FLD_CAN_WR_MEM_FZ;
    /* Clear SMP bit when CAN FD is enabled (CAN FD only can use one regular sample point plus one optional secondary
     * sampling point). */
   // base->CTRL1 &= ~CAN_CTRL1_SMP_MASK;
     reg_can_ctrl1_0(chn)&= ~FLD_CAN_SMP;

    ///* Enable Timer Sync? */
    //ctrl1Temp = (pConfig->enableTimerSync) ? (ctrl1Temp | CAN_CTRL1_TSYN_MASK) : (ctrl1Temp & ~CAN_CTRL1_TSYN_MASK);
     reg_can_ctrl1_0(chn) |= FLD_CAN_TSYN;

     reg_can_mcr2(chn) |= FLD_CAN_WRN_EN;
    // reg_can_mcr1(chn) |= FLD_CAN_DMA;

    //the default disable configuration
     can_self_reception_disable(chn);
     can_listen_only_disable(chn);
     can_loop_back_disable(chn);
}

/*!
 * @brief Check if Message Buffer is occupied by Rx FIFO.
 * @param chn  - can_chn_e.
 * @param mb_index -  The CAN Message Buffer index.
 * @return TRUE: if the index MB is occupied by Rx FIFO, FALSE: if the index MB not occupied by Rx FIFO.
 */
static _Bool can_is_mb_occupied(can_chn_e chn, unsigned char mb_index)
{
    unsigned char last_occupied_mb;
    _Bool fg_ret;

    /* Is Rx FIFO enabled? */
    if (0 != (reg_can_mcr3(chn) & FLD_CAN_RFEN))
    {
        /* Get RFFN value. */
        last_occupied_mb = (unsigned char)(reg_can_ctrl2_3(chn)&FLD_CAN_RFFN);
        /* Calculate the number of last Message Buffer occupied by Rx FIFO. */
        last_occupied_mb = ((last_occupied_mb + 1U) * 2U) + 5U;

        fg_ret = (mb_index <= last_occupied_mb);
    }
    else
    {

        fg_ret = 0;
    }

    return fg_ret;
}

/*!
 * @brief  This function aborts the previous transmission, cleans the Message Buffer, and configures it as a Transmit Message Buffer.
 * @param[in]  chn      - can_chn_e
 * @param[in]  mb_index - the Message Buffer index.
 * @param[in]  enable   - Enable/disable Tx Message Buffer.
 * @return     - 0: Enable Tx Message Buffer. - 1: the legacy rxfifo is occupied
 */
unsigned char can_set_tx_mailbox_cfg(can_chn_e chn,unsigned char mb_index, unsigned char enable){
    if(can_is_mb_occupied(chn,mb_index)){
        return 1;
    }
     /* Inactivate Message Buffer. */
   if(enable){
       can[chn]->MB[mb_index].cs = (CAN_TX_MB_INACTIVE)<<24;
   }else{
       can[chn]->MB[mb_index].cs =0;
   }
    /* Clean Message Buffer content. */
   can[chn]->MB[mb_index].id    = 0x00;
   can[chn]->MB[mb_index].word0 = 0x00;
   can[chn]->MB[mb_index].word1 = 0x00;
   return 0;
}

/*!
 * @brief  This function aborts the previous transmission, cleans the Message Buffer, and configures it as a Transmit Message Buffer.
 * @param[in]  chn      - can_chn_e
 * @param[in]  mb_index - the Message Buffer index.
 * @param[in]  enable   - Enable/disable Tx Message Buffer.
 * @return     none
 */
void canfd_set_tx_mailbox_cfg(can_chn_e chn,unsigned char mb_index,unsigned char enable){
   unsigned char cnt =0;
   unsigned char payload_dword =1;
   can_mb_data_size_e data_size =reg_can_fdctrl_2(chn)&FLD_CAN_MBDSR0;
   volatile unsigned int *mb_addr = &(can[chn]->MB[0].cs);
   unsigned int offset = canfd_get_mailbox_offset(data_size,mb_index);
   if(enable){
       mb_addr[offset] = (CAN_TX_MB_INACTIVE)<<24;
   }else{
       mb_addr[offset] = 0;
   }
   /* Calculate the DWORD number, dataSize 0/1/2/3 corresponds to 8/16/32/64
          Bytes payload. */
   for(cnt=0; cnt <(data_size +1);cnt++){
       payload_dword *= 2;
   }
   /* Clean ID. */
  mb_addr[offset + 1] = 0x0;
  /* Clean Message Buffer content, DWORD by DWORD. */
  for (cnt = 0; cnt < payload_dword; cnt++)
  {
      mb_addr[offset + 2 + cnt] = 0x0;
  }
}

/*!
 * @brief this function cleans a build-in Message Buffer and configures it as a Receive Message Buffer.
 * @param[in]  chn      - can_chn_e
 * @param[in]  mb_index - the Message Buffer index.
 * @param[in]  rx_mb_cfg- RxMbConfig Pointer to the CAN Message Buffer configuration structure.
 * @param[in]  enable   - enable/disable Rx Message Buffer.
 * @return     - 0: Enable rx Message Buffer. - 1: the legacy rxfifo is occupied
 */
unsigned char can_set_rx_mailbox_cfg(can_chn_e chn,unsigned char mb_index, can_rx_mb_config_t *rx_mb_cfg, unsigned char enable){
    if(can_is_mb_occupied(chn,mb_index)){
        return 1;
    }
    unsigned int cs_temp=0;
    //inactive message buffer
    can[chn]->MB[mb_index].cs  =0;
    can[chn]->MB[mb_index].id  =0;
    can[chn]->MB[mb_index].word0 =0;
    can[chn]->MB[mb_index].word1 =0;
    if(enable){
          /* Setup Message Buffer ID. */
        can[chn]->MB[mb_index].id = rx_mb_cfg->id;
        /* Setup Message Buffer format. */
        if(rx_mb_cfg->format == CAN_FRAME_FORMAT_EXTEND){
            cs_temp |= CAN_CS_IDE_MASK;
        }
        /* Setup Message Buffer type. */
//      if(rx_mb_cfg->type == CAN_FRAME_TYPE_REMOTE){
//          //cs_temp |= CAN_CS_RTR_MASK;
//      }
         /* Activate Rx Message Buffer. */
        if((rx_mb_cfg->type == CAN_FRAME_TYPE_REMOTE) && (rx_mb_cfg->remote_type == REMOTE_REQUEST_STORING)){
            cs_temp |= (CAN_RX_MB_EMPTY<<24);
            reg_can_ctrl2_2(chn) |= FLD_CAN_RRS;
        }else if((rx_mb_cfg->type == CAN_FRAME_TYPE_REMOTE) && (rx_mb_cfg->remote_type == REMOTE_REQUSET_NO_STORING)){
            cs_temp |= (CAN_RX_MB_RANSWER<<24);
            reg_can_ctrl2_2(chn) &= ~FLD_CAN_RRS;
        }else{
            cs_temp |= (CAN_RX_MB_EMPTY<<24);
        }
        can[chn]->MB[mb_index].cs = cs_temp;
    }
    return 0;
}

/*!
 * @brief this function cleans a build-in Message Buffer and configures it as a Receive Message Buffer by can fd.
 * @param[in]  chn      - can_chn_e
 * @param[in]  mb_index - the Message Buffer index.
 * @param[in]  rx_mb_cfg- RxMbConfig Pointer to the CAN Message Buffer configuration structure.
 * @param[in]  enable   - enable/disable Rx Message Buffer.
 * @return     none
 */
void canfd_set_rx_mailbox_cfg(can_chn_e chn,unsigned char mb_index, can_fd_rx_mb_config_t *rx_mb_cfg, unsigned char enable){
     unsigned int cs_temp =0;
     unsigned char cnt=0;
     volatile unsigned int* mb_addr = &(can[chn]->MB[0].cs);
     can_mb_data_size_e data_size =reg_can_fdctrl_2(chn)&FLD_CAN_MBDSR0;
     unsigned int offset = canfd_get_mailbox_offset(data_size,mb_index);
     unsigned char payload_dword;
     //inactive message buffer
     mb_addr[offset] = 0;
     //clean message id
     mb_addr[offset+1] = 0;
     //clean payload
     payload_dword = (2<<data_size);
     for (cnt = 0; cnt < payload_dword; cnt++)
     {
         mb_addr[offset + 2 + cnt] = 0x0;
     }
     if(enable){
         //setup message buffer id
         mb_addr[offset+1] = rx_mb_cfg->id;
         /* Setup Message Buffer format. */
        if(rx_mb_cfg->format == CAN_FRAME_FORMAT_EXTEND){
            cs_temp |= CAN_CS_IDE_MASK;
        }
         /* Activate Rx Message Buffer. */
        cs_temp |= (CAN_RX_MB_EMPTY<<24);

        mb_addr[offset] = cs_temp;
     }
}

/*!
 * @brief This function sets the individual mask for the CAN matching process.
 * The configuration is only effective when the Rx individual mask is enabled in the can_init().
 * If the legacy Rx FIFO is disabled, the individual mask is applied to the corresponding Message Buffer.
 * If the legacy Rx FIFO is enabled, the individual mask for Rx FIFO occupied Message Buffer is applied the Rx Filter with the same index.
 * Note that only the first 32 individual masks can be used as the Rx FIFO filter mask.
 *
 * @param[in]  chn      - can_chn_e
 * @param[in]  mb_index - the Message Buffer index.
 * @param[in]  mask     - the Rx Individual Mask value.
 */
void can_set_rx_individual_mask(can_chn_e chn,unsigned char mask_index,unsigned int* mask){

    can_rx_mb_im[chn]->rx_mb_imr[mask_index] = *mask;
}

/*!
 * @brief This function sets the global mask for the CAN message buffer in a matching process.
 * The configuration is only effective when the Rx individual mask is disabled in the can_init().
 *
 * @param[in]  chn      - can_chn_e
 * @param[in]  mask     - Rx Message Buffer Global Mask value.
 */
void can_set_rxmb_global_mask(can_chn_e chn,unsigned int mask){
    reg_can_rx_mail_mask(chn)= mask;
    reg_can_rx_mail_mask_14(chn)= mask;
    reg_can_rx_mail_mask_15(chn)= mask;
}

/*!
 * brief This function sets the global mask for CAN FIFO in a matching process.
 *
 * @param[in] chn  - can_chn_e
 * @param[in] mask - Rx Fifo Global Mask value.
 */
void can_set_legacy_rxfifo_global_mask(can_chn_e chn, unsigned int mask){
    reg_can_legacy_rxfifo_gm(chn) = mask;
}

/*!
 * @brief This function configures the CAN Rx FIFO with given configuration.
 *        note 1.Legacy Rx FIFO only can receive classic CAN message.
 *             2.by (8* (n + 1)) >= rxfifo_config->rxfifo_config.id_filter_num, the number of mb occupied by rxfifo is (n+1)*8/4+6,
 *               can_set_tx_mailbox_cfg()/can_set_rx_mailbox_cfg()/can_write_tx_mb()/can_read_rx_mb(),
 *               When legacy rxfifo is enabled, these interfaces determine whether mb is occupied.
 *             3.when the rx individual mask is enabled in the can_init(), only the first 32 individual masks can be used as the Rx FIFO filter mask.
 *             4.rxfifo_config.rxfifo_config,decide whether to traverse the legacy rxfifo or the rx mailbox first.
 * @param[in] chn           - can_chn_e
 * @param[in] rxfifo_config - RxFifoConfig Pointer to the CAN Legacy Rx FIFO configuration structure. Can be NULL when enable parameter is false.
 * @param[in] enable        - Enable/disable Legacy Rx FIFO.
 * @return    - 1: Enable Legacy Rx FIFO. 0: set Legacy Rx FIFO fail.
 */
unsigned char can_set_legacy_rxfifo_cfg(can_chn_e chn,can_rx_fifo_config_t *rxfifo_config, unsigned char enable){
    volatile unsigned int *mb_addr;
    unsigned char i,j,k,rffn=0,nummb_occupy;
    unsigned char setup_mb=0;

    if(enable){
        //get the setup_mb value
        setup_mb = reg_can_mcr0(chn)| FLD_CAN_MAXMB;
        //determine rffn value
        for(i=0;i<=0x0f;i++){
            if((8* (i + 1)) >= rxfifo_config->id_filter_num){
                rffn =i;
                if(((setup_mb -8) - (2* rffn)) < 0){
                    return 0;
                }
                reg_can_ctrl2_3(chn) = (reg_can_ctrl2_3(chn) & ~FLD_CAN_RFFN) | rffn;
                break;
            }
        }
        nummb_occupy = 6 + (rffn + 1)*2;
        //copy id filter table to message buffer region
        j=0;
        for(i = 6; i < nummb_occupy; i++){
            //get address for current mail box
            mb_addr = &(can[chn]->MB[i].cs);
            //one mail box contain 4 dword registers
            for(k=0;k < 4;k++){
                //fill all valid filter in the mail box occupied by filter.
                //disable unused rx fifo filter, the other reset of register in the last mail box occupied by filter set as 0xffffffff.
                mb_addr[k] = (j < rxfifo_config->id_filter_num) ? (rxfifo_config->id_filter_table[j]) : 0xffffffff;
                // try to fill next filter in current mail box
                j++;
            }
        }
        //setup id filter type
        switch(rxfifo_config->id_filter_type){
            case CAN_RXFIFO_FILTER_TYPE_A:
               reg_can_mcr1(chn) = (reg_can_mcr1(chn) &(~FLD_CAN_IDAM)) | (0x00);
               break;
            case CAN_RXFIFO_FILTER_TYPE_B:
               reg_can_mcr1(chn) = (reg_can_mcr1(chn) &(~FLD_CAN_IDAM)) | (0x01);
               break;
            case CAN_RXFIFO_FILTER_TYPE_C:
               reg_can_mcr1(chn) = (reg_can_mcr1(chn) &(~FLD_CAN_IDAM)) | (0x02);
               break;
            case CAN_RXFIFO_FILTER_TYPE_D:
               /* All frames rejected. */
               reg_can_mcr1(chn) = (reg_can_mcr1(chn) &(~FLD_CAN_IDAM)) | (0x03);
               break;
            default:
                /* All the cases have been listed above, the default clause should not be reached. */
               break;
        }
        /* Setting Message Reception Priority. */
        reg_can_ctrl2_2(chn) = (rxfifo_config->priority == CAN_RXFIFO_PRIO_HIGH) ? (reg_can_ctrl2_2(chn) & ~(FLD_CAN_MRP)):
                                                                                       (reg_can_ctrl2_2(chn) | (FLD_CAN_MRP));
        /* Enable legacy Rx FIFO. */
        reg_can_mcr3(chn) |= FLD_CAN_RFEN;
    }
    else
    {
        rffn = reg_can_ctrl2_3(chn) & FLD_CAN_RFFN;
        /* calculate the Number of Mailboxes occupied by RX Legacy FIFO and the filter. */
        nummb_occupy = 6U + (rffn + 1) * 2;

        /* Disable Rx Message FIFO. */
        reg_can_mcr3(chn) &= ~FLD_CAN_RFEN;

        /* Clean MB0 ~ MB5 and all MB occupied by ID filters */

        for (i = 0; i < nummb_occupy; i++)
        {
            can_set_rx_mailbox_cfg(chn, i, NULL, 0);
        }
    }
    return 1;
}

/*!
 * @brief This function configures the Enhanced Rx FIFO with given configuration.
 *        note:Enhanced Rx FIFO support receive classic CAN or CAN FD messages, Legacy Rx FIFO and Enhanced Rx FIFO
 *        cannot be enabled at the same time.
 * @param[in] chn   - can_chn_e
 * @param[in] cfg   - Config Pointer to the CAN Enhanced Rx FIFO configuration structure. Can be NULL when enable parameter is false.
 * @param[in] enable- Enable/disable Enhanced Rx FIFO.
 * @return    - none
 */
void can_set_enhanced_rxfifo_cfg(can_chn_e chn,can_enhanced_rx_fifo_config_t *cfg,unsigned char enable){
    if(enable){
        //setting enhanced rx fifo
        reg_can_erfcr_1(chn) = (reg_can_erfcr_1(chn) & ~FLD_CAN_NFE) | (cfg->id_filter_pairnum -1);
        reg_can_erfcr_2(chn) = (reg_can_erfcr_2(chn) & ~FLD_CAN_NEXIF) | (cfg->extendid_filternum);
        //copy id filter table to enhanced rx fifo filter element register
        for(unsigned char i=0;i<128;i++){
            can_enhanced_rxfifo_filter[chn]->filter[i] = (i< (cfg->id_filter_pairnum)*2) ? cfg->id_filter_table[i] : 0xffffffff;
        }
        //setting message reception priority
        reg_can_ctrl2_2(chn) = (cfg->priority ==CAN_RXFIFO_PRIO_HIGH )? (reg_can_ctrl2_2(chn) & ~FLD_CAN_MRP) :(reg_can_ctrl2_2(chn)|FLD_CAN_MRP);
        //dma
        reg_can_erfcr_3(chn) = (reg_can_erfcr_3(chn)&(~FLD_CAN_DMALW))|(19<<2);
//        reg_can_ctrl1_1(chn) |=FLD_DMA_ADDR_CTRL;
        //enable enhanced rx fifo
        reg_can_erfcr_3(chn) |= FLD_CAN_ERFEN;
    }else{
        //disable enhanced rx fifo
         reg_can_erfcr_3(chn) &= ~FLD_CAN_ERFEN;
         //clean all enhanced rxfifo filter element register
         for(unsigned char i=0; i< 128; i++){
             can_enhanced_rxfifo_filter[chn]->filter[i] = 0xffffffff;
         }
    }

}

/**
  * @brief     Configures the legacy rxfifo or enhanced rxfifo dma channel control register.
  * @param[in] chn       - can_chn_e
  * @param[in] dma_chn   - dma channel.
  * @return    none
 */
void can_set_rx_dma_config(can_chn_e chn, dma_chn_e dma_chn){
    dma_config(dma_chn, &can_rx_dma_config[chn]);
    reg_can_mcr1(chn) |= FLD_CAN_DMA;
    reg_can_ctrl1_1(chn) |=FLD_DMA_ADDR_CTRL;
}

/**
 * @brief       This function serves to receive data function by DMA, this function tell the DMA to get data from the rx fifo.
 *              regardless of the length of the DMA configuration, write_num is available,the dma automatically writes back the data receive length to the first four bytes of the rec_buff without manual calculation,
 *              so need addr = the receive buff addr +4.
 * @param[in]   chn       - can_chn_e
 * @param[in]   dma_chn   - dma channel.
 * @param[in]   rx_mode   - rx_mode.
 * @param[in]   addr     - pointer to the buffer receive data.
 * @param[in]   rev_size - the receive length of DMA,The maximum transmission length of DMA is 0xFFFFFC bytes, so dont'n over this length.
 * @return      none
 * @note        When it is CAN_ENHANCED_RXFIFO, for example, the number of triggers set by can_set_enhanced_rxfifo_watermark is n.
 *              When the number of packets in rxfifo reaches n, dma is triggered to carry n packets, and if the consecutively received packets are larger than 2n,
 *              and do not have the time to configure dma, then, after configuring the dma to carry n, and then configuring the next dma, the dma will not work.
 */
void can_receive_dma(can_chn_e chn,dma_chn_e dma_chn,can_rxfifo_mode_e rx_mode, unsigned char *addr, unsigned int rev_size)
{
    if(rx_mode == CAN_LEGACY_RXFIFO){
         dma_set_address(dma_chn, can_mb_addr_start(chn), (unsigned int)(addr));
    }else if(rx_mode == CAN_ENHANCED_RXFIFO){
         dma_set_address(dma_chn, can_enhanced_rx_fifo_addr_start(chn) , (unsigned int)(addr));
    }
    dma_set_size(dma_chn, rev_size, DMA_WORD_WIDTH);
    dma_chn_en(dma_chn);
}

/**
 * @brief      Configure DMA head node.
 * @param[in]  chn         - can_chn_e
 * @param[in]  dma_chn   - dma channel.
 * @param[in]  rx_mode   - rx_mode.
 * @param[in]  dst_addr    - Pointer to data buffer, which must be 4 bytes aligned.
 * @param[in]  data_len    - any length less than 0xfffffc(multiple of four).
 * @param[in]  head_of_list - the head address of DMA llp.
 * @return     none.
 * @note       there are no usage restrictions, there is no need to configure the dma length to a maximum length, and there is no limit to the send length:
 *              - The condition of linked list jump: one is when the length of the dma configuration is reached, the other rxfifo is avaid,
 *                no matter which condition has a write back length.
*/
void can_set_dma_chain_llp(can_chn_e chn, dma_chn_e dma_chn, can_rxfifo_mode_e rx_mode,unsigned char *dst_addr, unsigned int data_len, dma_chain_config_t *head_of_list)
{
    dma_config(dma_chn, &can_rx_dma_config[chn]);
    reg_can_mcr1(chn) |= FLD_CAN_DMA;
    reg_can_ctrl1_1(chn) |=FLD_DMA_ADDR_CTRL;
    if(rx_mode == CAN_LEGACY_RXFIFO){
       dma_set_address(dma_chn, can_mb_addr_start(chn), (unsigned int)(dst_addr));
    }else if(rx_mode == CAN_ENHANCED_RXFIFO){
       dma_set_address(dma_chn, can_enhanced_rx_fifo_addr_start(chn), (unsigned int)(dst_addr));
    }
    dma_set_size(dma_chn, data_len, DMA_WORD_WIDTH);
    reg_dma_llp(dma_chn) = (unsigned int)(head_of_list);
}

/**
 * @brief      Configure DMA cycle chain node.
 * @param[in]  chn         - can_chn_e
 * @param[in]  dma_chn   - dma channel.
 * @param[in]  rx_mode   - rx_mode.
 * @param[in]  config_addr - to servers to configure the address of the current node.
 * @param[in]  llpointer   - to configure the address of the next node.
 * @param[in]  dst_addr    - Pointer to data buffer, which must be 4 bytes aligned.
 * @param[in]  data_len    - any length less than 0xfffffc(multiple of four).
 * @return     none.
 * @note       there are no usage restrictions, there is no need to configure the dma length to a maximum length, and there is no limit to the send length:
 *             - The condition of linked list jump: one is when the length of the dma configuration is reached, the other is rx_done,
 *               no matter which condition has a write back length.
 */
void can_rx_dma_add_list_element(can_chn_e chn, dma_chn_e dma_chn, can_rxfifo_mode_e rx_mode,dma_chain_config_t *config_addr, dma_chain_config_t *llpointer, unsigned char *dst_addr, unsigned int data_len)
{
    config_addr->dma_chain_ctl = reg_dma_ctrl(dma_chn) | BIT(0)|BIT(1);
    if(rx_mode == CAN_LEGACY_RXFIFO){
         config_addr->dma_chain_src_addr = can_mb_addr_start(chn);
    }else if(rx_mode == CAN_ENHANCED_RXFIFO){
         config_addr->dma_chain_src_addr = can_enhanced_rx_fifo_addr_start(chn);
    }
    config_addr->dma_chain_dst_addr = (unsigned int)(dst_addr);
    config_addr->dma_chain_data_len = dma_cal_size(data_len, 4);
    config_addr->dma_chain_llp_ptr = (unsigned int)(llpointer);
}

/*!
 * @brief This function configures the Pretended Networking with given configuration.
 * @param[in] chn    - can_chn_e
 * @param[in] pn_cfg - Pretended Networking Configure Pointer to the Pretended Networking configuration structure.
 * @return    none
 */
void can_set_pn_config(can_chn_e chn, can_pn_config_t* pn_cfg){
    unsigned int pn_ctrl;
    pn_ctrl = (pn_cfg->match_num >1) ? (0x02|pn_cfg->match_src) : (pn_cfg->match_src);
    pn_ctrl = pn_ctrl|(pn_cfg->id_match_mode<<2) | (pn_cfg->data_match_mode<<4)|(pn_cfg->match_num<<8);
    reg_can_ctrl1_pn(chn) = pn_ctrl;
    if(pn_cfg->timeout_is_en)
    {
        reg_can_ctrl2_pn_0(chn) = pn_cfg->timeout_value;
    }
    else
    {
        reg_can_ctrl2_pn_0(chn) = 0;
    }
    reg_can_flt_id1(chn) = pn_cfg->id_lower;
    reg_can_flt_id2_idmask(chn) = pn_cfg->id_upper;
    reg_can_flt_dlc_0(chn) = pn_cfg->length_upper;
    reg_can_flt_dlc_2(chn) = pn_cfg->length_lower;
    reg_can_pl1_lo(chn) = pn_cfg->lower_word0;
    reg_can_pl1_hi(chn) = pn_cfg->lower_word1;
    reg_can_pl2_plmask_lo(chn)=pn_cfg->upper_word0;
    reg_can_pl2_plmask_hi(chn)=pn_cfg->upper_word1;
    reg_can_mcr1(chn) = FLD_CAN_PNET_EN;
}

/*!
 * @brief This function writes a CAN Message to the specified Transmit Message Buffer
 *        and changes the Message Buffer state to start CAN Message transmit. After
 *        that the function returns immediately.
 * @param[in] chn    - can_chn_e
 * @param[in] mb_index - The CAN Message Buffer index.
 * @param[in] frame -TxFrame Pointer to CAN message frame to be sent.
 * @param[in] is_rtr_response - can_remote_received_type_e
 * @return 0 - Write Tx Message Buffer Successfully.1 - Tx Message Buffer is currently in use.
 * @note  1. Sends data in big-end mode by default; 2. the length is sent according to the configured length, but the data is fetched in word units;
 */
unsigned char can_write_tx_mb(can_chn_e chn,unsigned char mb_index,can_frame_t* frame,unsigned char is_rtr_response){
    if(can_is_mb_occupied(chn,mb_index)){
        return 1;
    }
    unsigned int cs_temp=0;
    //id
    can[chn]->MB[mb_index].id = (frame->id | (frame->prio<<29));
    //format
    if(frame->format == CAN_FRAME_FORMAT_EXTEND){
        cs_temp |= CAN_CS_SRR_MASK | CAN_CS_IDE_MASK;
    }
    //type
    if(frame->type == CAN_FRAME_TYPE_REMOTE){
        cs_temp |=CAN_CS_RTR_MASK;
    }

    if(is_rtr_response){
        cs_temp |= CAN_CS_CODE(CAN_TX_MB_TANSWER) | CAN_CS_DLC(frame->length);
    }else{
          cs_temp |= CAN_CS_CODE(CAN_TX_MB_DATA_OR_REMOTE) | CAN_CS_DLC(frame->length);
    }
//    gpio_toggle(GPIO_PA0);
    can[chn]->MB[mb_index].word0 = frame->data_word0;
    can[chn]->MB[mb_index].word1 = frame->data_word1;
    can[chn]->MB[mb_index].cs    = cs_temp;
//    gpio_toggle(GPIO_PA0);
    return 0;
}

/*!
 * @brief This function writes a CAN FD Message to the specified Transmit Message Buffer
 *        and changes the Message Buffer state to start CAN FD Message transmit. After
 *        that the function returns immediately.
 * @param[in] chn    - can_chn_e
 * @param[in] mb_index - The CAN Message Buffer index.
 * @param[in] frame -TxFrame Pointer to CAN FD message frame to be sent.
 * @return  - none
 * @note  1. Sends data in big-end mode by default; 2. the length is sent according to the configured length, but the data is fetched in word units;
 */
void canfd_write_tx_mb(can_chn_e chn,unsigned char mb_index,can_fd_frame_t* frame){
    unsigned int cs_temp=0;
    unsigned char payload_dword=1;
    can_mb_data_size_e data_size =reg_can_fdctrl_2(chn)&FLD_CAN_MBDSR0;
     volatile unsigned int *mb_addr = &(can[chn]->MB[0].cs);
     unsigned int offset = canfd_get_mailbox_offset(data_size,mb_index);

     mb_addr[offset+1] = (frame->id|  (frame->prio<<29));
     if(frame->format == CAN_FRAME_FORMAT_EXTEND){
        cs_temp |= CAN_CS_SRR_MASK | CAN_CS_IDE_MASK;
     }
     if(frame->type == CAN_FRAME_TYPE_REMOTE){
         cs_temp |= CAN_CS_RTR_MASK;
     }
     cs_temp |= CAN_CS_CODE(CAN_TX_MB_DATA_OR_REMOTE) | CAN_CS_DLC(frame->length) | CAN_CS_EDL(frame->edl) |
             CAN_CS_BRS(frame->brs);
     /* Calculate the DWORD number, dataSize 0/1/2/3 corresponds to 8/16/32/64
       Bytes payload. */
    for (unsigned char cnt = 0; cnt < (data_size + 1); cnt++)
    {
        payload_dword *= 2;
    }

    /* Load Message Payload and Activate Tx Message Buffer. */
    for (unsigned char cnt = 0; cnt < payload_dword; cnt++)
    {
        mb_addr[offset + 2 + cnt] = frame->data_word[cnt];
    }
    mb_addr[offset] = cs_temp;
}

/*!
 * @brief This function reads a CAN message from a specified Receive Message Buffer.
 * The function fills a receive CAN message frame structure with
 * just received data and activates the Message Buffer again.
 * The function returns immediately.
 *
 * @param[in] chn    - can_chn_e
 * @param[in] mb_index - The CAN Message Buffer index.
 * @param[in] frame - pRxFrame Pointer to CAN message frame structure for reception.
 * @return    1           - Rx Message Buffer is full and has been read successfully.
 *            0           - Rx Message Buffer is already overflowed and has been read successfully.
 *            2           - Tx Message Buffer is currently in use.
 */
unsigned char can_read_rx_mb(can_chn_e chn,unsigned char mb_index,can_frame_t* frame){
    if(can_is_mb_occupied(chn,mb_index)){
        return 2;
    }
    unsigned int cs_temp;
    unsigned int rx_code;
    unsigned char status = 0xff;
    /* Read CS field of Rx Message Buffer to lock Message Buffer. */
    cs_temp = can[chn]->MB[mb_index].cs;
    /* Get Rx Message Buffer Code field. */
    rx_code = (cs_temp & CAN_CS_CODE_MASK) >> CAN_CS_CODE_SHIFT;

    /* Check to see if Rx Message Buffer is full. */
    if (((unsigned int)CAN_RX_MB_FULL == rx_code) || ((unsigned int)CAN_RX_MB_OVERRUN == rx_code))
    {
        /* Store Message ID. */
        frame->id = can[chn]->MB[mb_index].id & (CAN_ID_EXT_MASK | CAN_ID_STD_MASK);

        /* Get the message ID and format. */
        frame->format = (cs_temp & CAN_CS_IDE_MASK) != 0 ? (unsigned char)CAN_FRAME_FORMAT_EXTEND :
                                                               (unsigned char)CAN_FRAME_FORMAT_STANDARD;

        /* Get the message type. */
        frame->type =
            (cs_temp & CAN_CS_RTR_MASK) != 0 ? (unsigned char)CAN_FRAME_TYPE_REMOTE : (unsigned char)CAN_FRAME_TYPE_DATA;

        /* Get the message length. */
        frame->length = (unsigned char)((cs_temp & CAN_CS_DLC_MASK) >> CAN_CS_DLC_SHIFT);

        /* Get the time stamp. */
        frame->timestamp = (unsigned short)((cs_temp & CAN_CS_TIME_STAMP_MASK) >> CAN_CS_TIME_STAMP_SHIFT);

        /* Store Message Payload. */
        frame->data_word0 = can[chn]->MB[mb_index].word0;
        frame->data_word1 = can[chn]->MB[mb_index].word1;

        /* Read free-running timer to unlock Rx Message Buffer. */
        (void)reg_can_timer(chn);

        if (CAN_RX_MB_FULL == rx_code)
        {
            status= 0;
        }
        else
        {
            status= 1;
        }
    }
    return status;
}

/*!
 * @brief This function reads a CAN FD message from a specified Receive Message Buffer.
 * The function fills a receive CAN FD message frame structure with
 * just received data and activates the Message Buffer again.
 * The function returns immediately.
 *
 * @param[in] chn    - can_chn_e
 * @param[in] mb_index - The CAN Message Buffer index.
 * @param[in] frame - pRxFrame Pointer to CAN FD message frame structure for reception.
 * @return    1           - Rx Message Buffer is full and has been read successfully.
 *            0           - Rx Message Buffer is already overflowed and has been read successfully.
 */
unsigned char canfd_read_rx_mb(can_chn_e chn,unsigned char mb_index,can_fd_frame_t* frame){
    unsigned char status = 0xff;
    unsigned int cs_temp;
    unsigned int can_id;
    unsigned char rx_code;
    volatile unsigned char* data_payload_flag=0;
    can_mb_data_size_e data_size =reg_can_fdctrl_2(chn)&FLD_CAN_MBDSR0;
    volatile unsigned int *mb_addr = &(can[chn]->MB[0].cs);
    unsigned int offset = canfd_get_mailbox_offset(data_size,mb_index);
    cs_temp = mb_addr[offset];
    can_id  = mb_addr[offset + 1];

    rx_code = (unsigned char)((cs_temp & CAN_CS_CODE_MASK) >> CAN_CS_CODE_SHIFT);
    if((rx_code == CAN_RX_MB_FULL) || (rx_code == CAN_RX_MB_OVERRUN)){
        frame->id  = can_id & (CAN_ID_EXT_MASK | CAN_ID_STD_MASK);
        frame->format = ((cs_temp & CAN_CS_IDE_MASK) != 0) ? CAN_FRAME_FORMAT_EXTEND : CAN_FRAME_FORMAT_STANDARD;
        frame->brs = (cs_temp & CAN_CS_BRS_MASK) != 0 ? 1 : 0;
        frame->edl = (cs_temp & CAN_CS_EDL_MASK) != 0 ? 1 : 0;
        frame->type = (cs_temp & CAN_CS_RTR_MASK) != 0 ? CAN_FRAME_TYPE_REMOTE : CAN_FRAME_TYPE_DATA;
        frame->length = ((cs_temp & CAN_CS_DLC_MASK) >> CAN_CS_DLC_SHIFT);
        frame->timestamp = (unsigned short)((cs_temp & CAN_CS_TIME_STAMP_MASK) >> CAN_CS_TIME_STAMP_SHIFT);
//      for (unsigned char cnt = 0; cnt < (data_size + 1); cnt++)
//      {
//          payload_dword *= 2;
//      }

        data_payload_flag = (volatile unsigned char*)(&mb_addr[offset + 2]);
        /* Store Message Payload. */
        for (unsigned char cnt = 0; cnt < DLC_LENGTH_DECODE(frame->length); cnt++)
        {
            *(unsigned char*)(&frame->data_byte0+cnt) = data_payload_flag[cnt];
        }

        /* Read free-running timer to unlock Rx Message Buffer. */
        (void)reg_can_timer(chn);

        if (CAN_RX_MB_FULL == rx_code)
        {
            status = 0;
        }
        else if(CAN_RX_MB_OVERRUN == rx_code)
        {
            status = 1;
        }
    }
    return status;
}

/*!
 * @brief  This function reads a CAN message from the CAN Legacy Rx FIFO.
 *
 * @param[in] chn    - can_chn_e
 * @param[in] frame  - pRxFrame Pointer to CAN message frame structure for reception.
 * @return  none
 */
void can_read_legacy_rxfifo(can_chn_e chn,can_frame_t* frame){
    unsigned int cs_temp;
    /* Read CS field of Rx Message Buffer to lock Message Buffer. */
    cs_temp = can[chn]->MB[0].cs;

    /* Read data from Rx FIFO output port. */
    /* Store Message ID. */
    frame->id = (can[chn]->MB[0].id) & (CAN_ID_EXT_MASK | CAN_ID_STD_MASK);

    /* Get the message ID and format. */
    frame->format = (cs_temp & CAN_CS_IDE_MASK) != 0 ? (unsigned char)CAN_FRAME_FORMAT_EXTEND :
                                                       (unsigned char)CAN_FRAME_FORMAT_STANDARD;

    /* Get the message type. */
    frame->type =
        (cs_temp & CAN_CS_RTR_MASK) != 0 ? (unsigned char)CAN_FRAME_TYPE_REMOTE : (unsigned char)CAN_FRAME_TYPE_DATA;

    /* Get the message length. */
    frame->length = (unsigned char)((cs_temp & CAN_CS_DLC_MASK) >> CAN_CS_DLC_SHIFT);

    /* Get the time stamp. */
    frame->timestamp = (unsigned short)((cs_temp & CAN_CS_TIME_STAMP_MASK) >> CAN_CS_TIME_STAMP_SHIFT);

    /* Store Message Payload. */
    frame->data_word0 = can[chn]->MB[0].word0;
    frame->data_word1 = can[chn]->MB[0].word1;

    /* Store ID Filter Hit Index. */
    frame->idhit = (unsigned short)(reg_can_rxfir(chn));

    /* Read free-running timer to unlock Rx Message Buffer. */
    (void)reg_can_timer(chn);
}

/*!
 * @brief This function reads a CAN or CAN FD message from the CAN Enhanced Rx FIFO.
 *
 * @param[in] chn    - can_chn_e
 * @param[in] frame  - pRxFrame Pointer to CAN FD message frame structure for reception.
 * @return  none
 */
void can_read_enhanced_rxfifo(can_chn_e chn, can_fd_frame_t *frame)
{
    unsigned int id_hit_off;
    /* Enhanced Rx FIFO ID HIT offset is changed dynamically according to data length code (DLC) . */
    id_hit_off = (DLC_LENGTH_DECODE(((can_fd_frame_t *)can_enhanced_rx_fifo_addr_start(chn))->length) + 3) / 4 + 3;
    /* Copy CAN FD Message from Enhanced Rx FIFO, should use the DLC value to identify the bytes that belong to the
     * message which is being read. */
    (void)memcpy((void *)frame, (void *)(unsigned int *)can_enhanced_rx_fifo_addr_start(chn) , sizeof(unsigned int) * (id_hit_off+1));
    frame->idhit = frame->data_word[id_hit_off - 3];
    if(!(reg_can_ctrl2_0(chn)|(FLD_CAN_TIME_STAMP_CAP_POINT))){
         frame->hr_time_stamp= frame->data_word[id_hit_off-3+1];
    }
    /* Clear the unused frame data. */
    for (unsigned int i = (id_hit_off - 3); i < 16; i++)
    {
        frame->data_word[i] = 0x0;
    }
}

/*!
 * @brief Since the ID HIT offset is changed dynamically according to data length code (DLC),
 *        needs to be reconfigured according to the frame structure to obtain the correct idhit
 * @param[in] frame  - pRxFrame Pointer to CAN FD message frame structure for reception.
 * @return  none
 */
void can_enhanced_rxfifo_dma_reset_idhit(can_chn_e chn,can_fd_frame_t* frame){
    unsigned int id_hit_off;
    /* Enhanced Rx FIFO ID HIT offset is changed dynamically according to data length code (DLC) . */
    id_hit_off = ((DLC_LENGTH_DECODE(frame->length)) + 3) / 4 + 3;
    frame->idhit =frame->data_word[id_hit_off - 3];
    if(!(reg_can_ctrl2_0(chn)|(FLD_CAN_TIME_STAMP_CAP_POINT))){
         frame->hr_time_stamp= frame->data_word[id_hit_off-3+1];
    }else{
         frame->hr_time_stamp= 0x00;
    }
   /* Clear the unused frame data. */
   for (unsigned int i = (id_hit_off - 3); i < 16; i++)
   {
       frame->data_word[i] = 0x0;
   }
}

/*!
 * @brief when the remote frame is REMOTE_REQUEST_NO_STORING, the payload needs to be filled in advance to automatically respond to the returned data.
 * @param[in] chn    - can_chn_e
 * @param[in] mb_index - The CAN Message Buffer index.
 * @param[in] payload - payload data.
 * @return  none
 */
void can_set_mb_remote_frame_payload(can_chn_e chn,unsigned char mb_index,unsigned int*payload,can_fd_frame_length_e dlc_len){
     can[chn]->MB[mb_index].cs =  (can[chn]->MB[mb_index].cs)|(dlc_len<<16);

    can[chn]->MB[mb_index].word0 = *(payload);
     can[chn]->MB[mb_index].word1 = *(payload+1);
}
