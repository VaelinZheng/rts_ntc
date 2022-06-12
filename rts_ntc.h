/*
 * Copyright (c) 2018-2021, Xiaojin.Zheng
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date             Author                  Notes
 * 2022-06-11       Xiaojin.Zheng           Frist add
*/

#ifndef _RTS_NTC_H_
#define _RTS_NTC_H_
#ifdef USE_RTS_CFG
#include "rts_cfg.h"
#endif
#if (RTS_NTC_ENABLE)
#include "stdint.h"
typedef enum
{
    RTS_NTC_OK=0,
    RTS_NTC_ERROR
}rts_ntc_error_t;

void rts_ntc_init(uint16_t *adc_tab,uint16_t tab_len,int16_t offset_tempX10);
rts_ntc_error_t rts_ntc_adc_to_temp(uint16_t adc_val,int16_t *tempX10);

#endif
#endif
